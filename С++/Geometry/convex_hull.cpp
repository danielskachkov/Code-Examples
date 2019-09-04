#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <math.h>
using namespace std;

class Point3D
{
    double x, y, z;

public:
    Point3D() {}
    Point3D(double x, double y, double z): x(x), y(y), z(z) {}

    void set_x(double new_x)
    {
        x = new_x;
    }

    void set_y(double new_y)
    {
        y = new_y;
    }

    void set_z(double new_z)
    {
        z = new_z;
    }

    double get_x() const
    {
        return x;
    }

    double get_y() const
    {
        return y;
    }

    double get_z() const
    {
        return z;
    }

    friend double distance_between_points(const Point3D &A, const Point3D &B)
    {
        return sqrt((A.get_x() - B.get_x())*(A.get_x() - B.get_x()) + (A.get_y() - B.get_y())*(A.get_y() - B.get_y()) + (A.get_z() - B.get_z())*(A.get_z() - B.get_z()));
    }
};

struct Vector3D
{
    double x, y, z;

public:
    Vector3D() {}
    Vector3D(Point3D p1, Point3D p2)
    {
        x = p2.get_x() - p1.get_x();
        y = p2.get_y() - p1.get_y();
        z = p2.get_z() - p1.get_z();
    }
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3D operator *(const Vector3D &v) // векторное произведение
    {
        return Vector3D(this->y*v.z - this->z*v.y, -this->x*v.z + this->z*v.x, this->x*v.y - this->y*v.x);
    }

    double length() const
    {
        return sqrt(x*x + y*y + z*z);
    }

    friend double cos_between_vectors(const Vector3D &v1, const Vector3D &v2)
    {
        return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)/(v1.length()*v2.length());
    }
};

int abs_mod(int k, int n)
{
    if(k >= 0)
        return k%n;
    return n - 1;
}

struct Face
{
    vector <int> vertices_id;
    Vector3D normal;

    Face() {}
    Face(int p1, int p2, int p3, int p4, vector <Point3D> &vertices)
    {
        vertices_id.push_back(p1);
        vertices_id.push_back(p2);
        vertices_id.push_back(p3);
        find_normal(p4, vertices);
        int min_id = 0;
        for(int i = 1; i < 3; i++)
            if(vertices_id[i] < vertices_id[min_id])
                min_id = i;

        int r0 = vertices_id[min_id];
        int r1 = vertices_id[(min_id + 1)%3];
        int r2 = vertices_id[abs_mod(min_id - 1, 3)];
        vertices_id.clear();
        vertices_id.push_back(r0);
        Vector3D v1(vertices[r0], vertices[r1]);
        Vector3D v2(vertices[r0], vertices[r2]);

        if(cos_between_vectors(normal, v1*v2) < 0)
        {
            vertices_id.push_back(r2);
            vertices_id.push_back(r1);
        }
        else
        {
            vertices_id.push_back(r1);
            vertices_id.push_back(r2);
        }
    }

    void find_normal(int p, vector <Point3D> &vertices)
    {
        normal = Vector3D(vertices[vertices_id[0]], vertices[vertices_id[1]]) * Vector3D(vertices[vertices_id[0]], vertices[vertices_id[2]]);
        if(cos_between_vectors(normal, Vector3D(vertices[vertices_id[0]], vertices[p])) > 0)
        {
            normal.x *= -1;
            normal.y *= -1;
            normal.z *= -1;
        }
    }

    bool seen_from_Point3D(Point3D p, vector <Point3D> &vertices) const
    {
        Vector3D v(vertices[vertices_id[0]], p);
        if(cos_between_vectors(v, normal) > 0)
        {
            return true;
        }

        return false;
    }

    static bool cmp(const Face &f1, const Face &f2)
    {
        for(int i = 0; i < 3; i++)
        {
            if(f1.vertices_id[i] < f2.vertices_id[i])
                return true;
            if(f1.vertices_id[i] > f2.vertices_id[i])
                return false;
        }
    }

    friend bool operator <(const Face &f1, const Face &f2)
    {
        for(int i = 0; i < 3; i++)
        {
            if(f1.vertices_id[i] < f2.vertices_id[i])
                return true;
            if(f1.vertices_id[i] > f2.vertices_id[i])
                return false;
        }
    }

    bool operator ==(const Face &f2) const
    {
        return (this->vertices_id[0] == f2.vertices_id[0]
        && (this->vertices_id[1] == f2.vertices_id[1] && this->vertices_id[2] == f2.vertices_id[2]
                           || this->vertices_id[1] == f2.vertices_id[2] && this->vertices_id[2] == f2.vertices_id[1]));
    }
};

struct PairHash
{
    size_t operator()(const pair <int, int> &f) const
    {
        return hash <int>()(f.second + f.first);
    }
};

struct PairEqual
{
    bool operator()(const pair<int, int> &lf, const pair <int, int> &rf) const
    {
        return lf == rf;
    }
};

void wrap_gift(vector <Point3D> &vertices, vector <Face> &wrap)
{
    list <Face> faces;

    faces.push_back(Face(0, 1, 2, 3, vertices));
    faces.push_back(Face(0, 1, 3, 2, vertices));
    faces.push_back(Face(0, 3, 2, 1, vertices));
    faces.push_back(Face(3, 1, 2, 0, vertices));

    for(int i = 4; i < vertices.size(); i++)
    {
        unordered_map <pair<int, int>, int, PairHash, PairEqual> faces_to_add;

        for(auto it = faces.begin(); it != faces.end(); it++)
        {
            if(it->seen_from_Point3D(vertices[i], vertices))
            {
                for (int j = 0; j < 3; j++)
                {
                    faces_to_add[{min(it->vertices_id[j], it->vertices_id[(j + 1) % 3]), max(it->vertices_id[j], it->vertices_id[(j + 1) % 3])}]++;
                }
            }
        }

        int faces_count = faces.size();
        int k = 0;

        for(auto it = faces.begin(); k < faces_count; k++)
        {
            if(it->seen_from_Point3D(vertices[i], vertices))
            {
                for(int j = 0; j < 3; j++)
                {
                    if(faces_to_add[{min(it->vertices_id[j], it->vertices_id[(j + 1) % 3]), max(it->vertices_id[j], it->vertices_id[(j + 1) % 3])}] == 1)
                        faces.push_back(Face(it->vertices_id[j], it->vertices_id[(j + 1) % 3], i, it->vertices_id[abs_mod(j - 1, 3)], vertices));
                }

                auto prev = it;
                it++;
                faces.erase(prev);
            }
            else it++;
        }
    }

    for(auto it = faces.begin(); it != faces.end(); it++)
    {
        wrap.push_back(*it);
    }

    sort(wrap.begin(), wrap.end(), Face::cmp);
}

int main()
{
    int m, n;
    cin >> m;
    for(int i = 0; i < m; i++)
    {
        cin >> n;

        vector <Point3D> vertices(n);
        for(int j = 0; j < n; j++)
        {
            double x, y, z;
            cin >> x >> y >> z;
            vertices[j] = {x, y, z};
        }

        vector <Face> wrap;
        wrap_gift(vertices, wrap);
        cout << wrap.size() << endl;
        for(Face &f : wrap)
        {
            cout << 3 << ' ';
            for(int v : f.vertices_id)
                cout << v << ' ';
            cout << endl;
        }
    }

    return 0;
}
