#include <galerie/Lacat.hpp>

namespace galerie {

Lacat::Lacat(Mutex* mutex)
    : mutex_(mutex)
{
    blocheaza(mutex_);
}

Lacat::~Lacat()
{
    deblocheaza(mutex_);
}

} // namespace galerie
