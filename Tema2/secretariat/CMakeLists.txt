# Proiectul 1: biblioteca statică "secretariat"
cmake_minimum_required(VERSION 3.10)

project(secretariat VERSION 1.0 LANGUAGES CXX)

# Standardul C++ cerut
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(myfiles
    src/FisaStudent.cpp     inc/secretariat/FisaStudent.hpp     # Item 4
    src/Configuratie.cpp    inc/secretariat/Configuratie.hpp    # Item 4
    src/Martor.cpp          inc/secretariat/Martor.hpp          # Item 5
                            inc/secretariat/Necopiabil.hpp      # Item 6
    src/Diploma.cpp         inc/secretariat/Diploma.hpp)        # Item 6

# Biblioteca statică => libsecretariat.a
add_library(${PROJECT_NAME} STATIC ${myfiles})

# Directorul inc/ e exportat: cine leagă biblioteca poate scrie #include <secretariat/...>
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:inc>)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Exportul pentru "cmake --install": biblioteca în lib/, header-ele în inc/
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY inc/ DESTINATION inc)
