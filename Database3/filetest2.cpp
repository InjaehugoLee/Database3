// ���̳ʸ� ������ ���� ���ڿ��� �����ϰ�, �б�.
#include <iostream>
using namespace std;

int main()
{
	const char* a = "���� ������";
	// fp��� �̸��� ������ ����
	FILE* fp;
	// ������ ����.
	errno_t err = fopen_s(&fp, "text.bin", "wb");
	if (err != 0)
	{
		printf("Fail to create file!\n");
		return 1;
	}

	// ���ڿ� ���̳ʸ��� ����
	int len = strlen(a);
	fwrite(&len, sizeof(int), 1, fp);
	fwrite(a, len, 1, fp);

	fclose(fp);

	// ���ڿ� �б�
	char* pp;
	err = fopen_s(&fp, "text.bin", "rb");
	int len1;
	fread(&len1, sizeof(int), 1, fp);

	char ttt[256];
	fread(ttt, len1, 1, fp);
	ttt[len1] = '\0';
	printf("%s\n", ttt);
	// �޸� �Ҵ�, ���ڿ� ���� �� ���ڸ� �ֱ� ���ؼ�.
	// ��� 1, ���� �޸� �Ҵ� malloc �̿�. - �� free�� �޸� ���� - Heap
	/*pp = (char*)malloc(len1 + 1);
	fread(pp, len1, 1, fp);
	pp[len1] = '\0';
	printf("%s\n", pp);

	fclose(fp);
	free(pp);
	*/

	// ��� 2, ���� �޸� �Ҵ� new �̿�. - �� delete�� �޸� ���� - Heap
	/*pp = new char[len1 + 1]; 
	fread(pp, len1, 1, fp);
	pp[len1] = '\0';
	printf("%s\n", pp);


	fclose(fp);
	delete[] pp;*/

	// ��� 3, �迭 �̿�. - �޸����� �ʿ� ����. - Stack

	fclose(fp);
}
