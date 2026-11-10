#include <galerie/Fabrica.hpp>

#include <iostream>
#include <map>

namespace galerie {

Bitmap* incarcaBitmap(const std::string& numeFisier)
{
    // Static local, nu obiect global (Item 4)
    static const std::map<std::string, std::string> fisiere = {
        {"apus.img", "..::--==++"},
        {"luna.img", " .:*@@*:. "},
        {"gol.img", ""},
        {"stricat.img", "..XX--XX++"},
    };

    std::cout << "    [incarcaBitmap] " << numeFisier << " -> new Bitmap (cine apelează trebuie să facă delete)"
              << std::endl;

    const auto gasit = fisiere.find(numeFisier);
    return new Bitmap(gasit != fisiere.end() ? gasit->second : std::string());
}

} // namespace galerie
