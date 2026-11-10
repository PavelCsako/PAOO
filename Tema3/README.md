# Tema 3 – Effective C++, Item 10–14

Aplicație C++ construită cu CMake, cu aceeași structură ca temele anterioare:

1. **galerie**: bibliotecă statică (`libgalerie.a`) cu clasele scrise **corect**
2. **aplicatie**: executabilul care leagă biblioteca și arată, pentru fiecare item, ce se întâmplă fără regula din carte
3. **erori_compilare** (opțional): cod care **nu trebuie** să compileze, verificat cu `make erori`

Tema are două părți, adăugate în commit-uri separate:

| Lucrare | Item | Titlu |
|---------|------|-------|
| 5 | 10 | Have assignment operators return a reference to *this |
| 5 | 11 | Handle assignment to self in operator= |
| 5 | 12 | Copy all parts of an object |
| 5 | – | Move assignment (C++11, nu e în carte) |
| 6 | 13 | Use objects to manage resources |
| 6 | 14 | Think carefully about copying behavior in resource-managing classes |

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
│   │   ├── ImagineCuFiltru.hpp        # Item 12: varianta corectă (PriorityCustomer din carte)
│   │   ├── Fabrica.hpp                # Item 13: incarcaBitmap() întoarce new Bitmap (createInvestment din carte)
│   │   ├── Mutex.hpp                  # Item 14: mutex cu interfață în stil C (blocheaza / deblocheaza), simulat
│   │   ├── Lacat.hpp                  # Item 14, opțiunea 1: copierea interzisă (Lock din carte)
│   │   ├── LacatPartajat.hpp          # Item 14, opțiunea 2: numărarea referințelor (shared_ptr + deleter)
│   │   └── LacatTransferabil.hpp      # Item 14, opțiunea 4: transferul proprietății (unique_ptr + deleter)
│   └── src/  (implementările)
├── aplicatie/                         # proiectul 2: folosește biblioteca
│   ├── CMakeLists.txt
│   └── src/
│       ├── main.cpp
│       ├── demonstratii.hpp
│       ├── exemple_imagini.hpp        # variantele de operator= pentru Item 10, 11 și move assignment
│       ├── exemple_item12.hpp         # copieri care uită o parte din obiect
│       ├── item10.cpp, item11.cpp, item12.cpp, mutare.cpp
│       ├── item13.cpp                 # delete manual vs unique_ptr, shared_ptr, unique_ptr<T[]>
│       ├── exemple_item14.hpp         # lacăt fără nicio decizie despre copiere
│       └── item14.cpp
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

Verificarea memoriei: `valgrind --leak-check=full ./aplicatie/aplicatie` raportează
`definitely lost: 120 bytes in 3 blocks`, adică exact cele trei Bitmap-uri pierdute intenționat: unul de
move assignment-ul greșit (`ImagineMutareCuLeak`) și două de funcția cu `delete` manual de la Item 13 (ieșire cu
`return` și cu excepție). Nu apare nicio citire invalidă și niciun double delete.

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

## Lucrarea 6

### Item 13: Use objects to manage resources

| Ce arată programul | Unde |
|---|---|
| Aceeași funcție scrisă cu `delete` manual și cu `std::unique_ptr`, pe trei drumuri de ieșire: normal, `return` mai devreme, excepție. Cu `delete` manual, ultimele două pierd Bitmap-ul; cu `unique_ptr`, destructorul îl eliberează mereu (RAII) | 13.a |
| `std::unique_ptr`: un singur proprietar, copierea nu compilează, `std::move` transferă (sursa devine `nullptr`). `std::auto_ptr` din carte a fost scos în C++17 | 13.b, `make erori` |
| `std::shared_ptr`: `use_count` crește la fiecare copie, iar ultimul proprietar face `delete` | 13.c |
| `std::unique_ptr<Bitmap[]>` apelează `delete[]` pentru toate elementele | 13.d |

`galerie::incarcaBitmap()` joacă rolul lui `createInvestment()` din carte: întoarce un pointer obținut cu `new`,
iar cine o apelează trebuie să-l elibereze.

### Item 14: Think carefully about copying behavior in resource-managing classes

Resursa este un `galerie::Mutex` cu interfață în stil C (`blocheaza` / `deblocheaza`, ca `pthread_mutex_lock` /
`pthread_mutex_unlock`). E simulat, fără fire de execuție: afișează fiecare operație și numără greșelile (deblocarea
unui mutex neblocat, blocarea unui mutex deja blocat) în loc să blocheze programul.

| Ce face copia | Clasă | Rezultat |
|---|---|---|
| nicio decizie: copia generată de compilator | `exemple::LacatCopiabil` | transmis prin valoare unei funcții, lacătul e deblocat de copie cât timp originalul încă există, apoi a doua oară: 1 greșeală (14.b) |
| 1. interzisă (`= delete`) | `galerie::Lacat` | copierea nu compilează (14.c, `make erori`) |
| 2. numărarea referințelor | `galerie::LacatPartajat` | copiile împart mutex-ul; `shared_ptr` cu deleter `deblocheaza` îl deblochează o singură dată, la ultima copie (14.d) |
| 3. copie în adâncime | `galerie::Imagine` | fiecare copie are Bitmap-ul ei; potrivit pentru memorie, nu pentru un mutex (14.e) |
| 4. transferul proprietății | `galerie::LacatTransferabil` | se mută cu `std::move` într-o funcție, care îl deblochează; sursa rămâne goală (14.f, `make erori`) |

14.a arată și RAII pentru o resursă care nu e memorie: fără obiect, o excepție lasă mutex-ul blocat; cu
`galerie::Lacat`, destructorul îl deblochează. `LacatPartajat` și `LacatTransferabil` nu au destructor scris de mână:
membrul `shared_ptr` / `unique_ptr` apelează deleter-ul.

Fiecare constructor, destructor și operator de atribuire afișează un mesaj la ieșirea standard.
