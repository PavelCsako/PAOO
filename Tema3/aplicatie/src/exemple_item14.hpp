#ifndef EXEMPLE_ITEM14_HPP
#define EXEMPLE_ITEM14_HPP

// Item 14: Think carefully about copying behavior in resource-managing classes.
//
// Clasa RAII pentru mutex, scrisă FĂRĂ să decidă ce înseamnă copierea ei.

#include <galerie/Mutex.hpp>

namespace exemple {

class LacatCopiabil
{
public:
    explicit LacatCopiabil(galerie::Mutex* mutex)
        : mutex_(mutex)
    {
        galerie::blocheaza(mutex_);
    }

    ~LacatCopiabil()
    {
        galerie::deblocheaza(mutex_);
    }

    // Nu am spus nimic despre copiere, deci compilatorul generează un copy constructor (Item 5)
    // care copiază doar pointerul: două lacăte pentru același mutex, fiecare îl va debloca.

private:
    galerie::Mutex* mutex_;
};

} // namespace exemple

#endif // EXEMPLE_ITEM14_HPP
