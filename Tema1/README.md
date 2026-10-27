# Tema 1 – Constructori, destructor, copy & move

Aplicație C++ construită cu CMake, din două proiecte:

1. **catalog** – bibliotecă statică (`libcatalog.a`) care exportă header-ul `catalog/Student.hpp`
2. **aplicatie** – executabilul care leagă biblioteca și demonstrează conceptele din temă

## Structura

```
Tema1/
├── CMakeLists.txt                 # soluția: add_subdirectory(catalog), add_subdirectory(aplicatie)
├── catalog/                       # proiectul 1: biblioteca statică
│   ├── CMakeLists.txt
│   ├── inc/catalog/Student.hpp    # header-ul exportat
│   └── src/Student.cpp
└── aplicatie/                     # proiectul 2: folosește biblioteca
    ├── CMakeLists.txt
    └── src/
        ├── main.cpp               # câte o demonstrație pentru fiecare punct din temă
        └── exemple.hpp            # clase scrise intenționat greșit, pentru comparație
```

## Build și rulare

```bash
sudo apt install build-essential cmake      # o singură dată
cd Tema1
mkdir build && cd build
cmake ..
make
./aplicatie/aplicatie
```

Opțional, verificarea memoriei cu valgrind (`sudo apt install valgrind`):

```bash
valgrind --leak-check=full ./aplicatie/aplicatie
```

Raportul arată `definitely lost: 40 bytes in 2 blocks` (leak-ul intenționat al clasei fără destructor)
și `Invalid free()` (dublă eliberare la clasa fără copy constructor). Pentru `catalog::Student` nu apare nicio eroare.

## Unde se găsește fiecare cerință

| # | Cerință | Varianta corectă | Ce se întâmplă fără ea |
|---|---------|------------------|------------------------|
| 1 | Build cu CMake | două proiecte, `add_library(... STATIC)` + `target_link_libraries` | – |
| 2 | Încapsulare | membri `private` în `Student`, acces doar prin metode care validează (nota 1–10, index, capacitate) | `ana.note_[0] = 100` nu compilează; fără verificări s-ar scrie în afara tabloului |
| 3 | Inițializarea membrilor în constructor | lista de inițializare din `Student::Student` (inclusiv membrul `const nrMatricol_`) | `InitializareInCorp`: membrul e construit gol și apoi atribuit (2 operații); un membru `const` nici nu poate fi setat în corp |
| 4 | Eliberarea heap-ului în destructor | `Student::~Student` face `delete[] note_` | `StudentFaraDestructor`: blocurile rămân alocate (memory leak) |
| 5 | Copy constructor | `Student(const Student&)` alocă memorie nouă (deep copy) | `StudentFaraCopyConstructor`: copia și originalul folosesc aceeași memorie → modificarea copiei schimbă originalul, apoi **double free** și prăbușire |
| 6 | Move constructor | `Student(Student&&) noexcept` preia pointerul, sursa rămâne goală | `StudentFaraMoveConstructor`: în `std::vector` fiecare realocare copiază (9 alocări în loc de 3 pentru 3 studenți) |

Fiecare constructor și destructor afișează un mesaj, iar adresele afișate arată când memoria e copiată
(adresă nouă) și când e doar preluată (aceeași adresă).

Exemplul cu double free rulează într-un proces copil (`fork`), ca prăbușirea să nu oprească restul programului.
