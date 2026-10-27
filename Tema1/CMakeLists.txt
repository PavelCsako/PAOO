# Versiunea minimă de CMake
cmake_minimum_required(VERSION 3.10)

# Numele soluției
project(Tema1 LANGUAGES CXX)

# Cele două proiecte ale soluției
add_subdirectory(catalog)       # proiectul 1: biblioteca statică libcatalog.a + header-ul ei
add_subdirectory(aplicatie)     # proiectul 2: executabilul care folosește biblioteca
