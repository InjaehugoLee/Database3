#include <iostream>

using namespace std;

int main()
{
	//==================================================
	// const�� ����ؼ� ����� �������� ��. Data��� �޸� ������ ����. ���� �Ұ�.
	const char* str = "hello world, Soy Hugo. Mucho gusto";

	FILE* wp; // wp��� �̸��� ������ ����.
	errno_t err = fopen_s(&wp, "test4.bin", "wb"); // wp��� ������ ���� ���
	if (err != 0)
	{
		printf("Fail to create bin file!\n");
		return 1;
	}

	// ���ڿ� ���̳ʸ� ����
	// ���̳ʸ��� ����� ���ڿ��� ���̰� ������ ���� �����ؾ� ��. 
	// �ؽ�Ʈ ���Ŀ��� �ʿ������, ���̳ʸ��� �ʿ���. 
	int len = strlen(str); // str�̶�� �̸��� ���ڿ��� ���̸� int�� len �̶�� �̸��� ����.
	fwrite(&len, sizeof(int), 1, wp);  // ����(len�̶�� �ּ�, int ũ�� ��ŭ, �� ����, wp��� ����)
	fwrite(str, len, 1, wp);		// ����(str, str ���� ��ŭ, �� ����, wp��� ����)

	fclose(wp);

	//==================================================
	
	//char ps[320];
	char* pp; // ���� ������ �������� pp�� ����.
	err = fopen_s(&wp, "test4.bin", "rb"); // ������ ����.
	int len1; // int ������ len1�� ����.
	fread(&len1, sizeof(int), 1, wp); // ������ �о 
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