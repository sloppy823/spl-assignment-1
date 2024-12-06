#include "Action.h"
#include "Simulation.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
using std::runtime_error;
extern Simulation* backup;
// BaseAction Implementation
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg = std::move(errorMsg);
    status = ActionStatus::ERROR;
    cout << "Error: " << this->errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

// SimulateStep Implementation
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; ++i) {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "step " + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

// AddPlan Implementation
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    try {
        Settlement *settlement = simulation.getSettlement(settlementName);
        if (!settlement) {
            throw std::runtime_error("Settlement not found: " + settlementName);
        }
        simulation.addPlan(settlement, simulation.createPolicy(selectionPolicy));
        complete();
    } catch (const runtime_error &e) {
        error("Cannot create this plan");
    }
}

const string AddPlan::toString() const {
    return "plan " + settlementName + " " + selectionPolicy;
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

// AddSettlement Implementation
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    try {
        auto settlement = new Settlement(settlementName, settlementType);
        if (!simulation.addSettlement(settlement)) {
            delete settlement; // Avoid memory leaks
        }
        complete();
    } catch (const runtime_error &e) {
        error("Settlement already exists");
    }
}

const string AddSettlement::toString() const {
    return "settlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType));
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

// AddFacility Implementation
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory,
                         const int price, const int lifeQualityScore, const int economyScore,
                         const int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price),
      lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    try {
        FacilityType facility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
        if (!simulation.addFacility(facility)) {
            throw std::runtime_error("Facility \"" + facilityName + "\" already exists.");
        }
        complete();
    } catch (const runtime_error &e) {
        error("Facility already exists");
    }
}

const string AddFacility::toString() const {
    return "facility " + facilityName + " " + std::to_string(static_cast<int>(facilityCategory)) + " " +
           std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " +
           std::to_string(economyScore) + " " + std::to_string(environmentScore);
}

AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}

// PrintPlanStatus Implementation
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    try {
        simulation.getPlanStatus(planId);
        complete();
    } catch (const runtime_error &e) {
        error("Plan doesn't exist");
    }
}

const string PrintPlanStatus::toString() const {
    return "planStatus " + std::to_string(planId);
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

// ChangePlanPolicy Implementation
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    try {
        simulation.changePlanPolicy(planId, newPolicy);
        complete();
    } catch (const runtime_error &e) {
        error("Cannot change selection policy");
    }
}

const string ChangePlanPolicy::toString() const {
    return "changePolicy " + std::to_string(planId) + " " + newPolicy;
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

// PrintActionsLog Implementation
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog();
    complete();
}

const string PrintActionsLog::toString() const {
    return "log";
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

// Close Implementation
Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

const string Close::toString() const {
    return "close";
}

Close *Close::clone() const {
    return new Close(*this);
}

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    backup = new Simulation(simulation);
    complete();
}

const string BackupSimulation::toString() const {
    return "backup COMPLETED";
}

BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this); 
}

// RestoreSimulation
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if (backup == nullptr) {
        error("No backup available");
        return;
    }
    simulation = *backup;
    complete();
}

RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this); 
}

const string RestoreSimulation::toString() const {
    return "restore COMPLETED";
}
