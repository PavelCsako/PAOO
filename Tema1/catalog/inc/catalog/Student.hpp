#ifndef CATALOG_STUDENT_HPP
#define CATALOG_STUDENT_HPP

#include <string>

namespace catalog {

///\brief Un student cu număr matricol fix și o listă de note alocată pe heap.
///
/// Clasa își gestionează singură memoria pentru note (new[] / delete[]),
/// de aceea are nevoie de:
///   - un constructor care inițializează toți membrii (lista de inițializare),
///   - un destructor care eliberează heap-ul,
///   - un copy constructor care face copie "în adâncime" (memorie nouă),
///   - un move constructor care preia memoria fără să o copieze.
class Student
{
public:
    Student(int nrMatricol, const std::string& nume, int capacitate = 10);

    Student(const Student& altul);          // copy constructor

    Student(Student&& altul) noexcept;      // move constructor
                                            // noexcept => std::vector îl folosește la realocare

    ~Student();                             // destructor

    // Atribuirea nu e cerută în temă. O dezactivăm explicit: varianta generată
    // de compilator ar copia doar pointerul (aceeași problemă ca la copy constructor).
    Student& operator=(const Student&) = delete;
    Student& operator=(Student&&) = delete;

public:
    // Interfața publică: singura cale de a citi/modifica datele (încapsulare)
    bool adaugaNota(int nota);

    bool modificaNota(int index, int notaNoua);

    double medie() const;

    int nrNote() const;

    int nrMatricol() const;

    const std::string& nume() const;

    void afiseaza() const;

public:
    // Statistici despre memoria alocată de toți studenții (pentru demonstrații)
    static int blocuriPeHeap();     // câte blocuri de note sunt alocate în acest moment

    static int totalAlocari();      // câte alocări s-au făcut de la pornirea programului

private:
    static bool notaValida(int nota);

private:
    // Membrii sunt inițializați în ordinea în care sunt declarați aici
    // (nu în ordinea din lista de inițializare), de aceea capacitate_ e înaintea lui note_.
    const int nrMatricol_;          // const => poate fi inițializat DOAR în lista de inițializare
    std::string nume_;
    int capacitate_;
    int nrNote_;
    int* note_;                     // memorie pe heap deținută de obiect

    inline static int blocuriPeHeap_ = 0;
    inline static int totalAlocari_ = 0;
};

} // namespace catalog

#endif // CATALOG_STUDENT_HPP
