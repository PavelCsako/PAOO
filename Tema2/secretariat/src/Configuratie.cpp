#include <secretariat/Configuratie.hpp>

#include <iostream>

namespace secretariat {

Configuratie::Configuratie(const std::string& provenienta)
    : anUniversitar_(2026)
    , locuriPeGrupa_(30)
{
    std::cout << "[Configuratie] constructor (" << provenienta << "): an universitar " << anUniversitar_ << "-"
              << anUniversitar_ + 1 << ", " << locuriPeGrupa_ << " locuri pe grupă" << std::endl;
}

int Configuratie::anUniversitar() const
{
    return anUniversitar_;
}

int Configuratie::locuriPeGrupa() const
{
    return locuriPeGrupa_;
}

Configuratie& configuratie()
{
    static Configuratie instanta("static local din secretariat::configuratie(), creat la primul apel");
    return instanta;
}

} // namespace secretariat
