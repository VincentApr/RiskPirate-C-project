cmake_minimum_required(VERSION 3.10)
set(CMAKE_OSX_ARCHITECTURES x86_64) #Apple Silicon
project(exo2)


list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
find_package(Imagine REQUIRED)

find_package(Qt5 COMPONENTS Widgets Core Gui OpenGL REQUIRED)

add_executable(exo2 exo2.cpp
    iles.h iles.cpp
    partie.h
    partie.cpp)

target_link_libraries(exo2
    Imagine::Graphics
    Imagine::LinAlg
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::OpenGL
    OpenGL::GL
)

# Ajouter cette ligne si OpenGL::GL n'est pas trouvé
find_package(OpenGL REQUIRED)
