#include <catalog/Student.hpp>   // header-ul exportat de biblioteca statică

#include "exemple.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

namespace {

void titlu(const std::string& text)
{
    std::cout << std::endl
              << "==============================================================" << std::endl
              << " " << text << std::endl
              << "==============================================================" << std::endl;
}

///\brief Rulează un exemplu care se prăbușește într-un proces copil (fork),
/// ca programul principal să poată continua și să spună ce s-a întâmplat.
void ruleazaInProcesSeparat(void (*exemplu)())
{
    std::cout.flush();
    const pid_t pid = fork();
    if (pid < 0) {
        std::cout << "fork() a eșuat, sar peste exemplu" << std::endl;
        return;
    }
    if (pid == 0) {
        exemplu();          // la ieșirea din funcție se apelează destructorii
        std::cout.flush();
        _exit(0);
    }

    int stare = 0;
    waitpid(pid, &stare, 0);
    if (WIFSIGNALED(stare)) {
        std::cout << ">>> Procesul s-a PRĂBUȘIT cu semnalul " << WTERMSIG(stare)
                  << " (" << strsignal(WTERMSIG(stare)) << "): aceeași memorie a fost eliberată de două ori"
                  << std::endl;
    } else {
        std::cout << ">>> Procesul nu s-a prăbușit de data asta, dar memoria a fost eliberată de două ori"
                  << " (comportament nedefinit)" << std::endl;
    }
}

void afiseazaPrinValoare(catalog::Student s)     // parametru transmis prin valoare => copie
{
    std::cout << "  în funcție: ";
    s.afiseaza();
}

// ---------------------------------------------------------------------------

void exempluIncapsulare()
{
    titlu("2. Încapsulare");

    catalog::Student ana(1001, "Ana", 3);
    ana.adaugaNota(9);
    ana.adaugaNota(10);

    // Datele sunt private, deci nu pot fi stricate din exterior. Nu compilează:
    //   ana.note_[0] = 100;   // error: 'int* catalog::Student::note_' is private within this context
    //   ana.nrNote_ = 50;     // altfel medie() ar citi 50 de note dintr-un tablou de 3
    // Singura cale e prin metodele publice, care verifică datele:
    std::cout << "ana.adaugaNota(11):" << std::endl;
    ana.adaugaNota(11);
    std::cout << "ana.modificaNota(7, 5):" << std::endl;
    ana.modificaNota(7, 5);
    std::cout << "ana.adaugaNota(8), apoi ana.adaugaNota(7) când tabloul e plin:" << std::endl;
    ana.adaugaNota(8);
    ana.adaugaNota(7);              // fără verificare s-ar fi scris în afara tabloului

    std::cout << "  ";
    ana.afiseaza();
}

void exempluInitializare()
{
    titlu("3. Inițializarea membrilor în constructor");

    std::cout << "a) Membru setat în CORPUL constructorului (2 operații: construire implicită + atribuire):"
              << std::endl;
    exemple::InitializareInCorp a("Ana");

    std::cout << "b) Membru setat în LISTA DE INIȚIALIZARE (1 operație: construire directă):" << std::endl;
    exemple::InitializareInLista b("Ana", 1001);

    std::cout << "c) Membrii const (ex. nrMatricol_ = " << b.nrMatricol()
              << ") și referințele se pot inițializa DOAR în listă." << std::endl
              << "   catalog::Student își inițializează toți membrii acolo, deci e valid imediat:" << std::endl;
    catalog::Student s(1002, "Bogdan", 4);
    std::cout << "  ";
    s.afiseaza();
}

void exempluDestructor()
{
    titlu("4. Eliberarea heap-ului în destructor");

    std::cout << "a) Clasa FĂRĂ destructor:" << std::endl;
    {
        exemple::StudentFaraDestructor x("Ana", 5);
        exemple::StudentFaraDestructor y("Bogdan", 5);
    }   // x și y dispar, dar memoria lor de pe heap rămâne alocată
    std::cout << "   după ieșirea din bloc, blocuri rămase pe heap: "
              << exemple::StudentFaraDestructor::blocuriPeHeap << "  <-- memory leak" << std::endl;

    std::cout << "b) catalog::Student (cu destructor):" << std::endl;
    const int inainte = catalog::Student::blocuriPeHeap();
    {
        catalog::Student x(2001, "Ana", 5);
        catalog::Student* y = new catalog::Student(2002, "Bogdan", 5);  // obiectul însuși e pe heap
        delete y;           // delete apelează destructorul
    }   // destructorul lui x se apelează automat aici
    std::cout << "   după ieșirea din bloc, blocuri rămase pe heap: "
              << catalog::Student::blocuriPeHeap() - inainte << std::endl;
}

void exempluCopieSuperficiala()
{
    exemple::StudentFaraCopyConstructor ana("Ana", 5);
    ana.adaugaNota(9);
    ana.adaugaNota(10);
    ana.adaugaNota(8);
    std::cout << "  original înainte de copiere: ";
    ana.afiseaza();
    {
        exemple::StudentFaraCopyConstructor copie = ana;   // copy constructor GENERAT de compilator
        copie.modificaNota(0, 4);                          // modificăm DOAR copia...
        std::cout << "  original: ";
        ana.afiseaza();                                    // ...dar s-a schimbat și originalul!
        std::cout << "  copie:    ";
        copie.afiseaza();
    }   // copia e distrusă => eliberează memoria pe care o folosește încă originalul

    std::cout << "  original după distrugerea copiei (memorie deja eliberată => valori aleatoare): ";
    ana.afiseaza();
    std::cout << "  la ieșire, destructorul originalului eliberează din nou aceeași adresă:" << std::endl;
}

void exempluCopyConstructor()
{
    titlu("5. Copy constructor");

    std::cout << "a) Clasa FĂRĂ copy constructor (rulează într-un proces separat, pentru că se prăbușește):"
              << std::endl;
    ruleazaInProcesSeparat(exempluCopieSuperficiala);

    std::cout << std::endl << "b) catalog::Student (copy constructor cu copie în adâncime):" << std::endl;
    catalog::Student ana(3001, "Ana", 5);
    ana.adaugaNota(9);
    ana.adaugaNota(10);
    ana.adaugaNota(8);
    {
        catalog::Student copie = ana;       // apelează copy constructor-ul nostru
        copie.modificaNota(0, 4);           // modificăm doar copia
        std::cout << "  original: ";
        ana.afiseaza();                     // originalul a rămas neschimbat, adresele diferă
        std::cout << "  copie:    ";
        copie.afiseaza();
    }   // copia își eliberează DOAR propria memorie

    std::cout << "  original după distrugerea copiei: ";
    ana.afiseaza();

    std::cout << "Copy constructor-ul se apelează și implicit, de ex. la transmiterea prin valoare:" << std::endl;
    afiseazaPrinValoare(ana);
}   // ana își eliberează memoria o singură dată => fără double free

void exempluMoveConstructor()
{
    titlu("6. Move constructor");

    std::cout << "a) Mutare explicită cu std::move:" << std::endl;
    {
        catalog::Student ana(4001, "Ana", 5);
        ana.adaugaNota(10);
        ana.adaugaNota(9);

        const int alocariInainte = catalog::Student::totalAlocari();
        catalog::Student mutat = std::move(ana);    // apelează move constructor-ul
        std::cout << "  sursa:      ";
        ana.afiseaza();
        std::cout << "  destinația: ";
        mutat.afiseaza();
        std::cout << "  alocări noi făcute de mutare: "
                  << catalog::Student::totalAlocari() - alocariInainte << std::endl;
    }

    std::cout << std::endl
              << "b) De ce e nevoie de el: std::vector mută (sau copiază) elementele când se realocă."
              << std::endl
              << "-- clasa FĂRĂ move constructor:" << std::endl;
    const int inainteFaraMove = exemple::StudentFaraMoveConstructor::totalAlocari;
    {
        std::vector<exemple::StudentFaraMoveConstructor> grupa;
        grupa.push_back(exemple::StudentFaraMoveConstructor("Ana", 5));
        grupa.push_back(exemple::StudentFaraMoveConstructor("Bogdan", 5));
        grupa.push_back(exemple::StudentFaraMoveConstructor("Cristina", 5));
    }
    const int alocariFaraMove = exemple::StudentFaraMoveConstructor::totalAlocari - inainteFaraMove;

    std::cout << "-- catalog::Student (cu move constructor noexcept):" << std::endl;
    const int inainteCuMove = catalog::Student::totalAlocari();
    {
        std::vector<catalog::Student> grupa;
        grupa.push_back(catalog::Student(5001, "Ana", 5));
        grupa.push_back(catalog::Student(5002, "Bogdan", 5));
        grupa.push_back(catalog::Student(5003, "Cristina", 5));
    }
    const int alocariCuMove = catalog::Student::totalAlocari() - inainteCuMove;

    std::cout << ">>> Pentru 3 studenți: " << alocariFaraMove << " alocări pe heap fără move constructor, "
              << alocariCuMove << " cu move constructor (doar cele de la creare)." << std::endl;
}

} // namespace

int main()
{
    std::cout << "PAOO - Tema 1: încapsulare, constructori, destructor, copy & move constructor" << std::endl;

    exempluIncapsulare();
    exempluInitializare();
    exempluDestructor();
    exempluCopyConstructor();
    exempluMoveConstructor();

    titlu("Final");
    std::cout << "Blocuri catalog::Student rămase pe heap: " << catalog::Student::blocuriPeHeap() << std::endl;
    std::cout << "Blocuri StudentFaraDestructor rămase pe heap: "
              << exemple::StudentFaraDestructor::blocuriPeHeap << " (pierdute definitiv)" << std::endl;

    return 0;
}
