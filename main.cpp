#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include "header.h"

//Méthode de la classe Rame qui permet de dessiner la texture et le texte précisant le nombre de passagers dans la rame
void Rame::dessiner(sf::RenderWindow& window) {
    window.draw(texture);
    window.draw(textRame);
}

//Méthode de la classe Station qui permet de dessiner la texture et le texte précisant le nombre de personne dans la station 
void Station::dessiner(sf::RenderWindow& window) {
    // Dessinez la station
    window.draw(texture);
    window.draw(textStation);
}
// Définition de la méthode circuler qui gère la direction, la vitesse, et la position de la rame au sien du circuit fermé
void Rame::circuler(vector<Station>& stations, sf::RenderWindow& window) {
    while (true) { //Il s'agit d'une boucle infinie
        Station& station = stations[stationActuelle]; //On seléctionne la station vers laquelle la rame approche
        if (abs(position - station.position) < 10 * vitesse) { // Commence à décélérer à une distance proportionnelle à la vitesse  
                vitesse = max(1, vitesse - 1); // Décélère jusqu'à une vitesse minimale de 1
        }
        if (position == station.position) { // Si la rame est à la station
            int flux = flux_passagers(station);
            sf::sleep(sf::milliseconds(1000 * flux));//La rame reste immobile pendant une durée proportionnelle au nombre de passagers rentrant et descendant de celle-ci
            flux_passagers(station);//Mise en place et affichage du flux de passagers rentrant et sortant de la rame
            if ((stationActuelle == 0 && direction == false) || (stationActuelle == stations.size() - 1 && direction == true)) {//Si la rame arrive à l'un des terminus
                direction = !direction; // Elle change de direction 
                if (direction == false) {//Dans le cas où la rame arrive au terminus de la direction true et passe à la direction false
                    for (int i = 0; i < 40; ++i) {//Déplacement de la position x de la  rame jusqu'au départ du nouveau départ dans la direction inverse
                        position += 2;
                        texture.setPosition(position, y_position);
                        sf::sleep(sf::milliseconds(100)); 
                    }
                    for (int i = 0; i < 30; ++i) {//Déplacement de la position y
                        y_position += 2;
                        texture.setPosition(position, y_position);
                        sf::sleep(sf::milliseconds(100)); // Pause pour rendre le mouvement visible
                    }
                }
                if (direction == true) {//Dans le cas où la rame arrive au terminus de la direction false et passe à la direction true
                    for (int i = 0; i < 40; ++i) { //Déplacement de la position x de la  rame jusqu'au départ du nouveau départ dans l'autre direction 
                        position -= 2;
                        texture.setPosition(position, y_position);
                        sf::sleep(sf::milliseconds(100)); // Pause pour rendre le mouvement visible
                    }
                    for (int i = 0; i < 30; ++i) { //Déplacement de la position y
                        y_position -= 2;
                        texture.setPosition(position, y_position);
                        sf::sleep(sf::milliseconds(100)); // Pause pour rendre le mouvement visible
                    }
                }
            }
            else {
                stationActuelle = direction ? (stationActuelle + 1) % stations.size() : (stationActuelle - 1 + stations.size()) % stations.size();//Définition de la prochaine station lors de l'arrivée de la rame à une station, selon la direction
            }
        }


        if (direction == true) {//La position est incrémentée positivement par la vitesse pour faire avancer la rame
            position += vitesse;
            if (stationActuelle > 0 && (position > (stations[stationActuelle - 1].position) && vitesse < 6 && abs(position - station.position) >= 10 * vitesse && stationActuelle <= stations.size() - 1)) {
                vitesse++;//Accélération de la rame après l'arrêt à une station
            }
        }

        if (direction == false) {//La position est incrémentée négativement par la vitesse pour faire avancer la rame dans la direction inverse
            position -= vitesse;
            if (stationActuelle < stations.size() - 1 && (position <(stations[stationActuelle + 1].position) && vitesse > -6 && abs(position - station.position) >= 10 * vitesse && stationActuelle >= 0)) {
                vitesse++;//Accélération de la rame après l'arrêt à une station dans la direction inverse
            }
        }
        texture.setPosition(position, y_position);//mise à jour de la position de la texture de la rame 
        sf::sleep(sf::milliseconds(100));//Pause pour rendre le mouvement visible
    }
}


