#include "PlanePath.h"
#include <vector>
#include <algorithm>  // std::reverse

bool PlanePath::find() {
    // Pokud start == finish, nen� co hledat
    if (start == finish) {
        path.clear();
        path.push_back(start);
        return true;
    }

    // P��prava pole pro vzd�lenosti a "parent" (pro rekonstrukci)
    std::vector<double> dist(map.nx * map.ny, std::numeric_limits<double>::infinity());
    std::vector<Point> parent(map.nx * map.ny, Point(-1, -1));

    // P��prava priority_queue
    std::priority_queue< PlaneState, std::vector<PlaneState>, std::greater<PlaneState> > pq;

    // Inicializace pro start
    dist[indexOf(start)] = 0.0;
    pq.push({ 0.0, start });

    // Definice 8 sm�r� (v�etn� diagon�l)
    static const int dx[8] = { 1,1,1,0,0,-1,-1,-1 };
    static const int dy[8] = { 1,0,-1,1,-1,1,0,-1 };

    // Dijkstra
    while (!pq.empty()) {
        auto [currCost, u] = pq.top();
        pq.pop();

        // Pokud je to zastaral� z�znam, p�esko��me
        if (currCost > dist[indexOf(u)]) continue;

        // Pokud jsme do�li do c�le
        if (u == finish) {
            // Rekonstrukce cesty
            path.clear();
            Point cur = finish;
            while (cur != Point(-1, -1)) {
                path.push_back(cur);
                cur = parent[indexOf(cur)];
            }
            std::reverse(path.begin(), path.end());
            return true;
        }

        // Projdeme 8 soused�
        for (int i = 0; i < 8; i++) {
            Point v(u.x + dx[i], u.y + dy[i]);
            if (!map.validCoords(v)) continue;

            // Letadlo nem� ��dn� omezen� na v��ku, cost = eukl. vzd�lenost
            double stepCost = euclidDist(u, v);
            double newCost = dist[indexOf(u)] + stepCost;

            // Ulo��me, pokud je lep��
            if (newCost < dist[indexOf(v)]) {
                dist[indexOf(v)] = newCost;
                parent[indexOf(v)] = u;
                pq.push({ newCost, v });
            }
        }
    }

    // Cesta nenalezena
    return false;
}
