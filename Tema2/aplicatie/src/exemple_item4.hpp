#ifndef EXEMPLE_ITEM4_HPP
#define EXEMPLE_ITEM4_HPP

// Item 4: Make sure that objects are initialized before they're used.
//
// Clase mici folosite doar pentru demonstrații. Cele "greșite" sunt scrise așa INTENȚIONAT;
// varianta corectă este secretariat::FisaStudent din biblioteca statică.
//
// OrdineInitializare și NotaFinalaGresita conțin exact greșelile din Item 4, pe care GCC le detectează
// (-Wreorder și -Wuninitialized). Ca build-ul obișnuit să rămână curat, avertismentele sunt oprite
// DOAR pentru aceste două clase. Ca să le vedeți:  cmake -DARATA_AVERTISMENTE=ON ..  și apoi make.

#include <secretariat/Martor.hpp>

#include <iostream>
#include <string>

namespace exemple {

// ---------------------------------------------------------------------------
// 4.a) Membri de tip predefinit care nu sunt inițializați
// ---------------------------------------------------------------------------

class FisaFaraInitializare
{
public:
    explicit FisaFaraInitializare(const std::string& nume)
        : nume_(nume)
    {
        // Doar nume_ are o valoare: std::string are constructor, deci se inițializează oricum.
        // an_, medie_ și nrConsultari_ sunt de tip predefinit (int, double) și NU primesc automat
        // nicio valoare: conțin ce se afla deja în memorie. Citirea lor = comportament nedefinit.
    }

    void consulta()
    {
        ++nrConsultari_;    // pornește de la o valoare aleatoare
    }

    void afiseaza() const
    {
        std::cout << nume_ << " | anul " << an_ << " | medie " << medie_ << " | consultată de " << nrConsultari_
                  << " ori" << std::endl;
    }

private:
    std::string nume_;
    int an_;
    double medie_;
    int nrConsultari_;
};

// ---------------------------------------------------------------------------
// 4.b) Când inițializează C++ singur și când nu
// ---------------------------------------------------------------------------

///\brief Agregat fără constructori: dacă e inițializat sau nu depinde de CUM e declarat obiectul.
struct Punctaj
{
    int laborator;
    int examen;
};

// ---------------------------------------------------------------------------
// 4.c) Lista de inițializare
// ---------------------------------------------------------------------------

///\brief Membrul e construit implicit și abia apoi primește valoarea, în corpul constructorului.
class AtribuireInCorp
{
public:
    AtribuireInCorp()
    {
        // Aici nume_ a fost DEJA construit implicit; acum se mai creează un obiect temporar
        // și se face o atribuire => 4 operații în loc de 1.
        nume_ = secretariat::Martor("nume");
    }

private:
    secretariat::Martor nume_;
};

///\brief Membrul e construit direct cu valoarea dorită.
class InitializareInLista
{
public:
    InitializareInLista()
        : nume_("nume")
    {
    }

private:
    secretariat::Martor nume_;
};

#ifndef ARATA_AVERTISMENTE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

///\brief Lista de inițializare e scrisă în ordine inversă, dar asta NU contează:
/// membrii se inițializează mereu în ordinea în care sunt DECLARAȚI în clasă.
class OrdineInitializare
{
public:
    OrdineInitializare()
        : alTreilea_("al treilea")      // GCC: warning: ... will be initialized after ... [-Wreorder]
        , alDoilea_("al doilea")
        , primul_("primul")
    {
    }

private:
    secretariat::Martor primul_;
    secretariat::Martor alDoilea_;
    secretariat::Martor alTreilea_;
};

///\brief Consecința: un membru calculat din alți membri care NU au fost încă inițializați.
class NotaFinalaGresita
{
public:
    NotaFinalaGresita(double notaExamen, double notaLaborator)
        : notaExamen_(notaExamen)
        , notaLaborator_(notaLaborator)
        , medie_((notaExamen_ + notaLaborator_) / 2)   // pare corect, dar medie_ se inițializează PRIMA
    {                                                  // GCC: warning: member ... is used uninitialized
    }

    void afiseaza() const
    {
        std::cout << "examen " << notaExamen_ << " | laborator " << notaLaborator_ << " | medie " << medie_
                  << std::endl;
    }

private:
    double medie_;          // declarată PRIMA => calculată din două valori încă neinițializate
    double notaExamen_;
    double notaLaborator_;
};

#ifndef ARATA_AVERTISMENTE
#pragma GCC diagnostic pop
#endif

} // namespace exemple

#endif // EXEMPLE_ITEM4_HPP
