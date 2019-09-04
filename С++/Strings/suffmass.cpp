#include <iostream>
#include <vector>
using namespace std;

const int alphabet_size = 27;  // a, b, ..., z + additional symbol

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
        int char_index = c >= 'a' ? (c - 'a' + 1) : 0;
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
        int char_index = s[i] >= 'a' ? (s[i] - 'a' + 1) : 0;
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

void build_suffix_array(const string &str, vector <int> &suffix_array)
{
    string s = str + '#';

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

    suffix_array.erase(suffix_array.begin());
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

long long different_substrings(const string &s)
{
    vector <int> suffix_array(s.size());
    build_suffix_array(s, suffix_array);

    vector <int> lcp(s.size());
    build_lcp(s, suffix_array, lcp);

    long long answer = (s.size()*(s.size() + 1))/2;

    for(int i = 0; i < s.size() - 1; i++)
    {
        answer -= lcp[i];
    }

    return answer;
}

int main()
{
    string s;
    cin >> s;
    cout << different_substrings(s);

    return 0;
}
