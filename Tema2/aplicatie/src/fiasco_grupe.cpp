#include "fiasco.hpp"

#include <iostream>

namespace fiasco {

Grupa::Grupa(const std::string& nume, const secretariat::Configuratie& configuratie)
    : nume_(nume)
    , locuri_(configuratie.locuriPeGrupa())
{
    std::cout << "[Grupa] constructor: " << nume_ << " -> am citit din configurație " << locuri_ << " locuri"
              << std::endl;
}

const std::string& Grupa::nume() const
{
    return nume_;
}

int Grupa::locuri() const
{
    return locuri_;
}

// PROBLEMA: configuratieGlobala e definită în ALT fișier (fiasco_configuratie.cpp).
// Nimic nu garantează că a fost deja construită atunci când se construiește grupa de mai jos.
// Dacă nu a fost, constructorul grupei citește un obiect "neconstruit" (comportament nedefinit;
// în practică memoria lui e încă plină de zerouri, deci grupa ajunge cu 0 locuri).
Grupa grupaCuObiectGlobal("grupa 3141 (citește obiectul global)", configuratieGlobala);

// SOLUȚIA: secretariat::configuratie() întoarce un obiect static LOCAL, construit la primul apel,
// deci garantat înainte de a fi folosit, indiferent de ordinea fișierelor.
Grupa grupaCuStaticLocal("grupa 3142 (folosește secretariat::configuratie())", secretariat::configuratie());

} // namespace fiasco
