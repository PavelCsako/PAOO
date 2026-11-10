#ifndef SECRETARIAT_NECOPIABIL_HPP
#define SECRETARIAT_NECOPIABIL_HPP

namespace secretariat {

///\brief Clasa de bază "Uncopyable" din Effective C++, Item 6.
///
/// O clasă care moștenește din ea nu mai poate fi copiată: copy constructor-ul și operator=
/// pe care compilatorul i le-ar genera ar trebui să le apeleze pe cele din bază, care sunt
/// private => compilatorul refuză să le genereze (Item 5).
///
/// Avantajul față de "declar private și nu definesc" direct în clasă: eroarea apare la
/// COMPILARE chiar și în funcțiile membre sau prietene ale clasei derivate, nu abia la
/// link-editare.
class Necopiabil
{
protected:
    Necopiabil() {}     // clasele derivate pot fi construite și distruse...
    ~Necopiabil() {}    // (nevirtual: clasa nu e făcută pentru a fi folosită polimorfic)

private:
    Necopiabil(const Necopiabil&);              // ...dar nu și copiate:
    Necopiabil& operator=(const Necopiabil&);   // declarate private și NEDEFINITE
};

} // namespace secretariat

#endif // SECRETARIAT_NECOPIABIL_HPP
