# Proiectul 2: aplicația care folosește biblioteca statică "secretariat"
cmake_minimum_required(VERSION 3.10)

project(aplicatie LANGUAGES CXX)

# Standardul C++ cerut
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dacă proiectul e compilat singur (nu din CMakeLists.txt-ul principal),
# aducem biblioteca din directorul vecin
if(NOT TARGET secretariat)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../secretariat ${CMAKE_CURRENT_BINARY_DIR}/secretariat)
endif()

set(myfiles
    # Item 4 (ordinea de inițializare a obiectelor globale): fiasco_grupe.cpp e pus INTENȚIONAT
    # înaintea lui fiasco_configuratie.cpp. Cu GCC + ld, obiectele globale se construiesc
    # în ordinea în care fișierele ajung la linker, dar standardul C++ NU garantează nicio ordine
    # între fișiere diferite. Inversați cele două linii și rezultatul se schimbă.
    src/fiasco_grupe.cpp
    src/fiasco_configuratie.cpp
    src/fiasco.hpp

    src/main.cpp
    src/demonstratii.hpp
    src/item4.cpp   src/exemple_item4.hpp
    src/item5.cpp   src/exemple_item5.hpp
    src/item6.cpp   src/exemple_item6.hpp)

add_executable(${PROJECT_NAME} ${myfiles})

# Legăm executabilul de biblioteca statică; calea spre secretariat/inc vine automat de la ea
target_link_libraries(${PROJECT_NAME} PRIVATE secretariat)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Item 4: exemplele greșite din exemple_item4.hpp produc avertismente -Wreorder și -Wuninitialized.
# Implicit sunt oprite (doar pentru acele clase); cu  cmake -DARATA_AVERTISMENTE=ON ..  se văd la compilare.
option(ARATA_AVERTISMENTE "Afișează avertismentele GCC pentru exemplele greșite de la Item 4" OFF)
if(ARATA_AVERTISMENTE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ARATA_AVERTISMENTE)
endif()
