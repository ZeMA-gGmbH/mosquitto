# include_directories(${mosquitto_SOURCE_DIR} ${mosquitto_SOURCE_DIR}/include
# 			${OPENSSL_INCLUDE_DIR} ${STDBOOL_H_PATH} ${STDINT_H_PATH})
# link_directories(${mosquitto_SOURCE_DIR})
#
# add_library(wamo MODULE wamo.c "json_help.c" "json_help.h")
# set_target_properties(wamo PROPERTIES
# 	POSITION_INDEPENDENT_CODE 1
# )
# set_target_properties(wamo PROPERTIES PREFIX "")
# if(WIN32)
#	 target_link_libraries(wamo mosquitto ${CJSON_LIBRARIES})
# endif(WIN32)

# Don't install, these are example plugins only.
#install(TARGETS wamo RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")


if (CJSON_FOUND )
	add_definitions("-DWITH_CJSON")

	set (MOSQUITTO_IDIR "" CACHE FILEPATH "C:/Program Files/mosquitto/devel/" )
	set (MOSQUITTO_LIBRARIES "" CACHE FILEPATH "C:/Program Files/mosquitto/devel/")
	set (PLUGIN_NAME wamo)

	add_library(${PLUGIN_NAME} MODULE
		${PLUGIN_NAME}.c
		json_help.c
		json_help.h
	)

	target_include_directories(${PLUGIN_NAME} PRIVATE
		"${CJSON_INCLUDE_DIRS}"
		"${STDBOOL_H_PATH} ${STDINT_H_PATH}"
		"${mosquitto_SOURCE_DIR}"
		"C:/Program Files/mosquitto/devel/"
	)

	target_link_directories(${PLUGIN_NAME} PRIVATE ${MOSQUITTO_LIBRARIES} )

	set_target_properties(${PLUGIN_NAME} PROPERTIES
		PREFIX ""
		POSITION_INDEPENDENT_CODE 1
	)

	set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
	set_target_properties(${PLUGIN_NAME} PROPERTIES IMPORTED_IMPLIB ${CJSON_LIBRARIES})

	target_link_libraries(${PLUGIN_NAME} PRIVATE ${CJSON_LIBRARIES})

	if(WIN32)
		target_link_libraries(${PLUGIN_NAME} PRIVATE mosquitto)		
	endif()	

endif()