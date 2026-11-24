#pragma once

#include <vector>

#include "InputSignal.h"
#include "Model.h"

class ISignalSource {
public:
    virtual ~ISignalSource() = default;
    virtual double sample(SignalType type, int step) const = 0;
};

class FunctionSignalSource final : public ISignalSource {
private:
    SignalType signalType;

public:
    explicit FunctionSignalSource(SignalType type);
    double sample(SignalType type, int step) const override;
};

class Simulator {
public:
    static std::vector<double> run(Model& model,
                                   const ISignalSource& signalSource,
                                   SignalType signalType,
                                   int steps,
                                   double initialValue = 0.0);
};