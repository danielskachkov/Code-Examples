#ifndef IGRAPH_H
#define IGRAPH_H

#include <vector>

struct IGraph
{
	virtual ~IGraph() {}

	virtual void add_edge(int out, int in) = 0;

	virtual int vertices_count() const  = 0;
	virtual void get_next_vertices(int vertex, std::vector <int> &output) const = 0;
	virtual void get_prev_vertices(int vertex, std::vector <int> &output) const = 0;
};

#endif
