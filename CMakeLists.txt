project(SimplePusher)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release")
set(platform x64)
set(THIRDPARTY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")

#加载自定义模块
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    message(STATUS "Release版本")
    set(BuildType "Release")
else ()
    set(BuildType "Debug")
    message(STATUS "Debug版本")
endif ()

#设置bin和lib库目录
set(RELEASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/release)
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(LIBRARY_OUTPUT_PATH ${RELEASE_DIR}/linux/${BuildType})
    SET(EXECUTABLE_OUTPUT_PATH ${RELEASE_DIR}/linux/${BuildType})
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(LIBRARY_OUTPUT_PATH ${RELEASE_DIR}/windows/${BuildType})
    SET(EXECUTABLE_OUTPUT_PATH ${RELEASE_DIR}/windows/${BuildType})
endif ()

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

if(WIN32)
    set(LINK_LIB_LIST WS2_32 Iphlpapi shlwapi)
else()
    set(LINK_LIB_LIST)
endif()

set(SimplePusher_Root ${CMAKE_CURRENT_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${SimplePusher_Root})

#s_pusher
set(S_PUSHER_ROOT_PATH ${THIRDPARTY}/S_Pusher)
set(S_PUSHER_INCLUDE_PATH ${S_PUSHER_ROOT_PATH}/include)
set(S_PUSHER_LIB_PATH ${S_PUSHER_ROOT_PATH}/lib/${CMAKE_SYSTEM_NAME}/${platform})
INCLUDE_DIRECTORIES(${S_PUSHER_INCLUDE_PATH})
link_directories(${S_PUSHER_LIB_PATH})

#mp4v2
set(MP4V2_ROOT_PATH ${THIRDPARTY}/mp4v2)
set(MP4V2_INCLUDE_PATH ${MP4V2_ROOT_PATH}/include)
set(MP4V2_LIB_PATH ${MP4V2_ROOT_PATH}/lib/${CMAKE_SYSTEM_NAME}/${platform})
INCLUDE_DIRECTORIES(${MP4V2_INCLUDE_PATH})
link_directories(${MP4V2_LIB_PATH})

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /Zc:sizedDealloc- /MP /wd4430")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} /MTd /Od /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} /MT /O2")
else (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fpermissive")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")
endif (WIN32)

if (WIN32)
    #防止Windows.h包含Winsock.h
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DMP4V2_USE_STATIC_LIB)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

file(GLOB SimplePusher_src_list ${SimplePusher_Root}/*/*.cpp ${SimplePusher_Root}/*/*.h ${SimplePusher_Root}/*/*.c)

add_subdirectory(src)
