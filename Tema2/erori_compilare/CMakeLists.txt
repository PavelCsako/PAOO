# Proiectul 3 (opțional): exemple scrise INTENȚIONAT greșit, care NU trebuie să compileze.
# Nu fac parte din build-ul obișnuit ("make"). Se verifică separat, din directorul build:
#
#     make erori
#
# Ținta compilează fiecare fișier .cpp de aici, afișează eroarea reală dată de compilator
# (sau de linker) și se oprește cu eroare dacă vreun exemplu reușește totuși să compileze.

add_custom_target(erori
    COMMAND ${CMAKE_COMMAND}
            -DCOMPILATOR=${CMAKE_CXX_COMPILER}
            -DSURSE=${CMAKE_CURRENT_SOURCE_DIR}
            -DINCLUDE_BIBLIOTECA=${CMAKE_CURRENT_SOURCE_DIR}/../secretariat/inc
            -DINCLUDE_APLICATIE=${CMAKE_CURRENT_SOURCE_DIR}/../aplicatie/src
            -DIESIRE=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/verifica_erori.cmake
    COMMENT "Compilez exemplele care NU trebuie să compileze"
    VERBATIM)
