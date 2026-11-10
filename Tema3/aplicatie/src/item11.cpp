// Item 11: Handle assignment to self in operator=.

#include "demonstratii.hpp"
#include "exemple_imagini.hpp"

#include <galerie/Bitmap.hpp>
#include <galerie/Imagine.hpp>

#include <cstddef>
#include <iostream>
#include <new>
#include <string>
#include <vector>

namespace {

const std::string pixeliApus = "..::--==++";
const std::string pixeliLuna = " .:*@@*:. ";

struct Rezultat
{
    std::string varianta;
    std::string autoatribuire;      // ce a rămas după imagini[i] = imagini[j], cu i == j
    std::string exceptie;           // ce a rămas după a = b, când copierea Bitmap-ului eșuează
};

bool esteValida(const exemple::BazaImagine& imagine)
{
    return imagine.valida();
}

bool esteValida(const galerie::Imagine& imagine)
{
    return imagine.bitmap() != nullptr && !imagine.bitmap()->corupt();
}

///\brief Aceleași două teste pentru fiecare variantă de operator=.
template <typename T>
Rezultat testeaza(const std::string& varianta, const std::string& codOperator)
{
    Rezultat rezultat;
    rezultat.varianta = varianta;

    std::cout << std::endl << ">>> " << varianta << std::endl << codOperator;

    std::cout << "  Test 1: autoatribuire ascunsă: imagini[i] = imagini[j], cu i == j == 1" << std::endl;
    {
        T imagini[2] = {T("apus", pixeliApus), T("luna", pixeliLuna)};
        const std::size_t i = 1;
        const std::size_t j = 1;
        imagini[i] = imagini[j];
        std::cout << "    imagini[1] după atribuire: " << imagini[i].descriere() << std::endl;
        const bool corecta = esteValida(imagini[i]) && imagini[i].pixeli() == pixeliLuna;
        rezultat.autoatribuire = corecta ? "corectă" : "STRICATĂ";
    }

    std::cout << "  Test 2: a = b, dar copierea Bitmap-ului eșuează (lipsă de memorie)" << std::endl;
    {
        T a("apus", pixeliApus);
        T b("luna", pixeliLuna);
        galerie::Bitmap::simuleazaLipsaMemorieLaUrmatoareaCopiere();
        try {
            a = b;
        } catch (const std::bad_alloc&) {
            std::cout << "    excepție prinsă: std::bad_alloc" << std::endl;
        }
        std::cout << "    a după excepție: " << a.descriere() << std::endl;
        const bool neschimbata = esteValida(a) && a.pixeli() == pixeliApus;
        rezultat.exceptie = neschimbata ? "neschimbată (corect)" : "STRICATĂ";
    }

    return rezultat;
}

void undeApareAutoatribuirea()
{
    subtitlu("11.a) Autoatribuirea nu e mereu vizibilă în cod");
    std::cout << "  w = w;              // evidentă, rar scrisă așa" << std::endl
              << "  a[i] = a[j];        // autoatribuire dacă i == j (de ex. utilizatorul alege aceeași imagine)"
              << std::endl
              << "  *px = *py;          // autoatribuire dacă px == py" << std::endl
              << "  void f(Imagine& x, Imagine& y) { x = y; }   // apelată cu f(a, a)" << std::endl
              << "  Testele de mai jos folosesc a doua formă, imagini[i] = imagini[j] cu i == j." << std::endl;
}

void afiseazaRezumat(const std::vector<Rezultat>& rezultate)
{
    subtitlu("Rezumat Item 11 (rezultatele de mai sus)");
    std::cout << "  " << aliniaza("varianta de operator=", 34) << aliniaza("imagini[i] = imagini[j]", 26)
              << "excepție la copiere" << std::endl;
    for (const Rezultat& rezultat : rezultate) {
        std::cout << "  " << aliniaza(rezultat.varianta, 34) << aliniaza(rezultat.autoatribuire, 26)
                  << rezultat.exceptie << std::endl;
    }
    std::cout << "=> Testul de identitate rezolvă doar autoatribuirea. Ordinea atentă și copy and swap le rezolvă"
              << std::endl
              << "   pe amândouă: un operator= sigur la excepții e, de obicei, sigur și la autoatribuire." << std::endl;
}

} // namespace

void demonstreazaItem11()
{
    titlu("Item 11: Handle assignment to self in operator=");

    undeApareAutoatribuirea();

    subtitlu("11.b) Aceleași două teste pentru patru variante de operator=");
    std::vector<Rezultat> rezultate;

    rezultate.push_back(testeaza<exemple::ImagineNaiva>(
        "exemple::ImagineNaiva",
        "    delete bitmap_;\n"
        "    bitmap_ = new Bitmap(*altul.bitmap_);\n"));

    rezultate.push_back(testeaza<exemple::ImagineTestIdentitate>(
        "exemple::ImagineTestIdentitate",
        "    if (this == &altul) return *this;\n"
        "    delete bitmap_;\n"
        "    bitmap_ = new Bitmap(*altul.bitmap_);\n"));

    rezultate.push_back(testeaza<exemple::ImagineOrdineAtenta>(
        "exemple::ImagineOrdineAtenta",
        "    Bitmap* original = bitmap_;\n"
        "    bitmap_ = new Bitmap(*altul.bitmap_);\n"
        "    delete original;\n"));

    rezultate.push_back(testeaza<galerie::Imagine>(
        "galerie::Imagine",
        "    Imagine copie(altul);   // copy and swap\n"
        "    swap(copie);\n"));

    afiseazaRezumat(rezultate);
}
