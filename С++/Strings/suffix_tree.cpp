#include <iostream>
#include <vector>
using namespace std;

const int alphabet_size = 28;  // a, b, ..., z + 2 additional symbols

class SuffixTree
{
    struct vertex
    {
        int parent, depth, left = -1, right = -1, index = -1, leaf; // leaf = id of the suffix, which ends at the current leaf (-1 if not a leaf)
        vector <int> edges_out;

        vertex() {}
        vertex(int parent, int depth, int leaf) : parent(parent), depth(depth), leaf(leaf) {}

        void add_edge(int vertex)
        {
            edges_out.push_back(vertex);
        }
    };

    vector <vertex> vertices;

    inline int depth(int vertex)
    {
        return vertex >= 0 ? vertices[vertex].depth : 0;
    }

    inline int parent(int vertex)
    {
        return vertices[vertex].parent;
    }

    inline int left(int vertex)
    {
        return vertices[vertex].left;
    }

    inline int right(int vertex)
    {
        return vertices[vertex].right;
    }

    inline int index(int vertex)
    {
        return vertices[vertex].index;
    }

    inline int leaf(int vertex)
    {
        return vertices[vertex].leaf;
    }

    void dfs(int current_vertex, const vector <int> &suffix_array, int &id)
    {
        int n = suffix_array.size();

        vertices[current_vertex].index = id;
        id++;

        if(leaf(current_vertex) != -1)
        {
            vertices[current_vertex].right = n - 1;
            vertices[current_vertex].left = suffix_array[leaf(current_vertex)] + depth(parent(current_vertex));
        }
        else
        {
            for(int child : vertices[current_vertex].edges_out)
            {
                dfs(child, suffix_array, id);
                vertices[current_vertex].left = (left(current_vertex) == -1  ? left(child) - (depth(current_vertex) - depth(parent(current_vertex))) : 
                                        min(left(current_vertex), left(child) - (depth(current_vertex) - depth(parent(current_vertex)))));
            }
            
            vertices[current_vertex].right = left(current_vertex) + depth(current_vertex) - depth(parent(current_vertex)) - 1;
        }
    }

    void create_new_leaf(int &current_vertex, int i, const vector <int> &suffix_array, int s_size)
    {
        vertices.push_back(vertex(current_vertex, s_size - suffix_array[i], i));
        vertices[current_vertex].add_edge(vertices.size() - 1);
        current_vertex = vertices.size()  - 1;
    }

public:
    SuffixTree(const string &s, const vector <int> &suffix_array, const vector <int> &lcp)
    {
        vertices.push_back(vertex(-1, 0, -1)); // root
        vertices.push_back(vertex(0, 1, 0));
        vertices[0].add_edge(1);

        int current_vertex = 1;

        for(int i = 1; i < suffix_array.size(); i++)
        {
            if(lcp[i - 1] == 0 || lcp[i - 1] == depth(current_vertex))
            {
                if(lcp[i - 1] == 0)
                {
                    current_vertex = 0;
                }

                create_new_leaf(current_vertex, i, suffix_array, s.size());
            }
            else
            {
                while(depth(current_vertex) > lcp[i - 1])
                {
                    current_vertex = parent(current_vertex);
                }

                if(depth(current_vertex) == lcp[i - 1])
                {
                    create_new_leaf(current_vertex, i, suffix_array, s.size());
                }
                else
                {
                    vertices.push_back(vertex(current_vertex, depth(current_vertex) + lcp[i - 1] - depth(current_vertex), -1));
                    vertices.back().edges_out.push_back(vertices[current_vertex].edges_out.back());
                    vertices[vertices[current_vertex].edges_out.back()].parent = vertices.size() - 1;
                    vertices[current_vertex].edges_out.pop_back();
                    vertices[current_vertex].add_edge(vertices.size() - 1);
                    current_vertex = vertices.size() - 1;
                    create_new_leaf(current_vertex, i, suffix_array, s.size());
                }
            }
        }

        int id = 0;
        dfs(0, suffix_array, id);
    }

    int size()
    {
        return vertices.size();
    }

    void output_for_printing(vector <vector <int> > &output, int s1_length)
    {
        for(int i = 1; i < vertices.size(); i++)
        {
            int word = left(i) < s1_length ? 0 : 1;
            int l = (word == 0) ? left(i) : left(i) - s1_length;
            int r = (word == 1) ? right(i) - s1_length + 1 : min(right(i) + 1, s1_length);

            output[index(i) - 1] = {index(parent(i)), word, l, r};
        }
    }
};

