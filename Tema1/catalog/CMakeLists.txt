# Proiectul 1: biblioteca statică "catalog"
cmake_minimum_required(VERSION 3.10)

project(catalog VERSION 1.0 LANGUAGES CXX)

# Standardul C++ cerut
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(myfiles src/Student.cpp inc/catalog/Student.hpp)

# Biblioteca statică => libcatalog.a
add_library(${PROJECT_NAME} STATIC ${myfiles})

# Directorul inc/ e exportat: cine leagă biblioteca poate scrie #include <catalog/Student.hpp>
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Exportul pentru "cmake --install": biblioteca în lib/, header-ul în inc/
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY inc/ DESTINATION inc)
