#include "Action.h"
#include "Simulation.h"

class SimulateStep : public BaseAction {
public:
    SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

    void act(Simulation &simulation) override {
        for (int i = 0; i < numOfSteps; i++) {
            simulation.step();
        }
    }

    const std::string toString() const override {
        return "SimulateStep: " + std::to_string(numOfSteps) + " steps";
    }

    SimulateStep *clone() const override {
        return new SimulateStep(*this);
    }

private:
    const int numOfSteps;
};

class AddPlan : public BaseAction {
    public:
        AddPlan(const string &settlementName, const string &selectionPolicy): settlementName(settlementName), selectionPolicy(selectionPolicy) {}
        void act(Simulation &simulation){
            Settlement *settlement = simulation.getSettlement(settlementName);
            SelectionPolicy *policy = simulation.Simulation::createPolicy(selectionPolicy);
            simulation.addPlan(settlement, policy);
            complete();
        }
    private:
        const string settlementName;
        const string selectionPolicy;
};