#include <gtest/gtest.h>
#include "backstack/backstack.h"

TEST(Test, Test) {
    auto result = PrintBackStack(NULL);
    printf("%s", result);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();;
}