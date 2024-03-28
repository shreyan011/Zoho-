CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
	
SET(CA_CRT /home/debian/zoho/Raptee_MQTT/Raptee_MQTT/ZohoIoTServerRootCA.pem)
SET(CLIENT_CRT /usr/local/certs/client.crt)
SET(CLIENT_KEY /usr/local/certs/client.key)

# Configure EMBED_CERT_MODE to ON if you want to embed certs along with application. else
# certificates needs to be copied along with application.
OPTION(EMBED_CERT_MODE "Embed certificates along with application" OFF)

#include_directories(${ZeroMQ_INCLUDE_DIRS} /usr/include)


include_directories(/usr/include)
INCLUDE(${PROJECT_SOURCE_DIR}/cmake/internalFuncs.cmake)

IF(EMBED_CERT_MODE)
    SET(CRT_PARSE_MODE EMBED)
ELSE()
    SET(CRT_PARSE_MODE REFERENCE)
ENDIF(EMBED_CERT_MODE)

LIST(APPEND FEATURES "-DCRT_PARSE_MODE=${CRT_PARSE_MODE}")
LIST(APPEND FEATURES "-D${CRT_PARSE_MODE}_MODE")
MESSAGE(STATUS "CERTIFICATE CONFIG MODE : " ${CRT_PARSE_MODE})

IF(Z_ENABLE_TLS)
    LIST(APPEND CERTS CA_CRT)
    IF(Z_USE_CLIENT_CERTS)
        LIST(APPEND CERTS CLIENT_CRT CLIENT_KEY)
    ENDIF(Z_USE_CLIENT_CERTS)

    IF(EXISTS ${PROJECT_SOURCE_DIR}/include/zclient_certificates.h)
        FILE(REMOVE ${PROJECT_SOURCE_DIR}/include/zclient_certificates.h)
    ENDIF()

    FOREACH(CERT ${CERTS})
        IF(EMBED_CERT_MODE)
            READ_FILE(${${CERT}} ${CERT})
        ELSE()
            LIST(APPEND CERTS_PATH "-D${CERT}=\"${${CERT}}\"")
        ENDIF(EMBED_CERT_MODE)
    ENDFOREACH(CERT)
ENDIF(Z_ENABLE_TLS)

ADD_EXECUTABLE(basic basic.c)
TARGET_COMPILE_DEFINITIONS(basic PUBLIC ${FEATURES})
IF(Z_ENABLE_TLS)
    TARGET_COMPILE_DEFINITIONS(basic PUBLIC ${CERTS_PATH})
ENDIF(Z_ENABLE_TLS)
find_library(ZMQ_LIBRARY NAMES zmq)
#set(ZMQ_ROOT "/usr/lib/aarch64-linux-gnu")
#find_package(ZMQ REQUIRED) 
#include_directories(${ZMQ_INCLUDE_DIRS}) 
#target_link_libraries(your_target_name ${ZMQ_LIBRARIES})

TARGET_LINK_LIBRARIES(basic zoho_iot_sdk ${MZQ_LIBRARY})
#TARGET_LINK_LIBRARIES(basic )
#target_link_options(basic PRIVATE "-lzmq")
#target_link_libraries(basic ${ZMQ_LIBRARY})
#target_link_libraries(basic ZMQ::ZMQ)
