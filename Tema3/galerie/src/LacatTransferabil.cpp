#include <galerie/LacatTransferabil.hpp>

namespace galerie {

LacatTransferabil::LacatTransferabil(Mutex* mutex)
    : mutex_(mutex, deblocheaza)
{
    blocheaza(mutex_.get());
}

bool LacatTransferabil::detineMutex() const
{
    return mutex_ != nullptr;
}

} // namespace galerie
