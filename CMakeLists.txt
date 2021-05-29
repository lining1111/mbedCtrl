cmake_minimum_required(VERSION 3.19)
project(mbedCtrl VERSION 0.0.1)
configure_file("${PROJECT_SOURCE_DIR}/include/version.h.in" "${PROJECT_SOURCE_DIR}/include/version.h")

set(CMAKE_CXX_STANDARD 11)

#根据实际情况设置
#set(ARCH arm)
set(ARCH x86)
#set(ARCH arm64)

if (${ARCH} STREQUAL arm)
    set(CROSS_COMPILE /usr/local/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-)

    set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
    set(CMAKE_C_COMPILER_AR ${CROSS_COMPILE}ar)
    set(CMAKE_C_COMPILER_RANLIB ${CROSS_COMPILE}ranlib)
    set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
endif ()

if (${ARCH} STREQUAL arm64)
    set(CROSS_COMPILE /usr/local/arm64/gcc-linaro-4.9-2016.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-)

    set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
    set(CMAKE_C_COMPILER_AR ${CROSS_COMPILE}ar)
    set(CMAKE_C_COMPILER_RANLIB ${CROSS_COMPILE}ranlib)
    set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
endif ()


add_definitions(-D${ARCH})

include_directories(${PROJECT_SOURCE_DIR}/include)
set(LINK_LIBS m pthread mysqlclient zlog crypto ssl curl freefare nfc jsoncpp rabbitmq)

set(SRC_DIRS src
        src/config
        src/ping
        src/dev_chargingPile
        )

foreach (src_dir ${SRC_DIRS})
    aux_source_directory(${src_dir} src_dir_file)
    set(SRC_FILES ${SRC_FILES} ${src_dir_file})
endforeach (src_dir)

#MyLibs
set(LIBCommon_FILES src/common/common_json.cpp
        src/common/common_database.cpp)
add_library(myCommon STATIC ${LIBCommon_FILES})

set(LIBDatabase_FILES src/database/DataBase.cpp)
add_library(myDatabase STATIC ${LIBDatabase_FILES})

set(LIBLog_FILES src/log/log.cpp)
add_library(myLog STATIC ${LIBLog_FILES})

set(LIBRingBuffer_FILES src/ringBuffer/RingBuffer.cpp)
add_library(myRingBuffer STATIC ${LIBRingBuffer_FILES})

#server
set(LIBServer_FILES src/server_interface/Server_Interface.cpp
        src/server_interface/ClientInfo.cpp)
add_library(myServer STATIC ${LIBServer_FILES})
#client
set(LIBClient_FILES src/client_interface/Client_Interface.cpp)
add_library(myClient STATIC ${LIBClient_FILES})
#rabbitmq
set(LIBRabbitmq_FILES src/rabbitmq_interface/Rabbitmq_Interface.cpp)
add_library(myRabbitmq STATIC ${LIBRabbitmq_FILES})

#ota
set(LIBOta_FILES src/ota/Ota.cpp)
add_library(myOta STATIC ${LIBOta_FILES})

set(LINK_LIBS myServer myClient myRabbitmq
        myRingBuffer myOta myDatabase myLog myCommon ${LINK_LIBS})

add_subdirectory(tools)
add_subdirectory(test)

add_executable(mbedCtrl main.cpp ${SRC_FILES})
target_link_libraries(mbedCtrl ${LINK_LIBS})


##doxygen
message(STATUS "Build documentation open")
include(docs/build_doxygen.cmake)
build_doxygen()
