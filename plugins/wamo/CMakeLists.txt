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

	set( CLIENT_INC ${mosquitto_SOURCE_DIR} ${mosquitto_SOURCE_DIR}/include
		${STDBOOL_H_PATH} ${STDINT_H_PATH} ${mosquitto_SOURCE_DIR}/deps 
		${mosquitto_SOURCE_DIR}/src ${CJSON_INCLUDE_DIRS} )

	set( CLIENT_DIR ${mosquitto_BINARY_DIR}/lib ${CJSON_DIR})

	include_directories(${CLIENT_INC})
	link_directories(${CLIENT_DIR} ${mosquitto_SOURCE_DIR})

	add_library(wamo MODULE
		json_help.c
		json_help.h
		wamo.c)

	set_target_properties(wamo PROPERTIES
		POSITION_INDEPENDENT_CODE 1
	)
	set_target_properties(wamo PROPERTIES PREFIX "")

	target_link_libraries(wamo ${CJSON_LIBRARIES})
	if(WIN32)
		target_link_libraries(wamo mosquitto)
		install(TARGETS wamo
			DESTINATION "${CMAKE_INSTALL_BINDIR}")
	else()
		install(TARGETS wamo
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
	endif()

endif()
