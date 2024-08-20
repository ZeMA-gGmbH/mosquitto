/*
Copyright (c) 2020 Roger Light <roger@atchoo.org>

All rights reserved. This program and the accompanying materials
are made available under the terms of the Eclipse Public License 2.0
and Eclipse Distribution License v1.0 which accompany this distribution.

The Eclipse Public License is available at
   https://www.eclipse.org/legal/epl-2.0/
and the Eclipse Distribution License is available at
  http://www.eclipse.org/org/documents/edl-v10.php.

SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause

Contributors:
   Roger Light - initial implementation and documentation.
*/

/*
 * This is an *example* plugin which demonstrates how to modify the payload of
 * a message after it is received by the broker and before it is sent on to
 * other clients.
 *
 * You should be very sure of what you are doing before making use of this feature.
 *
 * Compile with:
 *   gcc -I<path to mosquitto-repo/include> -fPIC -shared mosquitto_payload_modification.c -o mosquitto_payload_modification.so
 *
 * Use in config with:
 *
 *   plugin /path/to/mosquitto_payload_modification.so
 *
 * Note that this only works on Mosquitto 2.0 or later.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mosquitto_broker.h"
#include "mosquitto_plugin.h"
#include "mosquitto.h"
#include "mqtt_protocol.h"

#include "json_help.h"

#define UNUSED(A) (void)(A)

static mosquitto_plugin_id_t* mosq_pid = NULL;
static cJSON* subscribedTopics = NULL;

static int callback_control(int event, void* event_data, void* userdata)
{
	struct mosquitto_evt_acl_check* ed = event_data;
	char* payload = NULL;
	uint32_t payload_len;

	UNUSED(event);
	UNUSED(userdata);

	const char * client_id = mosquitto_client_id(ed->client);
	const char* topic = ed->topic;
	const int access = ed->access;



	if (access == MOSQ_ACL_SUBSCRIBE) {
		json_create_array(subscribedTopics, topic);
		json_add_id_to_array(subscribedTopics, topic, client_id);
		char* json_string = cJSON_Print(subscribedTopics);
		payload = cJSON_PrintUnformatted(subscribedTopics);
		mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: client with id %s subscribed to topic %s", client_id, topic);
		mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: subscribed topics %s", json_string);

		if (payload == NULL) return MOSQ_ERR_MALFORMED_PACKET;

		payload_len = strlen(payload);
		if (payload_len > MQTT_MAX_PAYLOAD) {
			free(payload);
			return MOSQ_ERR_PAYLOAD_SIZE;
		}
		mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: DEBUG MESSAGE BEFORE PUBLISHING ...");

		mosquitto_broker_publish(NULL, "mqtt/subscriptions",
			(int)payload_len, payload, 0, true, NULL);
	}
	else if (access == MOSQ_ACL_UNSUBSCRIBE) {
		json_del_id_from_array(subscribedTopics, topic, client_id);
		char* json_string = cJSON_Print(subscribedTopics);
		payload = cJSON_PrintUnformatted(subscribedTopics);
		mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: client with id %s unscribed to topic %s", client_id, topic);
		mosquitto_log_printf(MOSQ_LOG_INFO, "wamo: subscribed topics %s", json_string);

		if (payload == NULL) return MOSQ_ERR_MALFORMED_PACKET;

		payload_len = strlen(payload);
		if (payload_len > MQTT_MAX_PAYLOAD) {
			free(payload);
			return MOSQ_ERR_PAYLOAD_SIZE;
		}
		mosquitto_broker_publish(NULL, "mqtt/subscriptions",
			(int)payload_len, payload, 0, true, NULL);
	}



	return MOSQ_ERR_SUCCESS; 
}


static int callback_message(int event, void* event_data, void* userdata)
{
	struct mosquitto_evt_message* ed = event_data;
	char* new_payload;
	uint32_t new_payloadlen;

	UNUSED(event);
	UNUSED(userdata);

	/* This simply adds "hello " to the front of every payload. You can of
	 * course do much more complicated message processing if needed. */

	 /* Calculate the length of our new payload */
	new_payloadlen = ed->payloadlen + (uint32_t)strlen("hello ") + 1;

	/* Allocate some memory - use
	 * mosquitto_calloc/mosquitto_malloc/mosquitto_strdup when allocating, to
	 * allow the broker to track memory usage */
	new_payload = mosquitto_calloc(1, new_payloadlen);
	if (new_payload == NULL) {
		return MOSQ_ERR_NOMEM;
	}

	/* Print "hello " to the payload */
	snprintf(new_payload, new_payloadlen, "hello ");
	memcpy(new_payload + (uint32_t)strlen("hello "), ed->payload, ed->payloadlen);

	/* Assign the new payload and payloadlen to the event data structure. You
	 * must *not* free the original payload, it will be handled by the
	 * broker. */
	ed->payload = new_payload;
	ed->payloadlen = new_payloadlen;

	return MOSQ_ERR_SUCCESS;
}

int mosquitto_plugin_version(int supported_version_count, const int* supported_versions)
{
	int i;

	for (i = 0; i < supported_version_count; i++) {
		if (supported_versions[i] == 5) {
			return 5;
		}
	}
	return -1;
}

int mosquitto_plugin_init(mosquitto_plugin_id_t* identifier, void** user_data, struct mosquitto_opt* opts, int opt_count)
{
	int rc;

	UNUSED(user_data);
	
	mosq_pid = identifier;

	subscribedTopics = cJSON_CreateObject();

	rc = mosquitto_callback_register(mosq_pid, MOSQ_EVT_ACL_CHECK, callback_control, NULL, NULL);
	if (rc == MOSQ_ERR_ALREADY_EXISTS) {
		mosquitto_log_printf(MOSQ_LOG_ERR, "Error: WaMo plugin can currently only be loaded once.");
		mosquitto_log_printf(MOSQ_LOG_ERR, "Note that this was previously incorrectly allowed but could cause problems with duplicate entries in the config.");
		return rc;
	}
	else if (rc == MOSQ_ERR_NOMEM) {
		mosquitto_log_printf(MOSQ_LOG_ERR, "Error: Out of memory.");
		return rc;
	}
	else if (rc != MOSQ_ERR_SUCCESS) {
		return rc;
	}

	return MOSQ_ERR_SUCCESS;
	

}

int mosquitto_plugin_cleanup(void* user_data, struct mosquitto_opt* opts, int opt_count)
{
	UNUSED(user_data);
	UNUSED(opts);
	UNUSED(opt_count);

	return mosquitto_callback_unregister(mosq_pid, MOSQ_EVT_ACL_CHECK, callback_control, NULL);
}
