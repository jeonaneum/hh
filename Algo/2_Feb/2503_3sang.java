import java.io.BufferedReader;
import java.io.InputStreamReader;

class Baseball{
	String num;
	int strike, ball;
	public Baseball(String line[]){
		num = line[0];
		strike = Integer.parseInt(line[1]);
		ball = Integer.parseInt(line[2]);
	}
}

public class baekjoon_2503{
	public static int n;
	public static Baseball baseball[];
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int i; n = Integer.parseInt(br.readLine());
		baseball = new Baseball[n];
		
		for(i=0;i<n;i++){
			baseball[i] = new Baseball(br.readLine().split(" "));
		}
			
		System.out.println(String.valueOf(getCandidate()));
	}
	
	//	123~987 중에 수 선택하고 맞으면 res++
	private static int getCandidate(){
		int i, res = 0;
		for(i=123;i<=987;i++)
			if(isPossible(String.valueOf(i)))
				res++;
		return res;
	}
	
	//	가능하지 판단
	private static boolean isPossible(String candidate){
		//	이부분 코드 레퍼런스 -> toCharArray() 메소드"324" -> {'3','2','4'}
		char c[] = candidate.toCharArray();
		
        // 첫번째 괄호 각 숫자들이 하나라도 서로 같거나, 하나라도 0이 있다면 false
		if (!((c[0]!=c[1] && c[1]!=c[2] && c[2]!=c[0]) && (c[0]>'0' && c[1]>'0' && c[2]>'0'))) return false;
		
		int i, SB[];
		for(i=0;i<n;i++){
			SB = getStrikeAndBall(baseball[i].num, c);
			//	정답인지 체크  S와 B가 같을 때
			if(!(baseball[i].strike==SB[0] && baseball[i].ball==SB[1]))
                return false;
		}
		
		return true;
	}
	
	private static int[] getStrikeAndBall(String num, char c[]){
		int i, j, SB[] = { 0, 0 };
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				if(num.charAt(i) == c[j]){
					//같은 자리에서 같으면 Strike+1
					if(i==j) {
						SB[0]++; 
					}
					//다른 자리에서 같으면 Ball+1
					else {
						SB[1]++; 
					}
				}
		return SB;
	}
}
