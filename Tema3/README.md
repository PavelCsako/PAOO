# Tema 3 – Effective C++, Item 10–14

Aplicație C++ construită cu CMake, cu aceeași structură ca temele anterioare:

1. **galerie**: bibliotecă statică (`libgalerie.a`) cu clasele scrise **corect**
2. **aplicatie**: executabilul care leagă biblioteca și arată, pentru fiecare item, ce se întâmplă fără regula din carte
3. **erori_compilare** (opțional): cod care **nu trebuie** să compileze, verificat cu `make erori`

Tema are două părți, adăugate în commit-uri separate. Partea 1, lucrarea 5:

| Lucrare | Item | Titlu |
|---------|------|-------|
| 5 | 10 | Have assignment operators return a reference to *this |
| 5 | 11 | Handle assignment to self in operator= |
| 5 | 12 | Copy all parts of an object |
| 5 | – | Move assignment (C++11, nu e în carte) |

Domeniul aplicației e o galerie de imagini. O imagine (`galerie::Imagine`, adică `Widget` din carte) are un titlu
și un `Bitmap` alocat pe heap. Pixelii sunt caractere pe scala de luminozitate `" .:-=+*#%@"`, ca imaginile să se
poată afișa în terminal: `apus [..::--==++]`.

## Structura

```
Tema3/
├── CMakeLists.txt                     # soluția: add_subdirectory(galerie / aplicatie / erori_compilare)
├── galerie/                           # proiectul 1: biblioteca statică
│   ├── CMakeLists.txt
│   ├── inc/galerie/
│   │   ├── Bitmap.hpp                 # pixelii; ține evidența Bitmap-urilor vii (leak-uri, copieri din memorie ștearsă)
│   │   ├── Imagine.hpp                # Item 10, 11 + move assignment: varianta corectă
│   │   └── ImagineCuFiltru.hpp        # Item 12: varianta corectă (PriorityCustomer din carte)
│   └── src/  (implementările)
├── aplicatie/                         # proiectul 2: folosește biblioteca
│   ├── CMakeLists.txt
│   └── src/
│       ├── main.cpp
│       ├── demonstratii.hpp
│       ├── exemple_imagini.hpp        # variantele de operator= pentru Item 10, 11 și move assignment
│       ├── exemple_item12.hpp         # copieri care uită o parte din obiect
│       ├── item10.cpp, item11.cpp, item12.cpp, mutare.cpp
└── erori_compilare/                   # proiectul 3: exemple care NU compilează (make erori)
```

## Build și rulare

```bash
cd Tema3
mkdir build && cd build
cmake ..
make
./aplicatie/aplicatie
make erori                              # erorile de compilare intenționate
cmake -DARATA_AVERTISMENTE=ON .. && make   # avertismentul GCC pentru exemplul greșit de la Item 12
```

Verificarea memoriei: `valgrind --leak-check=full ./aplicatie/aplicatie` raportează un singur bloc pierdut,
`definitely lost: 40 bytes in 1 blocks`: Bitmap-ul pe care move assignment-ul greșit (`ImagineMutareCuLeak`) îl uită,
intenționat. Nu apare nicio citire invalidă și niciun double delete.

## Lucrarea 5

### Item 10: Have assignment operators return a reference to *this

| Ce arată programul | Unde |
|---|---|
| `x = y = z = 15` și `(w = 3) += 2` merg la `int`, pentru că atribuirea întoarce operandul din stânga | 10.a |
| `galerie::Imagine`: `operator=`, `operator+=` și `operator=(char)` întorc `Imagine&` (`return *this;`), deci `a = b = c`, `(a = c) += 3` și `a = b = '#'` merg ca la `int` | 10.b, `static_assert`-uri în `item10.cpp` |
| `exemple::ImagineReturnValoare` întoarce o **copie**: `a = b = c` face 4 Bitmap-uri în loc de 2, iar `(a = c) += 3` luminează copia temporară, nu pe `a` | 10.c |
| Un `operator=` care întoarce `void`: `a = b = c` nu compilează | 10.d, `make erori` |

### Item 11: Handle assignment to self in operator=

Aceleași două teste pentru patru variante de `operator=` (11.b):

| Variantă | `imagini[i] = imagini[j]`, cu `i == j` | `a = b` când copierea Bitmap-ului aruncă `std::bad_alloc` |
|---|---|---|
| `exemple::ImagineNaiva` (`delete`, apoi `new`) | STRICATĂ: copiază dintr-un Bitmap deja șters | STRICATĂ: pointer către memorie ștearsă |
| `exemple::ImagineTestIdentitate` (`if (this == &altul)`) | corectă | STRICATĂ |
| `exemple::ImagineOrdineAtenta` (copiază, apoi șterge) | corectă | neschimbată (corect) |
| `galerie::Imagine` (copy and swap) | corectă | neschimbată (corect) |

`Bitmap` ține evidența obiectelor vii, așa că recunoaște o copiere dintr-un Bitmap deja șters și nu citește din el.
Fără această verificare, programul ar citi din memorie eliberată (comportament nedefinit), iar destructorul ar face
un double delete. Lipsa memoriei e simulată cu `Bitmap::simuleazaLipsaMemorieLaUrmatoareaCopiere()`.

### Item 12: Copy all parts of an object

| Clasă | Copy constructor | `operator=` | Rezultat |
|---|---|---|---|
| `galerie::ImagineCuFiltru` | `Imagine(altul)` + fiecare membru | `Imagine::operator=(altul)` + fiecare membru | copia e identică (12.a) |
| `exemple::ImagineCuFiltruFaraBaza` | uită `Imagine(altul)` | uită `Imagine::operator=(altul)` | copia are partea de bază goală; destinația păstrează titlul și pixelii vechi (12.b) |
| `exemple::ImagineCuFiltruNeactualizata` | uită membrul adăugat ulterior | la fel | intensitatea filtrului se pierde (12.c) |

GCC avertizează (`-Wextra`) doar pentru copy constructor-ul care uită clasa de bază; pentru `operator=` și pentru
membrul uitat nu spune nimic. 12.d explică de ce o funcție de copiere nu se implementează prin cealaltă.

### Move assignment (C++11)

`a = std::move(b)` (`galerie::Imagine`): Bitmap-ul vechi al lui `a` e șters, cel al lui `b` e preluat fără copiere,
iar `b` rămâne gol. `exemple::ImagineMutareCuLeak` uită să șteargă Bitmap-ul vechi: memory leak (singurul raportat
de valgrind).

Fiecare constructor, destructor și operator de atribuire afișează un mesaj la ieșirea standard.
