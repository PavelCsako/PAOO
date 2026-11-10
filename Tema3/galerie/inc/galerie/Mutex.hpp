#ifndef GALERIE_MUTEX_HPP
#define GALERIE_MUTEX_HPP

#include <string>

namespace galerie {

///\brief O resursă care NU e memorie: un mutex cu interfață în stil C (Effective C++, Item 14).
///
/// Imită perechea pthread_mutex_lock / pthread_mutex_unlock: o structură simplă și două funcții
/// libere, fără constructor sau destructor care să elibereze ceva automat. Mutex-ul e simulat
/// (fără fire de execuție), ca rezultatele să fie aceleași la fiecare rulare:
///   - fiecare blocare și deblocare e afișată;
///   - greșelile sunt numărate în loc să blocheze sau să strice programul:
///       blocarea unui mutex deja blocat   -> în realitate, programul s-ar bloca definitiv (deadlock);
///       deblocarea unui mutex neblocat    -> în realitate, comportament nedefinit.
struct Mutex
{
    std::string nume;
    bool blocat = false;
    int erori = 0;
};

void blocheaza(Mutex* mutex);       // ca pthread_mutex_lock

void deblocheaza(Mutex* mutex);     // ca pthread_mutex_unlock

} // namespace galerie

#endif // GALERIE_MUTEX_HPP
