#include "SelectionPolicy.h"
#include <algorithm> 


NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size(); 
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive Selection Policy";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this); // Use default copy constructor
}

// BalancedSelection Implementation
BalancedSelection::BalancedSelection(int lifeQualityScore, int economyScore, int environmentScore)
    : LifeQualityScore(lifeQualityScore), EconomyScore(economyScore), EnvironmentScore(environmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    return *std::max_element(facilitiesOptions.begin(), facilitiesOptions.end(), [this](const FacilityType& a, const FacilityType& b) {
        int aScore = a.getLifeQualityScore() * LifeQualityScore +
                     a.getEconomyScore() * EconomyScore +
                     a.getEnvironmentScore() * EnvironmentScore;
        int bScore = b.getLifeQualityScore() * LifeQualityScore +
                     b.getEconomyScore() * EconomyScore +
                     b.getEnvironmentScore() * EnvironmentScore;
        return aScore < bScore; // Compare based on weighted scores
    });
}


BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this); // Use default copy constructor
}

// EconomySelection Implementation
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Select the cheapest facility
    return *std::min_element(facilitiesOptions.begin(), facilitiesOptions.end(), [](const FacilityType& a, const FacilityType& b) {
        return a.getEconomyScore() < b.getEconomyScore();
    });
}

const string EconomySelection::toString() const {
    return "Economy Selection Policy";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this); // Use default copy constructor
}

// SustainabilitySelection Implementation
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Select the facility with the highest environment score
    return *std::max_element(facilitiesOptions.begin(), facilitiesOptions.end(), [](const FacilityType& a, const FacilityType& b) {
        return a.getEnvironmentScore() < b.getEnvironmentScore();
    });
}

const string SustainabilitySelection::toString() const {
    return "Sustainability Selection Policy";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); // Use default copy constructor
}
