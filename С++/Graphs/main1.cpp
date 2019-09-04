#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
using namespace std;

struct component
{
    int out;
    int in;
    
    component()
    {
        in = 0;
        out = 0;
    }
};

struct IGraph
{
	virtual ~IGraph() {}

	virtual void add_edge(int out, int in) = 0;

	virtual int vertices_count() const  = 0;
	virtual void get_next_vertices(int vertex, std::vector <int> &output) const = 0;
	virtual void get_prev_vertices(int vertex, std::vector <int> &output) const = 0;
};

class CListGraph : public IGraph
{
    vector <vector <int> > to;
    vector <vector <int> > from;

public:
    CListGraph(int vertices_count)
    {
		to = vector <vector <int> > (vertices_count);
		from = vector <vector <int> > (vertices_count);
	}
	
    CListGraph(const IGraph *graph)
    {
	    to = vector <vector <int> > (graph->vertices_count());
	    from = vector <vector <int> > (graph->vertices_count());
	    vector <int> vertices;
	
	    for(int i = 0; i < graph->vertices_count(); i++)
	    {
		    graph->get_next_vertices(i, vertices);
		    for(int j : vertices)
			    add_edge(i, j);
		    vertices.clear();
	    }
    }

    void add_edge(int out, int in)
    {
        to[out].push_back(in);
        from[in].push_back(out);
    }

    int vertices_count() const
    {
        return to.size();
    }

    void get_next_vertices(int vertex, vector<int>& output) const
    {
        for(int i : to[vertex])
            output.push_back(i);
    }

    void get_prev_vertices(int vertex, vector<int>& output) const
    {
        for(int i : from[vertex])
            output.push_back(i);
    }
};

void dfs1(IGraph *graph, int vertex, vector <bool> &visited, stack <int> &order)
{
    visited[vertex] = true;
    vector <int> next_vertices;
    graph->get_next_vertices(vertex, next_vertices);
    for(int i : next_vertices)
        if(!visited[i])
            dfs1(graph, i, visited, order);
    order.push(vertex);
}

void dfs2(IGraph *graph, int vertex, int cur_component, vector <component> &components, vector <bool> &visited, vector <int> &vertice_component)
{
    visited[vertex] = true;
    vertice_component[vertex] = cur_component;
    vector <int> next_vertices;
    graph->get_next_vertices(vertex, next_vertices);
    for(int i : next_vertices)
    {
        if(!visited[i])
            dfs2(graph, i, cur_component, components, visited, vertice_component);
        else if(vertice_component[i] < cur_component)
        {
            components[cur_component].out++;
            components[vertice_component[i]].in++;
        }
    }
}

void transpose(const IGraph *graph, IGraph *gt)
{
    for(int i = 0; i < graph->vertices_count(); i++)
    {
        vector <int> next_vertices;
        vector <int> prev_vertices;
        graph->get_next_vertices(i, next_vertices);
        graph->get_prev_vertices(i, prev_vertices);
        for(int j : next_vertices)
            gt->add_edge(j, i);
        for(int j : prev_vertices)
            gt->add_edge(i, j);
    }
}

int edges_to_add(IGraph *graph)
    {
        IGraph *gt = new CListGraph(graph->vertices_count());
        vector <int> vertice_component(graph->vertices_count(), -1);
        transpose(graph, gt);
        stack <int> order;
        vector <bool> visited(graph->vertices_count(), false);
        for(int i = 0; i < visited.size(); i++)
            if(!visited[i])
                dfs1(gt, i, visited, order);
        for(int i = 0; i < visited.size(); i++)
            visited[i] = false;
        int comp = 0;
        vector <component> components;
        
        while(!order.empty())
        {
            int v = order.top();
            order.pop();
            if(!visited[v])
            {
                component c;
                components.push_back(c);
                dfs2(graph, v, comp, components, visited, vertice_component);
                comp++;
            }
        }
        
        int k = 0, r = 0, q = 0;
        
        for(int i = 0; i < comp; i++)
        {
            if(components[i].in == 0 && components[i].out == 0)
                k++;
            else if(components[i].in == 0)
                r++;
            else if(components[i].out == 0)
                q++;
        }
        
        delete gt;
        
        if(comp > 1)
            return k + max(r, q);
        return 0;
    }


int main()
{
    int N, M, v, w;
	cin >> N >> M;
	IGraph *graph = new CListGraph(N);
	for(int i = 0; i < M; i++)
	{
	    cin >> v >> w;
	    graph->add_edge(v - 1, w - 1);
	}
	
	cout << edges_to_add(graph) << endl;
	delete graph;
	
	return 0;
}
