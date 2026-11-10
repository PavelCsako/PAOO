#ifndef SECRETARIAT_FISASTUDENT_HPP
#define SECRETARIAT_FISASTUDENT_HPP

#include <string>

namespace secretariat {

///\brief Fișa unui student de la secretariat (varianta CORECTĂ pentru Item 4).
///
/// Corespunde clasei ABEntry din Effective C++, Item 4:
///   - TOȚI membrii sunt inițializați în lista de inițializare, inclusiv cei de tip
///     predefinit (int, double), care altfel ar conține "gunoi" din memorie;
///   - chiar și constructorul implicit îi inițializează pe toți;
///   - membrii apar în listă în ORDINEA DECLARĂRII lor, care e și ordinea reală de inițializare.
/// Varianta greșită, pentru comparație: exemple::FisaFaraInitializare (aplicatie/src/exemple_item4.hpp).
class FisaStudent
{
public:
    FisaStudent();          // fișă goală, dar validă

    FisaStudent(const std::string& nume, int an, double notaExamen, double notaLaborator);

public:
    void consulta();        // secretariatul ține evidența consultărilor fișei

    double medie() const;

    int nrConsultari() const;

    void afiseaza() const;

private:
    // Ordinea de aici este ordinea în care se inițializează membrii.
    // medie_ se calculează din cele două note, de aceea e declarată DUPĂ ele.
    std::string nume_;
    int an_;
    double notaExamen_;
    double notaLaborator_;
    double medie_;
    int nrConsultari_;
};

} // namespace secretariat

#endif // SECRETARIAT_FISASTUDENT_HPP
