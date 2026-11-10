#ifndef DEMONSTRATII_HPP
#define DEMONSTRATII_HPP

#include <cstddef>
#include <string>

// Câte o funcție pentru fiecare item din Effective C++

// Lucrarea 5
void demonstreazaItem10();          // item10.cpp
void demonstreazaItem11();          // item11.cpp
void demonstreazaItem12();          // item12.cpp
void demonstreazaMutarea();         // mutare.cpp (move assignment, C++11)

// Lucrarea 6
void demonstreazaItem13();          // item13.cpp
void demonstreazaItem14();          // item14.cpp

// Utilitare pentru afișare (main.cpp)
void titlu(const std::string& text);
void subtitlu(const std::string& text);

///\brief Completează textul cu spații până la `latime` caractere afișate.
/// (std::setw numără octeți, iar literele cu diacritice ocupă 2 octeți în UTF-8.)
inline std::string aliniaza(const std::string& text, std::size_t latime)
{
    std::size_t caractere = 0;
    for (unsigned char octet : text) {
        if ((octet & 0xC0) != 0x80) {   // octeții de continuare UTF-8 nu sunt caractere noi
            ++caractere;
        }
    }
    return caractere >= latime ? text : text + std::string(latime - caractere, ' ');
}

#endif // DEMONSTRATII_HPP
