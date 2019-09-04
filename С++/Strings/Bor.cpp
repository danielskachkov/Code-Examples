#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Bor
{
    struct vertex
    {
        vector <int> next = vector <int> (26, -1);
        int suffix_link = 0;
        vector <int> words_end;
    };

    vector <vertex> vertices;

public:
    Bor()
    {
        vertices.push_back(vertex());
    }

    void add_word(string &word, int word_id)
    {
        int v = 0;

        for(int i = 0; i < word.size(); i++)
        {
            char c = word[i];

            if(vertices[v].next[c-'a'] == -1)
            {
                vertices.push_back(vertex());
                vertices[v].next[c-'a'] = vertices.size() - 1;
            }

            v = vertices[v].next[c-'a'];
        }

        vertices[v].words_end.push_back(word_id);
    }

    void count_links()
    {
        queue <int> q;

        for(int v : vertices[0].next)
            if(v != -1)
                q.push(v);

        while(!q.empty())
        {
            int v = q.front();
            q.pop();

            for(int i = 0; i < 26; i++)
                if(vertices[v].next[i] != -1)
                {
                    int u = vertices[v].next[i];
                    q.push(u);

                    int w = vertices[v].suffix_link;

                    while(w > 0 && vertices[w].next[i] == -1)
                        w = vertices[w].suffix_link;

                    if(vertices[w].next[i] != -1)
                        vertices[u].suffix_link = vertices[w].next[i];

                    w = vertices[u].suffix_link;

                    while(w > 0 && vertices[w].words_end.size() == 0)
                        w = vertices[w].suffix_link;

                    for(int word : vertices[w].words_end)
                        vertices[u].words_end.push_back(word);
                }
        }
    }

    void processing(string &word, vector <int> &enterances, vector <pair <int, int> > &parts)
    {
        int v = 0;

        for(int i = 0; i < word.size(); i++)
        {
            int c = word[i] - 'a';

            if(vertices[v].next[c] == -1)
            {
                int u = vertices[v].suffix_link;

                while(u > 0 && vertices[u].next[c] == -1)
                    u = vertices[u].suffix_link;

                if(vertices[u].next[c] != -1)
                    v = vertices[u].next[c];
                else v = 0;
            }
            else
                v = vertices[v].next[c];

            for(int k : vertices[v].words_end)
            {
                if(i - parts[k].second >= 0)
                    enterances[i-parts[k].second] += 1;
            }
        }
    }

    void print()
    {
        for(int i = 0; i < vertices.size(); i++)
        {
            cout << i << ": " << vertices[i].suffix_link << "  ";
            for(int word : vertices[i].words_end)
                cout << word << ' ';
            cout << endl;
        }
    }
};

void split_templ(string &templ, vector <pair <int, int> > &parts)
{
    int beg;

    if(templ[0] != '?')
        beg = 0;

    for(int i = 1; i < templ.size(); i++)
    {
        if(templ[i] == '?' && templ[i-1] != '?')
            parts.push_back(pair<int, int>(beg, i - 1));
        else if(templ[i] != '?' && templ[i-1] == '?')
            beg = i;
    }

    if(templ.back() != '?')
        parts.push_back(pair <int, int> (beg, templ.size() - 1));
}

int main()
{
    string templ, s;
    cin >> templ >> s;
    vector <pair <int, int> > templ_parts;
    split_templ(templ, templ_parts);

    if(templ_parts.size() == 0)
        for(int i = 0; i <= (int)s.size() - (int)templ.size(); i++)
            cout << i << ' ';

    else
    {
        Bor bor;

        for (int i = 0; i < templ_parts.size(); i++)
        {
            string s;
            for (int j = templ_parts[i].first; j <= templ_parts[i].second; j++)
                s.push_back(templ[j]);
            bor.add_word(s, i);
        }

        bor.count_links();

        vector <int> enterances(s.size(), 0);

        bor.processing(s, enterances, templ_parts);

        for(int i = 0; i <= (int)s.size() - (int)templ.size(); i++)
            if(enterances[i] == templ_parts.size())
                cout << i << ' ';
    }

    return 0;
}
