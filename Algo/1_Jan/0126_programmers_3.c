/*
문제 설명
빙고는 NxN 크기의 게임 보드 칸에 1부터 NxN까지의 자연수를 중복 없이 하나씩 적은 후 숫자를 하나씩 지워나가는 게임입니다. 이때, 가로, 세로, 대각선 방향으로 한 줄에 적힌 숫자를 모두 지울 경우 빙고를 1개 만들었다고 합니다.
다음은 4X4 크기의 게임 보드를 이용해 게임을 진행한 예시입니다.

위와 같이 각 칸에 숫자가 적혀 있을 때, 위 게임 보드에서 순서대로 지운 숫자가 [14,3,2,4,13,1,16,11,5,15]인 경우 아래와 같이 빙고 3개가 만들어집니다.
빙고 게임 보드에 적힌 숫자가 담겨있는 배열 board, 게임 보드에서 순서대로 지운 숫자가 들어있는 배열 nums가 매개변수로 주어질 때, 
board에서 nums에 들어있는 숫자를 모두 지우면 몇 개의 빙고가 만들어지는지 return하도록 solution함수를 완성해주세요.

제한사항
board는 게임 보드 칸에 적힌 숫자를 뜻하는 NxN크기의 2차원 배열이며, N은 2 이상 500이하의 자연수입니다.
board의 각 칸에는 1 이상 NxN이하의 자연수가 중복 없이 하나씩 들어있습니다.
nums는 board에서 지울 숫자가 들어있는 배열이며, 길이는 1 이상 NxN이하입니다.
nums에 들어있는 숫자는 1 이상 NxN이하의 자연수이며, 중복된 수가 들어있지 않습니다.

입출력 예
board				nums									result
[[11,13,15,16],
 [12, 1, 4, 3],
 [10, 2, 7, 8],
 [ 5,14, 6, 9]]		[14,3,2,4,13,1,16,11,5,15]				3
 
[[ 6,15,17,14,23], 
 [ 5,12,16,13,25],
 [21, 4, 2, 1,22],
 [10,20, 3,18, 8],
 [11, 9,19,24, 7]]	[15,7,2,25,9,16,12,18,5,4,10,13,20]		2
*/

#include <vector>
using namespace std;

int h_chk(int r, int c, vector<vector<int>> m)
{
    int bingo = 0;
    int nr = r;
    int nc = c - 1;
    
    while(nc>=0){
        if(m[nr][nc]) return 0;
        bingo++;
        nc--;
    }
    nc = c;
    while(nc < m.size()){
        if(m[nr][nc])   return 0;
        bingo++;
        nc++;
    }
    
    if(bingo == m.size())   return 1;
    else return 0;
}


int v_chk(int r, int c, vector<vector<int>> m)
{
    int bingo = 0;
    int nr = r - 1;
    int nc = c;
    
    while(nr>=0){
        if(m[nr][nc]) return 0;
        bingo++;
        nr--;
    }         
    nr = r;
    while(nr< m.size()){
        if(m[nr][nc])   return 0;
        bingo++;
        nr++;
    }
    
    if(bingo == m.size())   return 1;
    else return 0;
}


int solution(vector<vector<int>> board, vector<int> nums) {
    int answer = 0;
    
    for(int k = 0; k < nums.size(); k++){
        bool find = false;
        for(int i = 0; i < board.size(); i++){
            for(int j = 0; j < board[i].size(); j++){ 
                if(board[i][j] == nums[k]){
                    board[i][j] = 0;
                    answer += h_chk(i, j, board);
                    answer += v_chk(i, j, board);
                    find = true;
                }
                if(find) break;
            }
            if(find) break;
        }
    }
      
    if(!board[0][0]){
        int r = 1;
        int c = 1;
        while(r < board.size() && c < board.size()){
            if(board[r][c]) break;
            else if(r == board.size() - 1) answer++;
            r++;
            c++;
        }
    }
        
    if(!board[0][board.size()-1]){
        int r = 1;
        int c = board.size() - 2;
        while(r < board.size() && c >= 0){
            if(board[r][c]) break;
            else if(r == board.size() - 1) answer++;
            r++;
            c--;
        }
    }
    return answer;
}

/*
def solution(board, nums):
    n = len(board)
    vertical = [0 for _ in range(n)]
    horizontal = [0 for _ in range(n)]
    lu_diag = 0
    ru_diag = 0

    # 탐색을 O(1)로 하기 위해 nums를 set 자료구조로 변환
    nums = set(nums)
    for p in range(n):
        for q in range(n):
            if board[p][q] in nums:
                horizontal[q]+=1
                vertical[p]+=1
                if p == q:
                    lu_diag+=1
                if p + q == n - 1:
                    ru_diag += 1

    cnt = 0
    cnt += vertical.count(n)
    cnt += horizontal.count(n)
    if lu_diag == n:
        cnt += 1
    if ru_diag == n:
        cnt += 1
    return cnt
*/