#include <iostream>
using namespace std;

int main()
{
	int a = 15;
	int b = 16;
	const char* str = "Hello Hugo Lee";
	int c[5] = { 0,1,2,3,4 };
	double d = 15.4;
	

	// �ؽ�Ʈ ����---------
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
	// ���̳ʸ� ����

	FILE* wp; //������ �����
	err = fopen_s(&wp, "test2.bin", "wb"); // ������ ����
	if (err != 0)
	{
		printf("Fail to create file.\n"); // ���� �߻���, ���� �ȳ��� ����Ʈ
		return 1;
	}
	//fwrite(&d, sizeof(double), 1, wp); // ���Ͽ� ����
	//fwrite(&a, sizeof(int), 1, wp);
	//fwrite(&b, sizeof(int), 1, wp);

	int len = strlen(str);
	fwrite(&len, sizeof(int), 1, wp);
	fwrite(str, len, 1, wp);	

	fclose(wp);

	err = fopen_s(&wp, "test.bin", "rb");
	//fread(&d, sizeof(double), 1, wp); // ������ �а�
	//printf("%f", d); // ȭ�鿡 ���� �������
	char ttt[256];
	int len1;
	fread(&len1, sizeof(int), 1, wp);
	fread(ttt, len1, 1, wp); // ������ �а�
	ttt[len1] = '\0';
	printf("%s", ttt); // ȭ�鿡 ���� �������
	fclose(wp);
}