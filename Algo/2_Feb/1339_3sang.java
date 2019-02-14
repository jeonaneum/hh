import java.util.Arrays;
import java.util.Scanner;
 
public class baekjoon_1339{
    static int n;
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        n = input.nextInt();
        
        String[] arr = new String[n];
        for(int i=0;i<n;i++)
            arr[i] = input.next();

        //	알파벳 배열
        int[] num = new int[26];
        
        //	각 단어에 해당하는배열의 인덱스에 자릿수 더해가기
        for(int i=0;i<n;i++) {
            String t = arr[i];
            for(int j=0;j<t.length();j++) {
                num[(t.charAt(j)-65)] += (int)Math.pow(10,(t.length()-j-1));
            }
        }
        //	배열 정렬 후 자리 수 별 더해가며 출력
        Arrays.sort(num);
        int result = 0;
        for(int i=25,k=9;i>=0;i--,k--) {
            if(num[i]==0) break;
            result += (num[i]*k);
        }
        System.out.println(result);
    }
}
