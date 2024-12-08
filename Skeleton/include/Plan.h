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
                
        // THIS IS CONSTRUCTOR THE ONLY SOLUTION WE FOUND TO FIX THE PROBLEM OF SETTLEMENT BEING REFERRENCE
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions,
            int life_quality_score, int economy_score, int environment_score, PlanStatus plan_status, vector<Facility*> _facilities,
            vector<Facility*> _underConstruction);
        const string getSelectionPolicyName();
        
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        void closePrintStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const int getPlanID() const;
        ~Plan();                               // Destructor
        Plan(const Plan &other);               // Copy Constructor
        Plan(Plan &&other);               // Move contructor
        Plan &operator=(const Plan &other); // Copy Assignment Operator
        Plan &operator=(Plan &&other) = delete;    // move Assignment Operator

        
    //ADDED THIS GETTERS TO FIX PLAN &REF PROBLEM
    // all of these getters were added to cope with the settlement being const ref field in Plan class.
        const vector<FacilityType>& getFacilitiesOptions() const;
        const vector<Facility*> &getFacilitiesUnderConstruction() const;
        const PlanStatus getStatus() const;
        const int getPlanId() const;
        const SelectionPolicy* getSelectionPolicy();
        const SelectionPolicy* GetSelectionPolicy() const;
        const Settlement& getSettlement() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const int getlifeQualityScore() const;



    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};