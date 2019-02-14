// 1. the lagest number of ranks who have superior
#include <algorithm>
using namespace std;

long long solution(vector<int> &ranks) {
    long long ans = 0;
    int cnt = 1;
    
    sort(ranks.begin(), ranks.end());    
    
    for(int i = 0; i < ranks.size() - 1; i++){
        if(ranks[i] == ranks[i+1] - 1){
            ans += (long long)cnt;
            cnt = 1;
        }
        else if(ranks[i] == ranks[i+1]){
            cnt++;
        }
        else{
            cnt = 1;
        }
    }
    return ans;
}

// 2. tangled necklace
int solution(vector<int> &A) {
    bool used[1000000] = {false, };
    int cnt, next, max = 0;
    
    for(int i = 0; i < A.size(); i++){
        if(used[i]) continue;
        
        used[i] = true;
        cnt = 1;
        next = A[i];
        
        while(!used[next]){
            used[next] = true;
            next = A[next];
            cnt++;
        }
        max = (cnt > max)?cnt:max;
    }
    return max;
}


// 3. pawn beat
#include <queue>
using namespace std;

typedef struct{
    int r, c, beat;
} PAWN;
queue<PAWN> q;

int solution(vector<string> &B) {
    int N = B.size();
    int ans = 0;
    int rl[2] = {-1, 1};
    
    while(!q.empty()) q.pop();    
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(B[i][j] == 'O'){
                PAWN jafar;
                jafar.r = i;
                jafar.c = j;
                jafar.beat = 0;
                q.push(jafar);
            }    
        }
    } 
    
    while(!q.empty()){
        int r = q.front().r;
        int c = q.front().c;
        int beat = q.front().beat;
        
        q.pop();
        
        if(beat > ans) ans = beat;
        
        for(int i = 0; i < 2; i++){
            int det_r = r - 1;
            int det_c = c + rl[i];
            
            if(det_r < 0 || det_c < 0 || det_r >= N || det_c >= N) continue;
            if(B[det_r][det_c] != 'X') continue;
            
            PAWN move;
            move.r = det_r - 1;
            move.c = det_c + rl[i];
            move.beat = beat + 1;
            
            if(move.r < 0 || move.c < 0 || move.r >= N || move.c >= N) continue;
            if(B[move.r][move.c] == 'X') continue;
            
            q.push(move);
        }
    }
    return ans;
}