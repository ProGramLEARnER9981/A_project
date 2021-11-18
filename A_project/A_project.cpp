#include "State.h"
using namespace std;


int main()
{
	table target = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
	table start = { 11,9,4,15,1,3,0,12,7,5,8,6,13,2,10,14 };
	
	State* node0 = new State(start, 0);

	deque<State*> solution = A_solution(node0, target);
	cout << solution.size() << endl;

	//unordered_map<int, pos> target_map = get_target_map(target);
	//cout << node0->evaluate(target_map) << endl;

	return 0;
}

