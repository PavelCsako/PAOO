#ifndef GALERIE_LACAT_HPP
#define GALERIE_LACAT_HPP

#include <galerie/Mutex.hpp>

namespace galerie {

///\brief Clasa RAII pentru un Mutex (Lock din Effective C++, Item 14).
///
/// Constructorul blochează mutex-ul (resursa e "dobândită" chiar la inițializare: RAII, Item 13),
/// iar destructorul îl deblochează, oricum s-ar ieși din bloc: la final, cu return sau cu o excepție.
///
/// Opțiunea 1 din Item 14: copierea e INTERZISĂ (Item 6). O copie a unui lacăt nu are sens:
/// cu copierea generată de compilator, mutex-ul ar fi deblocat de două ori.
class Lacat
{
public:
    explicit Lacat(Mutex* mutex);       // blochează

    ~Lacat();                           // deblochează

    Lacat(const Lacat&) = delete;
    Lacat& operator=(const Lacat&) = delete;

private:
    Mutex* mutex_;
};

} // namespace galerie

#endif // GALERIE_LACAT_HPP
