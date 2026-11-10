#ifndef GALERIE_FABRICA_HPP
#define GALERIE_FABRICA_HPP

#include <galerie/Bitmap.hpp>

#include <string>

namespace galerie {

///\brief "Încarcă" un Bitmap după numele fișierului (createInvestment() din Effective C++, Item 13).
///
/// Întoarce un pointer simplu către un obiect alocat cu new. CINE APELEAZĂ funcția devine
/// responsabil de delete. Fișierele cunoscute (simulat, fără citire de pe disc):
///   "apus.img"    -> [..::--==++]
///   "luna.img"    -> [ .:*@@*:. ]
///   "gol.img"     -> []             (imagine fără pixeli)
///   "stricat.img" -> [..XX--XX++]   (conține pixeli necunoscuți)
/// Orice alt nume dă un Bitmap gol.
Bitmap* incarcaBitmap(const std::string& numeFisier);

} // namespace galerie

#endif // GALERIE_FABRICA_HPP
