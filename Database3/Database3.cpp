// dbstudy.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
#include <iostream>
#include <vector>
#include <string>

#include "mysql.h"
#include "Sakila.h"

#define TABLECOUNT 5
using namespace std;
string fields[19] = {
    "actor_id, first_name, last_name, last_update",
    "actor_id, film_info, first_name, last_name",
    "film_id, title, description, release_year, language_id",
    "active, address_id, create_date",
    "payment_id, rental_id, amount"

};

string tables[5] = {
    "actor", "actor_info", "film", "customer", "payment"
};

// �� / ������ �Ǹ��ϴ� �������� queryData��� �̸����� ���� - �޴� �����ʹ� Mysql�� ����, ���̺�, �ʵ�, Ŭ���� sakila
BOOL queryData(MYSQL& mysql, string tables, string fields, vector<sakila*> &vec)
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
        if (tables == "actor") sk = new actor;
        else if (tables == "actor_info") sk = new actor_info;
        else if (tables == "film") sk = new film;
        else if (tables == "customer") sk = new customer;
        else if (tables == "payment") sk = new payment;

        sk->save(row);
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

    CreateDirectory("sakila", NULL);
    
    vector<sakila*> vec;
    FILE* fp = NULL;
    char filename[256];
    for ( int i = 0; i < TABLECOUNT; i++)
    { 
        if (!queryData(mysql, tables[i], fields[i], vec)) return 1;  // ������ false ���� ��, 

        sprintf_s(filename, "./sakila/%s.txt", tables[i].c_str());
        errno_t err = fopen_s(&fp, filename, "wt");
        if (err != 0)
        {
            char errmsg[512];
            sprintf_s(errmsg, "Fail to create file![%s]", filename);
            printf("%s\n", errmsg);
            break;
        }

        for (auto pc : vec)
        {
            printf("----------------------------------------------------\n");
            pc->print(fp);
            delete pc;
        }
        vec.clear();
        fclose(fp);
        
        printf("========================================================\n");
       
    }
   
    mysql_close(&mysql);
}