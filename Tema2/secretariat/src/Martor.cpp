#include <secretariat/Martor.hpp>

#include <iostream>

namespace secretariat {

Martor::Martor()
    : eticheta_("(gol)")
{
    anunta("constructor implicit");
}

Martor::Martor(const std::string& eticheta)
    : eticheta_(eticheta)
{
    anunta("constructor cu parametru");
}

Martor::Martor(const Martor& altul)
    : eticheta_(altul.eticheta_)
{
    anunta("copy constructor");
}

Martor::Martor(Martor&& altul)
    : eticheta_(altul.eticheta_)    // eticheta o copiem intenționat, ca mesajele să rămână lizibile
{
    anunta("move constructor");
}

Martor& Martor::operator=(const Martor& altul)
{
    eticheta_ = altul.eticheta_;
    anunta("copy assignment (operator=)");
    return *this;
}

Martor& Martor::operator=(Martor&& altul)
{
    eticheta_ = altul.eticheta_;
    anunta("move assignment (operator=)");
    return *this;
}

Martor::~Martor()
{
    anunta("destructor");
}

const std::string& Martor::eticheta() const
{
    return eticheta_;
}

void Martor::anunta(const std::string& ceFunctie) const
{
    std::cout << "      [" << eticheta_ << "] " << ceFunctie << std::endl;
}

} // namespace secretariat
