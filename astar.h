#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <utility>

using std::vector;
using std::pair;

struct Node {
    int x, y, cost, priority;
    bool operator>(const Node& o) const { return priority > o.priority; }
};

static const int INF = 1e9;
static const int dx[4] = {-1,1,0,0}, dy[4] = {0,0,-1,1};

inline int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

vector<pair<int,int>> a_star(const vector<vector<int>>& grid,
                              int sx, int sy, int gx, int gy);

#endif // ASTAR_H
