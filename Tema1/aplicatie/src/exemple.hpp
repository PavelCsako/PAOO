#ifndef EXEMPLE_HPP
#define EXEMPLE_HPP

// Clase mici folosite doar pentru demonstrații.
// Cele cu "Fara" în nume sunt scrise intenționat GREȘIT: fiecăreia îi lipsește
// exact unul dintre conceptele din temă, ca să se vadă ce se întâmplă fără el.
// Varianta corectă este catalog::Student din biblioteca statică.

#include <algorithm>
#include <iostream>
#include <string>

namespace exemple {

// ---------------------------------------------------------------------------
// 3. Inițializarea membrilor: în corpul constructorului vs. în lista de inițializare
// ---------------------------------------------------------------------------

///\brief Un membru care anunță fiecare operație care i se aplică
class Nume
{
public:
    Nume()
    {
        std::cout << "    Nume(): constructor implicit (valoare goală)" << std::endl;
    }

    Nume(const std::string& text)
        : text_(text)
    {
        std::cout << "    Nume(\"" << text_ << "\"): constructor cu parametru" << std::endl;
    }

    Nume& operator=(const std::string& text)
    {
        text_ = text;
        std::cout << "    Nume::operator=(\"" << text_ << "\"): atribuire" << std::endl;
        return *this;
    }

private:
    std::string text_;
};

class InitializareInCorp
{
public:
    InitializareInCorp(const std::string& nume)
    {
        // Când ajungem aici, nume_ a fost DEJA construit implicit (gol),
        // deci acum doar îi atribuim valoarea => două operații în loc de una.
        nume_ = nume;

        // Un membru const nu poate fi setat aici. Linia de mai jos nu compilează:
        //   nrMatricol_ = 1001;   // error: assignment of read-only member
    }

private:
    Nume nume_;
};

class InitializareInLista
{
public:
    InitializareInLista(const std::string& nume, int nrMatricol)
        : nrMatricol_(nrMatricol)   // pentru const (și pentru referințe) e singura variantă
        , nume_(nume)               // construit direct cu valoarea dorită
    {
    }

    int nrMatricol() const
    {
        return nrMatricol_;
    }

private:
    const int nrMatricol_;
    Nume nume_;
};

// ---------------------------------------------------------------------------
// 4. Clasă FĂRĂ destructor => memory leak
// ---------------------------------------------------------------------------

class StudentFaraDestructor
{
public:
    StudentFaraDestructor(const std::string& nume, int capacitate)
        : nume_(nume)
        , note_(new int[capacitate]())
    {
        ++blocuriPeHeap;
        std::cout << "[StudentFaraDestructor] constructor: " << nume_
                  << ", am alocat note la " << note_ << std::endl;
    }

    // LIPSEȘTE destructorul: cel generat de compilator distruge nume_,
    // dar NU face delete[] note_ => memoria rămâne alocată până la finalul programului.

    inline static int blocuriPeHeap = 0;

private:
    std::string nume_;
    int* note_;
};

// ---------------------------------------------------------------------------
// 5. Clasă FĂRĂ copy constructor => copie superficială (shallow copy)
// ---------------------------------------------------------------------------

class StudentFaraCopyConstructor
{
public:
    StudentFaraCopyConstructor(const std::string& nume, int capacitate)
        : nume_(nume)
        , nrNote_(0)
        , note_(new int[capacitate]())
    {
        std::cout << "[StudentFaraCopyConstructor] constructor: " << nume_
                  << ", am alocat note la " << note_ << std::endl;
    }

    // LIPSEȘTE copy constructor-ul: cel generat de compilator copiază membru cu membru,
    // adică doar VALOAREA pointerului note_. Copia și originalul ajung să folosească
    // aceeași zonă de memorie, iar fiecare o va elibera în destructor.

    ~StudentFaraCopyConstructor()
    {
        std::cout << "[StudentFaraCopyConstructor] destructor: " << nume_
                  << ", eliberez notele de la " << note_ << std::endl;
        delete[] note_;
    }

    void adaugaNota(int nota)
    {
        note_[nrNote_++] = nota;    // fără verificări, pentru simplitate
    }

    void modificaNota(int index, int nota)
    {
        note_[index] = nota;
    }

    void afiseaza() const
    {
        std::cout << nume_ << " note: ";
        for (int i = 0; i < nrNote_; ++i) {
            std::cout << note_[i] << ' ';
        }
        std::cout << "| notele la " << note_ << std::endl;
    }

private:
    std::string nume_;
    int nrNote_;
    int* note_;
};

// ---------------------------------------------------------------------------
// 6. Clasă FĂRĂ move constructor => orice "mutare" devine copiere
// ---------------------------------------------------------------------------

class StudentFaraMoveConstructor
{
public:
    StudentFaraMoveConstructor(const std::string& nume, int capacitate)
        : nume_(nume)
        , capacitate_(capacitate)
        , note_(new int[capacitate]())
    {
        ++totalAlocari;
        std::cout << "[StudentFaraMoveConstructor] constructor: " << nume_ << std::endl;
    }

    StudentFaraMoveConstructor(const StudentFaraMoveConstructor& altul)
        : nume_(altul.nume_)
        , capacitate_(altul.capacitate_)
        , note_(new int[altul.capacitate_]())
    {
        std::copy(altul.note_, altul.note_ + altul.capacitate_, note_);
        ++totalAlocari;
        std::cout << "[StudentFaraMoveConstructor] copy constructor: " << nume_
                  << " (alocare nouă + copiere)" << std::endl;
    }

    // LIPSEȘTE move constructor-ul. Pentru că am declarat copy constructor și destructor,
    // compilatorul NU mai generează unul implicit, așa că temporarele și realocările
    // din std::vector folosesc copy constructor-ul.

    ~StudentFaraMoveConstructor()
    {
        delete[] note_;
    }

    inline static int totalAlocari = 0;

private:
    std::string nume_;
    int capacitate_;
    int* note_;
};

} // namespace exemple

#endif // EXEMPLE_HPP
