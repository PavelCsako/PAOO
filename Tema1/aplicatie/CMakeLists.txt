# Proiectul 2: aplicația care folosește biblioteca statică "catalog"
cmake_minimum_required(VERSION 3.10)

project(aplicatie LANGUAGES CXX)

# Standardul C++ cerut
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dacă proiectul e compilat singur (nu din CMakeLists.txt-ul principal),
# aducem biblioteca din directorul vecin
if(NOT TARGET catalog)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../catalog ${CMAKE_CURRENT_BINARY_DIR}/catalog)
endif()

set(myfiles src/main.cpp src/exemple.hpp)

add_executable(${PROJECT_NAME} ${myfiles})

# Legăm executabilul de biblioteca statică; calea spre catalog/inc vine automat de la ea
target_link_libraries(${PROJECT_NAME} PRIVATE catalog)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
