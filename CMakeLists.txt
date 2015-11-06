SET(MYSQL_INC_DIR "/usr/include/mysql")
SET(MYSQL_PLUGIN_DIR "/usr/lib/mysql/plugin/")
INCLUDE_DIRECTORIES(${MYSQL_INC_DIR})

ADD_LIBRARY(earth_distance MODULE earth_distance.cc)
INSTALL(TARGETS earth_distance DESTINATION ${MYSQL_PLUGIN_DIR})
