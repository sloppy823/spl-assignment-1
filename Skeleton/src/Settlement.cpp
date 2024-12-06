#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

const string& Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const int Settlement::getsize() const{
    return (int)(type) +1;
}

