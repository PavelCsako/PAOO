// Item 6: Explicitly disallow the use of compiler-generated functions you do not want.

#include "demonstratii.hpp"
#include "exemple_item6.hpp"

#include <secretariat/Diploma.hpp>

#include <iostream>
#include <type_traits>

// Dovezi verificate la compilare: dacă o diplomă protejată ar putea fi copiată,
// programul NU s-ar mai compila.
static_assert(std::is_copy_constructible_v<exemple::DiplomaCopiabila>
                  && std::is_copy_assignable_v<exemple::DiplomaCopiabila>,
              "fără protecție, compilatorul generează copierea");
static_assert(!std::is_copy_constructible_v<exemple::DiplomaCpp98> && !std::is_copy_assignable_v<exemple::DiplomaCpp98>,
              "copierea e privată");
static_assert(!std::is_copy_constructible_v<exemple::DiplomaNecopiabila>
                  && !std::is_copy_assignable_v<exemple::DiplomaNecopiabila>,
              "copierea nu poate fi generată, pentru că a bazei e privată");
static_assert(!std::is_copy_constructible_v<secretariat::Diploma> && !std::is_copy_assignable_v<secretariat::Diploma>,
              "copierea e ștearsă (= delete)");
static_assert(!std::is_move_constructible_v<secretariat::Diploma> && !std::is_move_assignable_v<secretariat::Diploma>,
              "copierea fiind declarată, nici mutarea nu se mai generează");

