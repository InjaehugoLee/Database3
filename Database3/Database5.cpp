#include <iostream>
#include <string>
#include "Sakila3.h"

using namespace std;

int main()
{
	FILE* fp;
	errno_t err = fopen_s(&fp, "sakila/actor.bin", "rb");
	if (err != 0)
	{
		printf("fail to open\n");
		return 1;
	}

	//fwrite(&actor_id, sizeof(actor_id), 1, fp);
	//writeString(fp, first_name);
	//writeString(fp, last_name);
	//writeString(fp, last_update);

	int aid, len;
	char f[64], l[64], u[64];
	while (fread(&aid, sizeof(int), 1, fp) != 0)
	{
		fread(&len, sizeof(int), 1, fp);
		fread(f, len, 1, fp);
		f[len] = '\0';
		fread(&len, sizeof(int), 1, fp);
		fread(l, len, 1, fp);
		l[len] = '\0';
		fread(&len, sizeof(int), 1, fp);
		fread(u, len, 1, fp);
		u[len] = '\0';

		printf("%d,%s,%s,%s\n", aid, f, l, u);
	}
	fclose(fp);
}