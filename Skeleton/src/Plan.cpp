#include "Plan.h"
#include <stdexcept>
#include <algorithm>
#include <iostream> 
#include <string>
#include <sstream>
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVAILABLE), 
        facilities(), underConstruction(),
        facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) {}
const int Plan::getLifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}
const int Plan::getPlanID() const{
    return plan_id;
}
const int Plan::getEnvironmentScore() const {
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy) {
    if (selectionPolicy != newSelectionPolicy) {
        selectionPolicy = newSelectionPolicy;
    }
}

const string Plan::getSelectionPolicyName() {
    if (selectionPolicy) {
        return selectionPolicy->getType();
    }
    return "No Selection Policy";
}


void Plan::step() {
    int limit = settlement -> getsize();
    // Step 2: Start new facility construction
    if (status == PlanStatus::AVAILABLE) {
        while ((int)underConstruction.size() < (limit)) {
            
            const FacilityType &selected = selectionPolicy->selectFacility(facilityOptions);
            if (selectionPolicy->getType() == "bal"){
                selectionPolicy->update(selected);
            }
            Facility *newFacility = new Facility(selected, settlement->getName());
            underConstruction.push_back(newFacility);
        }
    }
    // Step 3: Update facilities under construction
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        (*it)->step();
        if ((*it)->getTimeLeft() == 0) {
            facilities.push_back(*it);

            // Update plan scores
            life_quality_score += (*it)->getLifeQualityScore();
            economy_score += (*it)->getEnvironmentScore();
            environment_score += (*it)->getEconomyScore();

            it = underConstruction.erase(it);
        } else {
            ++it;
        }
    }

    // Step 4: Update plan status
    status = ((int)underConstruction.size() == limit) ? PlanStatus::BUSY : PlanStatus::AVAILABLE;
}

void Plan::addFacility(Facility *facility) {
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
        underConstruction.push_back(facility);
    } else if (facility->getStatus() == FacilityStatus::OPERATIONAL) {
        facilities.push_back(facility);
    }
}

const string Plan::toString() const {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id << "\n";
    oss << "SettlementName: " << settlement->getName() << "\n";
    oss << "PlanStatus: " << ((status == PlanStatus::AVAILABLE) ? "AVAILABLE" : "BUSY") << "\n";
    oss << "SelectionPolicy: " << selectionPolicy->getType() << "";
    return oss.str();
}

void Plan::printStatus() {
    std::cout << toString() << std::endl;
    std::cout << "LifeQualityScore: " << life_quality_score << std::endl;
    std::cout << "EconomyScore: " << economy_score << std::endl;
    std::cout << "EnvironmentScore: " << environment_score << std::endl;
    for (const auto &facility : underConstruction) {
        std::cout << "FacilityName: " << facility->getName() << std::endl;
        std::cout << "FacilityStatus: " << ((facility->getStatus() == FacilityStatus::OPERATIONAL) ? "OPERATIONAL" : "UNDER_CONSTRUCTION") << std::endl;
    }
    for (const auto &facility : facilities) {
        std::cout << "FacilityName: " << facility->getName() << std::endl;
        std::cout << "FacilityStatus: " << ((facility->getStatus() == FacilityStatus::OPERATIONAL) ? "OPERATIONAL" : "UNDER_CONSTRUCTION") << std::endl;
    }
    
}

Plan::~Plan() {
    delete selectionPolicy;
    for (auto facility : facilities) {
        delete facility;
    }
    for (auto facility : underConstruction) {
        delete facility;
    }
}

// Copy Constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(new Settlement(*other.settlement)), 
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    // Deep copy facilities
    for (const auto facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    // Deep copy underConstruction facilities
    for (const auto facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

Plan& Plan::operator=(const Plan& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }

    // Assign other members
    plan_id = other.plan_id;
    status = other.status;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;

    // Deep copy settlement
    delete settlement;
    settlement = new Settlement(*other.settlement);

    // Deep copy selectionPolicy
    delete selectionPolicy;
    selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;

    // Deep copy facilities and underConstruction
    for (auto* facility : facilities) {
        delete facility;
    }
    facilities.clear();
    for (auto* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();
    for (auto* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (auto* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }

    // Do not copy facilityOptions because it is const
    return *this;
}

