#pragma once

#include <ostream>

class StabilityChecker {
public:
    // Для линейной модели y[t] = a * y[t-1] + b * u[t-k] характеристическое уравнение z - a = 0,
    // корень на Z-плоскости z = a. Устойчивость выполняется при |a| < 1 и конечном параметре.
    static bool isModel1_2Stable(double a);

    // Проверяет устойчивость и при неустойчивости выводит предупреждение в поток.
    static bool checkAndWarn(double a, std::ostream& out);
};