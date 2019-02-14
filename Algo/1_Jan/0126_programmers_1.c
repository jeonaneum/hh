/*배열의 회전이란 모든 원소를 오른쪽으로 한 칸씩 이동시키고, 마지막 원소는 배열의 맨 앞에 넣는 것을 말합니다.
[4, 3, 2, 1]
[1, 4, 3, 2]
[2, 1, 4, 3]
[3, 2, 1, 4]

두 배열 arrA와 arrB가 매개변수로 주어질 때, arrA를 회전해 arrB로 만들 수 있으면 true를, 
그렇지 않으면 false를 return 하는 solution 함수를 작성해주세요.

제한 조건
arrA는 길이가 1 이상 1,500 이하인 배열입니다.
arrA의 원소는 0 이상 1,500 이하인 정수입니다.
arrB는 길이가 1 이상 1,500 이하인 배열입니다
arrB의 원소는 0 이상 1,500 이하인 정수입니다.
입출력 예
arrA	arrB	return
[7, 8, 10]	[10, 7, 8]	true
[4, 3, 2, 1]	[5, 4, 1, 2]	false
입출력 예 설명
예시 #1
arrA 배열을 한 번 회전하면 arrB와 같아집니다.

예시 #2
arrA 배열은 아무리 회전해도 arrB와 같지 않습니다.*/

#include <string>
#include <vector>

using namespace std;

bool solution(vector<int> arrA, vector<int> arrB) {
    bool answer = false;
    int cnt = 0;
    
    while(!answer){
        for(int i = 0; i < arrA.size(); i++){
            if(arrA[i] == arrB[i]){
                answer = true;
            } 
            else {
                answer = false;
                break;
            }
        }
        
        if(cnt == arrA.size())  break;
        cnt++;
        
        int tmp = 0;
        for(int i = 0; i < arrA.size() - 1; i++){
            tmp = arrA[i];
            arrA[i] = arrA[i + 1];
            arrA[i + 1] = tmp;
        }
    }
    
    return answer;
}

/*
def rotate(arr):
    return arr[-1:] + arr[:-1]

def solution(arrA, arrB):
    arrA_len = len(arrA)
    arrB_len = len(arrB)

    # 길이가 다른 배열은 회전해도 같아질 수 없으므로, early return
    if(arrA_len!=arrB_len):
        return False

    # arrA를 한칸씩 회전하며, arrB와 같은지 확인
    for _ in range(arrA_len):
        if arrA == arrB:
            return True
        arrA = rotate(arrA)
    return False
*/