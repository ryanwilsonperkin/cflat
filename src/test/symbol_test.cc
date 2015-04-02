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

typedef SymbolTest SymbolDeathTest;

TEST_F(SymbolTest, CreateSymbolTable)
{
        struct symbol_table *st;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);
        EXPECT_EQ(0, st->n_items);
        EXPECT_EQ(NULL, st->items);
}

TEST_F(SymbolTest, CreateSymbolTableItem)
{
        struct symbol_table_item *st_item;
        char id[] = "test";
        struct symbol s;

        st_item = create_symbol_table_item(id, &s);
        ASSERT_TRUE(st_item != NULL);
        EXPECT_EQ(id, st_item->id);
        EXPECT_EQ(&s, st_item->symbol);
}

TEST_F(SymbolTest, AddSymbol)
{
        struct symbol_table *st;
        char id[] = "test";
        struct symbol s;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_symbol(st, id, &s);
        EXPECT_EQ(st->n_items, 1);
}

TEST_F(SymbolTest, AddSymbolMultiple)
{
        struct symbol_table *st;
        char id1[] = "test1";
        char id2[] = "test2";
        struct symbol s1, s2;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_symbol(st, id1, &s1);
        add_symbol(st, id2, &s2);
        EXPECT_EQ(st->n_items, 2);
}

TEST_F(SymbolTest, AddTempSymbol)
{
        struct symbol_table *st;
        struct symbol s;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_temp_symbol(st, &s);
        EXPECT_EQ(st->n_items, 1);
}

TEST_F(SymbolTest, AddTempSymbolMultiple)
{
        struct symbol_table *st;
        struct symbol s1, s2;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_temp_symbol(st, &s1);
        add_temp_symbol(st, &s2);
        EXPECT_EQ(st->n_items, 2);
}

TEST_F(SymbolDeathTest, AddSymbolDuplicate)
{
        struct symbol_table *st;
        char id[] = "test1";
        struct symbol s;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_symbol(st, id, &s);
        ASSERT_DEATH(add_symbol(st, id, &s), "redefinition of symbol");
}

TEST_F(SymbolTest, GetSymbolEmpty)
{
        struct symbol_table *st;
        char id[] = "test";

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);
        EXPECT_EQ(NULL, get_symbol(st, id));
}

TEST_F(SymbolTest, GetSymbolCorrectId)
{
        struct symbol_table *st;
        char id[] = "test";
        struct symbol s;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_symbol(st, id, &s);
        EXPECT_EQ(&s, get_symbol(st, id));
}

TEST_F(SymbolTest, GetSymbolIncorrectId)
{
        struct symbol_table *st;
        char id1[] = "test";
        char id2[] = "";
        struct symbol s;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_symbol(st, id1, &s);
        EXPECT_EQ(NULL, get_symbol(st, id2));
}

TEST_F(SymbolTest, GetSymbolCorrectIdMultiple)
{
        struct symbol_table *st;
        char id1[] = "test";
        char id2[] = "test2";
        struct symbol s1, s2;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_symbol(st, id1, &s1);
        add_symbol(st, id2, &s2);
        EXPECT_EQ(&s1, get_symbol(st, id1));
        EXPECT_EQ(&s2, get_symbol(st, id2));
}

TEST_F(SymbolTest, GetSymbolTemp)
{
        struct symbol_table *st;
        struct symbol s;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_temp_symbol(st, &s);
        EXPECT_EQ(&s, get_symbol(st, (char *)"temp:1"));
}

TEST_F(SymbolTest, GetSymbolTempMultiple)
{
        struct symbol_table *st;
        struct symbol s1, s2;

        st = create_symbol_table();
        ASSERT_TRUE(st != NULL);

        add_temp_symbol(st, &s1);
        add_temp_symbol(st, &s2);
        EXPECT_EQ(&s1, get_symbol(st, (char *)"temp:1"));
        EXPECT_EQ(&s2, get_symbol(st, (char *)"temp:2"));
}
