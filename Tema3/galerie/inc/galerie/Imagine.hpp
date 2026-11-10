#ifndef GALERIE_IMAGINE_HPP
#define GALERIE_IMAGINE_HPP

#include <galerie/Bitmap.hpp>

#include <string>

namespace galerie {

///\brief O imagine din galerie: un titlu și un Bitmap alocat pe heap (Widget din Effective C++).
///
/// Varianta CORECTĂ pentru lucrarea 5:
///   - Item 10: toți operatorii de atribuire (=, +=, = cu un pixel) întorc o referință la *this;
///   - Item 11: operator= folosește "copy and swap", deci e corect și la autoatribuire (a = a),
///     și dacă apare o excepție în timpul copierii;
///   - move assignment (C++11): eliberează Bitmap-ul vechi, îl preia pe cel nou, golește sursa.
/// Variantele greșite, pentru comparație, sunt în aplicatie/src/exemple_imagini.hpp.
class Imagine
{
public:
    Imagine();                                              // imagine goală, fără Bitmap

    Imagine(const std::string& titlu, const std::string& pixeli);

    Imagine(const Imagine& altul);                          // copie în adâncime: Bitmap nou

    Imagine(Imagine&& altul) noexcept;                      // preia Bitmap-ul, sursa rămâne goală

    ~Imagine();

public:
    // Item 10: toți operatorii de atribuire întorc Imagine&, adică *this
    Imagine& operator=(const Imagine& altul);               // copy assignment (copy and swap)

    Imagine& operator=(Imagine&& altul) noexcept;           // move assignment

    Imagine& operator=(char pixel);                         // umple imaginea cu un singur pixel

    Imagine& operator+=(int trepte);                        // luminează (sau întunecă, pentru trepte < 0)

    void swap(Imagine& altul) noexcept;                     // schimbă conținutul, fără copieri

public:
    const std::string& titlu() const;

    std::string pixeli() const;                             // "" pentru o imagine goală

    const Bitmap* bitmap() const;

    std::string descriere() const;                          // titlu [pixeli] (Bitmap #id)

private:
    std::string titlu_;
    Bitmap* bitmap_;                                        // deținut de imagine; nullptr = imagine goală
};

} // namespace galerie

#endif // GALERIE_IMAGINE_HPP
