#include "fiasco.hpp"

namespace fiasco {

// Obiect static non-local (global). E construit înainte de main(), dar NU se știe dacă
// înainte sau după obiectele globale din fiasco_grupe.cpp care îl folosesc.
secretariat::Configuratie configuratieGlobala("obiectul global fiasco::configuratieGlobala");

} // namespace fiasco
