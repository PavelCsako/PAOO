#include <galerie/Imagine.hpp>

#include <iostream>
#include <utility>

namespace galerie {

namespace {

// După o mutare, titlul sursei rămâne gol: în mesaje apare ca "(mutată)"
std::string deAfisat(const std::string& titlu)
{
    return titlu.empty() ? "(mutată)" : titlu;
}

} // namespace

Imagine::Imagine()
    : titlu_("(fără titlu)")
    , bitmap_(nullptr)
{
    std::cout << "    [Imagine] constructor implicit: imagine goală" << std::endl;
}

Imagine::Imagine(const std::string& titlu, const std::string& pixeli)
    : titlu_(titlu)
    , bitmap_(new Bitmap(pixeli))
{
    std::cout << "    [Imagine] constructor: " << titlu_ << std::endl;
}

Imagine::Imagine(const Imagine& altul)
    : titlu_(altul.titlu_)
    , bitmap_(altul.bitmap_ != nullptr ? new Bitmap(*altul.bitmap_) : nullptr)
{
    std::cout << "    [Imagine] copy constructor: " << titlu_ << std::endl;
}

Imagine::Imagine(Imagine&& altul) noexcept
    : titlu_(std::move(altul.titlu_))
    , bitmap_(altul.bitmap_)                    // preluăm Bitmap-ul, fără alocare
{
    altul.bitmap_ = nullptr;                    // sursa nu mai deține nimic
    std::cout << "    [Imagine] move constructor: " << titlu_ << std::endl;
}

Imagine::~Imagine()
{
    std::cout << "    [Imagine] destructor: " << deAfisat(titlu_) << std::endl;
    delete bitmap_;                             // delete nullptr nu face nimic
}

// Item 11: "copy and swap".
//   1. Copia se face ÎNAINTE de a modifica *this. Dacă ea aruncă o excepție, *this a rămas neatins.
//   2. swap nu poate eșua: schimbă doar un string și un pointer.
//   3. La ieșire, "copie" distruge datele VECHI ale lui *this.
// La autoatribuire (a = a) copia se face dintr-un obiect încă intact, deci rezultatul e corect.
Imagine& Imagine::operator=(const Imagine& altul)
{
    std::cout << "    [Imagine] operator=(const Imagine&): copy and swap" << std::endl;
    Imagine copie(altul);
    swap(copie);
    return *this;                               // Item 10
}

// Move assignment (C++11, nu e în carte). Spre deosebire de move constructor, *this deține deja
// un Bitmap: trebuie șters înainte, altfel rămâne alocat (memory leak).
Imagine& Imagine::operator=(Imagine&& altul) noexcept
{
    std::cout << "    [Imagine] operator=(Imagine&&): move assignment" << std::endl;
    if (this != &altul) {                       // a = std::move(a) nu trebuie să șteargă Bitmap-ul
        delete bitmap_;                         // 1. eliberăm Bitmap-ul vechi
        bitmap_ = altul.bitmap_;                // 2. îl preluăm pe al sursei
        altul.bitmap_ = nullptr;                // 3. sursa rămâne goală, dar validă
        titlu_ = std::move(altul.titlu_);
    }
    return *this;                               // Item 10
}

Imagine& Imagine::operator=(char pixel)
{
    if (bitmap_ != nullptr) {
        bitmap_->umple(pixel);
    }
    return *this;                               // Item 10: la fel pentru orice operator de atribuire
}

Imagine& Imagine::operator+=(int trepte)
{
    if (bitmap_ != nullptr) {
        bitmap_->lumineaza(trepte);
    }
    return *this;                               // Item 10: la fel pentru +=, -=, *= ...
}

void Imagine::swap(Imagine& altul) noexcept
{
    using std::swap;
    swap(titlu_, altul.titlu_);
    swap(bitmap_, altul.bitmap_);
}

const std::string& Imagine::titlu() const
{
    return titlu_;
}

std::string Imagine::pixeli() const
{
    return bitmap_ != nullptr ? bitmap_->pixeli() : std::string();
}

const Bitmap* Imagine::bitmap() const
{
    return bitmap_;
}

std::string Imagine::descriere() const
{
    if (bitmap_ == nullptr) {
        return deAfisat(titlu_) + " (goală)";
    }
    return titlu_ + " [" + bitmap_->pixeli() + "] (Bitmap #" + std::to_string(bitmap_->id()) + ")"
           + (bitmap_->corupt() ? " CORUPTĂ" : "");
}

} // namespace galerie
