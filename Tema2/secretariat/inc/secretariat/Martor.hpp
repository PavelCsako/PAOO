#ifndef SECRETARIAT_MARTOR_HPP
#define SECRETARIAT_MARTOR_HPP

#include <string>

namespace secretariat {

///\brief "Martorul": un obiect care anunță la ieșirea standard fiecare funcție specială apelată pentru el.
///
/// Pus ca membru (sau în clasa de bază) al unei clase care NU își scrie singură constructorii,
/// operatorii de atribuire și destructorul, arată:
///   - ce funcții scrie și apelează compilatorul în locul nostru (Item 5),
///   - în ce ordine sunt inițializați membrii unei clase (Item 4).
class Martor
{
public:
    Martor();                                   // constructor implicit

    explicit Martor(const std::string& eticheta);

    Martor(const Martor& altul);                // copy constructor

    Martor(Martor&& altul);                     // move constructor

    Martor& operator=(const Martor& altul);     // copy assignment

    Martor& operator=(Martor&& altul);          // move assignment

    ~Martor();                                  // destructor

public:
    const std::string& eticheta() const;

private:
    void anunta(const std::string& ceFunctie) const;

private:
    std::string eticheta_;      // apare în fiecare mesaj, ca să știm despre ce membru e vorba
};

} // namespace secretariat

#endif // SECRETARIAT_MARTOR_HPP
