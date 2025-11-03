
/**
 * Author: Alexander Yoshida (I think)
 * Date: 202X
 * License: idc
 * Description: Finds articulation points (cut vertices) and bridges in an undirected graph.
 *   - `findArticulationPoints(adj)` returns all articulation points.
 *   - If `bridgesOnly = true`, it only computes bridges (pairs of vertices whose removal
 *     increases the number of connected components) and returns an empty vector for vertices.
 * Time: O(V + E)
 */
#pragma once

vector<int> disc;
vector<int> low;
vector<bool> artic;
vector<pair<int,int>> bridges;
int nextTime = 0;

int artic_dfs(vector<vector<int>> &adj, int curr, int parent) {
    int children = 0;
    disc[curr] = low[curr] = nextTime++;

    for (int next : adj[curr]) {
        if (next == parent) continue;

        if (disc[next] != -1) {
            low[curr] = min(low[curr], disc[next]);
            continue;
        }

        ++children;
        artic_dfs(adj, next, curr);
        if (disc[curr] <= low[next]) artic[curr] = true;
        if (disc[curr] < low[next]) bridges.emplace_back(min(curr, next), max(curr, next));
        low[curr] = min(low[curr], low[next]);
    }

    return children;
}

vector<int> findArticulationPoints(vector<vector<int>> &adj, bool bridgesOnly = false) {
    int n = adj.size();
    low = disc = vector<int>(n, -1);
    bridges.clear();
    artic = vector<bool>(n, false);

    for (int root = 0; root < n; ++root) {
        if (disc[root] != -1) continue;

        artic[root] = artic_dfs(adj, root, -1) > 1;
    }

    if (bridgesOnly) return {};
    vector<int> res;
    for (int i = 0; i < n; ++i) if (artic[i]) res.push_back(i);
    
    return res;
}
