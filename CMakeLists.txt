cmake_minimum_required(VERSION 3.0.0)
project(neural_net)
set(EXEC nn)

# Bring the headers into the project
include_directories(include)

# The file(GLOB...) allows for wildcard additions
file(GLOB SOURCES "src/*.c" "src/*.cpp")

# Setting the executable to build
add_executable(${EXEC} ${SOURCES})

#-----------------------------
# Setting common dependencies
#-----------------------------

# math.h
IF(UNIX)
  TARGET_LINK_LIBRARIES(${EXEC} m)
ENDIF(UNIX)

# cblas.h
IF(UNIX)
  TARGET_LINK_LIBRARIES(${EXEC} cblas)
ENDIF(UNIX)

# pthreads.h
#FIND_PACKAGE(Threads)
IF(UNIX)
  TARGET_LINK_LIBRARIES(${EXEC} pthread)
ENDIF(UNIX)



#----------------
# CUSTOM MODULES
#----------------

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")