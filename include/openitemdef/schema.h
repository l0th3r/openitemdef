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
    OID_SCHEMA_SUCCESS = 0,
    OID_SCHEMA_INVALID_ARGUMENT,
    OID_SCHEMA_OUT_OF_MEMORY,
    OID_SCHEMA_UNEXPECTED_JSON, /* provided json is not valid for Steam Inventory Schema */
    OID_SCHEMA_JANSSON_PARSING, /* jansson parsing error, 'last_json_err' will be populated */
    OID_SCHEMA_ERR_UNKNOWN
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

/**
 * @brief Pre-allocate data for parsing.
 * 
 * @param schema Pointer to the schema. If NULL, the function return 1.
 * @param alloc_size Amout of elements to allocate (not the size).
 * 
 * @note Allocation is dynamic this function will not overwrite or free any data.
 * @note Item counter will not be updated.
 * 
 * @return schema error
 */
oid_schema_error_t oid_alloc_item_defs(oid_schema_t* schema, size_t alloc_size);

/**
 * @brief Free all parsed item def allocated in schema.
 * 
 * @param schema Pointer to the schema. If NULL, the function performs no action.
 * 
 * @note Item counter will be set to 0.
 */
void oid_free_item_defs(oid_schema_t* schema);

/**
 * @brief Get schema item definition allocated capacity.
 *
 * @param schema Pointer to a valid schema. Must not be NULL.
 * 
 * @return Schema item definitions capacity.
 */
size_t oid_get_schema_capacity(const oid_schema_t* schema);

/**
 * @brief Get schema stored item definition size.
 *
 * @param schema Pointer to a valid schema. Must not be NULL.
 * 
 * @return Schema item definitions size.
 */
size_t oid_get_schema_size(const oid_schema_t* schema);

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