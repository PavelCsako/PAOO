#include <secretariat/Diploma.hpp>

#include <iostream>
#include <set>

namespace secretariat {

namespace {

// Registrul seriilor aflate în circulație.
// Aplicăm Item 4: nu un obiect global (std::set<int> registru;), ci un static LOCAL, construit
// la prima folosire. Așa registrul există garantat chiar dacă o diplomă ar fi creată
// din constructorul unui obiect global aflat în alt fișier .cpp.
std::set<int>& seriiInCirculatie()
{
    static std::set<int> serii;
    return serii;
}

} // namespace

Diploma::Diploma(const std::string& titular, const std::string& specializare)
    : serie_(urmatoareaSerie())
    , titular_(titular)
    , specializare_(specializare)
{
    seriiInCirculatie().insert(serie_);
    std::cout << "[Diploma] constructor: emisă diploma nr. " << serie_ << " pentru " << titular_ << std::endl;
}

Diploma::~Diploma()
{
    seriiInCirculatie().erase(serie_);
    std::cout << "[Diploma] destructor: diploma nr. " << serie_ << " (" << titular_ << ") a fost arhivată"
              << std::endl;
}

int Diploma::serie() const
{
    return serie_;
}

const std::string& Diploma::titular() const
{
    return titular_;
}

void Diploma::afiseaza() const
{
    std::cout << "diploma nr. " << serie_ << ", titular " << titular_ << ", specializarea " << specializare_
              << std::endl;
}

int Diploma::inCirculatie()
{
    return static_cast<int>(seriiInCirculatie().size());
}

int Diploma::urmatoareaSerie()
{
    static int ultimaSerie = 0;
    return ++ultimaSerie;
}

} // namespace secretariat
