#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;

class Point2D
{
    double x, y;

public:
    Point2D() {}
    Point2D(double x, double y): x(x), y(y) {}

    void set_x(double new_x)
    {
        x = new_x;
    }

    void set_y(double new_y)
    {
        y = new_y;
    }

    double get_x() const
    {
        return x;
    }

    double get_y() const
    {
        return y;
    }

    Point2D operator +(const Point2D &A) const
    {
        return {this->x + A.x, this->y + A.y};
    }

    Point2D operator -(const Point2D &A) const
    {
        return {this->x - A.x, this->y - A.y};
    }

    void operator *=(double k)
    {
        x *= -1;
        y *= -1;
    }
};

class Edge
{
    double x, y, angle;

public:
    Edge() {}
    
    Edge(double x, double y, double angle = 0): x(x), y(y), angle(angle) {}
    
    Edge(const Point2D &A, const Point2D &B)
    {
        x = B.get_x() - A.get_x();
        y = B.get_y() - A.get_y();
    }

    void set_x(double new_x)
    {
        x = new_x;
    }

    void set_y(double new_y)
    {
        y = new_y;
    }

    double get_x() const
    {
        return x;
    }

    double get_y() const
    {
        return y;
    }

    void set_angle(double new_angle)
    {
        angle = new_angle;
    }

    double get_angle() const
    {
        return angle;
    }

    double operator *(const Edge &edge) const
    {
        return this->x * edge.x + this->y * edge.y;
    }

    void operator *=(double k)
    {
        x *= k;
        y *= k;
    }

    double length() const
    {
        return sqrt(x*x + y*y);
    }

    friend double cos_angle_between_edges(Edge &edge1, Edge &edge2)
    {
        return (edge1 * edge2)/(edge1.length() * edge2.length());
    }

    friend int orientation(const Edge &edge1, const Edge &edge2)
    {
        double z = edge1.x*edge2.y - edge1.y*edge2.x;
        if(z > 0)
            return 1;
        else if(z == 0)
            return 0;
        else
            return -1;
    }
};

class ConvexPolygon
{
    vector <Edge> edges;
    vector <Point2D> vertices;
    
    void sort_edges()
    {
        int min_edge_id = 0;
        Edge ox(1, 0);
        for(int i = 0; i < edges.size(); i++)
        {
            edges[i].set_angle(edges[i].get_y() <= 0 ? -cos_angle_between_edges(edges[i], ox) : (cos_angle_between_edges(edges[i], ox) + 10));
            if(edges[i].get_angle() < edges[min_edge_id].get_angle())
            {
                min_edge_id = i;
            }
        }
        
        vector <Edge> output;
        for(int i = 0; i < edges.size(); i++)
        {
            output.push_back(edges[(min_edge_id + i)%edges.size()]);
        }
        edges = output;
    }

    static void merge(const vector <Edge> &edges1, const vector <Edge> &edges2, vector <Edge> &merged_edges)
    {
        int j = 0;

        for(int i = 0; i < edges1.size(); i++)
        {
            while(j < edges2.size() && edges2[j].get_angle() < edges1[i].get_angle())
            {
                merged_edges.push_back(edges2[j]);
                j++;
            }

            merged_edges.push_back(edges1[i]);
        }

        for( ; j < edges2.size(); j++)
        {
            merged_edges.push_back(edges2[j]);
        }
    }
    
    Point2D find_left_top()
    {
        //cout << vertices.size() << endl;

        Point2D A = vertices[0];
        for(int i = 1; i < vertices.size(); i++)
        {
            if(vertices[i].get_y() > A.get_y())
                A = vertices[i];
            else if(vertices[i].get_y() == A.get_y() && vertices[i].get_x() < A.get_x())
                A = vertices[i];
        }
        
        return A;
    }

    void reverse()
    {
        for(Point2D &point : vertices)
        {
            point *= -1;
        }

        for(Edge &edge : edges)
        {
            edge *= -1;
        }
    }

    bool contains_point(const Point2D &point)
    {
        int orientation_sign = 0;
        Edge edge1, edge2;

        for(int i = 0; i < vertices.size(); i++)
        {
            edge1 = edges[i];
            edge2 = Edge(vertices[i], point);
            int new_sign = orientation(edge1, edge2);

            if(new_sign == orientation_sign + 2 || new_sign == orientation_sign - 2)
            {
                return false;
            }
            if(new_sign == -1 || new_sign == 1)
                orientation_sign = new_sign;
        }

        return true;
    }

public:
    ConvexPolygon() {}
    
    ConvexPolygon(const vector <Point2D> &vertices, const vector <Edge> &edges): vertices(vertices), edges(edges) {}
    
    ConvexPolygon(const vector <Point2D> &vertices)
    {
        this->vertices = vertices;

        for(int i = 1; i < vertices.size(); i++)
        {
            edges.push_back(Edge(vertices[i - 1], vertices[i]));
        }
        edges.push_back(Edge(vertices.back(), vertices[0]));
    }
    
    ConvexPolygon(const vector <Edge> &edges, const Point2D &first_edge_begging)
    {
        this->edges = edges;
        vertices.push_back(first_edge_begging);

        for(int i = 0; i < edges.size() - 1; i++)
        {
            double x = vertices[i].get_x();
            double y = vertices[i].get_y();
            vertices.push_back(Point2D(x + edges[i].get_x(), y + edges[i].get_y()));
        }
    }

    ConvexPolygon operator +(ConvexPolygon &polygon) //Minkowski sum
    {
        sort_edges();
        polygon.sort_edges();
        vector <Edge> merged_edges;
        merge(edges, polygon.edges, merged_edges);
        return ConvexPolygon(merged_edges, find_left_top() + polygon.find_left_top());
    }
    
    friend bool intesects(ConvexPolygon &polygon1, ConvexPolygon &polygon2)
    {
        polygon2.reverse();
        ConvexPolygon sum_polygon = polygon1 + polygon2;
        return sum_polygon.contains_point(Point2D(0, 0));
    }

    void print()
    {
        for(Point2D p : vertices)
            cout << p.get_x() << ' ' << p.get_y() << endl;
    }
};

int main()
{
    int n, m;
    cin >> n;
    vector <Point2D> vertices1(n);
    for(int i = 0; i < n; i++)
    {
        double x, y;
        cin >> x >> y;
        vertices1[i].set_x(x);
        vertices1[i].set_y(y);
    }
    cin >> m;
    vector <Point2D> vertices2(m);
    for(int i = 0; i < m; i++)
    {
        double x, y;
        cin >> x >> y;
        vertices2[i].set_x(x);
        vertices2[i].set_y(y);
    }

    ConvexPolygon polygon1(vertices1);
    ConvexPolygon polygon2(vertices2);

    if(intesects(polygon1, polygon2))
        cout << "YES\n";
    else cout << "NO\n";
    return 0;
}
