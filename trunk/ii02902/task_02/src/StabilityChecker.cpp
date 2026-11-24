#include "StabilityChecker.h"

#include <cmath>

bool StabilityChecker::isModel1_2Stable(double a) {
    return std::isfinite(a) && (std::abs(a) < 1.0);
}

bool StabilityChecker::checkAndWarn(double a, std::ostream& out) {
    const bool stable = isModel1_2Stable(a);
    if (!stable) {
        out << "[ПРЕДУПРЕЖДЕНИЕ] Линейная модель 1.2 неустойчива при заданном коэффициенте "
            << "(a=" << a << ").\n";
    }
    return stable;
}