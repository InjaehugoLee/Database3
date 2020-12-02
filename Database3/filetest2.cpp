// 바이너리 파일을 만들어서 문자열을 저장하고, 읽기.
#include <iostream>
using namespace std;

int main()
{
	const char* a = "나는 이인재";
	// fp라는 이름의 파일을 선언
	FILE* fp;
	// 파일을 연다.
	errno_t err = fopen_s(&fp, "text.bin", "wb");
	if (err != 0)
	{
		printf("Fail to create file!\n");
		return 1;
	}

	// 문자열 바이너리로 저장
	int len = strlen(a);
	fwrite(&len, sizeof(int), 1, fp);
	fwrite(a, len, 1, fp);

	fclose(fp);

	// 문자열 읽기
	char* pp;
	err = fopen_s(&fp, "text.bin", "rb");
	int len1;
	fread(&len1, sizeof(int), 1, fp);

	char ttt[256];
	fread(ttt, len1, 1, fp);
	ttt[len1] = '\0';
	printf("%s\n", ttt);
	// 메모리 할당, 문자열 끝의 널 문자를 넣기 위해서.
	// 방법 1, 동적 메모리 할당 malloc 이용. - 꼭 free로 메모리 해제 - Heap
	/*pp = (char*)malloc(len1 + 1);
	fread(pp, len1, 1, fp);
	pp[len1] = '\0';
	printf("%s\n", pp);

	fclose(fp);
	free(pp);
	*/

	// 방법 2, 동적 메모리 할당 new 이용. - 꼭 delete로 메모리 해제 - Heap
	/*pp = new char[len1 + 1]; 
	fread(pp, len1, 1, fp);
	pp[len1] = '\0';
	printf("%s\n", pp);


	fclose(fp);
	delete[] pp;*/

	// 방법 3, 배열 이용. - 메모리해제 필요 없음. - Stack

	fclose(fp);
}
