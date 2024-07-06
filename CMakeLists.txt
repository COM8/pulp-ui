cmake_minimum_required(VERSION 3.27)

set(PROJECT_DESCRIPTION "GTK4 UI for Pulp.")

project("pulp-ui" VERSION 1.0.0
    DESCRIPTION "${PROJECT_DESCRIPTION}"
    HOMEPAGE_URL "https://github.com/COM8/pulp-ui")
set(VERSION_NAME "dev")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

macro(pulp_ui_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})

    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()

    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()

    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()

message(STATUS "C++ Pulp UI Options")
message(STATUS "=======================================================")
pulp_ui_option(PULP_UI_STATIC_ANALYZE "Set to ON to enable the GCC 10 static analysis. If enabled, PULP_UI_ENABLE_LINTING has to be disabled." OFF)
pulp_ui_option(PULP_UI_ENABLE_LINTING "Set to ON to enable clang linting. If enabled, PULP_UI_STATIC_ANALYZE has to be disabled." OFF)
pulp_ui_option(PULP_UI_FLATPAK_BUILD "Set to ON in case it is built for flatpak." OFF)
message(STATUS "=======================================================")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(sanitizer)
include(gcc_analyze)
include(glib-resource-compiler)
include(clear_variable)
include(FetchContent)

if(${PULP_UI_ENABLE_LINTING})
    message(STATUS "Enabling linting")
    include(clang-tidy)
else()
    message(STATUS "Linting is disabled")
endif()

if(PULP_UI_FLATPAK_BUILD)
    add_compile_definitions(PULP_UI_FLATPAK_BUILD=1)
endif()

# gtk4 stuff
include(glib-resource-compiler)
find_package(PkgConfig REQUIRED)

# Additional search path since gtkmm-4.0 will usually be installed in /usr
if(PULP_UI_FLATPAK_BUILD)
    set(ENV{PKG_CONFIG_PATH} "/app/lib/pkgconfig")
else()
    set(ENV{PKG_CONFIG_PATH} "/usr/lib/pkgconfig")
endif()

pkg_check_modules(GTKMM gtkmm-4.0>=4.4.0 REQUIRED IMPORTED_TARGET)
pkg_check_modules(EPOXY epoxy REQUIRED IMPORTED_TARGET)

# Lib adwaita
pkg_check_modules(ADWAITA REQUIRED libadwaita-1>=1.1.0)
include_directories(${ADWAITA_INCLUDE_DIRS})

if(PULP_UI_FLATPAK_BUILD)
    find_package(cpr CONFIG REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(eventpp CONFIG REQUIRED)
    find_package(date CONFIG REQUIRED)
else()
    # Disable linting for fetch content projects
    clear_variable(DESTINATION CMAKE_CXX_CLANG_TIDY BACKUP CMAKE_CXX_CLANG_TIDY_BKP)

    set(CPR_BUILD_TESTS OFF CACHE INTERNAL " " FORCE)
    set(CURL_CA_FALLBACK ON CACHE INTERNAL " " FORCE) # Workaround for failing SSL requests
    FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 923f83a5946f8d276be757d7808101aafc900dd6) # master as of 29.06.2024
    FetchContent_MakeAvailable(cpr)

    FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3)
    FetchContent_MakeAvailable(json)

    set(SPDLOG_INSTALL ON CACHE INTERNAL " " FORCE)
    FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1)
    FetchContent_MakeAvailable(spdlog)

    set(BUILD_TZ_LIB ON CACHE INTERNAL " " FORCE)
    FetchContent_Declare(date GIT_REPOSITORY https://github.com/HowardHinnant/date.git
        GIT_TAG 1ead6715dec030d340a316c927c877a3c4e5a00c)
    FetchContent_MakeAvailable(date)

    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.1)
    FetchContent_MakeAvailable(fmt)

    FetchContent_Declare(eventpp
        GIT_REPOSITORY https://github.com/wqking/eventpp.git
        GIT_TAG v0.1.3)
    FetchContent_MakeAvailable(eventpp)

    restore_variable(DESTINATION CMAKE_CXX_CLANG_TIDY BACKUP CMAKE_CXX_CLANG_TIDY_BKP)
endif()

if(PULP_UI_FLATPAK_BUILD)
    # Disable Werror flags for flatpak builds to prevent issues with outdated flatpak CIs
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

if(CMAKE_BUILD_TYPE_LOWER MATCHES ".*release.*")
    message(STATUS "Some sort of release build detected.")
else()
    message(STATUS "Some sort of debug build detected.")
    add_compile_definitions(DEBUG)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
