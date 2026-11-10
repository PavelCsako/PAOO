#ifndef FIASCO_HPP
#define FIASCO_HPP

// Item 4.d: ordinea de inițializare a obiectelor statice non-locale din fișiere .cpp diferite
// ("static initialization order fiasco").
//
// Un obiect static non-local = obiect global, din namespace, membru static de clasă sau
// declarat static la nivel de fișier. Toate se construiesc înainte de main(), dar standardul
// NU spune în ce ordine se construiesc cele definite în fișiere (unități de compilare) diferite.

#include <secretariat/Configuratie.hpp>

#include <string>

namespace fiasco {

///\brief O grupă își ia numărul de locuri din configurație, chiar în constructor.
class Grupa
{
public:
    Grupa(const std::string& nume, const secretariat::Configuratie& configuratie);

public:
    const std::string& nume() const;

    int locuri() const;

private:
    std::string nume_;
    int locuri_;
};

// Obiectele globale (statice non-locale), definite în fișiere .cpp DIFERITE:

extern secretariat::Configuratie configuratieGlobala;   // în fiasco_configuratie.cpp

extern Grupa grupaCuObiectGlobal;                       // în fiasco_grupe.cpp, citește configuratieGlobala

extern Grupa grupaCuStaticLocal;                        // în fiasco_grupe.cpp, citește secretariat::configuratie()

} // namespace fiasco

#endif // FIASCO_HPP
