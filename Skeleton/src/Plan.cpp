#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVAILABLE,  
    BUSY,      
};

class Plan {
    Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVAILABLE),
      facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) {}

    const int Plan::getLifeQualityScore() const {
        return life_quality_score;
    }

    const int Plan::getEconomyScore() const {
        return economy_score;
    }

    const int Plan::getEnvironmentScore() const {
        return environment_score;
    }

    void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
        this->selectionPolicy = selectionPolicy;
    }


    // Execute one step of the plan (e.g., construct a new facility or progress construction)
    void Plan:: step();



    // Print the status of the plan
    void printStatus();

    // Accessor for the facilities managed by this plan (operational and under construction)
    const vector<Facility*> &getFacilities() const;

    // Add a facility to the plan (either operational or under construction)
    void addFacility(Facility* facility);

    // Convert the plan to a string (for display or logging purposes)
    const string toString() const;

private:
    int plan_id;  // Unique ID for this plan
    const Settlement *settlement;  // Settlement associated with this plan
    SelectionPolicy *selectionPolicy;  // Selection strategy for the facilities
    PlanStatus status;  // Current status of the plan (AVAILABLE or BUSY)
    vector<Facility*> facilities;  // List of all facilities (both operational and under construction)
    vector<Facility*> underConstruction;  // List of facilities under construction
    const vector<FacilityType> &facilityOptions;  // Available facility options
    int life_quality_score, economy_score, environment_score;  // Plan's score attributes
};
