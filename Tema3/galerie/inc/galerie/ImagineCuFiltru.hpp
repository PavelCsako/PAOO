#ifndef GALERIE_IMAGINECUFILTRU_HPP
#define GALERIE_IMAGINECUFILTRU_HPP

#include <galerie/Imagine.hpp>

#include <string>

namespace galerie {

///\brief O imagine cu un filtru aplicat (PriorityCustomer din Effective C++, Item 12).
///
/// Varianta CORECTĂ: funcțiile de copiere scrise de mână copiază TOATE părțile obiectului:
///   - partea moștenită din Imagine (titlu + Bitmap), prin funcțiile clasei de bază;
///   - fiecare membru propriu (filtru_, intensitate_).
/// Funcțiile sunt scrise de mână doar ca să afișeze un mesaj (ca logCall din carte). Fără mesaj,
/// cel mai sigur ar fi să nu le scriem deloc: cele generate de compilator copiază tot.
/// Variantele greșite sunt în aplicatie/src/exemple_item12.hpp.
class ImagineCuFiltru : public Imagine
{
public:
    ImagineCuFiltru(const std::string& titlu, const std::string& pixeli, const std::string& filtru, int intensitate);

    ImagineCuFiltru(const ImagineCuFiltru& altul);

    ImagineCuFiltru& operator=(const ImagineCuFiltru& altul);

    // Regula celor cinci: odată ce am scris copierea, mutarea nu mai e generată (Item 5),
    // așa că o scriem și pe ea, tot cu partea de bază inclusă.
    ImagineCuFiltru(ImagineCuFiltru&& altul) noexcept;

    ImagineCuFiltru& operator=(ImagineCuFiltru&& altul) noexcept;

    ~ImagineCuFiltru() = default;

public:
    std::string descriere() const;      // descrierea imaginii + filtrul

private:
    std::string filtru_;
    int intensitate_;                   // procente, 0-100
};

} // namespace galerie

#endif // GALERIE_IMAGINECUFILTRU_HPP
