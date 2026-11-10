#ifndef EXEMPLE_ITEM5_HPP
#define EXEMPLE_ITEM5_HPP

// Item 5: Know what functions C++ silently writes and calls.
//
// Niciuna dintre clasele de aici nu își scrie singură copy constructor-ul, operator= sau
// move-ul: tot ce se vede în item5.cpp e generat (sau refuzat) de compilator.

#include <secretariat/Martor.hpp>

#include <iostream>
#include <string>

namespace exemple {

// ---------------------------------------------------------------------------
// 5.a) O clasă goală
// ---------------------------------------------------------------------------

///\brief Nu declară nimic, dar e ca și cum am fi scris (compilatorul le generează doar dacă
/// sunt folosite, și le face public și inline):
///
///     class Gol {
///     public:
///         Gol() {}                                // constructor implicit
///         Gol(const Gol& altul) { ... }           // copy constructor
///         Gol& operator=(const Gol& altul) { ... }// copy assignment
///         ~Gol() {}                               // destructor (nevirtual)
///         Gol(Gol&& altul) { ... }                // din C++11: move constructor
///         Gol& operator=(Gol&& altul) { ... }     // din C++11: move assignment
///     };
class Gol
{
};

// ---------------------------------------------------------------------------
// 5.b) Funcțiile generate apelează funcțiile clasei de bază și ale membrilor
// ---------------------------------------------------------------------------

class DocumentDeBaza
{
private:
    secretariat::Martor antet_{"antet (din clasa de bază)"};
};

///\brief Nu declară nicio funcție specială. Cele generate lucrează membru cu membru:
/// întâi clasa de bază, apoi membrii în ordinea declarării (destructorul, invers).
class FisaGenerata : public DocumentDeBaza
{
private:
    secretariat::Martor nume_{"nume"};
    secretariat::Martor nota_{"nota"};
};

// ---------------------------------------------------------------------------
// 5.c) Constructorul implicit e generat doar dacă nu declarăm NICIUN constructor
// ---------------------------------------------------------------------------

class FaraConstructorImplicit
{
public:
    explicit FaraConstructorImplicit(int an)
        : an_(an)
    {
    }

    int an() const
    {
        return an_;
    }

private:
    int an_;
};

// ---------------------------------------------------------------------------
// 5.d) NamedObject din carte: operator= NU poate fi generat
// ---------------------------------------------------------------------------

template <typename T>
class ObiectCuNume
{
public:
    ObiectCuNume(std::string& nume, const T& valoare)
        : nume_(nume)
        , valoare_(valoare)
    {
    }

    void afiseaza() const
    {
        std::cout << nume_ << " (" << valoare_ << ")";
    }

private:
    std::string& nume_;     // o referință rămâne legată mereu de același obiect
    const T valoare_;       // un membru const nu mai poate fi modificat după inițializare
};

// ---------------------------------------------------------------------------
// 5.e) (C++11) Un destructor declarat oprește generarea move constructor-ului
// ---------------------------------------------------------------------------

class FaraDestructorDeclarat
{
private:
    secretariat::Martor date_{"date"};
};

class CuDestructorDeclarat
{
public:
    ~CuDestructorDeclarat()     // un destructor gol, aparent inofensiv...
    {
    }

private:
    secretariat::Martor date_{"date"};
};

} // namespace exemple

#endif // EXEMPLE_ITEM5_HPP
