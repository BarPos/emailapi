cmake_minimum_required (VERSION 3.8)

project ("emailapi")

set(SYSTEM_OPENSSL ON)
add_subdirectory("external/openssl-cmake")


add_subdirectory("external/curl")

SET(CMAKE_CXX_STANDARD 20)

add_executable("emailapi")

set(OS_LIB "")
set(OS_DEF "")
if (WIN32)
    set(OS_DEF "_WINDOWS")
endif (WIN32)
if (UNIX)
    set(OS_DEF "_LINUX")
    set(OS_LIB "pthread")
endif (UNIX)

target_compile_definitions("emailapi" PRIVATE "FMT_HEADER_ONLY" "${OS_DEF}")

set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/curl/include")

target_include_directories("emailapi" PRIVATE "${CMAKE_SOURCE_DIR}/src/" "${CMAKE_SOURCE_DIR}/external/asio/include" "${CURL_INCLUDE_DIR}" "${CMAKE_SOURCE_DIR}/external/fmt/include")
target_link_libraries("emailapi" PUBLIC CURL::libcurl "${OS_LIB}")

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/**.cpp")
target_sources("emailapi" PRIVATE "${APP_SOURCES}")
