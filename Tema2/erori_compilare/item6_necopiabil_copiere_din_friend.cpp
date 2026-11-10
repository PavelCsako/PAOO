// Item 6, varianta cu clasa de bază Necopiabil (Uncopyable): aceeași funcție prietenă ca în
// item6_cpp98_copiere_din_friend.cpp, dar acum eroarea apare deja la COMPILARE.
// Copy constructor-ul lui DiplomaNecopiabila ar trebui generat de compilator, iar cel generat
// ar apela copy constructor-ul privat din Necopiabil => compilatorul îl declară șters (deleted).

#include "exemple_item6.hpp"

namespace exemple {

void fotocopiaza(const DiplomaNecopiabila& diploma)
{
    DiplomaNecopiabila copie(diploma);  // eroare de compilare, chiar și într-un friend
    copie.afiseaza();
}

} // namespace exemple

int main()
{
    exemple::DiplomaNecopiabila diploma("Ana Popescu");
    fotocopiaza(diploma);
}
