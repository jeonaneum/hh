
#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <string>

using namespace std;

typedef struct qnode
{
	int command, n, level;
	string list;
	qnode(int command, int n, int level, string &list):command(command), n(n), level(level), list(list){}
}qnode;

void rotation(int &n, bool dir)
{
	deque<int> dq;
	for(int i=0; i<4; i++){
		dq.push_front(n % 10);
		n /= 10;
	}
	if(dir){
		n = dq.back();	dq.pop_back();
		dq.push_front(n);
	}
	else{
		n = dq.front();	dq.pop_front();
		dq.push_back(n);
	}
	n = 0;
	for(int i=3, weight = 1; weight < 10000; weight *= 10, i--){
		n += dq[i]*weight;
	}
}

int execute(int command, int n)
{
	if(command == 0){
		n *= 2;
		if(n > 9999){
			n %= 10000;
		}
	}
	else if(command == 1){
		n--;
		if(n < 0){
			n = 9999;
		}
	}
	else if(command == 2){
		rotation(n, false);
	}
	else if(command == 3){
		rotation(n, true);
	}
	return n;
}

void addList(int command, string &list)
{
	if(command == 0){
		list += 'D';
	}
	else if(command == 1){
		list += 'S';
	}
	else if(command == 2){
		list += 'L';
	}
	else if(command == 3){
		list += 'R';
	}
}

void bfs(int a, int b)
{
	queue<qnode> q;
	for(int i=0; i<4; i++){
		string list;
		addList(i, list);
		q.push(qnode(i, a, 1, list));
	}
	while(1){
		int command = q.front().command, n = q.front().n, level = q.front().level;
		string list = q.front().list;	q.pop();
		int next = execute(command, n);
		if(next == b){
			//addList(command, list);
			cout << list << '\n';
			break;
		}
		for(int i=0; i<4; i++){
			string temp = list;
			addList(i, temp);
			q.push(qnode(i, next, level+1, temp));
		}
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--){
		int a, b;
		cin >> a >> b;
		bfs(a, b);
	}	
	return 0;
}
