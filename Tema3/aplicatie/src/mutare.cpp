// Move assignment (C++11, nu e în carte, dar e în laboratorul 5).

#include "demonstratii.hpp"
#include "exemple_imagini.hpp"

#include <galerie/Bitmap.hpp>
#include <galerie/Imagine.hpp>

#include <iostream>
#include <utility>

namespace {

void mutareCorecta()
{
    subtitlu("M.a) galerie::Imagine: a = std::move(b)");

    galerie::Imagine a("a", "..........");
    galerie::Imagine b("b", "..::--==++");
    const int createInainte = galerie::Bitmap::totalCreate();
    const int viiInainte = galerie::Bitmap::inViata();

    std::cout << "  a = std::move(b);" << std::endl;
    a = std::move(b);

    std::cout << "    a: " << a.descriere() << "   <- a preluat Bitmap-ul lui b" << std::endl
              << "    b: " << b.descriere() << "   <- sursa e goală, dar validă" << std::endl
              << "  Bitmap-uri noi: " << galerie::Bitmap::totalCreate() - createInainte
              << ", Bitmap-uri în viață: " << viiInainte << " -> " << galerie::Bitmap::inViata()
              << " (Bitmap-ul vechi al lui a a fost șters)" << std::endl;
}

void mutareCuLeak()
{
    subtitlu("M.b) exemple::ImagineMutareCuLeak: move assignment fără delete pe Bitmap-ul vechi");

    const int viiInainte = galerie::Bitmap::inViata();
    {
        exemple::ImagineMutareCuLeak a("a", "..........");
        exemple::ImagineMutareCuLeak b("b", "..::--==++");
        std::cout << "  a = std::move(b);" << std::endl;
        a = std::move(b);
        std::cout << "    a: " << a.descriere() << std::endl << "    b: " << b.descriere() << std::endl;
    }
    const int ramase = galerie::Bitmap::inViata() - viiInainte;
    std::cout << "  După ce a și b au fost distruse, Bitmap-uri rămase alocate: " << ramase << " (memory leak)" << std::endl
              << "  => Spre deosebire de move constructor (obiect nou, nimic de eliberat), destinația unui move" << std::endl
              << "     assignment deține deja o resursă: trebuie eliberată înainte de a o prelua pe cea nouă." << std::endl;
}

void rezumat()
{
    subtitlu("Copy assignment vs move assignment (galerie::Imagine)");
    std::cout << "  " << aliniaza("", 20) << aliniaza("copy: a = b", 22) << "move: a = std::move(b)" << std::endl
              << "  " << aliniaza("parametru", 20) << aliniaza("const Imagine&", 22) << "Imagine&&" << std::endl
              << "  " << aliniaza("Bitmap nou", 20) << aliniaza("da", 22) << "nu" << std::endl
              << "  " << aliniaza("Bitmap-ul vechi", 20) << aliniaza("șters", 22) << "șters" << std::endl
              << "  " << aliniaza("sursa după", 20) << aliniaza("neschimbată", 22) << "goală" << std::endl
              << "  " << aliniaza("întoarce", 20) << aliniaza("*this (Item 10)", 22) << "*this (Item 10)" << std::endl;
}

} // namespace

void demonstreazaMutarea()
{
    titlu("C++11: move assignment (operator=(Imagine&&))");

    mutareCorecta();
    mutareCuLeak();
    rezumat();
}
