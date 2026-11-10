# Proiectul 2: aplicația care folosește biblioteca statică "galerie"
cmake_minimum_required(VERSION 3.10)

project(aplicatie LANGUAGES CXX)

# Standardul C++ cerut
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dacă proiectul e compilat singur (nu din CMakeLists.txt-ul principal),
# aducem biblioteca din directorul vecin
if(NOT TARGET galerie)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../galerie ${CMAKE_CURRENT_BINARY_DIR}/galerie)
endif()

set(myfiles
    src/main.cpp
    src/demonstratii.hpp

    # Lucrarea 5: Item 10, 11, 12 (+ move assignment)
    src/exemple_imagini.hpp
    src/item10.cpp
    src/item11.cpp
    src/exemple_item12.hpp
    src/item12.cpp
    src/mutare.cpp

    # Lucrarea 6: Item 13, 14
    src/item13.cpp
    src/exemple_item14.hpp
    src/item14.cpp)

add_executable(${PROJECT_NAME} ${myfiles})

# Legăm executabilul de biblioteca statică; calea spre galerie/inc vine automat de la ea
target_link_libraries(${PROJECT_NAME} PRIVATE galerie)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Item 12: GCC avertizează (-Wextra) când un copy constructor scris de mână uită clasa de bază.
# Pentru exemplul greșit avertismentul e oprit; cu  cmake -DARATA_AVERTISMENTE=ON ..  se vede.
option(ARATA_AVERTISMENTE "Afișează avertismentele GCC pentru exemplele scrise intenționat greșit" OFF)
if(ARATA_AVERTISMENTE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ARATA_AVERTISMENTE)
endif()
