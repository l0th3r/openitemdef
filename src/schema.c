#include "schema.h"
#include "steam_types.h"

#include <jansson.h>

#define OID_SCHEMA_ROOT_UNPACK_FORMAT "{s:I, s:o}" /* format : appid = int, items = json_t* */

struct oid_schema_t
{
    oid_itemdef_t* item_defs;    /* parsed item definitions */
    size_t item_defs_capacity;   /* allocated item definitions */
    size_t item_defs_size;       /* amount of item definitions */

    json_int_t appid;   /* parsed schema appid */

    json_error_t last_json_err; /* latest jansson parsing error */
};

oid_schema_t* oid_init_schema(void)
{
    return calloc(1, sizeof(oid_schema_t));
}

void oid_free_schema(oid_schema_t* schema)
{
    if (schema)
    {
        oid_free_item_defs(schema);
        free(schema);
    }
}

/* PARSING */

oid_schema_error_t oid_parse_json_items(oid_schema_t* sch, json_t* jitems)
{
    if (!sch || !jitems || !json_is_array(jitems))
    {
        return OID_SCHEMA_INVALID_ARGUMENT;
    }

    size_t alloc_size = json_array_size(jitems);

    if (alloc_size == 0)
    {
        return OID_SCHEMA_SUCCESS;
    }
    
    oid_schema_error_t alloc_res = oid_alloc_item_defs(sch, alloc_size);
    if (alloc_res != OID_SCHEMA_SUCCESS)
    {
        return alloc_res;
    }

    // TODO Parse Items

    return OID_SCHEMA_SUCCESS;
}

oid_schema_error_t oid_parse_json_to_struct(oid_schema_t* sch, json_t* jroot)
{
    if (!sch || !jroot || !json_is_object(jroot))
    {
        return OID_SCHEMA_INVALID_ARGUMENT;
    }

    oid_free_item_defs(sch);

    json_error_t root_parsing_err;
    json_int_t root_appid = 0;
    json_t* root_items = NULL;

    int root_parsing_ret = json_unpack_ex(
        jroot,
        &root_parsing_err,
        JSON_STRICT,
        OID_SCHEMA_ROOT_UNPACK_FORMAT,
        "appid", &root_appid,
        "items", &root_items
    );

    if (root_parsing_ret != 0)
    {
        oid_set_json_error(sch, &root_parsing_err);
        return OID_SCHEMA_JANSSON_PARSING;
    }

    if (!json_is_array(root_items))
    {
        return OID_SCHEMA_UNEXPECTED_JSON;
    }

    sch->appid = root_appid;

    return oid_parse_json_items(sch, root_items);
}

oid_schema_error_t oid_load_itemdef_schema(oid_schema_t* sch, const char* file_path)
{
    if (!sch || !file_path)
    {
        return 1;
    }

    json_error_t json_err;
    json_t* root = json_load_file(file_path, 0, &json_err);
    if(!root)
    {
        oid_set_json_error(sch, &json_err);
        return OID_SCHEMA_JANSSON_PARSING;
    }

    oid_schema_error_t res = oid_parse_json_to_struct(sch, root);

    json_decref(root);
    return res;
}

oid_schema_error_t oid_alloc_item_defs(oid_schema_t* sch, size_t size)
{
    if (!sch)
    {
        return OID_SCHEMA_INVALID_ARGUMENT;
    }

    size_t newsz = sch->item_defs_capacity + size;
    oid_itemdef_t* tmp = realloc(sch->item_defs, newsz * sizeof(oid_itemdef_t));
    if (!tmp)
    {
        return OID_SCHEMA_OUT_OF_MEMORY;
    }
    
    sch->item_defs = tmp;
    sch->item_defs_capacity = newsz;
    return OID_SCHEMA_SUCCESS;
}

void oid_free_item_defs(oid_schema_t* sch)
{
    if (!sch)
    {
        return;
    }

    free(sch->item_defs);
    sch->item_defs_capacity = 0;
    sch->item_defs_size = 0;
}

/* GETTERS */

size_t oid_get_schema_capacity(const oid_schema_t* sch)
{
    if (!sch)
    {
        return 0;
    }

    return sch->item_defs_capacity;
}

size_t oid_get_schema_size(const oid_schema_t* sch)
{
    if (!sch)
    {
        return 0;
    }

    return sch->item_defs_size;
}

/* ERROR HANDLING */

const char* oid_schema_error_to_string(oid_schema_error_t err)
{
    switch (err)
    {
        case OID_SCHEMA_SUCCESS:            return "Success";
        case OID_SCHEMA_INVALID_ARGUMENT:   return "Invalid argument";
        case OID_SCHEMA_OUT_OF_MEMORY:      return "Out of memory";
        case OID_SCHEMA_UNEXPECTED_JSON:    return "Unexpected Steam Inventory Schema JSON";
        case OID_SCHEMA_JANSSON_PARSING:    return "Jansson JSON parsing error";
        case OID_SCHEMA_ERR_UNKNOWN:        return "Unknown error";
        default:                            return oid_schema_error_to_string(OID_SCHEMA_ERR_UNKNOWN);
    }
}

const json_error_t* oid_get_last_json_err(const oid_schema_t* sch)
{
    if (!sch)
    {
        return NULL;
    }

    return &sch->last_json_err;
}

void oid_set_json_error(oid_schema_t* sch, const json_error_t* err)
{
    if (!sch || !err)
    {
        return;
    }

    sch->last_json_err = *err;
}