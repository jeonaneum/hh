int in, out;
int people = 0;
int ans = 0;

int main()
{	
	for (int i = 0; i < 4; i++) {
		scanf_s("%d %d", &out, &in);
		people += in;
		people -= out;

		ans = (people > ans) ? people : ans;
	}

	printf("%d\n", ans);
}
