#ifndef CARCGRAPH_H
#define CARCGRAPH_H

#include "IGraph.h"

class CArcGraph : public IGraph
{
	struct edge
	{
		int to, from;
	};

	int vertices_num;
    std::vector <edge> edges;

public:
    CArcGraph(int vertices_count)
    {
    	vertices_num = vertices_count;
	}

    CArcGraph(const IGraph *graph);
	void add_edge(int out, int in);
	int vertices_count() const;
	void get_next_vertices(int vertex, std::vector <int> &output) const;
	void get_prev_vertices(int vertex, std::vector <int> &output) const;
};

#endif
