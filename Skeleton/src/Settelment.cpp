#include "Settlement.h"
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {

    }
const string& Settlement::getName() const {
    return name;
}
SettlementType Settlement::getType() const {
    return type;
}
Settlement::Settlement(const Settlement &other)
    : name(other.name), type(other.type) {}   
Settlement::~Settlement() = default;



