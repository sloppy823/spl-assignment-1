#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>

using std::logic_error;
using std::runtime_error;
using std::find_if;
using std::cin;
using std::istringstream;
using std::ifstream;
using std::getline;

// Constructor
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions() {
    ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw runtime_error("Failed to open configuration file: " + configFilePath);
    }

    string line;
    while (getline(configFile, line)) {
        istringstream stream(line);
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

// Starts the simulation
void Simulation::start() {
    open();
    while (isRunning) {
        string line;
        getline(cin, line);
        if (line.empty() || line[0] == '#') {
            continue; // Skip comments and empty lines
        }

        istringstream stream(line);
        string command;
        stream >> command;

        try {
            BaseAction *action = nullptr;

            if (command == "step") {
                int numOfSteps;
                stream >> numOfSteps;
                action = new SimulateStep(numOfSteps);
            } else if (command == "plan") {
                string settlementName, policyType;
                stream >> settlementName >> policyType;
                action = new AddPlan(settlementName, policyType);
            } else if (command == "planStatus") {
                int planId;
                stream >> planId;
                action = new PrintPlanStatus(planId);
            } else if (command == "actionsLog") {
                action = new PrintActionsLog();
            } else if (command == "changePolicy") {
                int planId;
                string newPolicyType;
                stream >> planId >> newPolicyType;
                action = new ChangePlanPolicy(planId, newPolicyType);
            } else {
                throw runtime_error("Unknown command: " + command);
            }

            if (action) {
                action->act(*this);
                actionsLog.push_back(action);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error processing command: " << line << "\n"
                      << e.what() << "\n";
        }
    }
}

// Remaining methods (unchanged)...

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
