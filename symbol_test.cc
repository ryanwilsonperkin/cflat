extern "C" {
#include "symbol.h"
}

#include "gtest/gtest.h"

class SymbolTest : public ::testing::Test {
        protected:
                SymbolTest() {}
                virtual ~SymbolTest() {}
                virtual void SetUp() {}
                virtual void TearDown() {}
};

TEST_F(SymbolTest, Placeholder) {}
