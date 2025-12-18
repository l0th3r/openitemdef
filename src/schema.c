#include "schema.h"
#include "steam_types.h"

#include <jansson.h>

#define OID_SCHEMA_ROOT_UNPACK_FORMAT "{s:I, s:o}" /* format : appid = int, items = json_t* */

struct oid_schema_t
{
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
        free(schema);
    }
}

/* PARSING */
oid_schema_error_t oid_parse_json_root(oid_schema_t* sch, json_t* jroot)
{
    if (!sch || !jroot || !json_is_object(jroot))
    {
        return oid_schema_invalid_argument;
    }

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
        return oid_schema_json_syntax;
    }

    if (!json_is_array(root_items))
    {
        return oid_schema_json_format;
    }

    sch->appid = root_appid;

    // TODO load item defs root in cache

    return oid_schema_success;
}

oid_schema_error_t oid_load_itemdef_schema(oid_schema_t* sch, const char* file_path)
{
    if (!sch || !file_path)
    {
        return oid_schema_invalid_argument;
    }

    json_error_t json_err;
    json_t* root = json_load_file(file_path, 0, &json_err);
    if(!root)
    {
        oid_set_json_error(sch, &json_err);
        return oid_schema_json_syntax;
    }

    oid_schema_error_t res = oid_parse_json_root(sch, root);

    json_decref(root);
    return res;
}

/* ERROR HANDLING */

const char* oid_schema_error_to_string(oid_schema_error_t err)
{
    switch (err)
    {
        case oid_schema_success:            return "Success";
        case oid_schema_invalid_argument:   return "Invalid argument";
        case oid_schema_out_of_memory:      return "Out of memory";
        case oid_schema_json_format:    return "Unexpected Steam Inventory Schema JSON";
        case oid_schema_json_syntax:        return "Jansson JSON parsing error";
        case oid_schema_unknown:        return "Unknown error";
        default:                            return oid_schema_error_to_string(oid_schema_unknown);
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