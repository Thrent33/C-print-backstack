#include <gtest/gtest.h>
#include "backstack/backstack.h"

void a() {
    auto result = PrintBackStack(NULL);
    printf("%s", result);
}

TEST(Test, Test) {
    a();
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();;
}