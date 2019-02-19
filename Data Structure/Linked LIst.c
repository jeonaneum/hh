struct NODE {
	int prev, val, next;
};

const int NODE_SIZE = 30000;

struct MY_LIST {
	int HEAD = NODE_SIZE;
	int TAIL = NODE_SIZE + 1;
	int pos;
	NODE node[NODE_SIZE + 2];

	MY_LIST() {
		pos = 0;
		node[HEAD].next = TAIL;
		node[TAIL].prev = HEAD;
	}

	void push_back(int data) {
		int prev = node[TAIL].prev;
		int next = node[prev].next;	/// TAIL

		node[pos].val = data;

		node[pos].prev = prev;
		node[prev].next = pos;

		node[pos].next = next;
		node[next].prev = pos;
		++pos;
	}

	void push_front(int data) {
		int next = node[HEAD].next;	
		int prev = node[next].prev;	/// HEAD

		node[pos].val = data;

		node[pos].prev = prev;
		node[prev].next = pos;

		node[pos].next = next;
		node[next].prev = pos;
		++pos;
	}

	void insert(int p, int data) {
		int next = node[HEAD].next;
		for (int i = 0; i < p; ++i) {
			next = node[next].next;
		}
		int prev = node[next].prev;

		node[pos].val = data;

		node[pos].prev = prev;
		node[prev].next = pos;

		node[pos].next = next;
		node[next].prev = pos;
		++pos;
	}

	int pop_back() {
		int target = node[TAIL].prev;
		
		int prev = node[target].prev;
		int next = node[target].next;

		node[prev].next = next;
		node[next].prev = prev;

		return node[target].val;
	}

	int pop_front() {
		int target = node[HEAD].next;

		int prev = node[target].prev;
		int next = node[target].next;

		node[prev].next = next;
		node[next].prev = prev;

		return node[target].val;
	}

	void erase(int p) {
		int target = node[HEAD].next;
		for (int i = 0; i < p; ++i) {
			target = node[target].next;
		}
		int prev = node[target].prev;
		int next = node[target].next;

		node[prev].next = next;
		node[next].prev = prev;
	}
};