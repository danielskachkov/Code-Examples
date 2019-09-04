#include "CListGraph.h"

CListGraph::CListGraph(const IGraph *graph)
{
	to = std::vector <std::vector <int> > (graph->vertices_count());
	from = std::vector <std::vector <int> > (graph->vertices_count());
	std::vector <int> vertices;
	
	for(int i = 0; i < graph->vertices_count(); i++)
	{
		graph->get_next_vertices(i, vertices);
		for(int j : vertices)
			add_edge(i, j);
		vertices.clear();
	}
}

void CListGraph::add_edge(int out, int in)
{
    to[out].push_back(in);
    from[in].push_back(out);
}

int CListGraph::vertices_count() const
{
    return to.size();
}

void CListGraph::get_next_vertices(int vertex, std::vector<int>& output) const
{
    for(int i : to[vertex])
        output.push_back(i);
}

void CListGraph::get_prev_vertices(int vertex, std::vector<int>& output) const
{
    for(int i : from[vertex])
        output.push_back(i);
}
