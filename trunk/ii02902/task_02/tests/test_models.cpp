#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cmath>
#include <initializer_list>

#include "../../task_01/src/InputSignal.h"
#include "../../task_01/src/Model1_2.h"
#include "../../task_01/src/Model2_4.h"
#include "../src/Model3_6.h"

using ::testing::HasSubstr;

namespace {

double runSequence(Model& model, std::initializer_list<double> inputs) {
    double last = model.getY();
    for (double u : inputs) {
        last = model.step(u);
    }
    return last;
}

}  // namespace

struct Model1_2Case {
    double a;
    double b;
    int delay_k;
    double y0;
    double u1;
    double u2;
    double u3;
    double expected_y3;
};

class Model1_2ParamTest : public ::testing::TestWithParam<Model1_2Case> {};

TEST_P(Model1_2ParamTest, ComputesDelayedStepsAccurately) {
    const Model1_2Case& p = GetParam();
    Model1_2 model(p.a, p.b, p.delay_k, p.y0);
    EXPECT_NEAR(runSequence(model, {p.u1, p.u2, p.u3}), p.expected_y3, 1e-12);
}

INSTANTIATE_TEST_SUITE_P(
    DelayVariants,
    Model1_2ParamTest,
    ::testing::Values(
        Model1_2Case{0.5, 0.4, 0, 1.0, 2.0, 1.0, 3.0, 1.725},
        Model1_2Case{0.5, 0.4, 1, 0.0, 2.0, 1.0, 3.0, 0.8},
        Model1_2Case{0.6, 0.5, 2, 0.0, 5.0, 5.0, 5.0, 2.5},
        Model1_2Case{0.0, 1.5, -2, 0.0, 2.0, 4.0, 6.0, 9.0}
    )
);

TEST(Model2_4Test, ZeroStateAndZeroInputGivesZero) {
    Model2_4 model(0.5, 0.3, 0.0);
    EXPECT_DOUBLE_EQ(model.step(0.0), 0.0);
}

TEST(Model2_4Test, MatchesAnalyticalFormula) {
    constexpr double a = 0.5;
    constexpr double b = 0.4;
    constexpr double y0 = 0.2;
    constexpr double u = -1.5;

    Model2_4 model(a, b, y0);
    const double expected = a * y0 * std::cos(y0) + b * std::log(1.0 + std::abs(u));
    EXPECT_NEAR(model.step(u), expected, 1e-12);
}

TEST(Model3_6Test, EulerStepMatchesAnalyticalFormula) {
    constexpr double a = 0.2;
    constexpr double b = 0.8;
    constexpr double dt = 0.1;
    constexpr double y0 = 1.0;
    constexpr double u = 2.0;

    Model3_6 model(a, b, dt, y0);
    const double expected = y0 + (a * (y0 * y0 * y0) + b * u) * dt;
    EXPECT_NEAR(model.step(u), expected, 1e-12);
}

TEST(Model3_6Test, ZeroDtPreservesInitialState) {
    Model3_6 model(1.5, 2.5, 0.0, 3.25);
    EXPECT_DOUBLE_EQ(model.step(5.0), 3.25);
}

TEST(InputSignalTest, StepAndImpulseAndHarmonicBehavior) {
    EXPECT_DOUBLE_EQ(InputSignal::getSignal(SignalType::STEP, 0), 1.0);
    EXPECT_DOUBLE_EQ(InputSignal::getSignal(SignalType::STEP, 10), 1.0);
    EXPECT_THAT(InputSignal::getSignalName(SignalType::STEP), HasSubstr("Step"));

    EXPECT_DOUBLE_EQ(InputSignal::getSignal(SignalType::IMPULSE, 0), 1.0);
    EXPECT_DOUBLE_EQ(InputSignal::getSignal(SignalType::IMPULSE, 1), 0.0);
    EXPECT_THAT(InputSignal::getSignalName(SignalType::IMPULSE), HasSubstr("Impulse"));

    EXPECT_THAT(InputSignal::getSignalName(SignalType::HARMONIC), HasSubstr("Harmonic"));
    for (int t = 0; t <= 20; ++t) {
        EXPECT_LE(std::abs(InputSignal::getSignal(SignalType::HARMONIC, t)), 1.0 + 1e-12);
    }
}

TEST(InputSignalTest, UnknownSignalTypeReturnsFallback) {
    const auto unknown_type = static_cast<SignalType>(99);
    EXPECT_DOUBLE_EQ(InputSignal::getSignal(unknown_type, 0), 0.0);
    EXPECT_THAT(InputSignal::getSignalName(unknown_type), HasSubstr("Unknown"));
}