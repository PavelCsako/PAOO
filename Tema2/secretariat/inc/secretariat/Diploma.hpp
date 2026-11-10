#ifndef SECRETARIAT_DIPLOMA_HPP
#define SECRETARIAT_DIPLOMA_HPP

#include <string>

namespace secretariat {

///\brief O diplomă emisă de secretariat (varianta CORECTĂ pentru Item 6).
///
/// Fiecare diplomă are un număr de serie unic, deci copierea ei nu are sens:
/// o copie ar fi o diplomă falsă, cu aceeași serie. Interzicem explicit funcțiile pe care
/// altfel compilatorul le-ar genera în tăcere (Item 5), cu "= delete" (C++11).
/// Variantele mai vechi și varianta fără protecție sunt în aplicatie/src/exemple_item6.hpp.
class Diploma
{
public:
    Diploma(const std::string& titular, const std::string& specializare);

    ~Diploma();

    Diploma(const Diploma&) = delete;               // fără copy constructor
    Diploma& operator=(const Diploma&) = delete;    // fără copy assignment

    // Pentru că am declarat operațiile de copiere, compilatorul nu mai generează nici
    // move constructor / move assignment: o diplomă nici nu poate fi "mutată" în alt obiect.

public:
    int serie() const;

    const std::string& titular() const;

    void afiseaza() const;

    static int inCirculatie();      // câte diplome există în acest moment

private:
    static int urmatoareaSerie();

private:
    const int serie_;
    std::string titular_;
    std::string specializare_;
};

} // namespace secretariat

#endif // SECRETARIAT_DIPLOMA_HPP
