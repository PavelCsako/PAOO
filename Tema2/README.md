# Tema 2 – Effective C++, Item 4, 5 și 6

Aplicație C++ construită cu CMake, cu aceeași structură ca Tema 1:

1. **secretariat**: bibliotecă statică (`libsecretariat.a`) cu clasele scrise **corect**
2. **aplicatie**: executabilul care leagă biblioteca și arată, pentru fiecare item, ce se întâmplă fără regula din carte
3. **erori_compilare** (opțional): cod care **nu trebuie** să compileze, verificat cu `make erori`

| Item | Titlu |
|------|-------|
| 4 | Make sure that objects are initialized before they're used |
| 5 | Know what functions C++ silently writes and calls |
| 6 | Explicitly disallow the use of compiler-generated functions you do not want |

## Structura

```
Tema2/
├── CMakeLists.txt                        # soluția: add_subdirectory(secretariat / aplicatie / erori_compilare)
├── secretariat/                          # proiectul 1: biblioteca statică
│   ├── CMakeLists.txt
│   ├── inc/secretariat/
│   │   ├── FisaStudent.hpp               # Item 4: toți membrii inițializați, în ordinea declarării
│   │   ├── Configuratie.hpp              # Item 4: configuratie() întoarce un static LOCAL
│   │   ├── Martor.hpp                    # Item 5: anunță fiecare constructor / operator= / destructor apelat
│   │   ├── Necopiabil.hpp                # Item 6: clasa de bază Uncopyable din carte
│   │   └── Diploma.hpp                   # Item 6: copierea interzisă cu = delete
│   └── src/  (implementările)
├── aplicatie/                            # proiectul 2: folosește biblioteca
│   ├── CMakeLists.txt
│   └── src/
│       ├── main.cpp
│       ├── demonstratii.hpp              # declarațiile demonstrațiilor + tabelul cu funcțiile speciale
│       ├── item4.cpp, exemple_item4.hpp  # câte un fișier pentru fiecare item;
│       ├── item5.cpp, exemple_item5.hpp  # exemple_*.hpp = clasele scrise INTENȚIONAT greșit
│       ├── item6.cpp, exemple_item6.hpp
│       ├── fiasco.hpp                    # Item 4: obiecte globale din fișiere diferite
│       ├── fiasco_grupe.cpp
│       └── fiasco_configuratie.cpp
└── erori_compilare/                      # proiectul 3: exemple care NU compilează
    ├── CMakeLists.txt                    # ținta "make erori"
    ├── verifica_erori.cmake
    └── item4_... / item5_... / item6_....cpp
```

## Build și rulare

```bash
sudo apt install build-essential cmake      # o singură dată
cd Tema2
mkdir build && cd build
cmake ..
make
./aplicatie/aplicatie
```

Erorile reale de compilare și link-editare, pentru codul care nu trebuie să compileze:

```bash
make erori
```

Avertismentele GCC pentru exemplele greșite de la Item 4 (implicit sunt oprite, ca build-ul să fie curat):

```bash
cmake -DARATA_AVERTISMENTE=ON .. && make
```

Verificarea memoriei (`sudo apt install valgrind`): `valgrind --leak-check=full ./aplicatie/aplicatie`
raportează `0 errors` și `All heap blocks were freed`.

## Item 4: Make sure that objects are initialized before they're used

| Problema din carte | Varianta corectă | Ce se întâmplă fără ea (în program) |
|---|---|---|
| Membrii de tip predefinit (`int`, `double`) nu se inițializează singuri | `secretariat::FisaStudent` inițializează **toți** membrii, chiar și în constructorul implicit | `exemple::FisaFaraInitializare`: „anul 1515870810”, „consultată de 1515870811 ori” |
| Regulile despre când inițializează C++ singur sunt complicate | inițializare manuală, mereu | `Punctaj p;` are gunoi, `Punctaj p{};` și `static Punctaj p;` au zero |
| Preferați lista de inițializare atribuirii în corp | `exemple::InitializareInLista`: 1 operație | `exemple::AtribuireInCorp`: 4 operații (construire implicită, temporar, atribuire, distrugere) |
| Membrii se inițializează în **ordinea declarării**, nu în ordinea din listă | `FisaStudent`: notele sunt declarate înaintea mediei | `exemple::OrdineInitializare` (ordinea reală, cu `Martor`), `exemple::NotaFinalaGresita`: media calculată din gunoi (`1.78389e+127`) |
| Membrii `const` și referințele se pot inițializa doar în listă | `FisaStudent`, `Diploma::serie_` | `make erori`: `uninitialized reference member`, `assignment of read-only member` |
| Ordinea de inițializare a obiectelor statice non-locale din fișiere diferite nu e definită | `secretariat::configuratie()` întoarce un **static local**, creat la primul apel | `fiasco::grupaCuObiectGlobal` citește `configuratieGlobala` înainte să fie construită: **0 locuri** în loc de 30 |

