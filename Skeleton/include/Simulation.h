#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement *getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        std::vector<Plan> &getPlans();
        void open();
        FacilityCategory parseFacilityCategory(const string &category);
        SelectionPolicy *createPolicy(const string &policyType);
        void changePlanPolicy(int planID, const string &newPolicy);
        void getPlanStatus(int planID);
        void backup1();
        void restore();
        void printActionsLog() const;
        ~Simulation();                         // Destructor
        Simulation(const Simulation &other);   // Copy Constructor
        Simulation &operator=(const Simulation &other); // Copy Assignment Operator
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};