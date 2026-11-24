#include "Simulator.h"

#include <stdexcept>

FunctionSignalSource::FunctionSignalSource(SignalType type)
    : signalType(type) {
}

double FunctionSignalSource::sample(SignalType type, int step) const {
    return InputSignal::getSignal(signalType, step);
}

std::vector<double> Simulator::run(Model& model,
                                   const ISignalSource& signalSource,
                                   SignalType signalType,
                                   int steps,
                                   double initialValue) {
    if (steps < 0) {
        throw std::invalid_argument("Количество шагов не может быть отрицательным");
    }

    model.reset(initialValue);

    std::vector<double> history;
    history.reserve(static_cast<size_t>(steps) + 1);
    history.push_back(model.getY());

    for (int t = 0; t < steps; ++t) {
        const double u = signalSource.sample(signalType, t);
        history.push_back(model.step(u));
    }

    return history;
}