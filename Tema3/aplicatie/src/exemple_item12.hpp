#ifndef EXEMPLE_ITEM12_HPP
#define EXEMPLE_ITEM12_HPP

// Item 12: Copy all parts of an object.
//
// Două variante GREȘITE ale lui galerie::ImagineCuFiltru (PriorityCustomer din carte).
// Compilatorul nu dă nicio eroare pentru ele: copierea pur și simplu nu copiază tot.

#include <galerie/Imagine.hpp>

#include <iostream>
#include <string>

namespace exemple {

// GCC observă doar una dintre greșeli: copy constructor-ul care nu inițializează explicit clasa de
// bază (avertisment -Wextra). Ca build-ul să rămână curat, avertismentul e oprit doar aici;
// cu  cmake -DARATA_AVERTISMENTE=ON ..  se vede. Pentru operator= și pentru membrul uitat nu există avertisment.
#ifndef ARATA_AVERTISMENTE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wextra"
#endif

///\brief Funcțiile de copiere scrise de mână UITĂ partea moștenită din Imagine.
class ImagineCuFiltruFaraBaza : public galerie::Imagine
{
public:
    ImagineCuFiltruFaraBaza(const std::string& titlu, const std::string& pixeli, const std::string& filtru,
                            int intensitate)
        : Imagine(titlu, pixeli)
        , filtru_(filtru)
        , intensitate_(intensitate)
    {
    }

    ImagineCuFiltruFaraBaza(const ImagineCuFiltruFaraBaza& altul)
        : filtru_(altul.filtru_)                // GREȘIT: lipsește Imagine(altul), deci partea de bază
        , intensitate_(altul.intensitate_)      // e construită cu constructorul IMPLICIT: imagine goală
    {
        std::cout << "    [ImagineCuFiltruFaraBaza] copy constructor (fără Imagine(altul))" << std::endl;
    }

    ImagineCuFiltruFaraBaza& operator=(const ImagineCuFiltruFaraBaza& altul)
    {
        std::cout << "    [ImagineCuFiltruFaraBaza] operator= (fără Imagine::operator=(altul))" << std::endl;
        filtru_ = altul.filtru_;                // GREȘIT: lipsește Imagine::operator=(altul), deci
        intensitate_ = altul.intensitate_;      // titlul și pixelii rămân cei VECHI
        return *this;
    }

    std::string descriere() const
    {
        return Imagine::descriere() + " | filtru " + filtru_ + " " + std::to_string(intensitate_) + "%";
    }

private:
    std::string filtru_;
    int intensitate_;
};

#ifndef ARATA_AVERTISMENTE
#pragma GCC diagnostic pop
#endif

///\brief Partea de bază e copiată corect, dar membrul intensitate_ a fost adăugat mai târziu,
/// iar funcțiile de copiere scrise de mână NU au fost actualizate.
class ImagineCuFiltruNeactualizata : public galerie::Imagine
{
public:
    ImagineCuFiltruNeactualizata(const std::string& titlu, const std::string& pixeli, const std::string& filtru,
                                 int intensitate)
        : Imagine(titlu, pixeli)
        , filtru_(filtru)
        , intensitate_(intensitate)
    {
    }

    ImagineCuFiltruNeactualizata(const ImagineCuFiltruNeactualizata& altul)
        : Imagine(altul)
        , filtru_(altul.filtru_)                // intensitate_ lipsește: primește valoarea implicită, 0
    {
        std::cout << "    [ImagineCuFiltruNeactualizata] copy constructor (fără intensitate_)" << std::endl;
    }

    ImagineCuFiltruNeactualizata& operator=(const ImagineCuFiltruNeactualizata& altul)
    {
        std::cout << "    [ImagineCuFiltruNeactualizata] operator= (fără intensitate_)" << std::endl;
        Imagine::operator=(altul);
        filtru_ = altul.filtru_;                // intensitate_ lipsește: rămâne cea veche
        return *this;
    }

    std::string descriere() const
    {
        return Imagine::descriere() + " | filtru " + filtru_ + " " + std::to_string(intensitate_) + "%";
    }

private:
    std::string filtru_;
    int intensitate_ = 0;                       // membru adăugat ULTERIOR claselor de copiere
};

} // namespace exemple

#endif // EXEMPLE_ITEM12_HPP
