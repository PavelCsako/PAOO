// Item 13: Use objects to manage resources.

#include "demonstratii.hpp"

#include <galerie/Bitmap.hpp>
#include <galerie/Fabrica.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

// Aruncă o excepție dacă Bitmap-ul are pixeli în afara scalei de luminozitate (de ex. "stricat.img")
void verificaPixeli(const galerie::Bitmap& bitmap)
{
    if (bitmap.pixeli().find_first_not_of(" .:-=+*#%@") != std::string::npos) {
        throw std::runtime_error("pixeli necunoscuți în [" + bitmap.pixeli() + "]");
    }
}

// Varianta din carte, cu pointer simplu: delete-ul de la final se execută doar dacă execuția ajunge acolo.
void previzualizeazaCuDelete(const std::string& fisier)
{
    galerie::Bitmap* bitmap = galerie::incarcaBitmap(fisier);

    if (bitmap->pixeli().empty()) {
        std::cout << "    imagine goală: ies mai devreme din funcție (return)" << std::endl;
        return;                                 // delete-ul de mai jos nu se mai execută
    }
    verificaPixeli(*bitmap);                    // dacă aruncă, delete-ul nu se mai execută
    std::cout << "    previzualizare: [" << bitmap->pixeli() << "]" << std::endl;

    delete bitmap;
}

// Aceeași funcție, dar resursa e dată IMEDIAT unui obiect care o gestionează (RAII).
void previzualizeazaCuUniquePtr(const std::string& fisier)
{
    std::unique_ptr<galerie::Bitmap> bitmap(galerie::incarcaBitmap(fisier));

    if (bitmap->pixeli().empty()) {
        std::cout << "    imagine goală: ies mai devreme din funcție (return)" << std::endl;
        return;                                 // destructorul lui unique_ptr face delete
    }
    verificaPixeli(*bitmap);                    // la excepție, tot destructorul face delete
    std::cout << "    previzualizare: [" << bitmap->pixeli() << "]" << std::endl;
}                                               // la final, la fel: delete automat

int ruleazaSiNumaraPierderile(void (*previzualizeaza)(const std::string&), const std::string& fisier)
{
    const int inainte = galerie::Bitmap::inViata();
    try {
        previzualizeaza(fisier);
    } catch (const std::exception& exceptie) {
        std::cout << "    excepție prinsă în afara funcției: " << exceptie.what() << std::endl;
    }
    const int pierdute = galerie::Bitmap::inViata() - inainte;
    std::cout << "    => Bitmap-uri rămase alocate: " << pierdute << (pierdute > 0 ? "  <-- MEMORY LEAK" : "")
              << std::endl;
    return pierdute;
}

void deleteManualSauObiect()
{
    subtitlu("13.a) Aceeași funcție, cu delete manual și cu std::unique_ptr, pe trei drumuri de ieșire");

    struct Caz
    {
        const char* drum;
        const char* fisier;
    };
    const Caz cazuri[] = {
        {"drumul normal", "apus.img"},
        {"return mai devreme", "gol.img"},
        {"excepție", "stricat.img"},
    };

    int pierduteManual[3] = {0, 0, 0};
    int pierduteObiect[3] = {0, 0, 0};

    for (int i = 0; i < 3; ++i) {
        std::cout << std::endl << "  Cazul " << i + 1 << ": " << cazuri[i].drum << " (" << cazuri[i].fisier << ")" << std::endl;
        std::cout << "  - cu delete manual:" << std::endl;
        pierduteManual[i] = ruleazaSiNumaraPierderile(previzualizeazaCuDelete, cazuri[i].fisier);
        std::cout << "  - cu std::unique_ptr:" << std::endl;
        pierduteObiect[i] = ruleazaSiNumaraPierderile(previzualizeazaCuUniquePtr, cazuri[i].fisier);
    }

    std::cout << std::endl
              << "  " << aliniaza("drum de ieșire", 22) << aliniaza("delete manual", 18) << "std::unique_ptr" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "  " << aliniaza(cazuri[i].drum, 22)
                  << aliniaza(pierduteManual[i] == 0 ? "eliberat" : "LEAK", 18)
                  << (pierduteObiect[i] == 0 ? "eliberat" : "LEAK") << std::endl;
    }
    std::cout << "=> Destructorul obiectului care deține resursa rulează oricum s-ar ieși din bloc. Resursa e dată"
              << std::endl
              << "   obiectului chiar în momentul obținerii ei: Resource Acquisition Is Initialization (RAII)." << std::endl;
}

void unSingurProprietar()
{
    subtitlu("13.b) std::unique_ptr: un singur proprietar");

    auto p1 = std::make_unique<galerie::Bitmap>("..::--==++");

    // auto p2 = p1;
    // error: use of deleted function 'std::unique_ptr<...>::unique_ptr(const std::unique_ptr<...>&)'
    std::cout << "  auto p2 = p1;              -> NU compilează: unique_ptr nu se copiază (make erori)" << std::endl;

    auto p3 = std::move(p1);
    std::cout << "  auto p3 = std::move(p1);   -> p1 " << (p1 == nullptr ? "e nullptr" : "deține ceva")
              << ", p3 deține Bitmap #" << p3->id() << std::endl
              << "  În carte, std::auto_ptr făcea același transfer la o copiere obișnuită, fără std::move," << std::endl
              << "  ceea ce surprindea; auto_ptr a fost scos din standard în C++17." << std::endl;
}

void proprietariMultipli()
{
    subtitlu("13.c) std::shared_ptr: mai mulți proprietari, ultimul face delete");

    std::shared_ptr<galerie::Bitmap> s1(galerie::incarcaBitmap("luna.img"));
    std::cout << "  s1 creat:                  use_count = " << s1.use_count() << std::endl;
    {
        std::shared_ptr<galerie::Bitmap> s2 = s1;
        std::cout << "  auto s2 = s1;              use_count = " << s1.use_count() << std::endl;
        std::shared_ptr<galerie::Bitmap> s3 = s2;
        std::cout << "  auto s3 = s2;              use_count = " << s1.use_count() << std::endl;
    }
    std::cout << "  s2 și s3 au dispărut:      use_count = " << s1.use_count()
              << " (Bitmap-ul există încă, are un proprietar)" << std::endl;
    std::cout << "  s1.reset();                // ultimul proprietar renunță:" << std::endl;
    s1.reset();
    std::cout << "  (în carte: tr1::shared_ptr; din C++11 face parte din standard, ca std::shared_ptr)" << std::endl;
}

void tablouri()
{
    subtitlu("13.d) Tablouri: std::unique_ptr<T[]> apelează delete[]");

    {
        std::unique_ptr<galerie::Bitmap[]> bitmapuri(
            new galerie::Bitmap[3]{galerie::Bitmap("...."), galerie::Bitmap("----"), galerie::Bitmap("####")});
        std::cout << "  la ieșirea din bloc, unique_ptr<Bitmap[]> face delete[]:" << std::endl;
    }
    std::cout << "  => Toate cele trei Bitmap-uri au fost șterse. Cu std::unique_ptr<Bitmap> (fără []) s-ar fi apelat"
              << std::endl
              << "     delete în loc de delete[]: comportament nedefinit. De obicei e mai simplu std::vector." << std::endl;
}

} // namespace

void demonstreazaItem13()
{
    titlu("Item 13: Use objects to manage resources");

    deleteManualSauObiect();
    unSingurProprietar();
    proprietariMultipli();
    tablouri();
}
