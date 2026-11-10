// Item 10: dacă operator= întoarce void, rezultatul lui (b = c) nu mai e un obiect,
// deci atribuirea în lanț a = b = c nu mai compilează (deși pentru int merge).

#include <string>

class ImagineVoid
{
public:
    explicit ImagineVoid(const std::string& titlu)
        : titlu_(titlu)
    {
    }

    void operator=(const ImagineVoid& altul)    // GREȘIT: ar trebui ImagineVoid& și return *this;
    {
        titlu_ = altul.titlu_;
    }

private:
    std::string titlu_;
};

int main()
{
    ImagineVoid a("a");
    ImagineVoid b("b");
    ImagineVoid c("c");

    b = c;          // merge: o singură atribuire
    a = b = c;      // eroare: a = (void)
}
