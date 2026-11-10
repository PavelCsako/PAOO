#include <galerie/LacatPartajat.hpp>

namespace galerie {

LacatPartajat::LacatPartajat(Mutex* mutex)
    : mutex_(mutex, deblocheaza)        // al doilea argument: ce se apelează în loc de delete
{
    blocheaza(mutex_.get());
}

long LacatPartajat::nrCopii() const
{
    return mutex_.use_count();
}

} // namespace galerie
