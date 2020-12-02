#pragma once
#include <string>
#include "mysql.h"

using namespace std;
//==============================================================================
// 무식하게 한개의 데이터 타입으로 저장하는 형태

//class SakilaData
//{
//public:
//    vector<string> values;
//public:
//    SakilaData() {}
//    ~SakilaData() {}
//
//    void print(FILE* fp = NULL)
//    {
//        for (auto t : values)
//        {
//            if (fp) fprintf(fp, "%s|", t.c_str());
//            printf("%s\n", t.c_str());
//        }
//    }
//    void save(MYSQL_ROW& row, int count)
//    {
//        for (int i = 0; i < count; i++)
//        {
//            if (row[i])     values.push_back(row[i]);
//            else            values.push_back("");
//        }
//    }
//};

//==============================================================================
// 엘리트하게 저장하는 형태

class SAField
{
public:
    int type;       // 0:None, 1:int, 2:double, 3:string
    int nValue;
    double dValue;
    string strValue;

public:
    SAField()
    {
        type = 0;
        nValue = 0;
        dValue = 0.0;
        strValue = "";
    }
    ~SAField() {}

    void save(string& str, int type)
    {
        this->type = type;
        if (type == 1)  nValue = atoi(str.c_str());
        else if (type == 2)  dValue = atof(str.c_str());
        else if (type == 3)  strValue = str;
    }
};

class SakilaDataU : public SAField
{
public:
    vector<SAField*> values;
public:
    SakilaDataU() {}
    ~SakilaDataU()
    {
        for (auto p : values)
            delete p;
        values.clear();
    }

    void print(FILE* fp = NULL)
    {
        for (auto t : values)
        {
            if (fp)
            {
                if (t->type == 1)        fprintf(fp, "%d|", t->nValue);
                else if (t->type == 2)   fprintf(fp, "%f|", t->dValue);
                else if (t->type == 3)   fprintf(fp, "%s|", t->strValue.c_str());
                else                     fprintf(fp, "|");
            }
            if (t->type == 1)        printf("%d\n", t->nValue);
            else if (t->type == 2)   printf("%f\n", t->dValue);
            else if (t->type == 3)   printf("%s\n", t->strValue.c_str());
            else                     printf("\n");
        }
        if (fp) fprintf(fp, "\n");

    }
    void save(MYSQL_ROW& row, vector<int>& typeinfo)
    {
        SAField* sf = new SAField;
        for (int i = 0; i < typeinfo.size(); i++)
        {
            if (typeinfo[i] == 1)
            {
                if (row[i])     sf->nValue = atoi(row[i]);
            }
            else if (typeinfo[i] == 2)
            {
                if (row[i])     sf->dValue = atof(row[i]);
            }
            else if (typeinfo[i] == 3)
            {
                if (row[i])     sf->strValue = row[i];
            }
            values.push_back(sf);
        }
    }
};
