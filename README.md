# A_project
A*算法解决十五数码问题

使用如下
~~~cpp
table target = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
table start = { 11,9,4,15,1,3,0,12,7,5,8,6,13,2,10,14 };
	
State* node0 = new State(start, 0);

deque<State*> solution = A_solution(node0, target, 0.5);
~~~
