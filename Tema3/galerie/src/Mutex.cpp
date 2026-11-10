#include <galerie/Mutex.hpp>

#include <iostream>

namespace galerie {

void blocheaza(Mutex* mutex)
{
    if (mutex->blocat) {
        ++mutex->erori;
        std::cout << "      !!! [Mutex " << mutex->nume
                  << "] blocat a doua oară (în realitate: programul ar rămâne blocat definitiv)" << std::endl;
        return;
    }
    mutex->blocat = true;
    std::cout << "      [Mutex " << mutex->nume << "] blocat" << std::endl;
}

void deblocheaza(Mutex* mutex)
{
    if (!mutex->blocat) {
        ++mutex->erori;
        std::cout << "      !!! [Mutex " << mutex->nume
                  << "] deblocat, deși NU era blocat (în realitate: comportament nedefinit)" << std::endl;
        return;
    }
    mutex->blocat = false;
    std::cout << "      [Mutex " << mutex->nume << "] deblocat" << std::endl;
}

} // namespace galerie
