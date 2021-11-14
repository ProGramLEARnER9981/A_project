#include "State.h"

State::State(table start_table, int d)
	:depth(d), fvalue(-1), distance(-1), zero_col(-1), zero_row(-1), father(0)
{
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j)
		{
			data[i][j] = start_table[i][j];
			if (!start_table[i][j]) {
				zero_row = i;
				zero_col = j;
			}
		}
	}
	if (zero_col < 0 || zero_row < 0)
		throw "No zero!";
}

State::~State()
{
	delete[] data;
}

int State::get_distance(unordered_map<int, pos> target_map)
{
	distance = 0;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			unordered_map<int, pos>::iterator it = target_map.find(data[i][j]);
			distance += abs(it->second.row - i) + abs(it->second.col - j);
		}
	}
	return distance;
}

void State::show_table()
{
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			cout << data[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

double State::evaluate(unordered_map<int, pos> target_map)
{
	if (distance < 0)
		get_distance(target_map);
	fvalue = distance + depth*0.5;
	//cout << depth << " " << distance<<" "<<fvalue << endl;
	return fvalue;
}

State* State::extend()
{
	State* child = new State(data, depth + 1);
	child->father = this;
	return child;
}

void State::move_up()
{
	data[zero_row][zero_col] = data[zero_row - 1][zero_col];
	data[zero_row - 1][zero_col] = 0;
	--zero_row;
}
void State::move_down()
{
	data[zero_row][zero_col] = data[zero_row + 1][zero_col];
	data[zero_row + 1][zero_col] = 0;
	++zero_row;
}
void State::move_left()
{
	data[zero_row][zero_col] = data[zero_row][zero_col - 1];
	data[zero_row][zero_col - 1] = 0;
	--zero_col;
}
void State::move_right()
{
	data[zero_row][zero_col] = data[zero_row][zero_col + 1];
	data[zero_row][zero_col + 1] = 0;
	++zero_col;
}


bool State::equal_to(table t)
{
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (data[i][j] != t[i][j])
				return false;
		}
	}
	return true;
}

bool State::equal_to(State* node)
{
	if (zero_row != node->zero_row || zero_col != node->zero_col)
		return false;
	return node->equal_to(data);
}



unordered_map<int, pos> get_target_map(table target)
{
	unordered_map<int, pos> target_map;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			pos p{ i, j };
			target_map.insert(pair<int, pos>(target[i][j], p));
		}
	}
	return target_map;
}

bool comp(const State* node0, const State* node1)
{
	return node0->fvalue < node1->fvalue;
}

bool node_find(State* node, deque<State*> node_list)
{
	deque<State*>::iterator it = node_list.begin();
	while (it != node_list.end()) {
		if ((*it)->equal_to(node))
			return true;
		++it;
	}
	return false;
}

State* process(State* new_node, deque<State*> &open, deque<State*> &close, const unordered_map<int, pos> target_map)
{
	State* final_node = 0;
	if (!node_find(new_node, open) && !node_find(new_node, close)) {
		new_node->evaluate(target_map);
		if (new_node->distance == 0) {
			final_node = new_node;
		}
		open.push_back(new_node);
	}
	return final_node;
}


deque<State*> A_solution(State* startnode, table target)
{
	deque<State*> open, close, solution;
	State* final_node = 0;
	unordered_map<int, pos> target_map = get_target_map(target);
	int node_num = 0;  // the totle num of node <==> the step to the final solution
	int loop_num = 0;
	startnode->evaluate(target_map);
	open.push_back(startnode);
	while (open.size() && loop_num < 1000000) {
		sort(open.begin(), open.end(), comp);  // O(n)
		State* node = open.front();
		if (loop_num++ % 100 == 0)
			cout << loop_num << ": " << node->fvalue << " " << node->distance << endl;
		open.pop_front();

		if (node->zero_row > 0) {
			State* new_node = node->extend();
			new_node->move_up();
			if (final_node = process(new_node, open, close, target_map))
				break;
			else
				++node_num;
		}
		if (node->zero_row < SIZE - 1) {
			State* new_node = node->extend();
			new_node->move_down();
			if (final_node = process(new_node, open, close, target_map))
				break;
			else
				++node_num;
		}
		if (node->zero_col > 0) {
			State* new_node = node->extend();
			new_node->move_left();
			if (final_node = process(new_node, open, close, target_map))
				break;
			else
				++node_num;
		}
		if (node->zero_col < SIZE - 1) {
			State* new_node = node->extend();
			new_node->move_right();
			if (final_node = process(new_node, open, close, target_map))
				break;
			else
				++node_num;
		}
		close.push_back(node);
	}
	cout << endl << "path: " << endl;
	while (final_node) {
		final_node->show_table();
		solution.push_front(final_node);
		final_node = final_node->father;
	}

	std::cout << "node num: " << node_num << ", loop_num: "<< loop_num << endl;
	return solution;
}

