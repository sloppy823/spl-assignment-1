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
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getLifeQualityScore() const;
        const int getEconomyScore() const;
        const string getSelectionPolicyName();

        const int getEnvironmentScore() const;
        PlanStatus getStatus() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const int getPlanID() const;
        ~Plan();                               // Destructor
        Plan(const Plan &other);               // Copy Constructor
        Plan(const Plan &&other);  
        Plan &operator=(const Plan &other) = delete;    // Copy Assignment Operator
        Plan &operator=(const Plan &other) = delete;    // Copy Assignment Operator

        
    private:
        int plan_id;
        const Settlement *settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};