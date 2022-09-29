cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_CXX_STANDARD EQUAL 20)
    message(FATAL_ERROR "DaqGrimoire requires C++20, please set CMAKE_CXX_STANDARD to 20")
endif()

project(DaqGrimoire)

set(DG_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(DG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(DaqGrimoire INTERFACE)

target_include_directories(DaqGrimoire INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(DEFINED GRIMOIRE_ASIO_INCLUDE_PATH)
    target_compile_definitions(DaqGrimoire DG_HAS_ASIO)
    target_include_directories(DaqGrimoire PRIVATE ${GRIMOIRE_ASIO_INCLUDE_PATH})
endif()