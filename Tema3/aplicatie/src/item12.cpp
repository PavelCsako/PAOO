// Item 12: Copy all parts of an object.

#include "demonstratii.hpp"
#include "exemple_item12.hpp"

#include <galerie/ImagineCuFiltru.hpp>

#include <iostream>

namespace {

// În fiecare demonstrație: originalul e "apus, sepia 80%", iar destinația atribuirii e "luna, alb-negru 30%".
// Corect ar fi ca și copia, și destinația să ajungă identice cu originalul.

void copiereCorecta()
{
    subtitlu("12.a) galerie::ImagineCuFiltru: copiază partea de bază ȘI fiecare membru propriu");

    galerie::ImagineCuFiltru original("apus", "..::--==++", "sepia", 80);
    galerie::ImagineCuFiltru destinatie("luna", " .:*@@*:. ", "alb-negru", 30);

    std::cout << "  ImagineCuFiltru copie(original);" << std::endl;
    galerie::ImagineCuFiltru copie(original);

    std::cout << "  destinatie = original;" << std::endl;
    destinatie = original;

    std::cout << "    original:   " << original.descriere() << std::endl
              << "    copie:      " << copie.descriere() << std::endl
              << "    destinatie: " << destinatie.descriere() << std::endl
              << "  => toate trei identice (Bitmap-uri diferite: fiecare imagine își are propriii pixeli)." << std::endl;
}

void faraClasaDeBaza()
{
    subtitlu("12.b) exemple::ImagineCuFiltruFaraBaza: funcțiile de copiere uită clasa de bază");

    exemple::ImagineCuFiltruFaraBaza original("apus", "..::--==++", "sepia", 80);
    exemple::ImagineCuFiltruFaraBaza destinatie("luna", " .:*@@*:. ", "alb-negru", 30);

    std::cout << "  ImagineCuFiltruFaraBaza copie(original);" << std::endl;
    exemple::ImagineCuFiltruFaraBaza copie(original);

    std::cout << "  destinatie = original;" << std::endl;
    destinatie = original;

    std::cout << "    original:   " << original.descriere() << std::endl
              << "    copie:      " << copie.descriere() << "   <- partea de bază e GOALĂ" << std::endl
              << "    destinatie: " << destinatie.descriere() << "   <- titlu și pixeli VECHI, filtru nou" << std::endl
              << "  => Copy constructor-ul a construit partea de bază cu constructorul implicit, iar operator=" << std::endl
              << "     n-a atins-o deloc. Corect: Imagine(altul) în lista de inițializare și Imagine::operator=(altul)."
              << std::endl;
}

void membruNeactualizat()
{
    subtitlu("12.c) exemple::ImagineCuFiltruNeactualizata: membru nou, funcții de copiere neactualizate");

    exemple::ImagineCuFiltruNeactualizata original("apus", "..::--==++", "sepia", 80);
    exemple::ImagineCuFiltruNeactualizata destinatie("luna", " .:*@@*:. ", "alb-negru", 30);

    std::cout << "  ImagineCuFiltruNeactualizata copie(original);" << std::endl;
    exemple::ImagineCuFiltruNeactualizata copie(original);

    std::cout << "  destinatie = original;" << std::endl;
    destinatie = original;

    std::cout << "    original:   " << original.descriere() << std::endl
              << "    copie:      " << copie.descriere() << "   <- intensitatea s-a pierdut (0%)" << std::endl
              << "    destinatie: " << destinatie.descriere() << "   <- intensitatea veche (30%)" << std::endl
              << "  => Când adaugi un membru, actualizezi și copy constructor-ul, și operator=. Compilatorul nu" << std::endl
              << "     avertizează: pentru el, funcțiile scrise de mână fac exact ce le-ai cerut." << std::endl;
}

} // namespace

void demonstreazaItem12()
{
    titlu("Item 12: Copy all parts of an object");

    copiereCorecta();
    faraClasaDeBaza();
    membruNeactualizat();

    subtitlu("12.d) Nu implementa o funcție de copiere cu ajutorul celeilalte");
    std::cout << "  - copy constructor-ul care apelează operator= ar atribui unui obiect încă neconstruit;" << std::endl
              << "  - operator= care apelează copy constructor-ul pe *this ar construi a doua oară un obiect existent."
              << std::endl
              << "  Codul comun se pune într-o a treia funcție, privată, apelată de amândouă. (Copy and swap e altceva:"
              << std::endl
              << "  operator= construiește un obiect temporar NOU, nu reconstruiește *this.)" << std::endl;
}
