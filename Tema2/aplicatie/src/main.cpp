#include "demonstratii.hpp"

#include <secretariat/Diploma.hpp>

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
    std::cout << std::endl
              << "PAOO - Tema 2: Effective C++, Item 4, 5 și 6" << std::endl
              << "(Mesajele de deasupra au apărut ÎNAINTE de main(): sunt constructorii obiectelor globale," << std::endl
              << " explicați la Item 4.d.)" << std::endl;

    demonstreazaItem4();
    demonstreazaItem5();
    demonstreazaItem6();

    titlu("Final");
    std::cout << "Diplome secretariat::Diploma rămase în circulație: " << secretariat::Diploma::inCirculatie()
              << std::endl
              << "După return, obiectele globale se distrug în ordinea inversă construirii lor." << std::endl;

    return 0;
}
