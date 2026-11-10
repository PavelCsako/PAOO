#ifndef GALERIE_LACATTRANSFERABIL_HPP
#define GALERIE_LACATTRANSFERABIL_HPP

#include <galerie/Mutex.hpp>

#include <memory>

namespace galerie {

///\brief Opțiunea 4 din Item 14: transferul proprietății (ca std::unique_ptr).
///
/// Un singur obiect deține mutex-ul la un moment dat. Copierea e interzisă, dar mutarea
/// (std::move) e permisă și TRANSFERĂ lacătul: sursa rămâne goală și nu mai deblochează nimic.
///
/// Nu scriem nicio funcție specială: membrul std::unique_ptr nu se poate copia, dar se poate muta,
/// așa că și funcțiile generate de compilator pentru această clasă se comportă la fel (Item 5).
/// Deleter-ul este deblocheaza, ca la LacatPartajat.
class LacatTransferabil
{
public:
    explicit LacatTransferabil(Mutex* mutex);

public:
    bool detineMutex() const;           // false după ce lacătul a fost mutat în alt obiect

private:
    std::unique_ptr<Mutex, void (*)(Mutex*)> mutex_;
};

} // namespace galerie

#endif // GALERIE_LACATTRANSFERABIL_HPP
