// Item 5: compilatorul generează constructorul implicit DOAR dacă clasa nu declară niciun constructor.
// FaraConstructorImplicit declară FaraConstructorImplicit(int), deci nu mai are FaraConstructorImplicit().

#include "exemple_item5.hpp"

int main()
{
    exemple::FaraConstructorImplicit fara;      // eroare: nu există constructor fără parametri
}
