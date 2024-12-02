#include <gtest/gtest.h>
#include "backstack/backstack.h"

TEST(Test, Test) {
    auto ret = backstack::ShowTraceStack(NULL);
    printf("%s\n", ret);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
