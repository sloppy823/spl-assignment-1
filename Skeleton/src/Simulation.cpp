#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include <stdexcept>
#include <iostream>
#include <algorithm> // for std::find_if

using std::logic_error;
using std::runtime_error;
using std::find_if;

// Constructor
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0) {
    // TODO: Parse the configuration file and populate settlements, facilities, or other resources.
    // Placeholder: initializing with empty vectors.
}

// Starts the simulation
void Simulation::start() {
    if (isRunning) {
        throw runtime_error("Simulation is already running.");
    }

    if (plans.empty()) {
        throw runtime_error("Cannot start simulation. No reconstruction plans available.");
    }

    isRunning = true;
    while (isRunning) {
        step();
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
    plans.emplace_back(plans, *settlement, &selectionPolicy, facilitiesOptions);
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
