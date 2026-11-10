# Script rulat de ținta "make erori" (cmake -P verifica_erori.cmake).
# Pentru fiecare fișier .cpp din erori_compilare/: încearcă să-l compileze ȘI să-l lege (link),
# apoi afișează doar liniile cu eroarea. Un exemplu e "corect" dacă NU reușește.

file(GLOB surse "${SURSE}/*.cpp")
list(SORT surse)

set(refuzate 0)
set(acceptate 0)

foreach(sursa IN LISTS surse)
    get_filename_component(nume "${sursa}" NAME)

    execute_process(
        COMMAND "${COMPILATOR}" -std=c++17
                "-I${INCLUDE_BIBLIOTECA}" "-I${INCLUDE_APLICATIE}"
                "${sursa}" -o "${IESIRE}/${nume}.out"
        RESULT_VARIABLE rezultat
        OUTPUT_VARIABLE mesaje
        ERROR_VARIABLE mesaje)

    message("")
    message("=== ${nume}")

    if(rezultat EQUAL 0)
        message("    !!! a compilat, deși NU trebuia")
        math(EXPR acceptate "${acceptate} + 1")
    else()
        # Păstrăm doar liniile cu eroarea propriu-zisă (de la compilator sau de la linker)
        string(REGEX MATCHALL "[^\n]*(error:|undefined reference)[^\n]*" linii "${mesaje}")
        foreach(linie IN LISTS linii)
            string(REPLACE "${SURSE}/" "" linie "${linie}")      # fără calea completă a fișierului
            message("    ${linie}")
        endforeach()
        message("    => refuzat, exact cum ne așteptam")
        math(EXPR refuzate "${refuzate} + 1")
    endif()
endforeach()

message("")
if(acceptate GREATER 0)
    message(FATAL_ERROR "${acceptate} exemple au compilat, deși nu trebuiau")
endif()
message("Rezultat: toate cele ${refuzate} exemple au fost refuzate de compilator / linker.")
