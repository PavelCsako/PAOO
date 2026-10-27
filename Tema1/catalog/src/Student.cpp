#include <catalog/Student.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

namespace catalog {

Student::Student(int nrMatricol, const std::string& nume, int capacitate)
    : nrMatricol_(nrMatricol)
    , nume_(nume)
    , capacitate_(capacitate > 0 ? capacitate : 1)
    , nrNote_(0)
    , note_(new int[capacitate_])   // capacitate_ e deja inițializat (e declarat înainte)
{
    ++blocuriPeHeap_;
    ++totalAlocari_;
    std::cout << "[Student] constructor: " << nume_ << " (matricol " << nrMatricol_
              << "), am alocat loc pentru " << capacitate_ << " note la " << note_ << std::endl;
}

Student::Student(const Student& altul)
    : nrMatricol_(altul.nrMatricol_)
    , nume_(altul.nume_)
    , capacitate_(altul.capacitate_)
    , nrNote_(altul.nrNote_)
    , note_(new int[altul.capacitate_]) // memorie NOUĂ, proprie copiei
{
    std::copy(altul.note_, altul.note_ + altul.nrNote_, note_);
    ++blocuriPeHeap_;
    ++totalAlocari_;
    std::cout << "[Student] copy constructor: " << nume_ << ", am copiat notele de la "
              << altul.note_ << " într-o zonă nouă la " << note_ << std::endl;
}

Student::Student(Student&& altul) noexcept
    : nrMatricol_(altul.nrMatricol_)
    , nume_(std::move(altul.nume_))
    , capacitate_(altul.capacitate_)
    , nrNote_(altul.nrNote_)
    , note_(altul.note_)                // preluăm memoria sursei, fără alocare
{
    // Sursa rămâne goală, dar validă: destructorul ei va face delete[] nullptr (inofensiv)
    altul.capacitate_ = 0;
    altul.nrNote_ = 0;
    altul.note_ = nullptr;
    std::cout << "[Student] move constructor: " << nume_ << ", am preluat notele de la "
              << note_ << " (fără alocare, fără copiere)" << std::endl;
}

Student::~Student()
{
    if (note_ == nullptr) {
        std::cout << "[Student] destructor: obiect gol (mutat), nu am ce elibera" << std::endl;
        return;
    }

    std::cout << "[Student] destructor: " << nume_ << ", eliberez notele de la " << note_ << std::endl;
    delete[] note_;
    --blocuriPeHeap_;
}

bool Student::adaugaNota(int nota)
{
    if (!notaValida(nota)) {
        std::cout << "  ! " << nume_ << ": nota " << nota << " respinsă (trebuie să fie între 1 și 10)" << std::endl;
        return false;
    }
    if (nrNote_ == capacitate_) {
        std::cout << "  ! " << nume_ << ": nota " << nota << " respinsă (nu mai e loc, capacitate "
                  << capacitate_ << ")" << std::endl;
        return false;
    }

    note_[nrNote_++] = nota;
    return true;
}

bool Student::modificaNota(int index, int notaNoua)
{
    if (index < 0 || index >= nrNote_) {
        std::cout << "  ! " << nume_ << ": nu există nota cu indexul " << index << std::endl;
        return false;
    }
    if (!notaValida(notaNoua)) {
        std::cout << "  ! " << nume_ << ": nota " << notaNoua << " respinsă (trebuie să fie între 1 și 10)" << std::endl;
        return false;
    }

    note_[index] = notaNoua;
    return true;
}

double Student::medie() const
{
    if (nrNote_ == 0) {
        return 0.0;
    }

    int suma = 0;
    for (int i = 0; i < nrNote_; ++i) {
        suma += note_[i];
    }
    return static_cast<double>(suma) / nrNote_;
}

int Student::nrNote() const
{
    return nrNote_;
}

int Student::nrMatricol() const
{
    return nrMatricol_;
}

const std::string& Student::nume() const
{
    return nume_;
}

void Student::afiseaza() const
{
    if (note_ == nullptr) {
        std::cout << "(obiect gol: resursele lui au fost mutate în alt obiect)" << std::endl;
        return;
    }

    std::cout << nume_ << " [matricol " << nrMatricol_ << "] note: ";
    for (int i = 0; i < nrNote_; ++i) {
        std::cout << note_[i] << ' ';
    }
    std::cout << "| medie " << std::fixed << std::setprecision(2) << medie()
              << " | notele la " << note_ << std::endl;
}

int Student::blocuriPeHeap()
{
    return blocuriPeHeap_;
}

int Student::totalAlocari()
{
    return totalAlocari_;
}

bool Student::notaValida(int nota)
{
    return nota >= 1 && nota <= 10;
}

} // namespace catalog
