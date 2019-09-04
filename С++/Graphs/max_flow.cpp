#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class CListGraph
{
    vector <vector <int> > to_list;

public:
    CListGraph(int vertices_count)
    {
        to_list = vector <vector <int> > (vertices_count);
    }

    void add_edge(int v1, int v2)
    {
        to_list[v1].push_back(v2);
        to_list[v2].push_back(v1);
    }

    int vertices_count()
    {
        return to_list.size();
    }

    void get_next_vertices(int vertex, vector <int> &output)
    {
        output = to_list[vertex];
    }
};

bool find_path(CListGraph &graph, vector <int> &path, const int source, const int sink, vector <vector <int> > &flow, const vector <vector <int> > &capacity)
{
    int v;
    queue <int> q;
    vector <int> parent(graph.vertices_count(), -2);
    parent[source] = -1;
    q.push(source);

    while(!q.empty())
    {
        v = q.front();
        q.pop();

        if(v == sink)
            break;

        vector <int> next_vertices;
        graph.get_next_vertices(v, next_vertices);

        for(int u : next_vertices)
            if(parent[u] == -2 && flow[v][u] < capacity[v][u])
            {
                parent[u] = v;
                q.push(u);
            }
    }

    if(v != sink)
        return false;

    path.clear();

    while(v != -1)
    {
        path.push_back(v);
        v = parent[v];
    }

    return true;
}

int modify_path(const vector <int> &path, vector <vector <int> > &flow, const vector <vector <int> > &capacity)
{
    const int inf = 1000000;
    int cmin = inf;

    for(int i = path.size() - 1; i > 0; i--)
        cmin = min(cmin, capacity[path[i]][path[i-1]] - flow[path[i]][path[i-1]]);

    for(int i = path.size() - 1; i > 0; i--)
    {
        flow[path[i]][path[i-1]] += cmin;
        flow[path[i-1]][path[i]] -= cmin;
    }

    return cmin;
}

int edmonds_karp(CListGraph &graph, const vector <vector <int> > &capacity, const int source, const int sink)
{
    int summary_flow = 0;
    vector <int> path;
    vector <vector <int> > flow(graph.vertices_count(), vector <int> (graph.vertices_count(), 0));

    while(find_path(graph, path, source, sink, flow, capacity))
        summary_flow += modify_path(path, flow, capacity);

    return summary_flow;
}

int main()
{
    int N, M;
    cin >> N >> M;
    CListGraph graph(N);
    vector <vector <int> > capacity(N, vector <int> (N, 0));

    for(int i = 0; i < M; i++)
    {
        int v1, v2, w;
        cin >> v1 >> v2 >> w;
        v1--;
        v2--;
        graph.add_edge(v1, v2);
        capacity[v1][v2] = w;
    }

    cout << edmonds_karp(graph, capacity, 0, N-1);

    return 0;
}
