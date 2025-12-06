cmake_minimum_required(VERSION 3.16)

project(openitemdef LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(OPENITEMDEF_BUILD_TESTS "Build tests" OFF)
option(OPENITEMDEF_ENABLE_COVERAGE "Build with coverage instrumentation" OFF)
# option(OPENITEMDEF_BUILD_DOC "Build Doxygen documentation" OFF)

if(OPENITEMDEF_ENABLE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang"
       AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Code coverage instrumentation enabled")

        # Cible de flags partagés
        add_library(openitemdef_coverage_flags INTERFACE)

        target_compile_options(openitemdef_coverage_flags INTERFACE --coverage -O0 -g)
        target_link_options(openitemdef_coverage_flags    INTERFACE --coverage)

        # Helper pour appliquer les flags à une cible donnée
        function(openitemdef_enable_coverage_for target_name)
            if(TARGET "${target_name}")
                target_link_libraries("${target_name}" PRIVATE openitemdef_coverage_flags)
            endif()
        endfunction()
    else()
        message(FATAL_ERROR "Coverage is only supported with GCC/Clang in Debug")
    endif()
endif()

add_subdirectory(src)

if(OPENITEMDEF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(third_party/unity EXCLUDE_FROM_ALL)
    add_subdirectory(tests)
endif()

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found: docs target disabled")
endif()
