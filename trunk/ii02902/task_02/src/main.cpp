#include <iomanip>
#include <iostream>

#include "InputSignal.h"
#include "Model1_2.h"
#include "Simulator.h"
#include "StabilityChecker.h"

int main() {
    constexpr double a = 0.7;
    constexpr double b = 0.3;
    constexpr int delay_k = 2;
    constexpr double y0 = 0.0;
    constexpr int steps = 10;
    constexpr SignalType sig = SignalType::STEP;

    StabilityChecker::checkAndWarn(a, std::cout);

    Model1_2 linearModel(a, b, delay_k, y0);
    FunctionSignalSource signalSource(sig);

    const auto trajectory = Simulator::run(linearModel, signalSource, sig, steps, y0);

    std::cout << "Сигнал: " << InputSignal::getSignalName(sig) << "\n";
    std::cout << std::fixed << std::setprecision(4);
    for (size_t i = 0; i < trajectory.size(); ++i) {
        std::cout << "t=" << i << " -> y=" << trajectory[i] << "\n";
    }

    return 0;
}