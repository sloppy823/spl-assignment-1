#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>

using std::logic_error;
using std::runtime_error;
using std::find_if;
using std::cin;
using std::istringstream;
using std::getline;

// Constructor
Simulation::Simulation(const string &configFilePath) : isRunning(false),planCounter(0),actionsLog(),plans(),settlements(),facilitiesOptions() {}

// Starts the simulation
void Simulation::start() {
    open();
    while (isRunning) {
        string line;
        getline(cin, line);
        istringstream stream(line);
        string command;
        stream >> command;
        if (line.empty() || line[0] == '#') {
            continue;
        }

        try {
            if (command == "step") {
                step();
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
                if (facilityCategory == FacilityCategory::LIFE_QUALITY && category != "life_quality") {
                    throw runtime_error("Invalid facility category: " + category);
                }

                FacilityType facility(facilityName, facilityCategory, price, lifeQImpact, ecoImpact, envImpact);
                if (!addFacility(facility)) {
                    std::cerr << "Facility \"" << facilityName << "\" already exists.\n";
                }
            } else if ((command == "planStatus")) {
                int planId;
                stream >> planId;
                auto it = find_if(plans.begin(), plans.end(),[planId](const Plan &plan) { return plan.getPlanID() == planId; });
                (*it).printStatus();
            } else {
                throw runtime_error("Unknown command: " + command);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error processing command: " << line << "\n"
                      << e.what() << "\n";
        }
    }
}

// Helper function to create a selection policy
SelectionPolicy *Simulation::createPolicy(const string &policyType) {
    if (policyType == "nve") {
        return new NaiveSelection();
    } else
    if (policyType == "eco") {
        return new EconomySelection();
    } else if (policyType == "bal") {
        return new BalancedSelection(0, 0, 0); // Default scores
    } else if (policyType == "env") {
        return new SustainabilitySelection();
    } else {
        return nullptr;
    }
}

// Helper function to parse facility category
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

// Adds a new plan
void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy) {
    if (!settlement) {
        throw logic_error("Invalid settlement provided for adding a plan.");
    }

    if (!selectionPolicy) {
        throw logic_error("Invalid selection policy provided for adding a plan.");
    }

    plans.emplace_back(planCounter++, *settlement, selectionPolicy, facilitiesOptions);
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

    settlements.push_back(settlement);
    return true;
}

// Adds a new facility type
bool Simulation::addFacility(FacilityType facility) {
    for (const auto &existingFacility : facilitiesOptions) {
        if (existingFacility.getName() == facility.getName()) {
            return false;
        }
    }

    facilitiesOptions.push_back(facility);
    return true;
}

// Checks if a settlement exists
bool Simulation::isSettlementExists(const string &settlementName) {
    return find_if(settlements.begin(), settlements.end(),
                   [&settlementName](const Settlement *settlement) {
                       return settlement->getName() == settlementName;
                   }) != settlements.end();
}

// Retrieves a settlement
Settlement *Simulation::getSettlement(const string &settlementName) {
    auto it = find_if(settlements.begin(), settlements.end(),
                      [&settlementName](const Settlement *settlement) {
                          return settlement->getName() == settlementName;
                      });
    return (it != settlements.end()) ? *it : nullptr;
}

// Executes one step
void Simulation::step() {
    if (!isRunning) {
        throw runtime_error("Cannot execute step. Simulation is not running.");
    }

    for (auto &plan : plans) {
        plan.step();
    }
}

// Starts the simulation
void Simulation::open() {
    if (isRunning) {
        throw runtime_error("Simulation is already running.");
    }
    isRunning = true;
}

// Stops the simulation
void Simulation::close() {
    if (!isRunning) {
        throw runtime_error("Simulation is not running.");
    }
    isRunning = false;
}
std::vector<Plan>& Simulation::getPlans() {
    return plans;
}
