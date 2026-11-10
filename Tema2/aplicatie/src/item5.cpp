// Item 5: Know what functions C++ silently writes and calls.

#include "demonstratii.hpp"
#include "exemple_item5.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// Dovezi verificate la compilare: dacă vreuna ar fi falsă, programul NU s-ar mai compila.
static_assert(std::is_default_constructible_v<exemple::Gol> && std::is_copy_constructible_v<exemple::Gol>
                  && std::is_copy_assignable_v<exemple::Gol> && std::is_move_constructible_v<exemple::Gol>
                  && std::is_move_assignable_v<exemple::Gol> && std::is_destructible_v<exemple::Gol>,
              "Gol nu declară nimic, dar are toate funcțiile speciale, generate de compilator");
static_assert(!std::has_virtual_destructor_v<exemple::Gol>, "destructorul generat este nevirtual");
static_assert(!std::is_default_constructible_v<exemple::FaraConstructorImplicit>,
              "am declarat un constructor => nu se mai generează cel implicit");
static_assert(std::is_copy_constructible_v<exemple::ObiectCuNume<int>>,
              "copy constructor-ul se poate genera: referința și const-ul sunt doar inițializate");
static_assert(!std::is_copy_assignable_v<exemple::ObiectCuNume<int>>,
              "operator= nu se poate genera: referința și const-ul ar trebui modificate");

