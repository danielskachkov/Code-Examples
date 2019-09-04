#include "CArcGraph.h"

CArcGraph::CArcGraph(const IGraph *graph)
{
    vertices_num = graph->vertices_count();
    std::vector <int> vertices;
	
	for(int i = 0; i < vertices_num; i++)
	{
		graph->get_next_vertices(i, vertices);
		for(int j : vertices)
			add_edge(i, j);
		vertices.clear();
	}
}

void CArcGraph::add_edge(int out, int in)
{
	edge edg;
	edg.from = out;
	edg.to = in;
    edges.push_back(edg);
}

int CArcGraph::vertices_count() const
{
    return vertices_num;
}

void CArcGraph::get_next_vertices(int vertex, std::vector <int> &output) const
{
    for(edge i : edges)
    	if(i.from == vertex)
    		output.push_back(i.to);
}

void CArcGraph::get_prev_vertices(int vertex, std::vector <int> &output) const
{
    for(edge i : edges)
    	if(i.to == vertex)
    		output.push_back(i.from);
}
