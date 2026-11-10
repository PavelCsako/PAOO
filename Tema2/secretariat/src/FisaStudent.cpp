#include <secretariat/FisaStudent.hpp>

#include <iostream>

namespace secretariat {

FisaStudent::FisaStudent()
    : nume_()               // și membrii care s-ar inițializa singuri (std::string) sunt trecuți în listă,
    , an_(0)                // ca să se vadă imediat dacă am uitat vreunul
    , notaExamen_(0.0)
    , notaLaborator_(0.0)
    , medie_(0.0)
    , nrConsultari_(0)
{
    std::cout << "[FisaStudent] constructor implicit: fișă goală, dar toți membrii au o valoare" << std::endl;
}

FisaStudent::FisaStudent(const std::string& nume, int an, double notaExamen, double notaLaborator)
    : nume_(nume)
    , an_(an)
    , notaExamen_(notaExamen)
    , notaLaborator_(notaLaborator)
    , medie_((notaExamen_ + notaLaborator_) / 2)    // corect: notele sunt declarate, deci inițializate, înaintea mediei
    , nrConsultari_(0)
{
    std::cout << "[FisaStudent] constructor: " << nume_ << ", toți membrii inițializați în lista de inițializare"
              << std::endl;
}

void FisaStudent::consulta()
{
    ++nrConsultari_;
}

double FisaStudent::medie() const
{
    return medie_;
}

int FisaStudent::nrConsultari() const
{
    return nrConsultari_;
}

void FisaStudent::afiseaza() const
{
    std::cout << (nume_.empty() ? "(fără nume)" : nume_) << " | anul " << an_ << " | examen " << notaExamen_
              << " | laborator " << notaLaborator_ << " | medie " << medie_ << " | consultată de " << nrConsultari_
              << " ori" << std::endl;
}

} // namespace secretariat
