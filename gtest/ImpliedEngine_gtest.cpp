//
// Created by charles on 10/10/16.
//

#include <gtest/gtest.h>

#include "SecPair.hpp"
#include "ImpliedEngine.hpp"

TEST(SecPairTest, SumofTwo) {
    ASSERT_EQ(SecPair(1, 3, 1), SecPair(1, 2, 1) + SecPair(2, 3, 1));
    ASSERT_EQ(SecPair(1, 5, 1), SecPair(1, 3, 1) + SecPair(3, 5, 1));
    ASSERT_EQ(SecPair(1, 8, 1), SecPair(1, 2, 1) + SecPair(2, 8, 1));
    ASSERT_EQ(SecPair(1, 3, 1), SecPair(1, 2, 1) + SecPair(3, 2, -1));
}


TEST(SecPairTest, SumofThreeorMore) {
    ASSERT_EQ(SecPair(12,22,1),  SecPair(16,22,1) + SecPair(12,16,1));
    ASSERT_EQ(SecPair(3,6,1),    SecPair(3,4,1) + SecPair(4,5,1) + SecPair(5,6,1));
    ASSERT_EQ(SecPair(4,8,1),    SecPair(4,5,1) + SecPair(6,5,-1) + SecPair(7,6,-1) + SecPair(7,8,1));
    ASSERT_EQ(SecPair(6,10,1),   SecPair(7,10,1) + SecPair(7,6,-1));
}

TEST(SecPairTest, DiffofTwo) {
    ASSERT_EQ(SecPair(8,9,1),    SecPair(8,9,1) - SecPair(-1,-1,-1));
    ASSERT_EQ(SecPair(-1,-1,1),  SecPair(8,9,1) - SecPair(9,8,-1));
    ASSERT_EQ(SecPair(10,12, 1), SecPair(10,12,1) - SecPair(-1,-1,1));
    ASSERT_EQ(SecPair(12,13,-1), SecPair(11,12,1) - SecPair(11,13,1));
}

TEST(SecPairTest, Mult) {
    ASSERT_EQ(SecPair(5,8,1),   SecPair(8,5,1) * -1);
    ASSERT_EQ(SecPair(5,6,1),   SecPair(5,6,-1) * -1);
    ASSERT_EQ(SecPair(-1,-1,1), SecPair(-1,-1,1) * -1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
