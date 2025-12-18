#include "unity.h"
#include "schema.h"

#include <stdio.h>

static oid_schema_t* schema = NULL;

void setUp(void)
{
    schema = oid_init_schema();
    TEST_ASSERT_NOT_NULL(schema);
}

void tearDown(void)
{
    oid_free_schema(schema);
    schema = NULL;
}

void test_oid_schema_error_to_string(void)
{
    TEST_ASSERT_EQUAL_STRING("Success", oid_schema_error_to_string(oid_schema_success));
    TEST_ASSERT_EQUAL_STRING("Invalid argument", oid_schema_error_to_string(oid_schema_invalid_argument));
    TEST_ASSERT_EQUAL_STRING("Out of memory", oid_schema_error_to_string(oid_schema_out_of_memory));
    TEST_ASSERT_EQUAL_STRING("Unexpected Steam Inventory Schema JSON", oid_schema_error_to_string(oid_schema_json_format));
    TEST_ASSERT_EQUAL_STRING("Jansson JSON parsing error", oid_schema_error_to_string(oid_schema_json_syntax));
    TEST_ASSERT_EQUAL_STRING("Unknown error", oid_schema_error_to_string(oid_schema_unknown));
    TEST_ASSERT_EQUAL_STRING("Unknown error", oid_schema_error_to_string(-1));
}

// tests the setting of last JSON error in schema
void test_set_and_get_json_error(void)
{
    json_error_t jerr;
    jerr.line = 42;
    jerr.column = 7;
    snprintf(jerr.text, sizeof(jerr.text), "Unexpected token");

    oid_set_json_error(schema, &jerr);

    const json_error_t* last = oid_get_last_json_err(schema);
    TEST_ASSERT_NOT_NULL(last);
    TEST_ASSERT_EQUAL_INT(42, last->line);
    TEST_ASSERT_EQUAL_INT(7, last->column);
    TEST_ASSERT_EQUAL_STRING("Unexpected token", last->text);

    // test that setting last json error with NULL doesnt perform any action and
    // doesn't override current error
    oid_set_json_error(schema, NULL);

    const json_error_t* last2 = oid_get_last_json_err(schema);
    TEST_ASSERT_NOT_NULL(last);
    TEST_ASSERT_EQUAL_INT(42, last->line);
    TEST_ASSERT_EQUAL_INT(7, last->column);
    TEST_ASSERT_EQUAL_STRING("Unexpected token", last->text);
}

// tests get last JSON error on NULL schema
void test_get_json_error_null_context(void)
{
    const json_error_t* last = oid_get_last_json_err(NULL);
    TEST_ASSERT_NULL(last);
}

// test loading schema with null arguments
void test_oid_load_itemdef_schema_null_args(void)
{
    TEST_ASSERT_EQUAL(oid_schema_invalid_argument, oid_load_itemdef_schema(NULL, "file.json"));
    TEST_ASSERT_EQUAL(oid_schema_invalid_argument, oid_load_itemdef_schema(schema, NULL));
}

// test loading schema from a file that doesn't exist
void test_oid_load_itemdef_schema_invalid_file(void)
{
    TEST_ASSERT_EQUAL(oid_schema_json_syntax, oid_load_itemdef_schema(schema, "does_not_exist.json"));

    const json_error_t* last = oid_get_last_json_err(schema);
    TEST_ASSERT_NOT_NULL(last);
}

// test loading schema from a valid file but invalid JSON syntax
void test_oid_load_itemdef_schema_valid_file_invalid_JSON(void)
{
    FILE* f = fopen("invalid_syntax.json", "w");
    fputs("{\n\"key\":\"value\"", f); // Missing '}' at the end
    fclose(f);

    oid_schema_error_t result = oid_load_itemdef_schema(schema, "invalid_syntax.json");
    const json_error_t* err = oid_get_last_json_err(schema);

    TEST_ASSERT_EQUAL(oid_schema_json_syntax, result);
    TEST_ASSERT_NOT_NULL(err);
    
    remove("invalid_syntax.json");
}

// test loading schema from a valid file and valid JSON syntax but not valid schema
void test_oid_load_itemdef_schema_valid_file_valid_JSON_invalid_schema(void)
{
    FILE* f = fopen("invalid_schema.json", "w");
    fputs("{\n\"key\":\"value\"\n}", f); // Valid JSON but not steam schema
    fclose(f);

    oid_schema_error_t result = oid_load_itemdef_schema(schema, "invalid_schema.json");
    const json_error_t* err = oid_get_last_json_err(schema);

    TEST_ASSERT_EQUAL(oid_schema_json_syntax, result);
    TEST_ASSERT_NOT_EQUAL_INT(json_error_unknown, json_error_code(err));
    
    remove("invalid_schema.json");
}

// test loading schema from a valid file with valid JSON syntax and valid Steam Inventory Schema
void test_oid_load_itemdef_schema_valid_file_valid_JSON_valid_schema(void)
{
    TEST_ASSERT_EQUAL(oid_schema_success, oid_load_itemdef_schema(schema, "data/item_def_steam_official_example.json"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_oid_schema_error_to_string);
    RUN_TEST(test_set_and_get_json_error);
    RUN_TEST(test_get_json_error_null_context);
    RUN_TEST(test_oid_load_itemdef_schema_null_args);
    RUN_TEST(test_oid_load_itemdef_schema_invalid_file);
    RUN_TEST(test_oid_load_itemdef_schema_valid_file_invalid_JSON);
    RUN_TEST(test_oid_load_itemdef_schema_valid_file_valid_JSON_invalid_schema);
    RUN_TEST(test_oid_load_itemdef_schema_valid_file_valid_JSON_valid_schema);
    return UNITY_END();
}
