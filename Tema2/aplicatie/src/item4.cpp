// Item 4: Make sure that objects are initialized before they're used.

#include "demonstratii.hpp"
#include "exemple_item4.hpp"
#include "fiasco.hpp"

#include <secretariat/FisaStudent.hpp>

#include <cstring>
#include <iostream>
#include <new>

namespace {

///\brief O zonă de memorie "folosită anterior", în care construim obiecte cu placement new.
///
/// Variabilele locale ajung pe stivă, în memorie folosită înainte de alte funcții, deci plină
/// de valori vechi. Ca rezultatul să fie același la fiecare rulare, simulăm asta: umplem zona
/// cu octeți 0x5A și construim obiectul chiar acolo. Un membru neinițializat păstrează ce era
/// deja în memorie (pentru int: 0x5A5A5A5A = 1515870810).
template <typename T>
struct MemorieFolosita
{
    MemorieFolosita()
    {
        std::memset(octeti, 0x5A, sizeof(octeti));
    }

    alignas(T) unsigned char octeti[sizeof(T)];
};

// Obiect cu durată de stocare statică: e umplut cu zero înainte de orice altă inițializare.
exemple::Punctaj punctajStatic;

void afiseazaPunctaj(const std::string& declaratie, const exemple::Punctaj& p)
{
    std::cout << "  " << aliniaza(declaratie, 46) << "-> laborator = " << p.laborator << ", examen = " << p.examen
              << std::endl;
}

void membriNeinitializati()
{
    subtitlu("4.a) Membrii de tip predefinit (int, double) NU se inițializează singuri");
    std::cout << "Construim ambele fișe în aceeași memorie \"folosită\" (umplută cu octeți 0x5A) și le consultăm o dată:"
              << std::endl;

    {
        MemorieFolosita<exemple::FisaFaraInitializare> memorie;
        auto* fisa = new (memorie.octeti) exemple::FisaFaraInitializare("Ana");
        fisa->consulta();
        std::cout << "  exemple::FisaFaraInitializare -> ";
        fisa->afiseaza();
        fisa->~FisaFaraInitializare();
    }
    {
        MemorieFolosita<secretariat::FisaStudent> memorie;
        auto* fisa = new (memorie.octeti) secretariat::FisaStudent("Ana", 2, 9, 10);
        fisa->consulta();
        std::cout << "  secretariat::FisaStudent      -> ";
        fisa->afiseaza();
        fisa->~FisaStudent();
    }
    {
        MemorieFolosita<secretariat::FisaStudent> memorie;
        auto* fisa = new (memorie.octeti) secretariat::FisaStudent();
        std::cout << "  secretariat::FisaStudent()    -> ";
        fisa->afiseaza();
        fisa->~FisaStudent();
    }
    std::cout << "=> Fișa fără inițializare a fost \"consultată\" de peste un miliard de ori: contorul a pornit"
              << std::endl
              << "   de la valoarea rămasă în memorie. secretariat::FisaStudent își inițializează toți membrii,"
              << std::endl
              << "   inclusiv în constructorul implicit, deci e corectă oriunde ar fi construită." << std::endl;
}

void reguliDeInitializare()
{
    subtitlu("4.b) Uneori C++ inițializează singur, alteori nu");
    std::cout << "Același tip (struct Punctaj { int laborator; int examen; }), declarat în feluri diferite:" << std::endl;

    MemorieFolosita<exemple::Punctaj> memorie1;
    auto* implicit = new (memorie1.octeti) exemple::Punctaj;       // inițializare implicită: nu se face nimic
    afiseazaPunctaj("Punctaj p;          (variabilă locală)", *implicit);

    MemorieFolosita<exemple::Punctaj> memorie2;
    auto* cuValoare = new (memorie2.octeti) exemple::Punctaj{};    // inițializare cu valoare: zero
    afiseazaPunctaj("Punctaj p{};        (cu acolade goale)", *cuValoare);

    afiseazaPunctaj("static Punctaj p;   (durată statică)", punctajStatic);

    const exemple::Punctaj explicitInitializat{9, 10};
    afiseazaPunctaj("Punctaj p{9, 10};   (valori explicite)", explicitInitializat);

    std::cout << "=> Regulile sunt prea complicate ca să ne bazăm pe ele. Regula din carte: inițializați MEREU" << std::endl
              << "   manual obiectele de tip predefinit, iar în clase lăsați constructorii să inițializeze tot."
              << std::endl;
}

void listaDeInitializare()
{
    subtitlu("4.c) Lista de inițializare: preferată atribuirii și scrisă în ordinea declarării");

    std::cout << "i) Membru setat în CORPUL constructorului (exemple::AtribuireInCorp), apoi distrus:" << std::endl;
    {
        exemple::AtribuireInCorp atribuire;
    }
    std::cout << "   => constructorul a făcut 4 operații: construire implicită, obiect temporar, atribuire," << std::endl
              << "      distrugerea temporarului (ultima linie e destructorul obiectului, la ieșirea din bloc)"
              << std::endl;

    std::cout << "   Membru setat în LISTA de inițializare (exemple::InitializareInLista), apoi distrus:" << std::endl;
    {
        exemple::InitializareInLista lista;
    }
    std::cout << "   => constructorul a făcut o singură operație: construire directă cu valoarea dorită" << std::endl;

    std::cout << std::endl
              << "ii) Lista din exemple::OrdineInitializare e scrisă: alTreilea_, alDoilea_, primul_." << std::endl
              << "    Ordinea în care se construiesc de fapt:" << std::endl;
    {
        exemple::OrdineInitializare ordine;
        std::cout << "    => ordinea DECLARĂRII din clasă, nu cea din listă. Distrugerea se face invers:" << std::endl;
    }

    std::cout << std::endl << "iii) Consecința: un membru calculat din membri încă neinițializați" << std::endl;
    {
        MemorieFolosita<exemple::NotaFinalaGresita> memorie;
        auto* gresita = new (memorie.octeti) exemple::NotaFinalaGresita(9, 10);
        std::cout << "  exemple::NotaFinalaGresita(9, 10) -> ";
        gresita->afiseaza();
    }
    {
        MemorieFolosita<secretariat::FisaStudent> memorie;
        auto* corecta = new (memorie.octeti) secretariat::FisaStudent("Ana", 2, 9, 10);
        std::cout << "  secretariat::FisaStudent(\"Ana\", 2, 9, 10) -> medie " << corecta->medie() << std::endl;
        corecta->~FisaStudent();
    }
    std::cout << "=> În NotaFinalaGresita, medie_ e declarată înaintea notelor, deci e calculată prima, din" << std::endl
              << "   valorile rămase în memorie. În FisaStudent notele sunt declarate înaintea mediei." << std::endl;
}

void fiascoObiecteGlobale()
{
    subtitlu("4.d) Obiecte globale din fișiere .cpp diferite: ordinea lor de inițializare nu e garantată");

    const int locuriCitite = fiasco::grupaCuObiectGlobal.locuri();
    const int locuriActuale = fiasco::configuratieGlobala.locuriPeGrupa();

    std::cout << "Primele mesaje ale programului (dinainte de main) arată ordinea în care s-au construit." << std::endl
              << "  " << fiasco::grupaCuObiectGlobal.nume() << ": " << locuriCitite
              << " locuri, deși configurația globală are acum " << locuriActuale << std::endl;
    if (locuriCitite != locuriActuale) {
        std::cout << "  => grupa a citit configurația ÎNAINTE ca aceasta să fie construită (comportament nedefinit)"
                  << std::endl;
    } else {
        std::cout << "  => de data asta ordinea a fost favorabilă, dar standardul nu o garantează" << std::endl;
    }

    std::cout << "  " << fiasco::grupaCuStaticLocal.nume() << ": " << fiasco::grupaCuStaticLocal.locuri() << " locuri"
              << std::endl
              << "  => corect: secretariat::configuratie() a construit obiectul static local chiar la primul apel,"
              << std::endl
              << "     adică exact când grupa a avut nevoie de el." << std::endl;
}

} // namespace

void demonstreazaItem4()
{
    titlu("Item 4: Make sure that objects are initialized before they're used");

    membriNeinitializati();
    reguliDeInitializare();
    listaDeInitializare();
    fiascoObiecteGlobale();
}
