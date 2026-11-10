// Item 14, opțiunea 4: galerie::LacatTransferabil are un membru std::unique_ptr, deci copy
// constructor-ul generat de compilator e șters (Item 5). Lacătul se poate doar muta, cu std::move.

#include <galerie/LacatTransferabil.hpp>
#include <galerie/Mutex.hpp>

#include <utility>

int main()
{
    galerie::Mutex mutex{"galerie"};
    galerie::LacatTransferabil lacat(&mutex);

    galerie::LacatTransferabil copie(lacat);                // eroare: nu se poate copia
    galerie::LacatTransferabil mutat(std::move(lacat));     // corect: transfer
}
