extern "C" {
#include "ast.h"
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

        add_symbol(st, id, &s);
        EXPECT_EQ(st->n_items, 1);
}

TEST_F(SymbolTest, AddSymbol_Multiple)
{
        struct symbol_table *st;
        char id1[] = "test1";
        char id2[] = "test2";
        struct symbol s1, s2;

        st = create_symbol_table();

        add_symbol(st, id1, &s1);
        add_symbol(st, id2, &s2);
        EXPECT_EQ(st->n_items, 2);
}

TEST_F(SymbolTest, AddTempSymbol)
{
        struct symbol_table *st;
        struct symbol s;

        st = create_symbol_table();

        add_temp_symbol(st, &s);
        EXPECT_EQ(st->n_items, 1);
}

TEST_F(SymbolTest, AddTempSymbol_Multiple)
{
        struct symbol_table *st;
        struct symbol s1, s2;

        st = create_symbol_table();

        add_temp_symbol(st, &s1);
        add_temp_symbol(st, &s2);
        EXPECT_EQ(st->n_items, 2);
}

TEST_F(SymbolDeathTest, AddSymbol_Duplicate)
{
        struct symbol_table *st;
        char id[] = "test1";
        struct symbol s;

        st = create_symbol_table();

        add_symbol(st, id, &s);
        ASSERT_DEATH(add_symbol(st, id, &s), "redefinition of symbol");
}

TEST_F(SymbolTest, GetSymbol_Empty)
{
        struct symbol_table *st;
        char id[] = "test";

        st = create_symbol_table();

        EXPECT_EQ(NULL, get_symbol(st, id));
}

TEST_F(SymbolTest, GetSymbol_CorrectId)
{
        struct symbol_table *st;
        char id[] = "test";
        struct symbol s;

        st = create_symbol_table();

        add_symbol(st, id, &s);
        EXPECT_EQ(&s, get_symbol(st, id));
}

TEST_F(SymbolTest, GetSymbol_IncorrectId)
{
        struct symbol_table *st;
        char id1[] = "test";
        char id2[] = "";
        struct symbol s;

        st = create_symbol_table();

        add_symbol(st, id1, &s);
        EXPECT_EQ(NULL, get_symbol(st, id2));
}

TEST_F(SymbolTest, GetSymbol_CorrectIdMultiple)
{
        struct symbol_table *st;
        char id1[] = "test";
        char id2[] = "test2";
        struct symbol s1, s2;

        st = create_symbol_table();

        add_symbol(st, id1, &s1);
        add_symbol(st, id2, &s2);
        EXPECT_EQ(&s1, get_symbol(st, id1));
        EXPECT_EQ(&s2, get_symbol(st, id2));
}

TEST_F(SymbolTest, GetSymbol_Temp)
{
        struct symbol_table *st;
        struct symbol s;

        st = create_symbol_table();

        add_temp_symbol(st, &s);
        EXPECT_EQ(&s, get_symbol(st, (char *)"temp:1"));
}

TEST_F(SymbolTest, GetSymbol_TempMultiple)
{
        struct symbol_table *st;
        struct symbol s1, s2;

        st = create_symbol_table();

        add_temp_symbol(st, &s1);
        add_temp_symbol(st, &s2);
        EXPECT_EQ(&s1, get_symbol(st, (char *)"temp:1"));
        EXPECT_EQ(&s2, get_symbol(st, (char *)"temp:2"));
}

TEST_F(SymbolTest, CreateSymbolBasic_CharType)
{
        struct symbol *s;

        s = create_symbol_basic(CHAR_TYPE);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_BASIC, s->type);
        EXPECT_EQ(CHAR_TYPE, s->val.basic_type);
}

TEST_F(SymbolTest, CreateSymbolBasic_FloatType)
{
        struct symbol *s;

        s = create_symbol_basic(FLOAT_TYPE);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_BASIC, s->type);
        EXPECT_EQ(FLOAT_TYPE, s->val.basic_type);
}

TEST_F(SymbolTest, CreateSymbolBasic_IntType)
{
        struct symbol *s;

        s = create_symbol_basic(INT_TYPE);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_BASIC, s->type);
        EXPECT_EQ(INT_TYPE, s->val.basic_type);
}

TEST_F(SymbolTest, CreateSymbolNamed)
{
        struct symbol s1, *s2;

        s2 = create_symbol_named(&s1);
        ASSERT_TRUE(s2 != NULL);
        EXPECT_EQ(SYMBOL_NAMED, s2->type);
        EXPECT_EQ(&s1, s2->val.symbol);
}

