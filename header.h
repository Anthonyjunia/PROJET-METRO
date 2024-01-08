#include <iostream>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>

using namespace std;

class Station {
public:
    //Déclaration des attributs 
    float position;
    int nb_passagers;
    sf::Sprite texture; 
    sf::Font font;
    sf::Text textStation;
    //Constructeur de la station
    Station(float pos, const sf::Texture& img)
        : position(pos), nb_passagers(0), texture(img) {
        texture.setScale(0.2f, 0.2f);  // Ajuster la taille de la texture 
        texture.setOrigin(texture.getLocalBounds().width / 2, texture.getLocalBounds().height / 2);//Centrer la position de la texture 
        texture.setPosition(position, 100);  // Positionner la texture à la position de la station
        nb_passagers = rand() % 11;//Génération d'un nombre aléatoire de personnes dans chaque station
        // Charge une police de caractères
        font.loadFromFile("C:\\Users\\Administrateur\\source\\repos\\CMakeProject3\\Raleway-Medium.ttf");

        // Configuration du texte
        textStation.setFont(font);
        textStation.setCharacterSize(14); // en pixels
        textStation.setFillColor(sf::Color::White);
        textStation.setPosition(position-40, 130); // Positionnez le texte sous la station

        // Met à jour le texte avec le nombre initial de passagers
        textStation.setString("Passagers : " + std::to_string(nb_passagers));
        
    }
    //Déclaration de la méthode (voir main.cpp)
    void Station::dessiner(sf::RenderWindow& window);
};

class Rame {
public:
    //Déclaration des attributs
    sf::RectangleShape shape;
    int numero;
    bool direction;
    float position;
    float y_position;
    vector<Station> stations;
    int stationActuelle;
    int vitesse;
    int nb_passagers;
    sf::Sprite texture;
    sf::Font font;
    sf::Text textRame;

    //Constructeur de la rame
    Rame(int num, vector<Station>& stations, int stationDepart, const sf::Texture& img)
        : numero(num), position(stations[stationDepart].position), y_position(70), vitesse(5), nb_passagers(0), stationActuelle(stationDepart), direction(1),texture(img){

        texture.setScale(0.05f, 0.1f);  // Ajuster la taille de la texture
        texture.setOrigin(texture.getLocalBounds().width / 2, texture.getLocalBounds().height / 2);//Centrer la position de la texture 
        texture.setPosition(position, y_position);// Positionner la texture en fonction de la position de la rame
        // Chargez une police de caractères
        font.loadFromFile("C:\\Users\\Administrateur\\source\\repos\\CMakeProject3\\Raleway-Medium.ttf");
        // Configuration du texte
        textRame.setFont(font);
        textRame.setCharacterSize(14); // en pixels
        textRame.setFillColor(sf::Color::White);
        textRame.setPosition((300*(num-1))+50,10); // Positionnez le texte au-dessus de la rame

        // Met à jour le texte avec le nombre initial de passagers
        textRame.setString("Passagers dans la rame " + std::to_string(numero) +" : " + std::to_string(nb_passagers));
    }
    //Déclaration des méthodes (voir main.cpp)
    void circuler(vector<Station>& stations, sf::RenderWindow& window);
    void dessiner(sf::RenderWindow& window);
    int flux_passagers(Station& station);
};

class Supervision {
public:
    vector<Rame>& rames;

    Supervision(vector<Rame>& rames) : rames(rames) {}

};