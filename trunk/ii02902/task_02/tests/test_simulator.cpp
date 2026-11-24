#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>

#include "Model1_2.h"
#include "Model3_6.h"
#include "Simulator.h"

using ::testing::Return;

class MockSignalSource : public ISignalSource {
public:
    MOCK_METHOD(double, sample, (SignalType type, int step), (const, override));
};

TEST(SimulatorMockTest, UsesMockedSignalSequence) {
    MockSignalSource mockSignal;
    Model1_2 model(0.5, 1.0, 0, 0.0);

    EXPECT_CALL(mockSignal, sample(SignalType::STEP, 0)).WillOnce(Return(2.0));
    EXPECT_CALL(mockSignal, sample(SignalType::STEP, 1)).WillOnce(Return(0.0));
    EXPECT_CALL(mockSignal, sample(SignalType::STEP, 2)).WillOnce(Return(1.0));

    const auto history = Simulator::run(model, mockSignal, SignalType::STEP, 3, 0.0);

    ASSERT_EQ(history.size(), 4u);
    EXPECT_DOUBLE_EQ(history[0], 0.0);
    EXPECT_DOUBLE_EQ(history[1], 2.0);
    EXPECT_DOUBLE_EQ(history[2], 1.0);
    EXPECT_DOUBLE_EQ(history[3], 1.5);
}

TEST(SimulatorTest, ThrowsOnNegativeSteps) {
    MockSignalSource mockSignal;
    Model3_6 model(0.1, 0.8, 0.1, 0.0);

    EXPECT_THROW(Simulator::run(model, mockSignal, SignalType::STEP, -1, 0.0),
                 std::invalid_argument);
}

TEST(SimulatorTest, RunsWithRealFunctionSignalSource) {
    FunctionSignalSource impulseSource(SignalType::IMPULSE);
    Model1_2 model(0.5, 1.0, 0, 0.0);

    const auto history = Simulator::run(model, impulseSource, SignalType::IMPULSE, 2, 0.0);
    ASSERT_EQ(history.size(), 3u);
    EXPECT_DOUBLE_EQ(history[0], 0.0);
    EXPECT_DOUBLE_EQ(history[1], 1.0);
    EXPECT_DOUBLE_EQ(history[2], 0.5);
}