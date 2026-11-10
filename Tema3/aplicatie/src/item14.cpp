// Item 14: Think carefully about copying behavior in resource-managing classes.

#include "demonstratii.hpp"
#include "exemple_item14.hpp"

#include <galerie/Imagine.hpp>
#include <galerie/Lacat.hpp>
#include <galerie/LacatPartajat.hpp>
#include <galerie/LacatTransferabil.hpp>
#include <galerie/Mutex.hpp>

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

// Ce permite fiecare clasă, verificat la compilare
static_assert(std::is_copy_constructible_v<exemple::LacatCopiabil>, "problema: copia generată de compilator");
static_assert(!std::is_copy_constructible_v<galerie::Lacat> && !std::is_copy_assignable_v<galerie::Lacat>,
              "opțiunea 1: copiere interzisă");
static_assert(std::is_copy_constructible_v<galerie::LacatPartajat>, "opțiunea 2: copiile împart mutex-ul");
static_assert(!std::is_copy_constructible_v<galerie::LacatTransferabil>
                  && std::is_move_constructible_v<galerie::LacatTransferabil>,
              "opțiunea 4: nu se copiază, se mută");

namespace {

void stare(const galerie::Mutex& mutex)
{
    std::cout << "    -> mutex \"" << mutex.nume << "\": " << (mutex.blocat ? "BLOCAT" : "deblocat")
              << ", greșeli de folosire: " << mutex.erori << std::endl;
}

void lacatPentruOResursaCareNuEMemorie()
{
    subtitlu("14.a) RAII și pentru resurse care nu sunt memorie: un mutex");

    std::cout << "  Fără obiect: blocheaza(&m), o excepție, apoi deblocheaza(&m) nu se mai execută" << std::endl;
    galerie::Mutex manual{"manual"};
    try {
        galerie::blocheaza(&manual);
        throw std::runtime_error("eroare la salvarea galeriei");
        // galerie::deblocheaza(&manual);   // nu se ajunge aici
    } catch (const std::exception& exceptie) {
        std::cout << "    excepție prinsă: " << exceptie.what() << std::endl;
    }
    stare(manual);
    std::cout << "    (mutex-ul a rămas blocat: orice alt fir care îl așteaptă s-ar bloca pentru totdeauna)" << std::endl;

    std::cout << "  Cu galerie::Lacat (constructorul blochează, destructorul deblochează), aceeași excepție:" << std::endl;
    galerie::Mutex cuLacat{"cu Lacat"};
    try {
        galerie::Lacat lacat(&cuLacat);
        throw std::runtime_error("eroare la salvarea galeriei");
    } catch (const std::exception& exceptie) {
        std::cout << "    excepție prinsă: " << exceptie.what() << std::endl;
    }
    stare(cuLacat);
}

void scrieInJurnal([[maybe_unused]] exemple::LacatCopiabil copie)    // parametru prin valoare => copie
{
    std::cout << "    în scrieInJurnal: lucrez cu o COPIE a lacătului; la ieșire, copia deblochează mutex-ul"
              << std::endl;
}

void copiaGenerataDeCompilator()
{
    subtitlu("14.b) Problema: copia generată de compilator (exemple::LacatCopiabil)");

    galerie::Mutex mutex{"galerie"};
    {
        exemple::LacatCopiabil lacat(&mutex);
        std::cout << "  scrieInJurnal(lacat);    // transmis prin valoare, deci copiat" << std::endl;
        scrieInJurnal(lacat);
        std::cout << "  Înapoi în bloc: lacat există încă, dar" << std::endl;
        stare(mutex);
        std::cout << "    (secțiunea critică nu mai e protejată, deși codul pare corect)" << std::endl
                  << "  La ieșirea din bloc, destructorul lui lacat deblochează a doua oară:" << std::endl;
    }
    stare(mutex);
}

void optiuneaInterzicere()
{
    subtitlu("14.c) Opțiunea 1: copierea interzisă (galerie::Lacat), cea mai potrivită pentru un lacăt");

    galerie::Mutex mutex{"galerie"};
    {
        galerie::Lacat lacat(&mutex);
        // galerie::Lacat copie(lacat);
        // error: use of deleted function 'galerie::Lacat::Lacat(const galerie::Lacat&)'
        std::cout << "  galerie::Lacat copie(lacat);  -> NU compilează: use of deleted function (make erori)" << std::endl;
    }
    stare(mutex);
}

void optiuneaNumarareReferinte()
{
    subtitlu("14.d) Opțiunea 2: numărarea referințelor (galerie::LacatPartajat, shared_ptr cu deleter)");

    galerie::Mutex mutex{"galerie"};
    {
        galerie::LacatPartajat l1(&mutex);
        std::cout << "  LacatPartajat l1(&mutex);    copii: " << l1.nrCopii() << std::endl;
        {
            galerie::LacatPartajat l2(l1);
            std::cout << "  LacatPartajat l2(l1);        copii: " << l1.nrCopii() << std::endl;
            galerie::LacatPartajat l3 = l2;
            std::cout << "  LacatPartajat l3 = l2;       copii: " << l1.nrCopii() << std::endl;
        }
        std::cout << "  l2 și l3 au dispărut:        copii: " << l1.nrCopii() << std::endl;
        stare(mutex);
        std::cout << "  Dispare și l1, ultima copie: deleter-ul shared_ptr-ului apelează deblocheaza" << std::endl;
    }
    stare(mutex);
}

void optiuneaCopieInAdancime()
{
    subtitlu("14.e) Opțiunea 3: copia în adâncime, potrivită când resursa e memorie");

    galerie::Imagine original("apus", "..::--==++");
    galerie::Imagine copie(original);
    std::cout << "  galerie::Imagine copie(original);  -> fiecare are Bitmap-ul ei:" << std::endl
              << "    original: " << original.descriere() << std::endl
              << "    copie:    " << copie.descriere() << std::endl
              << "  La fel face std::string. Pentru un mutex nu are sens: o „copie” ar fi alt mutex, care nu mai"
              << std::endl
              << "  protejează aceleași date." << std::endl;
}

void salveazaGaleria(galerie::LacatTransferabil lacat)      // primește lacătul (prin mutare)
{
    std::cout << "    în salveazaGaleria: dețin lacătul (" << (lacat.detineMutex() ? "da" : "nu")
              << "); îl deblochez la ieșire" << std::endl;
}

void optiuneaTransfer()
{
    subtitlu("14.f) Opțiunea 4: transferul proprietății (galerie::LacatTransferabil, unique_ptr cu deleter)");

    galerie::Mutex mutex{"galerie"};
    {
        galerie::LacatTransferabil lacat(&mutex);
        // galerie::LacatTransferabil copie(lacat);
        // error: use of deleted function 'galerie::LacatTransferabil::LacatTransferabil(const galerie::LacatTransferabil&)'
        std::cout << "  salveazaGaleria(std::move(lacat));   // lacătul trece în funcție" << std::endl;
        salveazaGaleria(std::move(lacat));
        std::cout << "  Înapoi în bloc: lacat.detineMutex() = " << std::boolalpha << lacat.detineMutex()
                  << std::noboolalpha << std::endl;
        stare(mutex);
        std::cout << "  La ieșirea din bloc, lacat e gol și nu mai deblochează nimic:" << std::endl;
    }
    stare(mutex);
}

void rezumat()
{
    subtitlu("Rezumat Item 14: ce face copia unui obiect care gestionează o resursă?");
    std::cout << "  " << aliniaza("opțiune", 26) << aliniaza("clasa din proiect", 30) << "potrivită pentru" << std::endl
              << "  " << aliniaza("1. interzice copierea", 26) << aliniaza("galerie::Lacat", 30)
              << "lacăte, conexiuni, fișiere" << std::endl
              << "  " << aliniaza("2. numără referințele", 26) << aliniaza("galerie::LacatPartajat", 30)
              << "resurse partajate (shared_ptr)" << std::endl
              << "  " << aliniaza("3. copie în adâncime", 26) << aliniaza("galerie::Imagine", 30)
              << "memorie (std::string, Bitmap)" << std::endl
              << "  " << aliniaza("4. transferă resursa", 26) << aliniaza("galerie::LacatTransferabil", 30)
              << "un singur proprietar (unique_ptr)" << std::endl
              << "  " << aliniaza("(nicio decizie)", 26) << aliniaza("exemple::LacatCopiabil", 30)
              << "nimic: copia deblochează de 2 ori" << std::endl;
}

} // namespace

void demonstreazaItem14()
{
    titlu("Item 14: Think carefully about copying behavior in resource-managing classes");

    lacatPentruOResursaCareNuEMemorie();
    copiaGenerataDeCompilator();
    optiuneaInterzicere();
    optiuneaNumarareReferinte();
    optiuneaCopieInAdancime();
    optiuneaTransfer();
    rezumat();
}
