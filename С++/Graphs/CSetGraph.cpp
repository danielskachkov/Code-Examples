#include "CSetGraph.h"

CSetGraph::CSetGraph(const IGraph *graph)
{
    to = std::vector <std::unordered_set <int> > (graph->vertices_count());
    from = std::vector <std::unordered_set <int> > (graph->vertices_count());
    std::vector <int> vertices;
	
	for(int i = 0; i < graph->vertices_count(); i++)
	{
		graph->get_next_vertices(i, vertices);
		for(int j : vertices)
			add_edge(i, j);
		vertices.clear();
	}
}

void CSetGraph::add_edge(int out, int in)
{
	to[out].insert(in);
	from[in].insert(out);
}

int CSetGraph::vertices_count() const
{
    return to.size();
}

void CSetGraph::get_next_vertices(int vertex, std::vector <int> &output) const
{
    for(int i : to[vertex])
        output.push_back(i);
}

void CSetGraph::get_prev_vertices(int vertex, std::vector <int> &output) const
{
    for(int i : from[vertex])
        output.push_back(i);
}
