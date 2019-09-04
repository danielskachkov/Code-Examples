#include "CMatrixGraph.h"

CMatrixGraph::CMatrixGraph(const IGraph *graph)
{
	adj_matrix = std::vector <std::vector <bool> > (graph->vertices_count(), std::vector <bool> (graph->vertices_count(), false));
	std::vector <int> vertices;
	
	for(int i = 0; i < graph->vertices_count(); i++)
	{
		graph->get_next_vertices(i, vertices);
		for(int j : vertices)
			add_edge(i, j);
		vertices.clear();
	}
}

void CMatrixGraph::add_edge(int out, int in)
{
    adj_matrix[out][in] = true;
}

int CMatrixGraph::vertices_count() const
{
    return adj_matrix.size();
}

void CMatrixGraph::get_next_vertices(int vertex, std::vector <int> &output) const
{
    for(int i = 0; i < adj_matrix[vertex].size(); i++)
    	if(adj_matrix[vertex][i])
    		output.push_back(i);
}

void CMatrixGraph::get_prev_vertices(int vertex, std::vector <int> &output) const
{
    for(int i = 0; i < adj_matrix.size(); i++)
    	if(adj_matrix[i][vertex])
    		output.push_back(i);
}