namespace {

void clasaGoala()
{
    subtitlu("5.a) class Gol {}; -- compilatorul îi scrie singur funcțiile speciale");

    exemple::Gol a;                     // constructor implicit generat
    exemple::Gol b(a);                  // copy constructor generat
    b = a;                              // copy assignment generat
    exemple::Gol c(std::move(a));       // move constructor generat (C++11)
    c = std::move(b);                   // move assignment generat (C++11)

    std::cout << "  Gol a;   Gol b(a);   b = a;   Gol c(std::move(a));   c = std::move(b);" << std::endl
              << "  => toate compilează, deși Gol nu declară nicio funcție. Compilatorul le generează doar dacă"
              << std::endl
              << "     sunt folosite, public și inline. Destructorul generat e nevirtual (std::has_virtual_destructor_v"
              << std::endl
              << "     = " << std::boolalpha << std::has_virtual_destructor_v<exemple::Gol> << std::noboolalpha
              << ") pentru că nici clasa de bază nu are destructor virtual." << std::endl;
}

void functiiGenerateApeleazaMembrii()
{
    subtitlu("5.b) Funcțiile generate apelează, în tăcere, funcțiile bazei și ale fiecărui membru");
    std::cout << "exemple::FisaGenerata : public DocumentDeBaza { Martor nume_; Martor nota_; } nu declară nicio funcție."
              << std::endl;

    std::cout << "  FisaGenerata a;                // constructor implicit generat" << std::endl;
    exemple::FisaGenerata a;

    std::cout << "  FisaGenerata b(a);             // copy constructor generat" << std::endl;
    exemple::FisaGenerata b(a);

    std::cout << "  b = a;                         // copy assignment generat" << std::endl;
    b = a;

    std::cout << "  FisaGenerata c(std::move(a));  // move constructor generat" << std::endl;
    exemple::FisaGenerata c(std::move(a));

    std::cout << "  c = std::move(b);              // move assignment generat" << std::endl;
    c = std::move(b);

    std::cout << "  => mereu: întâi clasa de bază, apoi membrii în ordinea declarării." << std::endl
              << "  La ieșirea din funcție, destructorii generați pentru c, b și a (fiecare: membrii invers, apoi baza):"
              << std::endl;
}

void faraConstructorImplicit()
{
    subtitlu("5.c) Dacă declarăm ORICE constructor, compilatorul nu mai generează constructorul implicit");

    exemple::FaraConstructorImplicit cuAn(2);
    std::cout << "  FaraConstructorImplicit cuAn(2);  -> merge, anul " << cuAn.an() << std::endl
              << "  FaraConstructorImplicit fara;     -> NU compilează: FaraConstructorImplicit() nu a fost generat"
              << std::endl;
    // error: no matching function for call to 'exemple::FaraConstructorImplicit::FaraConstructorImplicit()'
    // (eroarea completă: "make erori", fișierul item5_constructor_implicit_negenerat.cpp)
}

void atribuireRefuzata()
{
    subtitlu("5.d) Compilatorul refuză să genereze operator= dacă are membri referință sau const");

    std::string catelNou("Azorel");
    std::string catelBatran("Grivei");
    exemple::ObiectCuNume<int> p(catelNou, 2);
    exemple::ObiectCuNume<int> s(catelBatran, 36);
    exemple::ObiectCuNume<int> copie(p);    // copy constructor generat: referința și const-ul doar se INIȚIALIZEAZĂ

    std::cout << "  p = ";
    p.afiseaza();
    std::cout << ",  s = ";
    s.afiseaza();
    std::cout << ",  copie(p) = ";
    copie.afiseaza();
    std::cout << std::endl;

    // p = s;
    // error: use of deleted function 'exemple::ObiectCuNume<int>& exemple::ObiectCuNume<int>::operator=(const exemple::ObiectCuNume<int>&)'
    // (eroarea completă: "make erori", fișierul item5_atribuire_obiect_cu_nume.cpp)
    std::cout << "  p = s;  -> NU compilează. Ce ar trebui să facă operator= generat?" << std::endl
              << "    - să lege referința p.nume_ de alt string? Imposibil: o referință nu se poate re-lega;" << std::endl
              << "    - să modifice stringul \"" << catelNou
              << "\"? Atunci s-ar schimba și pentru celelalte obiecte care îl folosesc;" << std::endl
              << "    - să modifice p.valoare_? E const." << std::endl
              << "  => compilatorul nu ghicește: declară operator= ca șters (deleted). Copy constructor-ul, în schimb,"
              << std::endl
              << "     se generează, pentru că el doar inițializează membrii. La fel se întâmplă dacă clasa de bază"
              << std::endl
              << "     are operator= privat: vezi Necopiabil, la Item 6." << std::endl;
}

void destructorulOpresteMutarea()
{
    subtitlu("5.e) (C++11) Un destructor scris de noi oprește, în tăcere, generarea move-ului");

    std::cout << "  FaraDestructorDeclarat y(std::move(x));" << std::endl;
    {
        exemple::FaraDestructorDeclarat x;
        exemple::FaraDestructorDeclarat y(std::move(x));
    }

    std::cout << "  CuDestructorDeclarat y(std::move(x));" << std::endl;
    {
        exemple::CuDestructorDeclarat x;
        exemple::CuDestructorDeclarat y(std::move(x));
    }

    std::cout << "  => cu un destructor declarat (chiar și gol), compilatorul nu mai generează move constructor-ul:"
              << std::endl
              << "     \"mutarea\" folosește copy constructor-ul. Pentru un membru mare, asta înseamnă o copie inutilă."
              << std::endl;
}

void rezumat()
{
    subtitlu("Rezumat: funcțiile speciale disponibile pentru fiecare clasă (răspunsul compilatorului)");
    antetFunctiiSpeciale();
    linieFunctiiSpeciale<exemple::Gol>("Gol");
    linieFunctiiSpeciale<exemple::FisaGenerata>("FisaGenerata");
    linieFunctiiSpeciale<exemple::FaraConstructorImplicit>("FaraConstructorImplicit");
    linieFunctiiSpeciale<exemple::ObiectCuNume<int>>("ObiectCuNume<int>");
}

} // namespace

void demonstreazaItem5()
{
    titlu("Item 5: Know what functions C++ silently writes and calls");

    clasaGoala();
    functiiGenerateApeleazaMembrii();
    faraConstructorImplicit();
    atribuireRefuzata();
    destructorulOpresteMutarea();
    rezumat();
}
