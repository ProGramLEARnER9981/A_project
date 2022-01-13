#pragma once
#include "iostream"  
#include <deque>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <ctime>
#include <set>
using namespace std;

#define SIZE 4
typedef int table[SIZE][SIZE];   // to store the data

typedef struct
{
	int row;
	int col;
}pos;

class State
{
public:
	int zero_row;  // position of zero
	int zero_col;
	double fvalue;
	int depth;  // the depth of state in the tree 
	double distance;   // the sum of the distance of the num out of position
	string str_num;    // str of data, used for open/close map
	State* father;   // previous node

	State(table, int);
	~State();
	int get_distance(unordered_map<int, pos>);   // city block distance
	double evaluate(unordered_map<int, pos>, double);  // return fvalue = depth*alpha + distance
	string get_str_num();  // data => string
	

	State* extend();  // generate a new node

	void move_up();  // move zero
	void move_left();
	void move_down();
	void move_right();
	void show_table();

private:
	table data;
};

struct cmp {
	bool operator()(const State* a, const State* b) const
	{
		if (a->fvalue == b->fvalue)
			return a < b;
		return a->fvalue < b->fvalue;
	}
};

unordered_map<int, pos> get_target_map(table target);   // write a table to a map
deque<State*> A_solution(State*, table, double);    // the main fun
State* process(State*, unordered_map<string, State*>&, set<State*, cmp>&,
	unordered_map<string, State*>&, const unordered_map<int, pos>, double);  // analyse a new extend node
string table2string(table);