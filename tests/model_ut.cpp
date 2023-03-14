#include "..//inc/model.hpp"
#include "scanner_mock.hpp"
#include "synchronizer_mock.hpp"
#include "timer_mock.hpp"

#include <chrono>
#include <istream>
#include <thread>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using namespace std::chrono_literals;

void wait_until(int ms, std::function<bool()> &&condition)
{
    while (ms >= 0)
    {
        if (condition())
        {
            return;
        }
        std::this_thread::sleep_for(1ms);
        ms -= 1;
    }
    // LOG("wait_until failed");
    ASSERT_TRUE(false);
}

// The fixture for testing class Foo.
class ModelTest : public ::testing::Test
{
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    ModelTest()
    {
        // You can do set-up work for each test here.
    }

    ~ModelTest() override
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

TEST_F(ModelTest, ModelReqestStartSync)
{
    auto timer = std::make_unique<MockTimer>();
    auto synchronizer = std::make_unique<MockSynchronizer>();
    auto scanner = std::make_unique<MockScanner>();
    std::unique_ptr<i_Serializer> serializer = nullptr;

    auto timer_ref = timer.get();
    auto synchronizer_ref = synchronizer.get();
    auto scanner_ref = scanner.get();

    Model m(std::move(timer), std::move(synchronizer), std::move(scanner), std::move(serializer));

    Callback cb;
    EXPECT_CALL(*timer_ref, start).WillOnce([&cb](const Interval &interval, const Callback &timeoutCallback) {
        EXPECT_EQ(interval, 1000ms);
        cb = timeoutCallback; });

    m.startSync();

    EXPECT_CALL(*scanner_ref, scan);
    EXPECT_CALL(*scanner_ref, scanForChangedDirs).Times(2);
    EXPECT_CALL(*scanner_ref, comparePreviousAndRecentScanning);

    EXPECT_CALL(*synchronizer_ref, synchronizeAdded);
    EXPECT_CALL(*synchronizer_ref, synchronizeRemoved);

    cb();
}

// TEST_F(ModelTest, ModelChangeInterval)
// {
//     MockTimer timer;
//     MockSynchronizer synchronizer;
//     MockScanner scanner;

//     Model m(&timer, &synchronizer, &scanner);

//     Callback cb;
//     EXPECT_CALL(timer, start).WillOnce([&cb](const Interval &interval, const Callback &timeoutCallback) {
//         EXPECT_EQ(interval, 2000ms);
//         cb = timeoutCallback; });

//     std::istringstream is("2000");
//     m.setIntervalTime(is);
//     m.startSync();
// }