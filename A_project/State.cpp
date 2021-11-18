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
		cout<< "No zero!";
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
	fvalue = distance + depth*1.1;
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
	get_str_num();
}

void State::move_down()
{
	data[zero_row][zero_col] = data[zero_row + 1][zero_col];
	data[zero_row + 1][zero_col] = 0;
	++zero_row;
	get_str_num();
}

void State::move_left()
{
	data[zero_row][zero_col] = data[zero_row][zero_col - 1];
	data[zero_row][zero_col - 1] = 0;
	--zero_col;
	get_str_num();
}

void State::move_right()
{
	data[zero_row][zero_col] = data[zero_row][zero_col + 1];
	data[zero_row][zero_col + 1] = 0;
	++zero_col;
	get_str_num();
}

string State::get_str_num()
{
	str_num =  table2string(this->data);
	return str_num;
}

string table2string(table a)
{
	string s;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j)
			s += to_string(a[i][j]) + "-";
	}
	return s;
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

State* process(State* new_node, unordered_map<string, State*>& open_map, multiset<State*, cmp>& open_set,
	unordered_map<string, State*>& close_map, const unordered_map<int, pos> target_map)
{
	State* final_node = 0;
	unordered_map<string, State*>::iterator it_open = open_map.find(new_node->str_num),
											it_close = close_map.find(new_node->str_num);
	if (it_open == open_map.end() && it_close == close_map.end()) {
		new_node->evaluate(target_map);
		if (new_node->distance == 0) {
			final_node = new_node;
		}
		open_map.insert(pair<string, State*>(new_node->str_num, new_node));
		open_set.insert(new_node);
	}
	return final_node;
}

deque<State*> A_solution(State* startnode, table target)
{
	State* final_node = 0;   // its data is target {1,2,3...15,0}
	deque<State*> solution;  // the node chain lead to final_node
	multiset<State*, cmp> open_set;  // it's sorted by fvalue
	unordered_map<int, pos> target_map = get_target_map(target);
	unordered_map<string, State*> open_map, close_map;
	int node_num = 0;  // the totle num of node <==> the step to the final solution
	int loop_num = 0;  // the loop num of while

	clock_t start1, end1;
	start1 = clock();

	startnode->evaluate(target_map);
	open_set.insert(startnode);
	open_map.insert(pair<string, State*>(startnode->get_str_num(), startnode));

	while (open_set.size() && loop_num < 1000000) {

		State* node = *open_set.begin();
		open_set.erase(open_set.begin());
		
		unordered_map<string, State*>::iterator node_it = open_map.find(node->str_num);
		open_map.erase(node_it);
		
		if (loop_num++ % 500 == 0)
			cout << loop_num << ": " << node->fvalue << " " 
				<< node->distance<< " "<< node->depth << endl;
		
		if (node->zero_row > 0) {
			State* new_node = node->extend();
			new_node->move_up();
			if (final_node = process(new_node, open_map, open_set, close_map, target_map))
				break;
			else
				++node_num;
		}
		if (node->zero_row < SIZE - 1) {
			State* new_node = node->extend();
			new_node->move_down();
			if (final_node = process(new_node, open_map, open_set, close_map, target_map))
				break;
			else
				++node_num;
		}
		if (node->zero_col > 0) {
			State* new_node = node->extend();
			new_node->move_left();
			if (final_node = process(new_node, open_map, open_set, close_map, target_map))
				break;
			else
				++node_num;
		}
		if (node->zero_col < SIZE - 1) {
			State* new_node = node->extend();
			new_node->move_right();
			if (final_node = process(new_node, open_map, open_set, close_map, target_map))
				break;
			else
				++node_num;
		}
		close_map.insert(pair<string, State*>(node->str_num, node));
	}
	end1 = clock();
	cout << endl << "path: " << endl;
	while (final_node) {
		solution.push_front(final_node);
		final_node = final_node->father;
	}
	for (auto i = solution.begin(); i != solution.end(); ++i) {
		(*i)->show_table();
	}

	std::cout << "node num: " << node_num << ", loop_num: " << loop_num << endl;
	cout << "total time: " << (double)(end1 - start1) / CLOCKS_PER_SEC << endl;
	return solution;
}