TEST_F(SymbolTest, CreateSymbolArray)
{
        int size = 10;
        struct symbol s1, *s2;

        s2 = create_symbol_array(&s1, size);
        ASSERT_TRUE(s2 != NULL);
        EXPECT_EQ(SYMBOL_ARRAY, s2->type);
        EXPECT_EQ(size, s2->val.array.size);
        EXPECT_EQ(&s1, s2->val.array.symbol);
}

TEST_F(SymbolTest, CreateSymbolStruct)
{
        struct symbol_table *st;
        struct struct_type struct_type;
        struct symbol *s;

        s = create_symbol_struct(&struct_type);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_STRUCT, s->type);
        EXPECT_EQ(&struct_type, s->val.struct_type);

        st = s->scoped_table;
        ASSERT_TRUE(st != NULL);
        EXPECT_EQ(0, st->n_items);
        EXPECT_EQ(NULL, st->items);
}

TEST_F(SymbolTest, CreateSymbolFunction)
{
        struct symbol_table *st;
        struct function_def function_def;
        struct symbol *s;

        s = create_symbol_function(&function_def);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_FUNCTION, s->type);
        EXPECT_EQ(&function_def, s->val.function_def);

        st = s->scoped_table;
        ASSERT_TRUE(st != NULL);
        EXPECT_EQ(0, st->n_items);
        EXPECT_EQ(NULL, st->items);
}

TEST_F(SymbolTest, TranslateFunctionCall_CharType)
{
        struct symbol_table *global;
        struct function_def *function_def;
        struct function_call *function_call;
        char id[] = "fn";
        struct symbol *s;

        global = create_symbol_table();
        function_def = create_basic_function_def(0, 0, CHAR_TYPE, id, NULL, NULL);
        function_call = create_function_call(0, 0, id, NULL);

        add_symbol(global, id, create_symbol_function(function_def));
        s = translate_function_call(global, NULL, function_call);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_BASIC, s->type);
        EXPECT_EQ(CHAR_TYPE, s->val.basic_type);
        EXPECT_EQ(1, global->n_temps);
}

TEST_F(SymbolTest, TranslateFunctionCall_FloatType)
{
        struct symbol_table *global;
        struct function_def *function_def;
        struct function_call *function_call;
        char id[] = "fn";
        struct symbol *s;

        global = create_symbol_table();
        function_def = create_basic_function_def(0, 0, FLOAT_TYPE, id, NULL, NULL);
        function_call = create_function_call(0, 0, id, NULL);

        add_symbol(global, id, create_symbol_function(function_def));
        s = translate_function_call(global, NULL, function_call);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_BASIC, s->type);
        EXPECT_EQ(FLOAT_TYPE, s->val.basic_type);
        EXPECT_EQ(1, global->n_temps);
}

TEST_F(SymbolTest, TranslateFunctionCall_IntType)
{
        struct symbol_table *global;
        struct function_def *function_def;
        struct function_call *function_call;
        char id[] = "fn";
        struct symbol *s;

        global = create_symbol_table();
        function_def = create_basic_function_def(0, 0, INT_TYPE, id, NULL, NULL);
        function_call = create_function_call(0, 0, id, NULL);

        add_symbol(global, id, create_symbol_function(function_def));
        s = translate_function_call(global, NULL, function_call);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(SYMBOL_BASIC, s->type);
        EXPECT_EQ(INT_TYPE, s->val.basic_type);
        EXPECT_EQ(1, global->n_temps);
}

