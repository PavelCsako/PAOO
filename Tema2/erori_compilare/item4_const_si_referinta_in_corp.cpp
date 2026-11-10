// Item 4: membrii const și referințele pot fi inițializați DOAR în lista de inițializare.
// În corpul constructorului e prea târziu: acolo se poate doar atribui, iar o referință
// sau o constantă trebuie să primească valoarea chiar în momentul creării.

#include <string>

class Inscriere
{
public:
    Inscriere(std::string& student, int an)     // eroare: referința și constanta nu sunt inițializate
    {
        student_ = student;     // asta NU leagă referința; ar fi o atribuire către un string inexistent
        an_ = an;               // eroare: atribuire către un membru const
    }

private:
    std::string& student_;
    const int an_;
};

int main()
{
    std::string ana = "Ana";
    Inscriere inscriere(ana, 2);
}
