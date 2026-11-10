#ifndef GALERIE_BITMAP_HPP
#define GALERIE_BITMAP_HPP

#include <string>

namespace galerie {

///\brief Pixelii unei imagini. Corespunde clasei Bitmap din Effective C++ (Item 11).
///
/// Un "pixel" este un caracter din scala de luminozitate " .:-=+*#%@" (de la întuneric la lumină),
/// ca imaginea să poată fi afișată în terminal, pe o singură linie.
///
/// Bitmap-ul e alocat pe heap de clasa care îl deține (galerie::Imagine). Pentru demonstrații,
/// clasa ține evidența obiectelor vii, ca să se vadă clar ce se întâmplă cu memoria:
///   - numără câte Bitmap-uri există (leak-urile se văd ca obiecte rămase în viață);
///   - recunoaște o copiere dintr-un Bitmap deja șters (Item 11) și NU citește din el.
///     Fără această verificare, copierea ar citi din memorie eliberată: comportament nedefinit;
///   - poate simula lipsa memoriei la următoarea copiere (aruncă std::bad_alloc), ca să se poată
///     verifica dacă operator= rămâne corect când apare o excepție (exception safety).
class Bitmap
{
public:
    explicit Bitmap(const std::string& pixeli);

    Bitmap(const Bitmap& altul);                    // poate arunca std::bad_alloc (simulat)

    Bitmap& operator=(const Bitmap&) = delete;      // nu e nevoie: Imagine copiază Bitmap-uri doar prin constructor

    ~Bitmap();

public:
    const std::string& pixeli() const;

    int id() const;                                 // numărul de ordine, apare în mesaje: "Bitmap #3"

    bool corupt() const;                            // a fost copiat dintr-un Bitmap deja șters

    void lumineaza(int trepte);                     // mută fiecare pixel pe scala de luminozitate

    void umple(char pixel);                         // toți pixelii primesc aceeași valoare

public:
    static int inViata();                           // câte Bitmap-uri există acum

    static int totalCreate();                       // câte au fost create de la pornirea programului

    static bool esteViu(const Bitmap* bitmap);      // adresa aparține unui Bitmap care încă există?

    static void simuleazaLipsaMemorieLaUrmatoareaCopiere();

private:
    std::string pixeli_;
    int id_;
    bool corupt_;
};

} // namespace galerie

#endif // GALERIE_BITMAP_HPP
