#include "SelectionPolicy.h"
#include <algorithm> // for std::min_element and std::max_element
#include <stdexcept> // for std::logic_error
#include <sstream>   // for std::ostringstream

// NaiveSelection Implementation
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size(); // Round-robin selection
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive Selection Policy";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this); // Copy the object
}

const string NaiveSelection::getType() const {
    return "nve";
}

// BalancedSelection Implementation
BalancedSelection::BalancedSelection(int lifeQualityScore, int economyScore, int environmentScore)
    : LifeQualityScore(lifeQualityScore), EconomyScore(economyScore), EnvironmentScore(environmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Select facility with the smallest balance difference
    int minBalanceDifference = balanceDifference(facilitiesOptions[0]);
    int minBalanceIndex = 0;
    for (size_t i = 1; i < facilitiesOptions.size(); ++i) {
        int currentBalanceDifference = balanceDifference(facilitiesOptions[i]);
        if (currentBalanceDifference < minBalanceDifference) {
            minBalanceDifference = currentBalanceDifference;
            minBalanceIndex = i;
        }
    }
    return facilitiesOptions[minBalanceIndex];
}

int BalancedSelection::balanceDifference(const FacilityType& facility) const {
    int updatedScores[] = {
        LifeQualityScore + facility.getLifeQualityScore(),
        EconomyScore + facility.getEconomyScore(),
        EnvironmentScore + facility.getEnvironmentScore()
    };
    return *std::max_element(updatedScores, updatedScores + 3) - *std::min_element(updatedScores, updatedScores + 3);
}

const string BalancedSelection::toString() const {
    std::ostringstream oss;
    oss << "Balanced Selection Policy (Current Scores: "
        << "Life Quality: " << LifeQualityScore << ", "
        << "Economy: " << EconomyScore << ", "
        << "Environment: " << EnvironmentScore << ")";
    return oss.str();
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this); // Copy the object
}

const string BalancedSelection::getType() const {
    return "bal";
}
void BalancedSelection::update(const FacilityType& selected) {
    LifeQualityScore += selected.getLifeQualityScore();
    EconomyScore += selected.getEconomyScore();
    EnvironmentScore += selected.getEnvironmentScore();
}

// EconomySelection Implementation
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Filter economy facilities
    vector<const FacilityType*> economyFacilities;
    for (const auto& facility : facilitiesOptions) {
        if (facility.getCategory() == FacilityCategory::ECONOMY) {
            economyFacilities.push_back(&facility);
        }
    }

    if (economyFacilities.empty()) {
        throw std::logic_error("No facilities in the ECONOMY category are available.");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % economyFacilities.size(); // Round-robin selection
    return *economyFacilities[lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "Economy Selection Policy";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this); // Copy the object
}

const string EconomySelection::getType() const {
    return "eco";
}

// SustainabilitySelection Implementation
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Filter environment facilities
    vector<const FacilityType*> environmentFacilities;
    for (const auto& facility : facilitiesOptions) {
        if (facility.getCategory() == FacilityCategory::ENVIRONMENT) {
            environmentFacilities.push_back(&facility);
        }
    }

    if (environmentFacilities.empty()) {
        throw std::logic_error("No facilities in the ENVIRONMENT category are available.");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % environmentFacilities.size(); // Round-robin selection
    return *environmentFacilities[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "Sustainability Selection Policy";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); // Copy the object
}

const string SustainabilitySelection::getType() const {
    return "env";
}

