#ifndef JSON_HELP_H
#define JSON_HELP_H
/*

Contributors:
   Dominik Kuhn Light - initial implementation and documentation.
*/
#include <cjson/cJSON.h>
#include <stdbool.h>

int json_create_array(cJSON* json, const char* name);
int json_add_id_to_array(cJSON* json, const char* name, const char* id);
int json_del_id_from_array(cJSON* json, const char* name, const char* id);

/* "optional==false" can also be taken to mean "only return success if the key exists and is valid" */
int json_get_bool(cJSON *json, const char *name, bool *value, bool optional, bool default_value);
int json_get_int(cJSON *json, const char *name, int *value, bool optional, int default_value);
int json_get_string(cJSON *json, const char *name, char **value, bool optional);

cJSON *cJSON_AddIntToObject(cJSON * const object, const char * const name, int number);
cJSON *cJSON_CreateInt(int num);

#endif
