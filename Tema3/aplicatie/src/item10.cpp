// Item 10: Have assignment operators return a reference to *this.

#include "demonstratii.hpp"
#include "exemple_imagini.hpp"

#include <galerie/Bitmap.hpp>
#include <galerie/Imagine.hpp>

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// Dovezi verificate la compilare: expresia "a = b" are tipul Imagine&, adică o referință
// chiar la obiectul din stânga, exact ca la int. Dacă n-ar fi așa, programul nu s-ar compila.
static_assert(std::is_same_v<decltype(std::declval<galerie::Imagine&>() = std::declval<const galerie::Imagine&>()),
                             galerie::Imagine&>,
              "operator= întoarce Imagine&");
static_assert(std::is_same_v<decltype(std::declval<galerie::Imagine&>() += 1), galerie::Imagine&>,
              "operator+= întoarce Imagine&");
static_assert(std::is_same_v<decltype(std::declval<galerie::Imagine&>() = '#'), galerie::Imagine&>,
              "operator=(char) întoarce Imagine&");

namespace {

void tipuriPredefinite()
{
    subtitlu("10.a) Tipurile predefinite permit atribuiri în lanț");

    int x = 0;
    int y = 0;
    int z = 0;
    x = y = z = 15;                     // se citește x = (y = (z = 15))
    std::cout << "  x = y = z = 15;      -> x = " << x << ", y = " << y << ", z = " << z << std::endl;

    int w = 1;
    (w = 3) += 2;                       // rezultatul lui (w = 3) este chiar variabila w
    std::cout << "  (w = 3) += 2;        -> w = " << w << std::endl;

    std::string s1;
    std::string s2;
    s1 = s2 = "galerie";
    std::cout << "  s1 = s2 = \"galerie\"; -> s1 = " << s1 << ", s2 = " << s2 << std::endl
              << "=> Pentru asta, atribuirea întoarce o referință la operandul din stânga. Biblioteca standard"
              << std::endl
              << "   (std::string, std::vector, std::shared_ptr) respectă convenția, deci și clasele noastre." << std::endl;
}

void imagineCorecta()
{
    subtitlu("10.b) galerie::Imagine: operatorii de atribuire întorc Imagine&, adică *this");

    galerie::Imagine a("a", "          ");
    galerie::Imagine b("b", "..........");
    galerie::Imagine c("c", "..::--==++");

    std::cout << "  a = b = c;           // se citește a = (b = c)" << std::endl;
    const int inainte = galerie::Bitmap::totalCreate();
    a = b = c;
    const int bitmapuriNoi = galerie::Bitmap::totalCreate() - inainte;
    std::cout << "    a: " << a.descriere() << std::endl
              << "    b: " << b.descriere() << std::endl
              << "    c: " << c.descriere() << std::endl
              << "  Bitmap-uri noi: " << bitmapuriNoi << " (câte unul pentru fiecare dintre cele două atribuiri)"
              << std::endl;

    std::cout << std::endl << "  (a = c) += 3;        // atribuie, apoi luminează REZULTATUL atribuirii, adică pe a" << std::endl;
    (a = c) += 3;
    std::cout << "    a: " << a.descriere() << "   <- luminată" << std::endl
              << "    c: " << c.descriere() << "   <- neschimbată" << std::endl;

    std::cout << std::endl << "  a = b = '#';         // și operator=(char) întoarce Imagine&, deci se poate înlănțui"
              << std::endl;
    a = b = '#';
    std::cout << "    a: " << a.descriere() << std::endl << "    b: " << b.descriere() << std::endl;
}

void imagineReturnValoare()
{
    subtitlu("10.c) exemple::ImagineReturnValoare: operator= și += întorc o COPIE (prin valoare)");

    exemple::ImagineReturnValoare a("a", "          ");
    exemple::ImagineReturnValoare b("b", "..........");
    exemple::ImagineReturnValoare c("c", "..::--==++");

    std::cout << "  a = b = c;" << std::endl;
    const int inainte = galerie::Bitmap::totalCreate();
    a = b = c;
    const int bitmapuriNoi = galerie::Bitmap::totalCreate() - inainte;
    std::cout << "    a: " << a.descriere() << std::endl
              << "  Bitmap-uri noi: " << bitmapuriNoi
              << " (2 necesare + 2 copii temporare întoarse de operator=, create și șterse degeaba)" << std::endl;

    std::cout << std::endl << "  (a = c) += 3;" << std::endl;
    (a = c) += 3;
    std::cout << "    a: " << a.descriere() << "   <- NU s-a luminat!" << std::endl
              << "  => += a lucrat pe copia temporară întoarsă de operator=, care a dispărut imediat." << std::endl
              << "     Codul compilează, dar face altceva decât pare: o greșeală care nu se vede." << std::endl;
}

} // namespace

void demonstreazaItem10()
{
    titlu("Item 10: Have assignment operators return a reference to *this");

    tipuriPredefinite();
    imagineCorecta();
    imagineReturnValoare();

    subtitlu("10.d) Dacă operator= întoarce void, a = b = c nici nu compilează");
    std::cout << "  error: no match for 'operator=' (operand types are '...' and 'void')" << std::endl
              << "  (eroarea reală: make erori, fișierul item10_operator_egal_void.cpp)" << std::endl;
}