namespace {

void afiseazaDiploma(const secretariat::Diploma& diploma)     // prin referință: nicio copie
{
    std::cout << "    ";
    diploma.afiseaza();
}

void problemaCopierii()
{
    subtitlu("6.a) Problema: o diplomă pe care compilatorul o poate copia (exemple::DiplomaCopiabila)");
    {
        exemple::DiplomaCopiabila ana("Ana Popescu");
        exemple::DiplomaCopiabila bogdan("Bogdan Ionescu");

        std::cout << "  DiplomaCopiabila duplicat = ana;  // copy constructor generat de compilator" << std::endl;
        {
            exemple::DiplomaCopiabila duplicat = ana;
            std::cout << "    ana:      ";
            ana.afiseaza();
            std::cout << "    duplicat: ";
            duplicat.afiseaza();
            std::cout << "  (duplicatul iese din bloc și e arhivat, deși originalul există încă)" << std::endl;
        }

        std::cout << "  bogdan = ana;                     // operator= generat de compilator" << std::endl;
        bogdan = ana;
        std::cout << "    bogdan:   ";
        bogdan.afiseaza();

        std::cout << "  (ieșire din bloc: se distrug bogdan și ana)" << std::endl;
    }
    exemple::DiplomaCopiabila::afiseazaRegistrul();
    std::cout << "=> Au existat trei diplome cu seria 1, seria 1 a fost \"arhivată\" de trei ori, iar diploma nr. 2"
              << std::endl
              << "   a lui Bogdan a dispărut fără să fie arhivată vreodată. O diplomă e unică: copierea ei nu are"
              << std::endl
              << "   sens, deci trebuie interzisă explicit, altfel compilatorul o permite în tăcere." << std::endl;
}

void solutiaCpp98()
{
    subtitlu("6.b) Soluția din C++98: copy constructor și operator= private și NEDEFINITE (exemple::DiplomaCpp98)");

    exemple::DiplomaCpp98 diploma("Ana Popescu");
    std::cout << "    ";
    diploma.afiseaza();

    // exemple::DiplomaCpp98 copie(diploma);
    // error: 'exemple::DiplomaCpp98::DiplomaCpp98(const exemple::DiplomaCpp98&)' is private within this context
    std::cout << "  DiplomaCpp98 copie(diploma);  din exterior   -> eroare la COMPILARE: is private" << std::endl
              << "                                dintr-un friend -> compilează, dar eroare la LINK-EDITARE:" << std::endl
              << "                                                   undefined reference (funcția nu are cod)" << std::endl
              << "  => protecția funcționează, dar o greșeală din interiorul clasei e descoperită abia de linker,"
              << std::endl
              << "     mai târziu și cu un mesaj mai greu de urmărit decât o eroare de compilare." << std::endl;
}

void solutiaNecopiabil()
{
    subtitlu("6.c) Soluția din carte: clasa de bază secretariat::Necopiabil (exemple::DiplomaNecopiabila)");

    exemple::DiplomaNecopiabila diploma("Ana Popescu");
    std::cout << "    ";
    diploma.afiseaza();

    std::cout << "  DiplomaNecopiabila copie(diploma);  -> eroare la COMPILARE peste tot, inclusiv într-un friend:"
              << std::endl
              << "     copy constructor-ul derivatei nu poate fi generat, pentru că al bazei e privat (Item 5)." << std::endl
              << "  Clasa de bază goală nu costă memorie: sizeof(DiplomaNecopiabila) = "
              << sizeof(exemple::DiplomaNecopiabila) << " = sizeof(DiplomaCpp98) = " << sizeof(exemple::DiplomaCpp98)
              << " octeți." << std::endl;
}

void solutiaModerna()
{
    subtitlu("6.d) Soluția modernă (C++11): = delete (secretariat::Diploma, din biblioteca statică)");

    const int inainte = secretariat::Diploma::inCirculatie();
    {
        secretariat::Diploma ana("Ana Popescu", "Calculatoare");
        secretariat::Diploma bogdan("Bogdan Ionescu", "Automatică");
        afiseazaDiploma(ana);
        afiseazaDiploma(bogdan);

        // secretariat::Diploma copie = ana;               // error: use of deleted function 'Diploma(const Diploma&)'
        // bogdan = ana;                                    // error: use of deleted function 'operator=(const Diploma&)'
        // secretariat::Diploma mutata = std::move(ana);   // error: use of deleted function 'Diploma(const Diploma&)'
        std::cout << "  Diploma copie = ana;   bogdan = ana;   Diploma mutata = std::move(ana);" << std::endl
                  << "  -> toate trei: eroare la COMPILARE, oriunde ar fi scrise: use of deleted function" << std::endl
                  << "  Diplome în circulație: " << secretariat::Diploma::inCirculatie() - inainte
                  << " (fiecare cu seria ei)" << std::endl;
    }
    std::cout << "=> Față de varianta C++98: eroarea apare mereu la compilare, mesajul spune clar că funcția" << std::endl
              << "   e ștearsă, iar intenția se vede direct în declarația clasei. Încercarea cu std::move arată"
              << std::endl
              << "   și legătura cu Item 5: odată ce am declarat copierea, compilatorul nu mai generează nici mutarea."
              << std::endl;
}

void rezumat()
{
    subtitlu("Rezumat: funcțiile speciale disponibile pentru fiecare diplomă (răspunsul compilatorului)");
    antetFunctiiSpeciale();
    linieFunctiiSpeciale<exemple::DiplomaCopiabila>("DiplomaCopiabila");
    linieFunctiiSpeciale<exemple::DiplomaCpp98>("DiplomaCpp98");
    linieFunctiiSpeciale<exemple::DiplomaNecopiabila>("DiplomaNecopiabila");
    linieFunctiiSpeciale<secretariat::Diploma>("secretariat::Diploma");
    std::cout << "(\"implicit\" = NU peste tot: toate au un constructor cu parametri, deci cel implicit nu se generează.)"
              << std::endl
              << "Erorile reale de compilare și link-editare se văd cu: make erori (din directorul build)." << std::endl;
}

} // namespace

void demonstreazaItem6()
{
    titlu("Item 6: Explicitly disallow the use of compiler-generated functions you do not want");

    problemaCopierii();
    solutiaCpp98();
    solutiaNecopiabil();
    solutiaModerna();
    rezumat();
}
