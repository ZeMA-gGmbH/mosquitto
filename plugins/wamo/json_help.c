/*

Contributors:
   Dominik Kuhn - initial implementation and documentation.
*/

#include "config.h"

#include <cjson/cJSON.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "json_help.h"
#include "mosquitto.h"
#include "mosquitto_broker.h"

json_err_t json_create_array(cJSON* json, const char* name) {
	cJSON* jtmp;

	jtmp = cJSON_GetObjectItem(json, name);
	if (jtmp) {
		if (cJSON_IsArray(jtmp) == true) {
			return JSON_ERR_ARRAY_EXISTS;
		}
		else {
			return JSON_ERR_TYPE_INVAL;
		}
				
	}
	else {
		cJSON_AddArrayToObject(json, name);
	}

	return JSON_ERR_SUCCESS;

}

json_err_t json_add_id_to_array(cJSON* json, const char* name, const char * id)
{	
	cJSON* jtmp;
	cJSON* tid;

	jtmp = cJSON_GetObjectItem(json, name);
	if (jtmp) {
		if (cJSON_IsArray(jtmp) == true) {
			tid = cJSON_CreateString(id);
			cJSON_AddItemToArray(jtmp, tid);
		}
		else {
			return JSON_ERR_TYPE_INVAL;
		}

	}
	else {
		return JSON_ERR_SYNTAX;
	}

	return JSON_ERR_SUCCESS;

}

json_err_t json_del_id_from_array(cJSON* json, const char* name, const char* id)
{
	cJSON* jtmp = NULL;
	cJSON* tid = NULL;
	cJSON* detached_item = NULL;

	int array_index = 0;

	jtmp = cJSON_GetObjectItem(json, name);
	if (jtmp) {
		if (cJSON_IsArray(jtmp) == true) {
			cJSON_ArrayForEach(tid, jtmp)
			{									
				if ( strcmp( cJSON_GetStringValue(tid),id) == 0)
				{
					detached_item = cJSON_DetachItemFromArray(jtmp, array_index);

				}
				array_index++;
			}
	
		}
		else {
			return JSON_ERR_TYPE_INVAL;
		}

	}
	else {
		return JSON_ERR_SYNTAX;
	}

	return JSON_ERR_SUCCESS;

}


json_err_t json_del_clientid(cJSON* json, const char* id)
{
	cJSON* next_object = NULL;

	cJSON_ArrayForEach(next_object, json)
	{
		if (next_object->string != NULL) {

			mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: deleting client with id %s from topic %s", id, next_object->string);
			json_del_id_from_array(json, next_object->string, id);
		}	

		
	}


	return JSON_ERR_SUCCESS;

}




int json_get_bool(cJSON *json, const char *name, bool *value, bool optional, bool default_value)
{
	cJSON *jtmp;

	if(optional == true){
		*value = default_value;
	}

	jtmp = cJSON_GetObjectItem(json, name);
	if(jtmp){
		if(cJSON_IsBool(jtmp) == false){
			return MOSQ_ERR_INVAL;
		}
		*value = cJSON_IsTrue(jtmp);
	}else{
		if(optional == false){
			return MOSQ_ERR_INVAL;
		}
	}
	return MOSQ_ERR_SUCCESS;
}


int json_get_int(cJSON *json, const char *name, int *value, bool optional, int default_value)
{
	cJSON *jtmp;

	if(optional == true){
		*value = default_value;
	}

	jtmp = cJSON_GetObjectItem(json, name);
	if(jtmp){
		if(cJSON_IsNumber(jtmp) == false){
			return MOSQ_ERR_INVAL;
		}
		*value  = jtmp->valueint;
	}else{
		if(optional == false){
			return MOSQ_ERR_INVAL;
		}
	}
	return MOSQ_ERR_SUCCESS;
}


int json_get_string(cJSON *json, const char *name, char **value, bool optional)
{
	cJSON *jtmp;

	*value = NULL;

	jtmp = cJSON_GetObjectItem(json, name);
	if(jtmp){
		if(cJSON_IsString(jtmp) == false){
			return MOSQ_ERR_INVAL;
		}
		*value  = jtmp->valuestring;
	}else{
		if(optional == false){
			return MOSQ_ERR_INVAL;
		}
	}
	return MOSQ_ERR_SUCCESS;
}


cJSON *cJSON_AddIntToObject(cJSON * const object, const char * const name, int number)
{
	char buf[30];

	snprintf(buf, sizeof(buf), "%d", number);
	return cJSON_AddRawToObject(object, name, buf);
}
