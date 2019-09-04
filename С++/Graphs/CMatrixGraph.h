#ifndef CMATRIXGRAPH_H
#define CMATRIXGRAPH_H

#include "IGraph.h"

class CMatrixGraph : public IGraph
{
    std::vector <std::vector <bool> > adj_matrix;

public:
    CMatrixGraph(int vertices_count)
    {
    	adj_matrix = std::vector <std::vector <bool> > (vertices_count, std::vector <bool> (vertices_count, false));
	}
	
    CMatrixGraph(const IGraph *graph);
	void add_edge(int out, int in);
	int vertices_count() const;
	void get_next_vertices(int vertex, std::vector <int> &output) const;
	void get_prev_vertices(int vertex, std::vector <int> &output) const;
};

#endif
