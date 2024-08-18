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

int json_create_array(cJSON* json, const char* name) {
	cJSON* jtmp;

	jtmp = cJSON_GetObjectItem(json, name);
	if (jtmp) {
		if (cJSON_IsArray(jtmp) == true) {
			return MOSQ_ERR_INVAL;
		}
		else {
			cJSON_AddArrayToObject(json, name);
		}
				
	}
	else {
		cJSON_AddArrayToObject(json, name);
	}

	return MOSQ_ERR_SUCCESS;

}

int json_add_id_to_array(cJSON* json, const char* name, const char * id)
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
			return MOSQ_ERR_INVAL;
		}

	}
	else {
		return MOSQ_ERR_INVAL;
	}

	return MOSQ_ERR_SUCCESS;

}

int json_del_id_from_array(cJSON* json, const char* name, const char* id)
{
	cJSON* jtmp = NULL;
	cJSON* tid = NULL;
	cJSON* detached_item = NULL;

	int array_index = 0;

	jtmp = cJSON_GetObjectItem(json, name);
	if (jtmp) {
		mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: deleting item from array .. debug 1");
		if (cJSON_IsArray(jtmp) == true) {
			// cJSON_DeleteItemFromObject(json, name);
			mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: deleting item from array .. debug 2");
			cJSON_ArrayForEach(tid, jtmp)
			{	
								
				if ( strcmp( cJSON_GetStringValue(tid),id) == 0)
				{
					mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: deleting item from array .. debug 3");
					detached_item = cJSON_DetachItemFromArray(jtmp, array_index);
						
				}
				else
				{
					mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: deleting item from array .. debug 4");
				}			
				array_index++;
			}
			// cJSON_ReplaceItemInObject(json, name, jtmp);
			// cJSON_AddItemToObject(json, id, jtmp);
		}
		else {
			return MOSQ_ERR_INVAL;
		}

	}
	else {
		return MOSQ_ERR_INVAL;
	}

	return MOSQ_ERR_SUCCESS;

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
