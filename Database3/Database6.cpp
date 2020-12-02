// dbstudy.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.

//#define WRITEMODE

#include <iostream>
#include <vector>
#include <string>

#include "mysql.h"
#include "Sakila4.h"

#define TABLECOUNT 5
using namespace std;

string tables[5] = {
    "actor", "actor_info", "film", "customer", "payment"
};

#ifdef WRITEMODE

string fields[5] = {
    "actor_id, first_name, last_name, last_update",
    "actor_id, film_info, first_name, last_name",
    "film_id, title, description, release_year, language_id",
    "active, address_id, create_date",
    "payment_id, rental_id, amount"

};

// �� / ������ �Ǹ��ϴ� �������� queryData��� �̸����� ���� - �޴� �����ʹ� Mysql�� ����, ���̺�, �ʵ�, Ŭ���� sakila
BOOL queryData(MYSQL& mysql, string tables, string fields, vector<sakila*>& vec,FILE* fp)
{
    //���� ����
    char sql[512] = ""; // ������ �������� �̸� sql�� ����� �ʱ�ȭ
    sprintf_s(sql, "select %s from %s", fields.c_str(), tables.c_str()); // ���ڿ��� �ҷ����� �Լ��� ���
    if (mysql_query(&mysql, sql) != 0) // ���� ������ ���� �ȳ��� ����Ʈ
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // ���� ��� ����
    MYSQL_RES* res = mysql_store_result(&mysql); //������ �޸� res ��� ������ �־����Ƿ� ���� �߻��� ���� ����� ���� �ڵ��ؾ� ��.
    if (res == NULL) // if(!res) ���� ǥ�� 
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // ���� ��� �ʵ� ��
    int fieldCount = mysql_num_fields(res);

    // �ʵ�� ��ºκ� �߰�
    sakila* sk = NULL; // Ŭ���� sakila ������ ������ sk�� �ʱ�ȭ

    // ���� ������� ���ڵ�(row)���� �ϳ��� ���� ����
    MYSQL_ROW row;

    // ��� ���ڵ带 �� ���� ������ �ݺ� ����
    while (row = mysql_fetch_row(res))
    {

        sk = new sakila;
        /*if (tables == "actor") sk = new actor;
        else if (tables == "actor_info") sk = new actor_info;
        else if (tables == "film") sk = new film;
        else if (tables == "customer") sk = new customer;
        else if (tables == "payment") sk = new payment;*/

        sk->save(row, fieldCount);
        sk->print(fp);

        vec.push_back(sk);
    }
    // ���� ��� �޸� ����
    mysql_free_result(res);

    return TRUE;
}


int main()
{
    // mysql db�� ����ϱ� ���� ���� ����
    MYSQL mysql;

    // ���� �ʱ�ȭ
    if (mysql_init(&mysql) == NULL)
    {
        printf("Fail to initialize mysql\n");
        return 1;
    }

    // �����ͺ��̽� ����
    if (mysql_real_connect(&mysql, "localhost", "scott", "tiger", "sakila", 0, 0, 0) == NULL)
    {
        printf("%s\n", mysql_error(&mysql));
        return 1;
    }

    vector<sakila*> vec;
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp,"test.bin", "wb");
   
    for (int i = 0; i < TABLECOUNT; i++)
    {    
        if (!queryData(mysql, tables[i], fields[i], vec, fp)) break;  // ������ false ���� ��, 
    }
    fclose(fp);
    mysql_close(&mysql);
}

#else
int main()
{
    FILE* fp;
    errno_t err = fopen_s(&fp, "test.bin", "rb");

    int cnt, len;
    char str[2048];
    while (fread(&cnt, sizeof(int), 1, fp))
    {
        for (int i = 0;i < cnt; i++)
        {
            fread(&len, sizeof(int), 1, fp);
            fread(str, len, 1, fp);
            str[len] = '\0';

            //if (i == 0 && strcmp(str, "200") == 0)
            //    int kkk = 0;

            if (i != 0) printf(",");
            printf("%s", str);
        }
        printf("\n");
    }
    fclose(fp);
}
#endif