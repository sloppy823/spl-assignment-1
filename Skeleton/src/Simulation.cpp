#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include <stdexcept>
#include <iostream>
#include <algorithm> // for std::find_if
#include <fstream>      // For std::ifstream
#include <sstream>      // For std::istringstream
using std::logic_error;
using std::runtime_error;
using std::find_if;
class Simulation;
extern Simulation* backup;
// Constructor
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions() {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw runtime_error("Failed to open configuration file: " + configFilePath);
    }

    string line;
    while (getline(configFile, line)) {
        std::istringstream stream(line);
        string command;
        stream >> command;

        if (line.empty() || line[0] == '#') {
            continue; // Skip comments and empty lines
        }

        try {
            if (command == "step") {
                int numOfSteps;
                stream >> numOfSteps;
                for (int i = 0; i < numOfSteps; ++i) {
                    step();
                }
            } else if (command == "plan") {
                string settlementName, policyType;
                stream >> settlementName >> policyType;
                Settlement *settlement = getSettlement(settlementName);
                if (!settlement) {
                    throw runtime_error("Settlement not found: " + settlementName);
                }
                SelectionPolicy *policy = createPolicy(policyType);
                if (!policy) {
                    throw runtime_error("Unknown selection policy: " + policyType);
                }
                addPlan(settlement, policy);
            } else if (command == "Settlement") {
                string settlementName;
                int settlementType;
                stream >> settlementName >> settlementType;
                auto settlement = new Settlement(settlementName, static_cast<SettlementType>(settlementType));
                if (!addSettlement(settlement)) {
                    delete settlement; // Prevent memory leak
                }
            } else if (command == "Facility") {
                string facilityName, category;
                int price, lifeQImpact, ecoImpact, envImpact;
                stream >> facilityName >> category >> price >> lifeQImpact >> ecoImpact >> envImpact;

                FacilityCategory facilityCategory = parseFacilityCategory(category);
                FacilityType facility(facilityName, facilityCategory, price, lifeQImpact, ecoImpact, envImpact);
                if (!addFacility(facility)) {
                    std::cerr << "Facility \"" << facilityName << "\" already exists.\n";
                }
            } else {
                throw runtime_error("Unknown command in configuration file: " + command);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error processing configuration command: " << line << "\n"
                      << e.what() << "\n";
        }
    }
}
void Simulation::start() {
    open();
    std::cout << "The simulation has started" << std::endl;

    string line;
    while (isRunning && std::getline(std::cin, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        string command;
        stream >> command;

        try {
            BaseAction *action = nullptr;

            if (command == "step") {
                int steps;
                stream >> steps;
                action = new SimulateStep(steps);
            } else if (command == "plan") {
                string settlementName, policyType;
                stream >> settlementName >> policyType;
                action = new AddPlan(settlementName, policyType);
            } else if (command == "backup") {
                action = new BackupSimulation();
            } else if (command == "restore") {
                action = new RestoreSimulation();
            } else if (command == "planStatus") {
                int planID;
                stream >> planID;
                action = new PrintPlanStatus(planID);
            } else if (command == "close") {
                action = new Close();
            } else {
                throw runtime_error("Unknown command: " + command);
            }

            if (action) {
                action->act(*this);
                actionsLog.push_back(action);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error processing command: " << e.what() << std::endl;
        }
    }
}


// Adds a new plan to the simulation
void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy) {
    if (!settlement) {
        throw logic_error("Invalid settlement provided for adding a plan.");
    }

    if (!selectionPolicy) {
        throw logic_error("Invalid selection policy provided for adding a plan.");
    }

    // Ownership of selectionPolicy is transferred to the Plan object
    plans.emplace_back(planCounter++, *settlement, selectionPolicy, facilitiesOptions);
}


// Adds a new action to the log
void Simulation::addAction(BaseAction *action) {
    if (!action) {
        throw logic_error("Invalid action provided.");
    }

    // Ownership of the action is transferred to the actions log
    actionsLog.push_back(action);
}

// Adds a new settlement
bool Simulation::addSettlement(Settlement *settlement) {
    if (!settlement) {
        throw logic_error("Cannot add a null settlement.");
    }

    if (isSettlementExists(settlement->getName())) {
        std::cerr << "Settlement \"" << settlement->getName() << "\" already exists.\n";
        return false;
    }

    // Ownership of settlement is transferred to the Simulation object
    settlements.push_back(settlement);
    return true;
}

// Adds a new facility type to the options
bool Simulation::addFacility(FacilityType facility) {
    for (const auto &existingFacility : facilitiesOptions) {
        if (existingFacility.getName() == facility.getName()) {
            std::cerr << "Facility \"" << facility.getName() << "\" already exists.\n";
            return false;
        }
    }

    facilitiesOptions.push_back(facility);
    return true;
}

// Checks if a settlement exists by name
bool Simulation::isSettlementExists(const string &settlementName) {
    return find_if(settlements.begin(), settlements.end(),
                   [&settlementName](const Settlement *settlement) {
                       return settlement->getName() == settlementName;
                   }) != settlements.end();
}

// Retrieves a settlement by name
Settlement *Simulation::getSettlement(const string &settlementName) {
    auto it = find_if(settlements.begin(), settlements.end(),
                      [&settlementName](const Settlement *settlement) {
                          return settlement->getName() == settlementName;
                      });
    return (it != settlements.end()) ? *it : nullptr;
}

// Retrieves a plan by ID
Plan &Simulation::getPlan(const int planID) {
    auto it = find_if(plans.begin(), plans.end(),
                      [planID](const Plan &plan) {
                          return plan.getPlanID() == planID;
                      });
    if (it == plans.end()) {
        throw runtime_error("Plan with ID " + std::to_string(planID) + " not found.");
    }
    return *it;
}

// Executes one step of the simulation
void Simulation::step() {
    if (!isRunning) {
        throw runtime_error("Cannot execute step. Simulation is not running.");
    }

    for (auto &plan : plans) {
        try {
            plan.step();
        } catch (const std::exception &e) {
            std::cerr << "Error during plan execution: " << e.what() << "\n";
            // Log error or take appropriate action
        }
    }

    // Additional logic for managing actions, logs, or events can be added here.
}

// Stops the simulation
void Simulation::close() {
    if (!isRunning) {
        throw runtime_error("Simulation is not running.");
    }
    isRunning = false;
}

// Reopens the simulation
void Simulation::open() {
    if (isRunning) {
        throw runtime_error("Simulation is already running.");
    }
    if (plans.empty()) {
        throw runtime_error("Cannot open simulation. No plans available.");
    }
    isRunning = true;
}

FacilityCategory Simulation::parseFacilityCategory(const string &category) {
    if (category == "life_quality") {
        return FacilityCategory::LIFE_QUALITY;
    } else if (category == "economy") {
        return FacilityCategory::ECONOMY;
    } else if (category == "environment") {
        return FacilityCategory::ENVIRONMENT;
    }
    throw runtime_error("Unknown facility category: " + category);
}

SelectionPolicy *Simulation::createPolicy(const string &policyType) {
    if (policyType == "eco") {
        return new EconomySelection();
    } else if (policyType == "bal") {
        return new BalancedSelection(0, 0, 0); // Example default scores
    } else if (policyType == "env") {
        return new SustainabilitySelection();
    } else if (policyType == "nve") {
        return new NaiveSelection();
    }
    return nullptr;
}

void Simulation::changePlanPolicy(int planID, const string &newPolicy) {
    auto &plan = getPlan(planID);
    SelectionPolicy *newPolicyObject = createPolicy(newPolicy);

    if (!newPolicyObject) {
        throw runtime_error("Unknown policy type: " + newPolicy);
    }

    string previousPolicyName = plan.getSelectionPolicyName();
    if (previousPolicyName == newPolicy) {
        throw runtime_error("Cannot change selection policy: new policy is the same as the current policy");
    }

    plan.setSelectionPolicy(newPolicyObject);
    std::cout << "planID: " << planID << "\npreviousPolicy: " << previousPolicyName 
              << "\nnewPolicy: " << newPolicy << std::endl;
}

void Simulation::getPlanStatus(int planID) {
    auto it = find_if(plans.begin(), plans.end(),
                      [planID](const Plan &plan) {
                          return plan.getPlanID() == planID;
                      });
    if (it == plans.end()) {
        throw runtime_error("Plan doesn't exist");
    }
    it->printStatus();
}

void Simulation::backup1() {
    if (backup != nullptr) {
        delete backup;
    }
    backup = new Simulation(*this); // Deep copy of the current simulation
}

void Simulation::restore() {
    if (backup == nullptr) {
        throw runtime_error("No backup available");
    }
    this = &backup; // Overwrite the current simulation with the backup
}

void Simulation::printActionsLog() const {
    for (const auto &action : actionsLog) {
        std::cout << action->toString() << std::endl;
    }
}

Simulation::~Simulation() {
    for (auto action : actionsLog) {
        delete action;
    }
    for (auto settlement : settlements) {
        delete settlement;
    }
}

// Copy Constructor
Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      plans(other.plans),
      facilitiesOptions(other.facilitiesOptions) {
    for (auto action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for (auto settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }
}

// Copy Assignment Operator
Simulation &Simulation::operator=(const Simulation &other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }

    // Cleanup existing resources
    for (auto action : actionsLog) {
        delete action;
    }
    for (auto settlement : settlements) {
        delete settlement;
    }
    
    // Copy data
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    plans = other.plans;
    facilitiesOptions = other.facilitiesOptions;

    // Deep copy actions
    actionsLog.clear();
    for (auto action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    // Deep copy settlements
    settlements.clear();
    for (auto settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    return *this;
}
Simulation::Simulation(Simulation&& other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      plans(std::move(other.plans)),
      facilitiesOptions(std::move(other.facilitiesOptions)), 
      actionsLog(std::move(other.actionsLog)),
      settlements(std::move(other.settlements)),{

    other.isRunning = false;
    other.planCounter = 0;
    other.plans.clear();
    other.facilitiesOptions.clear();
    for (auto action : other.actionsLog) {
        delete action;
    }
    for (auto settlement : other.settlements) {
        delete settlement;
    }
}

// Move assignment operator
Simulation& Simulation::operator=(Simulation&& other) {
    if (this == &other) {
        return *this;
    }

    // Clean up existing resources
    for (auto settlement : settlements) {
        delete settlement;
    }
    for (auto action : actionsLog) {
        delete action;
    }

    // Move data
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    plans = std::move(other.plans);
    facilitiesOptions = std::move(other.facilitiesOptions);
    actionsLog = std::move(other.actionsLog);
    settlements = std::move(other.settlements);

    // Reset the source object
    other.isRunning = false;
    other.planCounter = 0;
    other.plans.clear();
    other.facilitiesOptions.clear();
    for (auto action : other.actionsLog) {
        delete action;
    }
    for (auto settlement : other.settlements) {
        delete settlement;
    }

    return *this;
}
