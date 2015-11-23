SET(MYSQL_INC_DIR "/usr/include/mysql")
SET(MYSQL_PLUGIN_DIR "/usr/lib/mysql/plugin/")
INCLUDE_DIRECTORIES(${MYSQL_INC_DIR})

ADD_LIBRARY(earth_distance MODULE earth_distance.cc)
ADD_LIBRARY(write_to_file MODULE write_to_file.cc)
INSTALL(TARGETS earth_distance write_to_file DESTINATION ${MYSQL_PLUGIN_DIR})