void sort_pairs(int length, vector <int> &order, const vector <int> &classes, const string &s, bool part) // part: 0 - sort by first, 1 - sort by second
{
    vector <int> new_order(order.size());
    vector <int> counter(s.size(), 0);

    for(int i = 0; i < s.size(); i++)
    {
        if(part == 0)
        {
            counter[classes[(i + length) % s.size()]]++;
        }
        else
        {
            counter[classes[order[i]]]++;
        }
    }

    int carry = 0;

    for(int i = 0; i < counter.size(); i++)
    {
        int temp = counter[i];
        counter[i] = carry;
        carry += temp;
    }

    for(int i = 0; i < new_order.size(); i++)
    {
        if(part == 0)
        {
            new_order[counter[classes[(i + length) % s.size()]]] = i;
            counter[classes[(i + length) % s.size()]]++;
        }
        else
        {
            new_order[counter[classes[order[i]]]] = order[i];
            counter[classes[order[i]]]++;
        }
    }

    order = new_order;
}

void sort_characters(vector <int> &order, const string &s)
{
    vector <int> counter(alphabet_size, 0);

    for(char c : s)
    {
        int char_index = c - 'a' + 2;
        counter[char_index]++;
    }

    int carry = 0;

    for(int i = 0; i < alphabet_size; i++)
    {
        int temp = counter[i];
        counter[i] = carry;
        carry += temp;
    }

    for(int i = 0; i < s.size(); i++)
    {
        int char_index = s[i] - 'a' + 2;
        order[counter[char_index]] = i;
        counter[char_index]++;
    }
}

void initialize_classes(vector <int> &classes, const string &s, const vector <int> &order)
{
    classes[order[0]] = 0;

    for(int i = 1; i < s.size(); i++)
    {
        int class_index = (s[order[i - 1]] == s[order[i]]) ? classes[order[i - 1]] : (classes[order[i - 1]] + 1);
        classes[order[i]] = class_index;
    }
}

void sort_doubled(int length, vector <int> &order, const vector <int> &classes, const string &s)
{
    sort_pairs(length, order, classes, s, 0);
    sort_pairs(length, order, classes, s, 1);
}

void update_classes(int length, vector <int> &classes, const vector <int> &order)
{
    vector <int> new_classes(classes.size());
    new_classes[order[0]] = 0;

    for(int i = 1; i < order.size(); i++)
    {
        if((classes[order[i]] == classes[order[i - 1]]) &&
           (classes[(order[i] + length) % order.size()] == classes[(order[i - 1] + length) % order.size()]))
        {
            new_classes[order[i]] = new_classes[order[i - 1]];
        }
        else
        {
            new_classes[order[i]] = new_classes[order[i - 1]] + 1;
        }
    }

    classes = new_classes;
}

void build_suffix_array(const string &s, vector <int> &suffix_array)
{
    if(suffix_array.size() != s.size())
    {
        suffix_array = vector <int> (s.size());
    }

    sort_characters(suffix_array, s);

    vector <int> classes(s.size());
    initialize_classes(classes, s, suffix_array);

    for(int length = 1; length < s.size(); length *= 2)
    {
        sort_doubled(length, suffix_array, classes, s);
        update_classes(length, classes, suffix_array);
    }
}

void build_lcp(const string &s, const vector <int> &suffix_array, vector <int> &lcp)
{
    if(lcp.size() != suffix_array.size())
    {
        lcp = vector <int> (suffix_array.size());
    }

    vector <int> suffix_at_position(suffix_array.size());

    for(int i = 0; i < s.size(); i++)
    {
        suffix_at_position[suffix_array[i]] = i;
    }

    int common_letters = 0;

    for(int i = 0; i < s.size(); i++)
    {
        if(common_letters > 0)
        {
            common_letters--;
        }

        if(suffix_at_position[i] == s.size() - 1)
        {
            lcp[s.size() - 1] = -1;
            common_letters = 0;
        }
        else
        {
            int next_suffix_start_position = suffix_array[suffix_at_position[i] + 1];

            while((max(i + common_letters, next_suffix_start_position + common_letters) < s.size()) &&
                  (s[i + common_letters] == s[next_suffix_start_position + common_letters]))
            {
                common_letters++;
            }

            lcp[suffix_at_position[i]] = common_letters;
        }
    }
}

void build_suffix_tree_for_two_strings(string s1, string s2, vector <vector <int> > &answer)
{
    s1.pop_back();
    s1.push_back('`');
    s2.pop_back();
    s2.push_back('_');
    string s = s1 + s2;

    vector<int> suffix_array;
    vector<int> lcp;

    build_suffix_array(s, suffix_array);
    build_lcp(s, suffix_array, lcp);

    SuffixTree suffix_tree(s, suffix_array, lcp);

    cout << suffix_tree.size() << endl;

    answer = vector <vector<int> > (suffix_tree.size() - 1);
    suffix_tree.output_for_printing(answer, s1.size());
}

int main()
{
    string s1, s2;
    cin >> s1 >> s2;

    vector <vector <int> > answer;
    build_suffix_tree_for_two_strings(s1, s2, answer);

    for(vector <int> &line : answer)
    {
        for (int i : line)
        {
            cout << i << ' ';
        }
        cout << '\n';
    }

    return 0;
}
