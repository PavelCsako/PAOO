#ifndef SECRETARIAT_CONFIGURATIE_HPP
#define SECRETARIAT_CONFIGURATIE_HPP

#include <string>

namespace secretariat {

///\brief Configurația secretariatului: anul universitar și numărul de locuri dintr-o grupă.
///
/// Alte obiecte (de ex. grupele) o citesc chiar în constructorul lor. Dacă și ele sunt globale,
/// apare problema din Item 4: ordinea în care se construiesc obiectele globale definite
/// în fișiere .cpp diferite NU este definită de standard.
class Configuratie
{
public:
    explicit Configuratie(const std::string& provenienta);     // provenienta: doar pentru mesaj

public:
    int anUniversitar() const;

    int locuriPeGrupa() const;

private:
    int anUniversitar_;
    int locuriPeGrupa_;
};

///\brief Soluția din Item 4: în loc de un obiect global, o funcție care întoarce o referință
/// la un obiect static LOCAL.
///
/// Obiectul e construit la primul apel al funcției, deci înainte de prima lui folosire,
/// indiferent din ce fișier sau din ce alt obiect global vine apelul.
/// Din C++11, construirea are loc o singură dată, chiar și cu mai multe fire de execuție.
Configuratie& configuratie();

} // namespace secretariat

#endif // SECRETARIAT_CONFIGURATIE_HPP
