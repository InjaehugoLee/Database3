#pragma once
#include <vector>
#include <string>
#include "mysql.h"

using namespace std;

class Sakila
{
public:
    vector<string> values;

    Sakila() {}
    ~Sakila() {}

    void print(FILE* fp = NULL)      //txt파일 프린트(values를 가지고)
    {
        for (auto t : values) // auto는 values이 사용하는 형식을 가져옴. 따라서, string으로만 이루어진 vector임.
        {
            if (fp) fprintf(fp, "%s|", t.c_str());
            printf("%s\n", t.c_str());
        }
        if (fp) fprintf(fp, "\n");
    }
    void write(FILE* fp)      //bin파일에다 입력(values를 가지고), bin파일 출력은 fighting1.cpp의 147행
    {
        if (!fp)   return;

        int cnt = values.size();
        fwrite(&cnt, sizeof(int), 1, fp);
        for (auto t : values)
        {
            int len = t.size();
            fwrite(&len, sizeof(int), 1, fp);
            fwrite(t.c_str(), len, 1, fp);
        }
    }

    void save(MYSQL_ROW& row, int fieldcount)      //11행의 values에 메모리 저장, valuse는 이 클래스에서만 유효
    {
        for (int i = 0; i < fieldcount; i++)
        {
            if (row[i])   values.push_back(row[i]);
            else      values.push_back("");
        }
    }
};