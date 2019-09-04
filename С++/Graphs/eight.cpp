#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

struct VectorHash
{
    size_t operator()(const vector <vector <short> > &v) const
    {
        hash<short> hasher;
        size_t seed = 0;
        for(vector <short> w : v)
            for(short i : w)
                seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        return seed;
    }
};

struct State
{
    vector <vector <short> > board;
    short null_x;
    short null_y;
    string way;
    
    State() {}
    
    State(int N)
    {
        board = vector <vector <short> > (N, vector <short> (N));
    }
    
    State(vector <vector <short> > &board) : board(board) {}
    
    void set_null_pos(short x, short y)
    {
        null_x = x;
        null_y = y;
    }
    
    bool visited(unordered_set <vector <vector <short> >, VectorHash > &visited_states)
    {
        return visited_states.find(board) != visited_states.end();
    }
    
    State move(const char dir)
    {
        State new_state(board);
        
        if(dir == 'u')
        {
            new_state.way = way + 'U';
            new_state.set_null_pos(null_x, null_y - 1);
            swap(new_state.board[null_y][null_x], new_state.board[null_y-1][null_x]);
        }
        else if(dir == 'd')
        {
            new_state.way = way + 'D';
            new_state.set_null_pos(null_x, null_y + 1);
            swap(new_state.board[null_y][null_x], new_state.board[null_y+1][null_x]);
        }
        else if(dir == 'l')
        {
            new_state.way = way + 'L';
            new_state.set_null_pos(null_x - 1, null_y);
            swap(new_state.board[null_y][null_x], new_state.board[null_y][null_x-1]);
        }
        else if(dir == 'r')
        {
            new_state.way = way + 'R';
            new_state.set_null_pos(null_x + 1, null_y);
            swap(new_state.board[null_y][null_x], new_state.board[null_y][null_x+1]);
        }
        
        return new_state;
    }
    
    bool end_position()
    {
        int N = board.size();
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                if(i != N - 1 || j != N - 1)
                    if(board[i][j] != i*N + j + 1)
                    return false;
        return true;
    }
};

void move_null(State &cur_state, queue <State> &states_to_visit, unordered_set <vector <vector <short> >, VectorHash > &visited_states, const int N)
{
    State new_state;
    
    if(cur_state.null_y < N - 1)
    {
        new_state = cur_state.move('d');
        if(!new_state.visited(visited_states))
        {
            visited_states.insert(new_state.board);
            states_to_visit.push(new_state);
        }
    }
    
    if(cur_state.null_y > 0)
    {
        new_state = cur_state.move('u');
        if(!new_state.visited(visited_states))
        {
            visited_states.insert(new_state.board);
            states_to_visit.push(new_state);
        }
    }
    
    if(cur_state.null_x < N - 1)
    {
        new_state = cur_state.move('r');
        if(!new_state.visited(visited_states))
        {
            visited_states.insert(new_state.board);
            states_to_visit.push(new_state);
        }
    }
    
    if(cur_state.null_x > 0)
    {
        new_state = cur_state.move('l');
        if(!new_state.visited(visited_states))
        {
            visited_states.insert(new_state.board);
            states_to_visit.push(new_state);
        }
    }
}


int main()
{
    const int N = 3;
    queue <State> states_to_visit;
    unordered_set <vector <vector <short> >, VectorHash > visited_states;
    State cur_state(N);
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            cin >> cur_state.board[i][j];
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            if(cur_state.board[i][j] == 0)
            {
                cur_state.null_y = i;
                cur_state.null_x = j;
            }
    
    cur_state.way = "";
    states_to_visit.push(cur_state);
    visited_states.insert(cur_state.board);
    
    
    while(!states_to_visit.empty())
    {
        cur_state = states_to_visit.front();
        states_to_visit.pop();
        if(cur_state.end_position())
            break;
        move_null(cur_state, states_to_visit, visited_states, N);
    }
    
    if(cur_state.end_position())
    {
        cout << cur_state.way.length() << endl;
        cout << cur_state.way << endl;
    }
    else cout << -1 << endl;
    
    return 0;
}
