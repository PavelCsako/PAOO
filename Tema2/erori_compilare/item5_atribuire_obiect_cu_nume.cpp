// Item 5: compilatorul refuză să genereze operator= pentru o clasă cu un membru referință
// sau un membru const (exemplul NamedObject din carte).

#include "exemple_item5.hpp"

#include <string>

int main()
{
    std::string catelNou("Azorel");
    std::string catelBatran("Grivei");

    exemple::ObiectCuNume<int> p(catelNou, 2);
    exemple::ObiectCuNume<int> s(catelBatran, 36);

    exemple::ObiectCuNume<int> copie(p);    // OK: copy constructor-ul se poate genera
    p = s;                                  // eroare: operator= e șters (deleted)
}
