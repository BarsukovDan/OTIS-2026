#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <limits>
#include <sstream>

#include "StabilityChecker.h"

using ::testing::HasSubstr;

TEST(StabilityCheckerTest, EvaluatesStabilityByPoleMagnitude) {
    EXPECT_TRUE(StabilityChecker::isModel1_2Stable(0.7));
    EXPECT_TRUE(StabilityChecker::isModel1_2Stable(-0.85));
    EXPECT_TRUE(StabilityChecker::isModel1_2Stable(0.0));

    EXPECT_FALSE(StabilityChecker::isModel1_2Stable(1.0));
    EXPECT_FALSE(StabilityChecker::isModel1_2Stable(-1.0));
    EXPECT_FALSE(StabilityChecker::isModel1_2Stable(1.2));
    EXPECT_FALSE(StabilityChecker::isModel1_2Stable(-1.5));
}

TEST(StabilityCheckerTest, RejectsNonFiniteValues) {
    constexpr double inf = std::numeric_limits<double>::infinity();
    constexpr double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_FALSE(StabilityChecker::isModel1_2Stable(inf));
    EXPECT_FALSE(StabilityChecker::isModel1_2Stable(nan));
}

TEST(StabilityCheckerTest, WarnsOnlyWhenUnstable) {
    std::ostringstream okOut;
    EXPECT_TRUE(StabilityChecker::checkAndWarn(0.5, okOut));
    EXPECT_TRUE(okOut.str().empty());

    std::ostringstream warnOut;
    EXPECT_FALSE(StabilityChecker::checkAndWarn(1.3, warnOut));
    EXPECT_THAT(warnOut.str(), HasSubstr("неустойчива"));
}