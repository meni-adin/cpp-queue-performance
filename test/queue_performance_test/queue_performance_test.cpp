
#include <gmock/gmock.h>

#include "mdn/Queue.hpp"
#include "mdn/QueueList.hpp"

using namespace testing;

class GTestExtensionTest : public ::testing::Test {};

TEST_F(GTestExtensionTest, Dummy) {
    mdn::QueueList q;
    q.hello();
    ASSERT_EQ(42, 42);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
