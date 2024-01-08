cmake_minimum_required (VERSION 3.8)


project (PROJET)
 set(SRCS "main.cpp")

set(HEADERS "header.h")
add_executable(PROJET ${SRCS} ${HEADERS})

set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
find_package(SFML 2.6.1 COMPONENTS graphics audio REQUIRED)
target_link_libraries(PROJET sfml-graphics sfml-audio)

