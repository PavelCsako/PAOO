#ifndef DEMONSTRATII_HPP
#define DEMONSTRATII_HPP

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>

// Câte o funcție pentru fiecare item din Effective C++ (item4.cpp, item5.cpp, item6.cpp)
void demonstreazaItem4();
void demonstreazaItem5();
void demonstreazaItem6();

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

// ---------------------------------------------------------------------------
// Tabel cu funcțiile speciale pe care le are o clasă (Item 5 și Item 6).
// Răspunsurile vin direct de la compilator: std::is_..._v<T> se evaluează la compilare
// și spun dacă expresia respectivă ar compila pentru tipul T.
// ---------------------------------------------------------------------------

inline void antetFunctiiSpeciale()
{
    std::cout << "  " << std::left << std::setw(28) << "clasa" << std::right << std::setw(11) << "implicit"
              << std::setw(11) << "copy ctor" << std::setw(11) << "copy =" << std::setw(11) << "move ctor"
              << std::setw(11) << "move =" << std::setw(11) << "destructor" << std::endl;
}

inline const char* daNu(bool raspuns)
{
    return raspuns ? "da" : "NU";
}

template <typename T>
void linieFunctiiSpeciale(const std::string& numeClasa)
{
    std::cout << "  " << std::left << std::setw(28) << numeClasa << std::right
              << std::setw(11) << daNu(std::is_default_constructible_v<T>)
              << std::setw(11) << daNu(std::is_copy_constructible_v<T>)
              << std::setw(11) << daNu(std::is_copy_assignable_v<T>)
              << std::setw(11) << daNu(std::is_move_constructible_v<T>)
              << std::setw(11) << daNu(std::is_move_assignable_v<T>)
              << std::setw(11) << daNu(std::is_destructible_v<T>) << std::endl;
}

#endif // DEMONSTRATII_HPP
