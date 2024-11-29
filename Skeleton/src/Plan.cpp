#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>
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


    void Plan::step() {

        //construction limit
        SettlementType type =settlement->getType();
        int limit = 0;
        if(type == SettlementType::VILLAGE)
            limit = 1;
        else {
            if(type == SettlementType::CITY)
                limit = 2;
            else
                limit=3;
        }
        
        // step 1:Check PlanStatus
        if (status == PlanStatus::AVAILABLE) {

            //step 2
            while (underConstruction.size() < limit) {
                try {
                    const FacilityType& selected = selectionPolicy->selectFacility(facilityOptions);
                    // Create and add new Facility to the underConstruction list
                    Facility* newFacility = new Facility(selected, settlement->getName());
                    underConstruction.push_back(newFacility);
                    } 
                catch (std::exception& e) {
                    // No more facilities can be selected, exit the loop
                    break;
                    }
            }
        }

        // Step 3: Update facilities under construction
        for (auto it = underConstruction.begin(); it != underConstruction.end();) {
            (*it)->step(); 
            //move to facilities
            if ((*it)->getTimeLeft() == 0) {
                facilities.push_back(*it);
                it = underConstruction.erase(it); 
            } else {
                ++it;
            }
        }

        //step 4: update status
        if (underConstruction.size() == limit) {
            status = PlanStatus::BUSY;
        } else {
            status = PlanStatus::AVAILABLE;
        }
    }



    void printStatus(){
        std::cout << "PlanID: " << plan_id << std::endl;
        std::cout << "SettlementName: " << settlement->getName() << std::endl;
        std::string planStatusStr = (status == PlanStatus::AVAILABLE) ? "AVAILABLE" : "BUSY";
        std::cout << "PlanStatus: " << planStatusStr << std::endl;    
    }

    // Accessor for the facilities managed by this plan (operational and under construction)
    const vector<Facility*> &getFacilities() const{
        return this->facilities;
    }

    // Add a facility to the plan (either operational or under construction)
    void addFacility(Facility* facility){
    }

    // Convert the plan to a string (for display or logging purposes)
    const string toString() const{}

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
