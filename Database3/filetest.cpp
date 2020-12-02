#include <iostream>

using namespace std;

int main()
{
	//==================================================
	// const를 사용해서 상수로 만들어줘야 함. Data라는 메모리 영역에 저장. 변경 불가.
	const char* str = "hello world, Soy Hugo. Mucho gusto";

	FILE* wp; // wp라는 이름의 파일을 만듦.
	errno_t err = fopen_s(&wp, "test4.bin", "wb"); // wp라는 파일을 여는 방법
	if (err != 0)
	{
		printf("Fail to create bin file!\n");
		return 1;
	}

	// 문자열 바이너리 저장
	// 바이너리로 저장시 문자열의 길이가 얼마인지 같이 저장해야 함. 
	// 텍스트 형식에서 필요없지만, 바이너리는 필요함. 
	int len = strlen(str); // str이라는 이름의 문자열의 길이를 int형 len 이라는 이름에 저장.
	fwrite(&len, sizeof(int), 1, wp);  // 저장(len이라는 주소, int 크기 만큼, 한 번만, wp라는 파일)
	fwrite(str, len, 1, wp);		// 저장(str, str 길이 만큼, 한 번만, wp라는 파일)

	fclose(wp);

	//==================================================
	
	//char ps[320];
	char* pp; // 문자 포인터 형식으로 pp를 만듦.
	err = fopen_s(&wp, "test4.bin", "rb"); // 파일을 연다.
	int len1; // int 형으로 len1을 만듦.
	fread(&len1, sizeof(int), 1, wp); // 파일을 읽어서 
	//fread(ps, len1, 1, wp);
	//ps[len1] = '\0';

	//pp = new char[len1 + 1];	// delete
	pp = (char*)malloc(len1 + 1);	// free
	fread(pp, len1, 1, wp);
	pp[len1] = '\0';

	printf("%s\n", pp);

	fclose(wp);

	//delete[] pp;
	free(pp);
}