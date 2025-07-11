
#include <ranges>
#include <gtest/gtest.h>

#include "mdn/Queue.hpp"
#include "mdn/QueueList.hpp"
#include "mdn/QueueVector.hpp"

using namespace testing;

using QueueTypes = ::testing::Types<
    mdn::QueueList<int>
    // mdn::QueueVector<int>
    >;

template<typename QueueT>
class QueueTest : public ::testing::Test {};

TYPED_TEST_SUITE(QueueTest, QueueTypes);

TYPED_TEST(QueueTest, CreateAndDestroy) {
    TypeParam queue;
}

TYPED_TEST(QueueTest, IsEmpty) {
    TypeParam queue;

    ASSERT_EQ(queue.isEmpty(), true);
}

TYPED_TEST(QueueTest, SizeOfEmptyQueue) {
    TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, EnqueueSingleElement) {
    TypeParam queue;

    queue.enqueue(1);
    ASSERT_EQ(queue.size(), 1);
}

TYPED_TEST(QueueTest, EnqueueMultipleElement) {
    TypeParam queue;
    constexpr int numElements = 100;

    for (int i : std::views::iota(1, numElements + 1)) {
        queue.enqueue(i);
    }

    ASSERT_EQ(queue.size(), numElements);
}

int
main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