TEST_F(SymbolTest, TranslateFunctionCall_VoidType)
{
        struct symbol_table *global; 
        struct function_def *function_def;
        struct function_call *function_call;
        char id[] = "fn";

        global = create_symbol_table();
        function_def = create_void_function_def(0, 0, id, NULL, NULL);
        function_call = create_function_call(0, 0, id, NULL);

        parse_function_def(global, function_def);
        ASSERT_EQ(NULL, translate_function_call(global, NULL, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgCharType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, CHAR_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def = create_void_function_def(0, 0, id, function_param_list, NULL);

        constant = create_constant_char(0, 0, '\0');
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgFloatType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, FLOAT_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def = create_void_function_def(0, 0, id, function_param_list, NULL);

        constant = create_constant_float(0, 0, 0.0f);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgIntType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, INT_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def = create_void_function_def(0, 0, id, function_param_list, NULL);

        constant = create_constant_int(0, 0, 0);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgNamedType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct type_decl *type_decl;
        struct var_decl *var_decl1, *var_decl2;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id1[] = "fn";
        char id2[] = "named";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl1 = create_var_decl_basic(0, 0, INT_TYPE, id2, NULL);
        type_decl = create_type_decl(0, 0, var_decl1);

        var_decl2 = create_var_decl_typedef(0, 0, id2, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl2, NULL);
        function_def = create_void_function_def(0, 0, id1, function_param_list, NULL);

        constant = create_constant_int(0, 0, 0);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id1, function_arg_list);

        parse_type_decl(global, type_decl);
        parse_function_def(global, function_def);
        EXPECT_EQ(2, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgArrayType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl1, *var_decl2;
        struct array_specifier *array_specifier1, *array_specifier2;
        struct constant *constant1, *constant2;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct var *var;
        char id1[] = "fn";
        char id2[] = "array";
        int size = 10;

        global = create_symbol_table();
        local = create_symbol_table();

        constant1 = create_constant_int(0, 0, size);
        array_specifier1 = create_array_specifier(0, 0, constant1);
        var_decl1 = create_var_decl_basic(0, 0, INT_TYPE, (char *)"arg", array_specifier1);
        function_param_list = create_function_param_list(0, 0, var_decl1, NULL);
        function_def = create_void_function_def(0, 0, id1, function_param_list, NULL);

        constant2 = create_constant_int(0, 0, size);
        array_specifier2 = create_array_specifier(0, 0, constant2);
        var_decl2 = create_var_decl_basic(0, 0, INT_TYPE, id2, array_specifier2);

        var = create_var_identifier(0, 0, id2);
        expr = create_postfix_expr_var(0, 0, var);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id1, function_arg_list);

        parse_var_decl(global, local, var_decl2);
        parse_function_def(global, function_def);
        EXPECT_EQ(1, local->n_items);
        EXPECT_EQ(1, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgStructType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl1, *var_decl2;
        struct struct_type *struct_type;
        struct var_decl_stmt_list *var_decl_stmt_list;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct var *var;
        char id1[] = "fn";
        char id2[] = "var";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl1 = create_var_decl_basic(0, 0, INT_TYPE, (char *)"inner", NULL);
        var_decl_stmt_list = create_var_decl_stmt_list(0, 0, var_decl1, NULL);
        struct_type = create_struct_type(0, 0, NULL, var_decl_stmt_list);
        var_decl2 = create_var_decl_struct(0, 0, struct_type, id2, NULL);
        function_param_list = create_function_param_list(0, 0, var_decl2, NULL);
        function_def = create_void_function_def(0, 0, id1, function_param_list, NULL);

        var = create_var_identifier(0, 0, id2);
        expr = create_postfix_expr_var(0, 0, var);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id1, function_arg_list);

        add_symbol(local, id2, create_symbol_struct(struct_type));
        parse_function_def(global, function_def);
        EXPECT_EQ(1, local->n_items);
        EXPECT_EQ(2, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call));
}

TEST_F(SymbolTest, TranslateFunctionCall_OneArgFunctionType)
{
        struct symbol_table *global, *local;
        struct function_def *function_def1, *function_def2;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call1, *function_call2;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        char id1[] = "fn1";
        char id2[] = "fn2";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, INT_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def1 = create_void_function_def(0, 0, id1, function_param_list, NULL);
        
        function_def2 = create_basic_function_def(0, 0, INT_TYPE, id2, NULL, NULL);

        function_call1 = create_function_call(0, 0, id2, NULL);
        expr = create_postfix_expr_function_call(0, 0, function_call1);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call2 = create_function_call(0, 0, id1, function_arg_list);

        parse_function_def(global, function_def1);
        parse_function_def(global, function_def2);
        EXPECT_EQ(2, global->n_items);

        ASSERT_EQ(NULL, translate_function_call(global, local, function_call2));
}

TEST_F(SymbolDeathTest, TranslateFunctionCall_CharFloatIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, CHAR_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def = create_void_function_def(0, 0, id, function_param_list, NULL);

        constant = create_constant_float(0, 0, 0.0f);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call), "passing argument to incompatible type");
}

TEST_F(SymbolDeathTest, TranslateFunctionCall_CharIntIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, CHAR_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def = create_void_function_def(0, 0, id, function_param_list, NULL);

        constant = create_constant_int(0, 0, 0);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call), "passing argument to incompatible type");
}

TEST_F(SymbolDeathTest, TranslateFunctionCall_IntFloatIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, INT_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def = create_void_function_def(0, 0, id, function_param_list, NULL);

        constant = create_constant_float(0, 0, 0.0f);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call), "passing argument to incompatible type");
}

