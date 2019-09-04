#ifndef CLISTGRAPH_H
#define CLISTGRAPH_H

#include "IGraph.h"

class CListGraph : public IGraph
{
    std::vector <std::vector <int> > to;
    std::vector <std::vector <int> > from;

public:
    CListGraph(int vertices_count)
    {
		to = std::vector <std::vector <int> > (vertices_count);
		from = std::vector <std::vector <int> > (vertices_count);
	}
	
    CListGraph(const IGraph * graph);
	void add_edge(int out, int in);
	int vertices_count() const;
	void get_next_vertices(int vertex, std::vector <int> &output) const;
	void get_prev_vertices(int vertex, std::vector <int> &output) const;
};

#endif
