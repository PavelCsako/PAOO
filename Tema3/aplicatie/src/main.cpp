#include "demonstratii.hpp"

#include <galerie/Bitmap.hpp>

#include <iostream>
#include <string>

void titlu(const std::string& text)
{
    std::cout << std::endl
              << "======================================================================" << std::endl
              << " " << text << std::endl
              << "======================================================================" << std::endl;
}

void subtitlu(const std::string& text)
{
    std::cout << std::endl << "--- " << text << std::endl;
}

int main()
{
    std::cout << "PAOO - Tema 3: Effective C++" << std::endl
              << "Lucrarea 5: Item 10, 11, 12 (+ move assignment din C++11)" << std::endl
              << "Pixelii sunt caractere pe scala de luminozitate \" .:-=+*#%@\" (întuneric -> lumină)." << std::endl;

    demonstreazaItem10();
    demonstreazaItem11();
    demonstreazaItem12();
    demonstreazaMutarea();

    titlu("Final");
    std::cout << "Bitmap-uri create în total: " << galerie::Bitmap::totalCreate() << std::endl
              << "Bitmap-uri rămase în viață: " << galerie::Bitmap::inViata()
              << " (leak-ul intenționat de la move assignment-ul greșit)" << std::endl;

    return 0;
}
