#include "astar.h"

vector<pair<int,int>> a_star(const vector<vector<int>>& grid,
                              int sx, int sy, int gx, int gy) {
    int n = grid.size(), m = grid[0].size();
    vector<vector<int>> dist(n, vector<int>(m, INF));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(m, {-1,-1}));
    std::priority_queue<Node, vector<Node>, std::greater<Node>> pq;

    dist[sx][sy] = 0;
    pq.push({sx, sy, 0, heuristic(sx, sy, gx, gy)});

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        int x = cur.x, y = cur.y;
        if (x == gx && y == gy) break;

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir], ny = y + dy[dir];
            if (nx < 0 || ny < 0 || nx >= n || ny >= m || grid[nx][ny] == 1) continue;

            int nc = dist[x][y] + 1;
            if (nc < dist[nx][ny]) {
                dist[nx][ny] = nc;
                parent[nx][ny] = {x, y};
                pq.push({nx, ny, nc, nc + heuristic(nx, ny, gx, gy)});
            }
        }
    }

    vector<pair<int,int>> path;
    if (parent[gx][gy].first == -1) return path;
    for (int x = gx, y = gy; !(x == sx && y == sy); ) {
        path.emplace_back(x, y);
        auto p = parent[x][y];
        x = p.first; y = p.second;
    }
    path.emplace_back(sx, sy);
    std::reverse(path.begin(), path.end());
    return path;
}
