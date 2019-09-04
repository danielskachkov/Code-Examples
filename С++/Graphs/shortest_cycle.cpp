#include <iostream>
#include <algorithm>
#include <vector>

void dfs(IGraph *graph, std::vector <int> &time_in, int vertex, int cur_time, int &ans)
{
    std::vector <int> next_vertices;
    time_in[vertex] = cur_time;
    graph->get_next_vertices(vertex, next_vertices);
    for(int i : next_vertices)
    {
        if(time_in[i] == -10)
            dfs(graph, time_in, i, cur_time + 1, ans);
        else if(time_in[i] < cur_time - 1)
        {
            if(ans == -1)
                ans = cur_time - time_in[i] + 1;
            else ans = std::min(ans, cur_time - time_in[i] + 1);
            if(ans == 3)
                return;
        }
    }
}

int shortest_cycle(IGraph *graph)
{
    int ans = -1;
    std::vector <int> time_in(graph->vertices_count(), -10);
    for(int i = 0; i < graph->vertices_count(); i++)
    {
        dfs(graph, time_in, i, 0, ans);
        if(ans == 3)
            break;
        for(int j = 0; j < time_in.size(); j++)
            time_in[j] = -10;
    }
    return ans;
}

int main()
{
    int N, M, v, w;
	std::cin >> N >> M;
	IGraph *graph = new CListGraph(N);
	for(int i = 0; i < M; i++)
	{
	    std::cin >> v >> w;
	    graph->add_edge(v, w);
	}
    
    std::cout << shortest_cycle(graph) << std::endl;
    
    delete graph;

    return 0;
}
