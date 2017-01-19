cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME rosbridge2cpp)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")
#add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DRAPIDJSON_HAS_STDSTRING=1)
add_definitions(-DBSON_COMPILATION)

set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/")

set (SOURCES
   ${SOURCE_DIR}/libbson/bson/bcon.c
   ${SOURCE_DIR}/libbson/bson/bson.c
   ${SOURCE_DIR}/libbson/bson/bson-atomic.c
   ${SOURCE_DIR}/libbson/bson/bson-clock.c
   ${SOURCE_DIR}/libbson/bson/bson-context.c
   ${SOURCE_DIR}/libbson/bson/bson-decimal128.c
   ${SOURCE_DIR}/libbson/bson/bson-error.c
   ${SOURCE_DIR}/libbson/bson/bson-iso8601.c
   ${SOURCE_DIR}/libbson/bson/bson-iter.c
   ${SOURCE_DIR}/libbson/bson/bson-json.c
   ${SOURCE_DIR}/libbson/bson/bson-keys.c
   ${SOURCE_DIR}/libbson/bson/bson-md5.c
   ${SOURCE_DIR}/libbson/bson/bson-memory.c
   ${SOURCE_DIR}/libbson/bson/bson-oid.c
   ${SOURCE_DIR}/libbson/bson/bson-reader.c
   ${SOURCE_DIR}/libbson/bson/bson-string.c
   ${SOURCE_DIR}/libbson/bson/bson-timegm.c
   ${SOURCE_DIR}/libbson/bson/bson-utf8.c
   ${SOURCE_DIR}/libbson/bson/bson-value.c
   ${SOURCE_DIR}/libbson/bson/bson-version-functions.c
   ${SOURCE_DIR}/libbson/bson/bson-writer.c
   ${SOURCE_DIR}/libbson/yajl/yajl_alloc.c
   ${SOURCE_DIR}/libbson/yajl/yajl_buf.c
   ${SOURCE_DIR}/libbson/yajl/yajl.c
   ${SOURCE_DIR}/libbson/yajl/yajl_encode.c
   ${SOURCE_DIR}/libbson/yajl/yajl_gen.c
   ${SOURCE_DIR}/libbson/yajl/yajl_lex.c
   ${SOURCE_DIR}/libbson/yajl/yajl_parser.c
   ${SOURCE_DIR}/libbson/yajl/yajl_tree.c
   ${SOURCE_DIR}/libbson/yajl/yajl_version.c

	 tests.cpp
)

INCLUDE_DIRECTORIES( libbson/bson libbson/ include/)

ADD_EXECUTABLE( tests
	${SOURCES}
)

#ADD_LIBRARY( rosbridge2cpp SHARED
#	src/client/socket_tcp_connection.cpp 
#	src/ros_bridge.cpp 
#	src/ros_topic.cpp 
#	src/ros_service.cpp 
#	src/ros_tf_broadcaster.cpp 
#	src/ros_time.cpp 
#)
