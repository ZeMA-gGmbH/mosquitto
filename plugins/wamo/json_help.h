#ifndef JSON_HELP_H
#define JSON_HELP_H
/*

Contributors:
   Dominik Kuhn Light - initial implementation and documentation.
*/
#include <cjson/cJSON.h>
#include <stdbool.h>


/* Enum: json_err_t
 * Integer values returned from many JSON helper function functions. 
 * 
 * JSON_ERR_SUCCESS - the message/packet ID of the PUBLISH message, assuming this is a.
 *
 * char *topic - the topic the message was delivered on.
 *
 * void *payload - the message payload. This will be payloadlen bytes long, and
 *                 may be NULL if a zero length payload was sent.
 *
 * int payloadlen - the length of the payload, in bytes.
 *
 * int qos - the quality of service of the message, 0, 1, or 2.
 */
typedef enum {

	JSON_ERR_SUCCESS = 0,
	JSON_ERR_FAILURE = 1,
	JSON_ERR_TYPE_INVAL = 3,
	JSON_ERR_VALUE = 4,
	JSON_ERR_SYNTAX = 5,
	JSON_ERR_ARRAY_EXISTS = 6, 

} json_err_t;


json_err_t json_create_array(cJSON* json, const char* name);
json_err_t json_add_id_to_array(cJSON* json, const char* name, const char* id);
json_err_t json_del_id_from_array(cJSON* json, const char* name, const char* id);
json_err_t json_del_clientid(cJSON* json, const char* id);


/* "optional==false" can also be taken to mean "only return success if the key exists and is valid" */
int json_get_bool(cJSON *json, const char *name, bool *value, bool optional, bool default_value);
int json_get_int(cJSON *json, const char *name, int *value, bool optional, int default_value);
int json_get_string(cJSON *json, const char *name, char **value, bool optional);

cJSON *cJSON_AddIntToObject(cJSON * const object, const char * const name, int number);
cJSON *cJSON_CreateInt(int num);

#endif
