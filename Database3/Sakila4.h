#pragma once
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "mysql.h"
using namespace std;

class sakila
{
public: 
	vector <string> values;
public:
	sakila() {} // 생성자
	~sakila() {} // 소멸자

	void print(FILE* fp)
	{
		if (fp)
		{
			int cnt = values.size();
			fwrite(&cnt, sizeof(int), 1, fp);
			for (auto t : values)
			{
				int len = strlen(t.c_str());
				fwrite(&len,sizeof(int),1,fp);
				fwrite(t.c_str(),len,1,fp);
			}
		}
		else { cout << "파일을 읽을 수 없습니다" << endl; }
	}
	void read(FILE* fp)
	{
		if (fp)
		{
			for (auto t : values)
			{
				int len;
				fread(&len,sizeof(int),1,fp);
				char str[256];
				fread(str, len, 1, fp);
				str[len] = '\0';
				printf("%s", str);

				/*
				char* pp;
				err = fopen_s(&fp, "text.bin", "rb");
				int len1;
				fread(&len1, sizeof(int), 1, fp);

				char ttt[256];
				fread(ttt, len1, 1, fp);
				ttt[len1] = '\0';
				printf("%s\n", ttt);*/

				//int len;
				//char str[256];
				//fread(&len, sizeof(int), 1, fp);
				//str[len] = '\0';
				//fread(str, len, 1, fp);
				//printf("%s", str);
			}
		}
		else { cout << "파일을 읽을 수 없습니다" << endl; }
	}
	void save(MYSQL_ROW& row, int count)
	{
		for (int i = 0; i < count; i++)
		{
			if (row[i])     values.push_back(row[i]);
			else            values.push_back("");
		}
	}
};
