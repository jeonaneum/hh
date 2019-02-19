/*
문제 설명
서울시에 일직선 모양의 새로운 도로가 생겼습니다. 새로운 도로의 전체 길이는 l이고 도로에는 총 n개의 가로등이 세워졌습니다. 이 도로의 모든 가로등에 전구를 사서 달려고 합니다. 전구를 선택하는 기준은 다음과 같습니다.

전구는 길의 좌측, 우측 방향으로 각각 d 길이만큼 길을 밝힐 수 있고, d는 자연수입니다.
모든 가로등에는 같은 종류(d 값이 같은)의 전구를 달아야 합니다.
안전을 위하여 도로위에 어두운 부분이 있어서는 안 됩니다.
이때, d 값이 충분히 크다면 전체 도로를 밝게 비출 수 있지만, d 값이 작아진다면 도로 위에 빛이 닿지 않는 부분이 생길 수도 있습니다. 따라서, 도로 위에 어두운 부분이 생기지 않도록 하는 d 값 중 최솟값을 구하려고 합니다. 전체 도로의 길이 l, 가로등이 세워져 있는 위치가 들어있는 배열 v가 매개변수로 주어질 때, 위의 모든 조건을 만족하는 d 의 최솟값을 return 하도록 solution 함수를 완성해주세요.

제한사항
l은 1 이상 1,000,000,000 이하의 자연수입니다.
v에는 가로등의 위치정보가 들어있습니다.
가로등의 위치는 0 이상 l 이하의 정수이며, 같은 위치에 2개 이상의 가로등이 있는 경우는 주어지지 않습니다.
가로등의 개수는 1이상 1,000 이하의 자연수입니다.
입출력 예
l	v					output
15	[15,5,3,7,9,14,0]	3
5	[2,5]				2

입출력 예 설명
입출력 예 #1
d가 3보다 작은 경우(아래 그림의 경우 d = 2) 다음과 같이 밝히지 못하는 부분이 생기게 됩니다.
따라서 모든 길을 밝게 비추기 위한 d의 최솟값은 3이 됩니다.

입출력 예 #2
d가 2보다 작은 경우(아래 그림의 경우 d = 1) 다음과 같이 2 위치에 있는 가로등이 길의 시작 부분을 밝히지 못하게 됩니다.
따라서 모든 길을 밝게 비추기 위한 d의 최솟값은 2가 됩니다.
*/

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int solution(int l, vector<int> v) {
    int answer = 0;
    int diffmax = 0;
    
    sort(v.begin(), v.end());
    
    for(int i = 0; i < v.size() - 1; i++){
        if(v[i+1] - v[i] > diffmax)    diffmax = v[i+1] - v[i];    
    }
    
    answer = (diffmax + 1) / 2;
    
    if(v[0] > answer){
        answer = v[0];
    } 
    
    if(l - v[v.size()-1] > answer){
        answer = l - v[v.size()-1];
    } 
    
    cout << answer << endl;

    return answer;
}

/*
▼ 정답 예시(소팅 & 그리디) - O( nlogn )
def solution(l, v):
    v.sort()
    ans = max(v[0], l - v[-1])
    for i in range(1, len(v)):
        ans = max(ans, (v[i] - v[i-1] + 1)//2)
    return ans
	
	
▼ 정답 예시(이분탐색) - O( nlogn )
def solution(l, v):    
    n = len(v)
    answer = l
    v.sort()

    left, right = 0, l
    while(left <= right) :
        mid = (left + right) // 2

        # 맨 앞 가로등과 맨 뒤 가로등이 도로의 양 끝을 밝히는지 확인
        if v[0] - mid > 0 or v[n-1] + mid < l :
            left = mid + 1
            continue

        # 나머지 가로등으로 이분 탐색
        flag = False
        for i in range(1, n) :
            if v[i-1] + mid < v[i] - mid :
                flag = True
                break
        if flag :
            left = mid + 1
        else :
            answer = mid 
            right = mid - 1
    return answer
*/