int Rame::flux_passagers(Station& station) { //Déclaration de la fonction mettant en place le flux des passagers au sein de la station et de la rame
    // Calculez le nombre maximum de passagers qui peuvent monter à bord
    int max_passagers = min(5 - nb_passagers, station.nb_passagers);//Le nombre maximal de passager entrant dans la rame est inférieur ou égal à 5 (capacité de la rame) et inférieur au nombre de personnes dans la station
    srand(time(NULL) + this->numero);
    // Génère un nombre aléatoire de passagers à embarquer
    int passagers_a_embarquer = rand() % (max_passagers + 1);
    int passagers_a_descendre = rand() % (nb_passagers + 1);

    // Met à jour le nombre de passagers dans la rame
    nb_passagers -= passagers_a_descendre;
    // Met à jour le nombre de passagers dans la rame et à la station
    nb_passagers += passagers_a_embarquer;
    station.nb_passagers -= passagers_a_embarquer;
    int tot_passagers = passagers_a_descendre + passagers_a_embarquer;//Nombre de passager descendant et montant dans la rame qui sera retourné dans la fonction circuler
    textRame.setString("Passagers dans la rame " + std::to_string(numero) + " : " + std::to_string(nb_passagers));
    station.textStation.setString("Passagers : " + std::to_string(station.nb_passagers));
    //affichage du texte du nombre de passagers dans les stations et dans la rame
    return tot_passagers;
}

void incrementeStationAsync(vector<Station>& stations) {//Déclaration de la fonction incrémentant le nombre de personnes entrant dans la station de manière aléatoire
    while (true) { // Boucle infinie
        for (Station& station : stations) {
            // Génère un intervalle de temps aléatoire
            int intervalle = rand() % 10000; // Par exemple, entre 0 et 10000 millisecondes
            sf::sleep(sf::milliseconds(intervalle));

            // Incrémente le nombre de passagers à la station
            station.nb_passagers += 1;

            // Met à jour le texte avec le nouveau nombre de passagers
            station.textStation.setString("Passagers : " + std::to_string(station.nb_passagers));
        }
    }
}


bool compareStations(const Station& s1, const Station& s2) {
    return s1.position < s2.position;
}

int main() {
    //Cration du cadre d'affichage SFML et de sa taille
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Metro Simulation");
    //Déclaration et initialisation des textures
    sf::Texture texture1;
    sf::Texture texture2;
    texture1.loadFromFile("C:\\Users\\Administrateur\\source\\repos\\CMakeProject3\\metro.png");
    texture2.loadFromFile("C:\\Users\\Administrateur\\source\\repos\\CMakeProject3\\station.jpg");
    //Initilisation des stations, de leur position, et de leur texture
    Station station1(200, texture2);
    Station station2(400, texture2);
    Station station3(600, texture2);
    Station station4(800, texture2);
    Station station5(1000, texture2);
    //On les range dans un vecteur
    vector<Station> stations = { station1, station2,station3,station4,station5 };
    std::sort(stations.begin(), stations.end(), compareStations);
    std::thread t(incrementeStationAsync, std::ref(stations));//Création de threads afin de rendre l'incrémentation des station indépendante pour chaque station

    //Initialisation des rames
    Rame rame1(1, stations, 1,texture1);
    Rame rame2(2, stations, 2,texture1);
    Rame rame3(3, stations, 3, texture1);
    vector<Rame> rames = { rame1, rame2,rame3};// On les range dans un vecteur
    Supervision supervision(rames);
    std::vector<std::thread> threads;
    for (Rame& rame : rames) {//Création de threads pour les rames afin de rendre la circulation et leur temps d'arrêt indépendant pour chaque rame 
        threads.push_back(std::thread(&Rame::circuler, &rame, std::ref(stations), std::ref(window)));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Dessine chaque station une fois
        for (Station& station : stations) {
            station.dessiner(window);
        }

        // Dessine chaque rame
        for (Rame& rame : rames) {
            rame.dessiner(window);
        }
        window.display();//Affichage
        sf::sleep(sf::milliseconds(100));//Pause pendant un certain temps
        window.clear();//Puis on efface


    }
        t.join();//On libère les threads

    return 0;
}
