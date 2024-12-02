#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

const string& Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

int Settlement::getConstructionLimit() const {
    switch (type) {
        case SettlementType::VILLAGE:
            return 1;
        case SettlementType::CITY:
            return 2;
        case SettlementType::METROPOLIS:
            return 3;
        default:
            throw std::logic_error("Unknown SettlementType.");
    }
}

Settlement::Settlement(const Settlement &other)
    : name(other.name), type(other.type) {}

Settlement::~Settlement() = default;