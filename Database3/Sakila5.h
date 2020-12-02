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

    void print(FILE* fp = NULL)      //txt���� ����Ʈ(values�� ������)
    {
        for (auto t : values) // auto�� values�� ����ϴ� ������ ������. ����, string���θ� �̷���� vector��.
        {
            if (fp) fprintf(fp, "%s|", t.c_str());
            printf("%s\n", t.c_str());
        }
        if (fp) fprintf(fp, "\n");
    }
    void write(FILE* fp)      //bin���Ͽ��� �Է�(values�� ������), bin���� ����� fighting1.cpp�� 147��
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

    void save(MYSQL_ROW& row, int fieldcount)      //11���� values�� �޸� ����, valuse�� �� Ŭ���������� ��ȿ
    {
        for (int i = 0; i < fieldcount; i++)
        {
            if (row[i])   values.push_back(row[i]);
            else      values.push_back("");
        }
    }
};