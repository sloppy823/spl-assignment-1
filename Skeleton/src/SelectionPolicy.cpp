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

// BalancedSelection Implementation
BalancedSelection::BalancedSelection(int lifeQualityScore, int economyScore, int environmentScore)
    : LifeQualityScore(lifeQualityScore), EconomyScore(economyScore), EnvironmentScore(environmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Select the facility minimizing the difference between max and min scores after construction
    auto comparator = [this](const FacilityType& a, const FacilityType& b) {
        int aScores[] = {LifeQualityScore + a.getLifeQualityScore(),
                         EconomyScore + a.getEconomyScore(),
                         EnvironmentScore + a.getEnvironmentScore()};
        int bScores[] = {LifeQualityScore + b.getLifeQualityScore(),
                         EconomyScore + b.getEconomyScore(),
                         EnvironmentScore + b.getEnvironmentScore()};

        int aBalance = *std::max_element(aScores, aScores + 3) - *std::min_element(aScores, aScores + 3);
        int bBalance = *std::max_element(bScores, bScores + 3) - *std::min_element(bScores, bScores + 3);

        return aBalance < bBalance; // Select the facility with the smaller balance difference
    };

    return *std::min_element(facilitiesOptions.begin(), facilitiesOptions.end(), comparator);
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

// EconomySelection Implementation
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Filter facilities by economy category and use round-robin selection
    vector<const FacilityType*> economyFacilities;
    for (const auto& facility : facilitiesOptions) {
        if (facility.getCategory() == FacilityCategory::ECONOMY) {
            economyFacilities.push_back(&facility);
        }
    }

    if (economyFacilities.empty()) {
        throw std::logic_error("No facilities in the economy category available.");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % economyFacilities.size();
    return *economyFacilities[lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "Economy Selection Policy";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this); // Copy the object
}

// SustainabilitySelection Implementation
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::logic_error("No facilities available for selection.");
    }

    // Filter facilities by environment category and use round-robin selection
    vector<const FacilityType*> environmentFacilities;
    for (const auto& facility : facilitiesOptions) {
        if (facility.getCategory() == FacilityCategory::ENVIRONMENT) {
            environmentFacilities.push_back(&facility);
        }
    }

    if (environmentFacilities.empty()) {
        throw std::logic_error("No facilities in the environment category available.");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % environmentFacilities.size();
    return *environmentFacilities[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "Sustainability Selection Policy";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); // Copy the object
}
