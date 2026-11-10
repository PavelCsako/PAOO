#ifndef GALERIE_LACATPARTAJAT_HPP
#define GALERIE_LACATPARTAJAT_HPP

#include <galerie/Mutex.hpp>

#include <memory>

namespace galerie {

///\brief Opțiunea 2 din Item 14: numărarea referințelor (reference counting).
///
/// Copiile împart ACELAȘI mutex. Mutex-ul e ținut într-un std::shared_ptr cu un "deleter" propriu:
/// când dispare ultima copie, shared_ptr-ul nu face delete, ci apelează deblocheaza(mutex).
///
/// Clasa nu are destructor scris de mână: destructorul generat distruge membrul shared_ptr,
/// iar acesta se ocupă de deblocare. Copierea generată de compilator e corectă: copiază
/// shared_ptr-ul, adică mărește contorul.
class LacatPartajat
{
public:
    explicit LacatPartajat(Mutex* mutex);

public:
    long nrCopii() const;               // câte obiecte LacatPartajat împart mutex-ul (use_count)

private:
    std::shared_ptr<Mutex> mutex_;
};

} // namespace galerie

#endif // GALERIE_LACATPARTAJAT_HPP
