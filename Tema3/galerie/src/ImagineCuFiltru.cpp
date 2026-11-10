#include <galerie/ImagineCuFiltru.hpp>

#include <iostream>
#include <utility>

namespace galerie {

ImagineCuFiltru::ImagineCuFiltru(const std::string& titlu, const std::string& pixeli, const std::string& filtru,
                                 int intensitate)
    : Imagine(titlu, pixeli)
    , filtru_(filtru)
    , intensitate_(intensitate)
{
    std::cout << "    [ImagineCuFiltru] constructor: " << titlu << ", filtru " << filtru_ << std::endl;
}

ImagineCuFiltru::ImagineCuFiltru(const ImagineCuFiltru& altul)
    : Imagine(altul)                    // partea de bază: copy constructor-ul lui Imagine
    , filtru_(altul.filtru_)            // apoi fiecare membru propriu
    , intensitate_(altul.intensitate_)
{
    std::cout << "    [ImagineCuFiltru] copy constructor" << std::endl;
}

ImagineCuFiltru& ImagineCuFiltru::operator=(const ImagineCuFiltru& altul)
{
    std::cout << "    [ImagineCuFiltru] operator=(const ImagineCuFiltru&)" << std::endl;
    Imagine::operator=(altul);          // partea de bază: operator= al lui Imagine
    filtru_ = altul.filtru_;            // apoi fiecare membru propriu
    intensitate_ = altul.intensitate_;
    return *this;                       // Item 10
}

ImagineCuFiltru::ImagineCuFiltru(ImagineCuFiltru&& altul) noexcept
    : Imagine(std::move(altul))         // și la mutare: întâi partea de bază
    , filtru_(std::move(altul.filtru_))
    , intensitate_(altul.intensitate_)
{
    std::cout << "    [ImagineCuFiltru] move constructor" << std::endl;
}

ImagineCuFiltru& ImagineCuFiltru::operator=(ImagineCuFiltru&& altul) noexcept
{
    std::cout << "    [ImagineCuFiltru] operator=(ImagineCuFiltru&&)" << std::endl;
    Imagine::operator=(std::move(altul));
    filtru_ = std::move(altul.filtru_);
    intensitate_ = altul.intensitate_;
    return *this;
}

std::string ImagineCuFiltru::descriere() const
{
    return Imagine::descriere() + " | filtru " + (filtru_.empty() ? "(niciunul)" : filtru_) + " "
           + std::to_string(intensitate_) + "%";
}

} // namespace galerie
