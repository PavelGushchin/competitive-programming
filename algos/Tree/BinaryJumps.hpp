#ifndef __BINARYJUMPS_HPP__
#define __BINARYJUMPS_HPP__
namespace algos {
namespace jumps {
template<int PMAX = 30>
struct BinaryJumps {

    vvi adj;
    vi dep;
    vi par[PMAX]{};
    
    void dfs(int u, int p) {
        dep[u] = dep[p] + 1;
        par[0][u] = p;
        for (int v : adj[u])
            if (v != p)
                dfs(v, u);
    }
    
    void build(int n, const vpii &edges, int root = 1) {
        adj.assign(n+1, {});
        par[0].assign(n+1, 0);
        dep.assign(n+1,0);
        for (auto &[u,v] : edges) adj[u] << v, adj[v] << u;
        dfs(root, 0);
        for (int p = 1; p < PMAX; p++) {
            par[p].resize(n+1);
            for (int u = 1; u <= n; u++)
                par[p][u] = par[p-1][par[p-1][u]];
        }
    }
    
    template<typename T>
    int jump(int u, T delta) const {
        for (int i = PMAX-1; delta >= 0 && i >= 0; i--)
            if (delta >> i & 1) {
                u = par[i][u];
                delta ^= (T(1) << i);
            }
        return u;
    }
    
    bool isParent(int u, int p) const {
        return jump(u, dep[u]-dep[p]) == p;
    }
    
    int directChild(int u, int p) const {
        return jump(u, dep[u]-dep[p]-1);
    }
    
};
} // namespace jumps
} // namespace algos
#endif // __BINARYJUMPS_HPP__