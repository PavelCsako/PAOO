# Versiunea minimă de CMake
cmake_minimum_required(VERSION 3.10)

# Numele soluției
project(Tema2 LANGUAGES CXX)

# Proiectele soluției
add_subdirectory(secretariat)       # proiectul 1: biblioteca statică libsecretariat.a + header-ele ei
add_subdirectory(aplicatie)         # proiectul 2: executabilul care folosește biblioteca
add_subdirectory(erori_compilare)   # opțional: "make erori" arată codul pe care compilatorul îl refuză
