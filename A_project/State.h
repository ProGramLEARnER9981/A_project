#pragma once
#include "iostream"  
#include <set>
#include <deque>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#define SIZE 4
using namespace std;
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
	State* father;   // previous node

	State(table, int);
	~State();
	int get_distance(unordered_map<int, pos>);
	double evaluate(unordered_map<int, pos>);
	bool equal_to(table);
	bool equal_to(State*);
	void show_table();

	State* extend();

	void move_up();  // move zero
	void move_left();
	void move_down();
	void move_right();

private:
	table data;
};


struct cmp {
	bool operator()(const State* a, const State* b) {
		return a->fvalue < b->fvalue;
	}
};

unordered_map<int, pos> get_target_map(table target);   // write a table to a map
bool comp(const State*, const State*);
bool node_find(State*, deque<State*>);
deque<State*> A_solution(State*, table);
State* process(State*, deque<State*>&, deque<State*>&, const unordered_map<int, pos>);
