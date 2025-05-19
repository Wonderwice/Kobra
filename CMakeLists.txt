# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Cobra VERSION 1.0)

# Spécifier la norme C++ (utiliser une norme moderne)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Inclure le répertoire src pour que les fichiers d'en-tête soient trouvés
include_directories(src)

# Lister tous les fichiers source (.cpp)
set(SOURCES
    src/main.cpp
    src/core/vec3.cpp
    src/image/image.cpp
    src/image/ppm_writer.cpp
    src/renderer/raytracer.cpp
    src/camera/camera.cpp
    src/core/ray.cpp
    src/scene/scene.cpp
    src/image/ppm_writer.cpp
    src/image/image_writer.cpp
    src/geometry/sphere.cpp
)

# Ajouter l'exécutable
add_executable(cobra ${SOURCES})

target_include_directories(cobra PRIVATE src)

# (Optionnel mais recommandé) Définir les options de compilation pour la cible
target_compile_options(cobra PRIVATE -Wall -Wextra) # Active les warnings