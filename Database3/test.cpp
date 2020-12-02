#include <iostream>
using namespace std;

int main()
{
	int a = 15;
	int b = 16;
	const char* str = "Hello Hugo Lee";
	int c[5] = { 0,1,2,3,4 };
	double d = 15.4;
	

	// 텍스트 형식---------
	FILE* fp;
	errno_t err = fopen_s(&fp, "test2.txt", "wt");
	if (err != 0)
	{
		printf("Fail to create file.\n");
		return 1;
	}
	fprintf(fp, "%d\n", a);
	fprintf(fp, "%d\n", b);
	fprintf(fp, "%s\n", str);

	fclose(fp);

	// ===================================================
	// 바이너리 형식

	FILE* wp; //파일을 만들고
	err = fopen_s(&wp, "test2.bin", "wb"); // 파일을 열고
	if (err != 0)
	{
		printf("Fail to create file.\n"); // 에러 발생시, 에러 안내문 프린트
		return 1;
	}
	//fwrite(&d, sizeof(double), 1, wp); // 파일에 쓰고
	//fwrite(&a, sizeof(int), 1, wp);
	//fwrite(&b, sizeof(int), 1, wp);

	int len = strlen(str);
	fwrite(&len, sizeof(int), 1, wp);
	fwrite(str, len, 1, wp);	

	fclose(wp);

	err = fopen_s(&wp, "test.bin", "rb");
	//fread(&d, sizeof(double), 1, wp); // 파일을 읽고
	//printf("%f", d); // 화면에 파일 내용출력
	char ttt[256];
	int len1;
	fread(&len1, sizeof(int), 1, wp);
	fread(ttt, len1, 1, wp); // 파일을 읽고
	ttt[len1] = '\0';
	printf("%s", ttt); // 화면에 파일 내용출력
	fclose(wp);
}