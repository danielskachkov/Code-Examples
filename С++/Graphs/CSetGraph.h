#ifndef CSETGRAPH_H
#define CSETGRAPH_H

#include "IGraph.h"
#include <unordered_set>

class CSetGraph : public IGraph
{
	std::vector <std::unordered_set <int> > to;
	std::vector <std::unordered_set <int> > from;

public:
    CSetGraph(int vertices_count)
    {
		to = std::vector <std::unordered_set <int> > (vertices_count);
		from = std::vector <std::unordered_set <int> > (vertices_count);
	}
    CSetGraph(const IGraph *graph);
	void add_edge(int out, int in);
	int vertices_count() const;
	void get_next_vertices(int vertex, std::vector <int> &output) const;
	void get_prev_vertices(int vertex, std::vector <int> &output) const;
};


#endif
