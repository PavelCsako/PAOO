// Item 13: std::unique_ptr are un singur proprietar, deci nu se poate copia.
// Transferul se face explicit, cu std::move (în carte, std::auto_ptr îl făcea la o copiere obișnuită).

#include <galerie/Bitmap.hpp>

#include <memory>
#include <utility>

int main()
{
    auto p1 = std::make_unique<galerie::Bitmap>("..::--==++");

    auto p2 = p1;                   // eroare: copy constructor-ul lui unique_ptr e șters
    auto p3 = std::move(p1);        // corect: transfer explicit, p1 devine nullptr
}
