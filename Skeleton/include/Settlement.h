#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        Settlement(const string &name, int type);
        const string &getName() const;
        SettlementType getType() const;
        const int getsize() const;
        const string toString() const;
        private:
            const string name;
            SettlementType type;
};