Pentru exemplul cu obiectele globale, `aplicatie/CMakeLists.txt` pune intenționat `fiasco_grupe.cpp` înaintea lui
`fiasco_configuratie.cpp`. Cu GCC obiectele globale se construiesc în ordinea în care fișierele ajung la linker;
dacă inversați cele două linii, grupa citește corect 30, iar programul afișează că „de data asta ordinea a fost
favorabilă”. Varianta cu static local dă 30 în ambele cazuri.

Ca valorile neinițializate să fie aceleași la fiecare rulare, obiectele sunt construite (cu placement new) într-o
zonă de memorie umplută cu octeți `0x5A`, adică „memorie folosită anterior”. Citirea lor rămâne comportament
nedefinit: compilat în Release (`-O2`/`-O3`), GCC poate elimina umplerea memoriei, valorile afișate diferă
(tot aberante) și apare avertismentul `-Wmaybe-uninitialized`.

## Item 5: Know what functions C++ silently writes and calls

| Problema din carte | Unde se vede |
|---|---|
| Pentru `class Gol {};` compilatorul scrie constructor implicit, copy constructor, copy assignment, destructor (și, din C++11, move constructor și move assignment), doar dacă sunt folosite, `public` și `inline` | 5.a: toate operațiile compilează; `static_assert`-uri în `item5.cpp`; tabelul de la final |
| Funcțiile generate apelează, în tăcere, funcțiile bazei și ale fiecărui membru, membru cu membru | 5.b: `exemple::FisaGenerata` nu declară nimic, iar membrii `Martor` arată fiecare apel |
| Constructorul implicit nu mai e generat dacă declarați orice constructor | 5.c: `exemple::FaraConstructorImplicit`; `make erori`: `no matching function for call to ...()` |
| `operator=` nu poate fi generat pentru membri referință sau `const` (exemplul `NamedObject`) | 5.d: `exemple::ObiectCuNume<int>` cu „Azorel” și „Grivei”; copy constructor-ul există, `p = s` nu compilează (`make erori`: `cannot use default assignment operator`) |
| La fel dacă baza are `operator=` privat | Item 6.c: `Necopiabil` |
| Destructorul generat e nevirtual | 5.a: `std::has_virtual_destructor_v<Gol> = false` |
| (C++11) Un destructor declarat oprește generarea move-ului | 5.e: `std::move` pe `CuDestructorDeclarat` apelează **copy** constructor-ul membrului |

Tabelul afișat la final conține răspunsul compilatorului (`std::is_copy_constructible_v` etc.) pentru fiecare clasă.

## Item 6: Explicitly disallow the use of compiler-generated functions you do not want

O diplomă are o serie unică (ca `HomeForSale` din carte), deci copierea ei nu are sens.

| Variantă | Clasă | Copiere din exterior | Copiere dintr-o funcție membră / friend |
|---|---|---|---|
| fără protecție | `exemple::DiplomaCopiabila` | compilează: apar diplome duplicate (problema) | compilează |
| C++98: private și nedefinite | `exemple::DiplomaCpp98` | eroare la **compilare** (`is private`) | eroare abia la **link-editare** (`undefined reference`) |
| clasa de bază `Necopiabil` (din carte) | `exemple::DiplomaNecopiabila` | eroare la **compilare** | eroare la **compilare** |
| C++11: `= delete` | `secretariat::Diploma` | eroare la **compilare** (`use of deleted function`) | eroare la **compilare** |

În 6.a, cu operațiile generate de compilator: trei diplome ajung să aibă seria 1, seria 1 e „arhivată” de trei ori,
iar diploma nr. 2 dispare fără să fie arhivată. Toate erorile din tabel sunt reproduse de `make erori`.
Pentru `secretariat::Diploma`, și `std::move` dă eroare: odată declarată copierea, mutarea nu mai e generată (Item 5).

Fiecare constructor și destructor afișează un mesaj la ieșirea standard.
