// Item 6, varianta modernă (C++11): secretariat::Diploma are copierea ștearsă cu "= delete".
// Orice copiere, oriunde ar fi scrisă, e o eroare de COMPILARE cu un mesaj clar.
// Bonus (Item 5): pentru că am declarat copierea, nici mutarea nu mai e generată.

#include <secretariat/Diploma.hpp>

#include <utility>

void trimiteLaAngajator(secretariat::Diploma diploma)   // parametru prin valoare => ar cere o copie
{
    diploma.afiseaza();
}

int main()
{
    secretariat::Diploma ana("Ana Popescu", "Calculatoare");
    secretariat::Diploma bogdan("Bogdan Ionescu", "Automatică");

    secretariat::Diploma copie = ana;               // eroare: copy constructor șters
    bogdan = ana;                                   // eroare: operator= șters
    trimiteLaAngajator(ana);                        // eroare: transmiterea prin valoare copiază
    secretariat::Diploma mutata = std::move(ana);   // eroare: nu există move constructor
}