TEST_F(SymbolDeathTest, TranslateFunctionCall_IntNamedIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct type_decl *type_decl;
        struct var_decl *var_decl1, *var_decl2;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id1[] = "fn";
        char id2[] = "named";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl1 = create_var_decl_basic(0, 0, CHAR_TYPE, id2, NULL);
        type_decl = create_type_decl(0, 0, var_decl1);

        var_decl2 = create_var_decl_typedef(0, 0, id2, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl2, NULL);
        function_def = create_void_function_def(0, 0, id1, function_param_list, NULL);

        constant = create_constant_int(0, 0, 0);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id1, function_arg_list);

        parse_type_decl(global, type_decl);
        parse_function_def(global, function_def);
        EXPECT_EQ(2, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call), "passing argument to incompatible type");
}

TEST_F(SymbolTest, TranslateFunctionCall_IntArrayIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl1;
        struct array_specifier *array_specifier1;
        struct constant *constant1, *constant2;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        char id1[] = "fn";
        int size = 10;

        global = create_symbol_table();
        local = create_symbol_table();

        constant1 = create_constant_int(0, 0, size);
        array_specifier1 = create_array_specifier(0, 0, constant1);
        var_decl1 = create_var_decl_basic(0, 0, INT_TYPE, (char *)"arg", array_specifier1);
        function_param_list = create_function_param_list(0, 0, var_decl1, NULL);
        function_def = create_void_function_def(0, 0, id1, function_param_list, NULL);

        constant2 = create_constant_int(0, 0, size);
        expr = create_postfix_expr_constant(0, 0, constant2);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id1, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(1, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call), "passing argument to incompatible type");
}

TEST_F(SymbolTest, TranslateFunctionCall_IntStructIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl1, *var_decl2;
        struct struct_type *struct_type;
        struct var_decl_stmt_list *var_decl_stmt_list;
        struct function_call *function_call;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        struct constant *constant;
        char id1[] = "fn";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl1 = create_var_decl_basic(0, 0, INT_TYPE, (char *)"inner", NULL);
        var_decl_stmt_list = create_var_decl_stmt_list(0, 0, var_decl1, NULL);
        struct_type = create_struct_type(0, 0, NULL, var_decl_stmt_list);
        var_decl2 = create_var_decl_struct(0, 0, struct_type, (char *)"outer", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl2, NULL);
        function_def = create_void_function_def(0, 0, id1, function_param_list, NULL);

        constant = create_constant_int(0, 0, 0);
        expr = create_postfix_expr_constant(0, 0, constant);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call = create_function_call(0, 0, id1, function_arg_list);

        parse_function_def(global, function_def);
        EXPECT_EQ(2, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call), "passing argument to incompatible type");
}

TEST_F(SymbolTest, TranslateFunctionCall_IntCharFunctionIncompatible)
{
        struct symbol_table *global, *local;
        struct function_def *function_def1, *function_def2;
        struct function_param_list *function_param_list;
        struct var_decl *var_decl;
        struct function_call *function_call1, *function_call2;
        struct function_arg_list *function_arg_list;
        struct expr *expr;
        char id1[] = "fn1";
        char id2[] = "fn2";

        global = create_symbol_table();
        local = create_symbol_table();

        var_decl = create_var_decl_basic(0, 0, INT_TYPE, (char *)"arg", NULL);
        function_param_list = create_function_param_list(0, 0, var_decl, NULL);
        function_def1 = create_void_function_def(0, 0, id1, function_param_list, NULL);
        
        function_def2 = create_basic_function_def(0, 0, CHAR_TYPE, id2, NULL, NULL);

        function_call1 = create_function_call(0, 0, id2, NULL);
        expr = create_postfix_expr_function_call(0, 0, function_call1);
        function_arg_list = create_function_arg_list(0, 0, expr, NULL);
        function_call2 = create_function_call(0, 0, id1, function_arg_list);

        parse_function_def(global, function_def1);
        parse_function_def(global, function_def2);
        EXPECT_EQ(2, global->n_items);

        ASSERT_DEATH(translate_function_call(global, local, function_call2), "passing argument to incompatible type");
}

TEST_F(SymbolDeathTest, TranslateFunctionCall_Undefined)
{
        struct symbol_table *global;
        struct function_call *function_call;
        char id[] = "fn";

        global = create_symbol_table();
        ASSERT_TRUE(global != NULL);

        function_call = create_function_call(0, 0, id, NULL);
        ASSERT_TRUE(function_call != NULL);
        ASSERT_DEATH(translate_function_call(global, NULL, function_call), "use of undeclared function");
}
