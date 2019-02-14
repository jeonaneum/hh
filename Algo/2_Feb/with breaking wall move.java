import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.LinkedList;
import java.util.Queue;
import java.util.StringTokenizer;
 
public class Main {
    static int n;
    static int m;
    static int[][] map = new int[1001][1001];
    static boolean[][][] check = new boolean[1001][1001][2];
    
    static int[] dx = {0, -1, 0, 1};
    static int[] dy = {-1, 0, 1, 0};
    static boolean isSuccess;
    public static void main(String[] args) throws IOException {
 
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer st = new StringTokenizer(br.readLine());
 
        n = Integer.parseInt(st.nextToken());
        m = Integer.parseInt(st.nextToken());        
 
        for(int i=0;i<n;i++) {
            String tmp = br.readLine();
            for(int j=0;j<m;j++) {
                map[i][j] = tmp.charAt(j)-'0';
            }
            
        }
        
        int step = counting_BFS();
        System.out.println(isSuccess? step : -1);
    }
    
    static int counting_BFS() {
        Queue<Node> queue = new LinkedList<Node>();
        queue.add(new Node(0,0,0));
        
        check[0][0][0] = true;
        check[0][0][1] = true;
        
        isSuccess = false;
        int step = 0;
        
        
        while(!queue.isEmpty() && !isSuccess) {
            step++;
            int size = queue.size();
            for(int i=0;i<size;i++) {
                Node node = queue.poll();
                
                if(node.x==n-1 && node.y==m-1) {
                    isSuccess = true;
                    break;
                }
                for(int j=0;j<4;j++) {
                    int nx = node.x + dx[j];
                    int ny = node.y + dy[j];
                
                    if(nx>=0 && nx<n && ny>=0 && ny<m) {
                        //벽의 경우
                        if(map[nx][ny]==1) {
                            //벽을 부시지 않은 상태 & 다음 좌표 방문하지 않았을 경우
                            if(node.destroy<1 && !check[nx][ny][1]){
                                queue.add(new Node(nx,ny,1));
                                check[nx][ny][1]=true;
                            }
                        }
                        //벽이 아닐 경우
                        else {
                            if(!check[nx][ny][node.destroy]) {
                                queue.add(new Node(nx, ny, node.destroy));
                                check[nx][ny][node.destroy]=true;
                            }
                        }
                    }    
                }    
            }    
        }
        return step;
    }
}
class Node{
    int x;
    int y;
    int destroy;
    
    Node(int x, int y, int destroy){
        this.x = x;
        this.y = y;
        this.destroy = destroy;
    }
}