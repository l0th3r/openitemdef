#ifndef OID_SCHEMA_H
#define OID_SCHEMA_H

#include <stddef.h>
#include <jansson.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represent parsed Steam Inventory Schema
 * 
 */
typedef struct oid_schema_t oid_schema_t;

/**
 * @brief Error codes associated with oid_schema_t
 * 
 */
typedef enum oid_schema_error_t
{
    oid_schema_success = 0,         /* success */
    oid_schema_unknown,             /* unknown error */
    oid_schema_invalid_argument,    /* provided arguments are invalid */
    oid_schema_out_of_memory,       /* failed to allocate memory */
    oid_schema_json_format,         /* provided json is not formatted as Steam Inventory Schema */
    oid_schema_json_syntax          /* jansson parsing error 'last_syntax_err' will be populated with details */
} oid_schema_error_t;

/**
 * @brief Initializes a new schema.
 *
 * @return A pointer to the newly created schema, NULL if memory allocation fails.
 * 
 */
oid_schema_t* oid_init_schema(void);

/**
 * @brief Loads and parses an Steam ItemDef Schema JSON file.
 *
 * @param schema Pointer to a valid schema.
 * @param file_path Path to the JSON file to load.
 * @return 0 on success, non-zero value if the file cannot be read or parsed.
 * 
 * @note The file is released once parsed.
 * 
 */
oid_schema_error_t oid_load_itemdef_schema(oid_schema_t* schema, const char* file_path);

/**
 * @brief Frees all resources associated with a schema.
 *
 * @param schema Pointer to the context to destroy. If NULL, the function performs no action.
 *
 * @note After this call, the pointer becomes invalid and must not be used in any further API calls.
 */
void oid_free_schema(oid_schema_t* schema);

/* ERROR HANDLING */

/**
 * @brief Get oid_schema_error_t as a string
 * 
 * @param error The error to get as a string
 * 
 * @return The error as a string
 */
const char* oid_schema_error_to_string(oid_schema_error_t error);

/**
 * @brief Retrieves the last JSON parsing error stored in the given schema.
 *
 * Provides read-only access to the most recent JSON parsing or serialization
 * error encountered by the library. The returned pointer remains valid for the
 * lifetime of the schema and must not be modified or freed by the caller.
 *
 * @param schema Pointer to a valid schema. Must not be NULL.
 * @return A pointer to the last stored JSON error, or NULL if schema is NULL.
 */
const json_error_t* oid_get_last_json_err(const oid_schema_t* schema);

/**
 * @brief Stores a JSON parsing error.
 *
 * This function is intended to be used internally whenever a Jansson parsing or loading operation fails.
 *
 * @param schema Pointer to the schema in which to store the error. If NULL, the function performs no action.
 * @param json_error Pointer to a @ref json_error_t structure provided by Jansson.
 */
void oid_set_json_error(oid_schema_t* schema, const json_error_t* json_error);

#ifdef __cplusplus
}
#endif
#endif