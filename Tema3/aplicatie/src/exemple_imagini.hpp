#ifndef EXEMPLE_IMAGINI_HPP
#define EXEMPLE_IMAGINI_HPP

// Variante de operator= pentru o imagine cu un Bitmap pe heap (Item 10, Item 11, move assignment).
//
// Toate moștenesc din BazaImagine, ca fiecare clasă să difere de celelalte DOAR prin operatorul
// de atribuire studiat. Varianta corectă folosită în rest este galerie::Imagine din bibliotecă.

#include <galerie/Bitmap.hpp>

#include <iostream>
#include <string>
#include <utility>

namespace exemple {

///\brief Partea comună a exemplelor: titlu, Bitmap pe heap, constructori, destructor, afișare.
/// Nu are operator= (fiecare clasă derivată îl scrie pe al ei).
class BazaImagine
{
public:
    BazaImagine(const std::string& titlu, const std::string& pixeli)
        : titlu_(titlu)
        , bitmap_(new galerie::Bitmap(pixeli))
    {
    }

    BazaImagine(const BazaImagine& altul)
        : titlu_(altul.titlu_)
        , bitmap_(new galerie::Bitmap(*altul.bitmap_))
    {
    }

    ~BazaImagine()
    {
        if (galerie::Bitmap::esteViu(bitmap_)) {
            delete bitmap_;
        } else if (bitmap_ != nullptr) {
            // Un operator= greșit a lăsat aici adresa unui Bitmap deja șters. Un delete ar fi un
            // "double delete" (comportament nedefinit, de obicei prăbușire). Demonstrația îl sare.
            std::cout << "      !!! destructor " << titlu_
                      << ": bitmap_ indică un Bitmap deja șters; delete aici ar fi un double delete" << std::endl;
        }
    }

    const std::string& titlu() const
    {
        return titlu_;
    }

    std::string pixeli() const
    {
        return galerie::Bitmap::esteViu(bitmap_) ? bitmap_->pixeli() : std::string();
    }

    bool valida() const     // are un Bitmap care există și care nu e corupt
    {
        return galerie::Bitmap::esteViu(bitmap_) && !bitmap_->corupt();
    }

    std::string descriere() const
    {
        if (bitmap_ == nullptr) {
            return (titlu_.empty() ? std::string("(mutată)") : titlu_) + " (goală)";
        }
        if (!galerie::Bitmap::esteViu(bitmap_)) {
            return titlu_ + " [POINTER INVALID: Bitmap-ul lui a fost șters]";
        }
        return titlu_ + " [" + bitmap_->pixeli() + "] (Bitmap #" + std::to_string(bitmap_->id()) + ")"
               + (bitmap_->corupt() ? " CORUPTĂ" : "");
    }

protected:
    BazaImagine& operator=(const BazaImagine&) = delete;

    std::string titlu_;
    galerie::Bitmap* bitmap_;
};

// ---------------------------------------------------------------------------
// Item 10: operator= care întoarce o COPIE (prin valoare), nu o referință
// ---------------------------------------------------------------------------

class ImagineReturnValoare : public BazaImagine
{
public:
    using BazaImagine::BazaImagine;

    ImagineReturnValoare(const ImagineReturnValoare&) = default;

    ImagineReturnValoare operator=(const ImagineReturnValoare& altul)     // GREȘIT: fără &
    {
        galerie::Bitmap* original = bitmap_;
        bitmap_ = new galerie::Bitmap(*altul.bitmap_);
        delete original;
        titlu_ = altul.titlu_;
        return *this;           // se face o copie a lui *this (încă un Bitmap), care e întoarsă
    }

    ImagineReturnValoare operator+=(int trepte)                          // GREȘIT: fără &
    {
        bitmap_->lumineaza(trepte);
        return *this;
    }
};

// ---------------------------------------------------------------------------
// Item 11: trei feluri de a scrie operator=
// ---------------------------------------------------------------------------

///\brief Varianta din carte, fără nicio protecție: întâi șterge, apoi copiază.
class ImagineNaiva : public BazaImagine
{
public:
    using BazaImagine::BazaImagine;

    ImagineNaiva(const ImagineNaiva&) = default;

    ImagineNaiva& operator=(const ImagineNaiva& altul)
    {
        delete bitmap_;                                     // la a = a, șterge și Bitmap-ul lui altul
        bitmap_ = new galerie::Bitmap(*altul.bitmap_);      // ...din care încearcă apoi să copieze
        titlu_ = altul.titlu_;
        return *this;
    }
};

///\brief Testul de identitate: corect la autoatribuire, dar tot nesigur la excepții.
class ImagineTestIdentitate : public BazaImagine
{
public:
    using BazaImagine::BazaImagine;

    ImagineTestIdentitate(const ImagineTestIdentitate&) = default;

    ImagineTestIdentitate& operator=(const ImagineTestIdentitate& altul)
    {
        if (this == &altul) {
            return *this;                                   // același obiect: nimic de făcut
        }
        delete bitmap_;
        bitmap_ = new galerie::Bitmap(*altul.bitmap_);      // dacă aruncă, bitmap_ indică memorie ștearsă
        titlu_ = altul.titlu_;
        return *this;
    }
};

///\brief Ordinea atentă: întâi copiază, abia apoi șterge. Corect în ambele situații.
class ImagineOrdineAtenta : public BazaImagine
{
public:
    using BazaImagine::BazaImagine;

    ImagineOrdineAtenta(const ImagineOrdineAtenta&) = default;

    ImagineOrdineAtenta& operator=(const ImagineOrdineAtenta& altul)
    {
        galerie::Bitmap* original = bitmap_;                // ținem minte Bitmap-ul vechi
        bitmap_ = new galerie::Bitmap(*altul.bitmap_);      // copiem: dacă aruncă, bitmap_ nu s-a schimbat
        delete original;                                    // ștergem vechiul abia la final
        titlu_ = altul.titlu_;
        return *this;
    }
};

// ---------------------------------------------------------------------------
// Move assignment (C++11) care uită să elibereze Bitmap-ul vechi
// ---------------------------------------------------------------------------

class ImagineMutareCuLeak : public BazaImagine
{
public:
    using BazaImagine::BazaImagine;

    ImagineMutareCuLeak& operator=(ImagineMutareCuLeak&& altul) noexcept
    {
        bitmap_ = altul.bitmap_;            // GREȘIT: lipsește "delete bitmap_;" înainte => leak
        altul.bitmap_ = nullptr;
        titlu_ = std::move(altul.titlu_);
        return *this;
    }
};

} // namespace exemple

#endif // EXEMPLE_IMAGINI_HPP
