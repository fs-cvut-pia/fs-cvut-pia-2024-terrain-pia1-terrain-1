#include "TerrainMap.h"
#include "Path.h"
#include <vector>
#include <iostream>
#include <string>

// P�id�v�me na�e odvozen� t��dy s implementac� find():
#include "PlanePath.h"
#include "BoatPath.h"
#include "RoadPath.h"
#include "RoadFerryPath.h"
#include "RailPath.h"

int main(int argc, char* argv[]) {
    const int nx = 256;
    const int ny = 256;

    // 1) Pokus�me se na��st n�zev souboru s mapou z argumentu
    std::string terrain_filename;
    if (argc > 1) {
        terrain_filename = argv[1];
    }
    else {
        std::cout << "No terrain file specified! Pou�ije se v�choz�: terrain.dat" << std::endl;
        terrain_filename = "terrain.dat";
    }

    // 2) Vytvo��me mapu
    TerrainMap m(nx, ny, terrain_filename);

    // 3) Zept�me se u�ivatele na sou�adnice
    int startX, startY;
    std::cout << "Zadejte sou�adnice po��tku (x y): ";
    std::cin >> startX >> startY;

    int finishX, finishY;
    std::cout << "Zadejte sou�adnice konce (x y): ";
    std::cin >> finishX >> finishY;

    // 4) Vytvo��me objekty typu Point
    Point start(startX, startY);
    Point finish(finishX, finishY);

    // 5) P�id�me p�t variant cest
    std::vector<Path*> paths = {
       new PlanePath(m, "Plane",       start, finish),
       new BoatPath(m,  "Boat",        start, finish),
       new RoadPath(m,  "Road",        start, finish),
       new RoadFerryPath(m, "Road+Ferry", start, finish),
       new RailPath(m,  "Rail",        start, finish)
    };

    // 6) Spust�me hled�n� pro ka�dou variantu
    for (auto& p : paths) {
        std::cout << "Path search: " << p->getName() << std::endl;
        std::cout << "=============" << std::endl;
        bool ok = p->find();
        if (!ok) {
            std::cout << "Path not found!" << std::endl;
        }
        p->printStats();
        std::cout << "=============" << std::endl;
        p->saveToFile(); // Ulo�� se nap�. do "Plane.dat", "Boat.dat", atd.
        delete p;
    }

    return 0;
}
