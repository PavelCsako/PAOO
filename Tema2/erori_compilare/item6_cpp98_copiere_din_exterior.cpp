// Item 6, varianta C++98: copy constructor-ul și operator= sunt private.
// Din exteriorul clasei, orice copiere e o eroare de COMPILARE.

#include "exemple_item6.hpp"

int main()
{
    exemple::DiplomaCpp98 diploma("Ana Popescu");
    exemple::DiplomaCpp98 copie(diploma);       // eroare: copy constructor privat
}
