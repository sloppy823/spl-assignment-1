#include "Plan.h"
#include <stdexcept>
#include <algorithm>
#include <iostream> 
#include <string>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVAILABLE),
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
    SettlementType type =settlement->getType();
    int limit = 0;
    if(type == SettlementType::VILLAGE)
        limit = 1;
    else {
        if(type == SettlementType::CITY)
            limit = 2;
        else
            limit = 3;
        }
    // Step 2: Start new facility construction
    if (status == PlanStatus::AVAILABLE) {
        while (underConstruction.size() < static_cast<size_t>(limit)) {
            try {
                const FacilityType &selected = selectionPolicy->selectFacility(facilityOptions);
                Facility *newFacility = new Facility(selected, settlement->getName());
                underConstruction.push_back(newFacility);
            } catch (std::exception &e) {
                // No more facilities can be selected
                break;
            }
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
    status = (underConstruction.size() == limit) ? PlanStatus::BUSY : PlanStatus::AVAILABLE;
    status = (underConstruction.size() == static_cast<size_t>(limit)) ? PlanStatus::BUSY : PlanStatus::AVAILABLE;
}

void Plan::addFacility(Facility *facility) {
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
        underConstruction.push_back(facility);
    } else if (facility->getStatus() == FacilityStatus::OPERATIONAL) {
        facilities.push_back(facility);
    }
}

const string Plan::toString() const {
    return "PlanID: " + std::to_string(plan_id) + ", Settlement: " + settlement->getName() + 
           ", Status: " + ((status == PlanStatus::AVAILABLE) ? "AVAILABLE" : "BUSY");
}

void Plan::printStatus() {
    std::cout << toString() << std::endl;
    std::cout << "Life Quality Score: " << life_quality_score << std::endl;
    std::cout << "Economy Score: " << economy_score << std::endl;
    std::cout << "Environment Score: " << environment_score << std::endl;
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
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),
      facilityOptions(other.facilityOptions),
      status(other.status),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    for (auto facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (auto facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

// Copy Assignment Operator
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
