#include <stdio.h>
#include <algorithm>
#include <vector>
using namespace std;

int n, l;

int Caltime(int pos, int dir) 
{
	return abs(pos - ((dir > 0) ? l : 0));
}


int main()
{
	vector<pair<int, int>> ant;
	ant.clear();

	scanf("%d %d", &n, &l);
	for (int i = 0; i < n; i++) {
		int start, tmp;
		scanf("%d", &start);
		tmp = abs(start);
		ant.push_back(make_pair(tmp, (start / tmp)));
	}
	
	int time = 0;
	int antno = 0;
	int left = 0, right = ant.size() - 1;
	sort(ant.begin(), ant.end());

	while (right - left > 1) { /// 개미가 2마리 또는 1마리가 남을 때까지
		for (int i = left; i <= right; i++) {
			///가장 바깥 개미의 방향 체크
			int dir = ant[i].second;
			if (i == left && dir == -1) {	/// 가장 왼쪽	 개미 방향이 '<-' 이면 다음 개미가 가장 왼쪽 개미
				left++;
				continue;
			}
			else if (i == right && dir == 1) {	/// 가장 오른쪽 개미 방향이 '->' 이면 다음 개미가 가장 오른쪽 개미
				right--;
				continue;
			}

			int next = i + dir;
			if (ant[i].first == ant[next].first) {	
				if (ant[i].second > ant[next].second) {	/// 같은 위치의 두 개미 방향이 엇갈렸는지 확인
					ant[i].second *= -1;
					ant[next].second *= -1;		///	엇갈리면 방향 변경 아니면 그대로
				}
			}

			int np = ant[i].first + dir;
			if (np == ant[next].first) {	/// 갈 방향 바로 옆에 개미 확인
				if (dir == ant[next].second) {	/// 방향 같으면 이동
					ant[i].first += dir;
				}
				else {						/// 다르면 방향 반대
					ant[i].second *= -1;
				}
			}
			else {							/// 갈 방향 바로 옆 개미 없으면 이동
				ant[i].first += dir;
			}
		}
		time++;
	}

	if (left == right) {	///	개미 1마리인 경우
		antno = left;
		time += Caltime(ant[antno].first, ant[antno].second);
	}
	else {	/// 개미 2마리인 경우
		int time1 = Caltime(ant[left].first, ant[left].second);
		int time2 = Caltime(ant[right].first, ant[right].second);

		if (time1 > time2) {
			time += time1;
			antno = left;
		}
		else {
			time += time2;
			antno = right;
		}
	}
	printf("%d %d\n", antno, time);
}
