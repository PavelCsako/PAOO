// Item 6, varianta C++98: o funcție prietenă (sau membră) ARE acces la copy constructor-ul privat,
// deci codul de mai jos COMPILEAZĂ. Dar copy constructor-ul nu e definit nicăieri, așa că eroarea
// apare abia la LINK-EDITARE ("undefined reference").

#include "exemple_item6.hpp"

namespace exemple {

void fotocopiaza(const DiplomaCpp98& diploma)
{
    DiplomaCpp98 copie(diploma);    // compilează (friend), dar nu se poate lega: funcția nu are cod
    copie.afiseaza();
}

} // namespace exemple

int main()
{
    exemple::DiplomaCpp98 diploma("Ana Popescu");
    fotocopiaza(diploma);
}
