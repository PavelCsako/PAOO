#include <galerie/Bitmap.hpp>

#include <iostream>
#include <new>
#include <set>

namespace galerie {

namespace {

// Scala de luminozitate: primul caracter e cel mai întunecat, ultimul cel mai luminos
const char* const scala = " .:-=+*#%@";

// Evidența Bitmap-urilor vii. Static LOCAL, nu obiect global (Effective C++, Item 4).
std::set<const Bitmap*>& bitmapuriVii()
{
    static std::set<const Bitmap*> vii;
    return vii;
}

int& ultimulId()
{
    static int ultimul = 0;
    return ultimul;
}

bool& esecLaUrmatoareaCopiere()
{
    static bool esec = false;
    return esec;
}

} // namespace

Bitmap::Bitmap(const std::string& pixeli)
    : pixeli_(pixeli)
    , id_(++ultimulId())
    , corupt_(false)
{
    bitmapuriVii().insert(this);
    std::cout << "      [Bitmap #" << id_ << "] creat: [" << pixeli_ << "]" << std::endl;
}

Bitmap::Bitmap(const Bitmap& altul)
    : pixeli_()
    , id_(0)
    , corupt_(false)
{
    if (esecLaUrmatoareaCopiere()) {
        esecLaUrmatoareaCopiere() = false;
        std::cout << "      [Bitmap] copierea a eșuat: nu mai e memorie (std::bad_alloc, simulat)" << std::endl;
        throw std::bad_alloc();
    }

    id_ = ++ultimulId();

    if (!esteViu(&altul)) {
        // Sursa a fost deja ștearsă: NU citim din ea (ar fi citire din memorie eliberată).
        corupt_ = true;
        pixeli_ = "??????????";
        std::cout << "      [Bitmap #" << id_ << "] !!! copiat dintr-un Bitmap DEJA ȘTERS: pixelii originali s-au pierdut"
                  << std::endl;
    } else {
        pixeli_ = altul.pixeli_;
        corupt_ = altul.corupt_;
        std::cout << "      [Bitmap #" << id_ << "] creat prin copiere din #" << altul.id_ << ": [" << pixeli_ << "]"
                  << std::endl;
    }

    bitmapuriVii().insert(this);
}

Bitmap::~Bitmap()
{
    bitmapuriVii().erase(this);
    std::cout << "      [Bitmap #" << id_ << "] șters" << std::endl;
}

const std::string& Bitmap::pixeli() const
{
    return pixeli_;
}

int Bitmap::id() const
{
    return id_;
}

bool Bitmap::corupt() const
{
    return corupt_;
}

void Bitmap::lumineaza(int trepte)
{
    const std::string scalaText(scala);
    const int ultimaTreapta = static_cast<int>(scalaText.size()) - 1;

    for (char& pixel : pixeli_) {
        const std::string::size_type pozitie = scalaText.find(pixel);
        if (pozitie == std::string::npos) {
            continue;                               // caracterele din afara scalei rămân neschimbate
        }
        int nou = static_cast<int>(pozitie) + trepte;
        nou = nou < 0 ? 0 : (nou > ultimaTreapta ? ultimaTreapta : nou);
        pixel = scalaText[static_cast<std::string::size_type>(nou)];
    }
}

void Bitmap::umple(char pixel)
{
    pixeli_.assign(pixeli_.size(), pixel);
}

int Bitmap::inViata()
{
    return static_cast<int>(bitmapuriVii().size());
}

int Bitmap::totalCreate()
{
    return ultimulId();
}

bool Bitmap::esteViu(const Bitmap* bitmap)
{
    return bitmap != nullptr && bitmapuriVii().count(bitmap) == 1;
}

void Bitmap::simuleazaLipsaMemorieLaUrmatoareaCopiere()
{
    esecLaUrmatoareaCopiere() = true;
}

} // namespace galerie
