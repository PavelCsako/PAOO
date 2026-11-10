#ifndef EXEMPLE_ITEM6_HPP
#define EXEMPLE_ITEM6_HPP

// Item 6: Explicitly disallow the use of compiler-generated functions you do not want.
//
// O diplomă are o serie unică, deci nu are sens să poată fi copiată (ca HomeForSale din carte).
// Aici sunt: varianta fără protecție (problema) și cele două variante de protecție din carte.
// Varianta modernă, cu "= delete" (C++11), este secretariat::Diploma din biblioteca statică.

#include <secretariat/Necopiabil.hpp>

#include <iostream>
#include <set>
#include <string>

namespace exemple {

// ---------------------------------------------------------------------------
// 6.a) Problema: copierea NU e interzisă
// ---------------------------------------------------------------------------

class DiplomaCopiabila
{
public:
    explicit DiplomaCopiabila(const std::string& titular)
        : serie_(++ultimaSerie_)
        , titular_(titular)
    {
        registru().insert(serie_);
        std::cout << "[DiplomaCopiabila] constructor: emisă diploma nr. " << serie_ << " pentru " << titular_
                  << std::endl;
    }

    // NU am declarat copy constructor și operator=, deci compilatorul le generează în tăcere (Item 5).
    // Ele copiază membru cu membru, inclusiv seria, care ar trebui să fie UNICĂ.

    ~DiplomaCopiabila()
    {
        if (registru().erase(serie_) == 1) {
            std::cout << "[DiplomaCopiabila] destructor: diploma nr. " << serie_ << " (" << titular_
                      << ") a fost arhivată" << std::endl;
        } else {
            std::cout << "[DiplomaCopiabila] destructor: !!! diploma nr. " << serie_ << " (" << titular_
                      << ") fusese deja arhivată => a existat un DUPLICAT" << std::endl;
        }
    }

    void afiseaza() const
    {
        std::cout << "diploma nr. " << serie_ << ", titular " << titular_ << std::endl;
    }

    static void afiseazaRegistrul()
    {
        std::cout << "  serii rămase în registru:";
        if (registru().empty()) {
            std::cout << " niciuna";
        }
        for (int serie : registru()) {
            std::cout << " nr. " << serie;
        }
        std::cout << std::endl;
    }

private:
    static std::set<int>& registru()
    {
        static std::set<int> serii;     // static local, nu obiect global (Item 4)
        return serii;
    }

private:
    int serie_;
    std::string titular_;

    inline static int ultimaSerie_ = 0;
};

// ---------------------------------------------------------------------------
// 6.b) Soluția din C++98: declarate private și NEDEFINITE
// ---------------------------------------------------------------------------

///\brief Copy constructor-ul și operator= sunt declarate private și nu sunt definite nicăieri.
///   - codul din exterior nu le poate apela => eroare la COMPILARE ("is private within this context");
///   - funcțiile membre și prietene au acces la ele, deci codul lor compilează, dar implementarea
///     lipsește => eroare abia la LINK-EDITARE ("undefined reference").
class DiplomaCpp98
{
public:
    explicit DiplomaCpp98(const std::string& titular)
        : titular_(titular)
    {
        std::cout << "[DiplomaCpp98] constructor: emisă diploma pentru " << titular_ << std::endl;
    }

    void afiseaza() const
    {
        std::cout << "diploma (C++98) pentru " << titular_ << std::endl;
    }

    // Funcție prietenă: are acces la membrii privați. Aplicația nu o definește; o definește
    // erori_compilare/item6_cpp98_copiere_din_friend.cpp, ca să arate eroarea de link-editare.
    friend void fotocopiaza(const DiplomaCpp98& diploma);

private:
    DiplomaCpp98(const DiplomaCpp98&);              // declarat, dar NEDEFINIT
    DiplomaCpp98& operator=(const DiplomaCpp98&);   // declarat, dar NEDEFINIT

private:
    std::string titular_;
};

// ---------------------------------------------------------------------------
// 6.c) Soluția din carte: clasa de bază Necopiabil (Uncopyable)
// ---------------------------------------------------------------------------

///\brief Moștenește privat din secretariat::Necopiabil și nu declară nimic legat de copiere.
/// Copy constructor-ul și operator= generați pentru ea ar trebui să le apeleze pe cele din bază,
/// care sunt private => compilatorul nu îi poate genera (Item 5), iar orice încercare de copiere
/// dă eroare la COMPILARE, inclusiv în funcțiile membre și prietene.
class DiplomaNecopiabila : private secretariat::Necopiabil
{
public:
    explicit DiplomaNecopiabila(const std::string& titular)
        : titular_(titular)
    {
        std::cout << "[DiplomaNecopiabila] constructor: emisă diploma pentru " << titular_ << std::endl;
    }

    void afiseaza() const
    {
        std::cout << "diploma (Necopiabil) pentru " << titular_ << std::endl;
    }

    // Definită doar în erori_compilare/item6_necopiabil_copiere_din_friend.cpp
    friend void fotocopiaza(const DiplomaNecopiabila& diploma);

private:
    std::string titular_;
};

} // namespace exemple

#endif // EXEMPLE_ITEM6_HPP
