// Item 14, opțiunea 1: galerie::Lacat interzice copierea (= delete), pentru că o copie a unui
// lacăt ar debloca mutex-ul a doua oară. Orice copiere, inclusiv transmiterea prin valoare, e o eroare.

#include <galerie/Lacat.hpp>
#include <galerie/Mutex.hpp>

void scrieInJurnal(galerie::Lacat lacat);   // parametru prin valoare => ar cere o copie

int main()
{
    galerie::Mutex mutex{"galerie"};
    galerie::Lacat lacat(&mutex);

    galerie::Lacat copie(lacat);            // eroare: copy constructor șters
    scrieInJurnal(lacat);                   // eroare: transmiterea prin valoare copiază
}
