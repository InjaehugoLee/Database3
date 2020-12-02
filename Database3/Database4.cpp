// dbstudy.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <iostream>
#include <vector>

#include "mysql.h"
#include "Sakila1.h"

using namespace std;

#define TABLECOUNT 7

string fields[16] = {
    "actor_id,first_name,last_name,last_update",
    "address_id,address,address2,district,city_id,postal_code,phone,location,last_update",
    "category_id,name,last_update",
    "city_id,city,country_id,last_update",
    "country_id,country,last_update",
    "customer_id,store_id,first_name,last_name,email,address_id,active,create_date,last_update",
    "film_id,title,description,release_year,language_id,original_language_id,rental_duration,rental_rate,length,replacement_cost,rating,special_features,last_update"
};

string tables[16] = {
    "actor", "address", "category", "city", "country",
    "customer", "film", "film_actor", "film_category", "film_text",
    "inventory", "language", "payment", "rental", "staff",
    "store"
};

BOOL queryData(MYSQL& mysql, string table, string fields, vector<SakilaDataU*>& vec)
//BOOL queryData(MYSQL& mysql, string table, string fields, vector<SakilaData*>& vec)
{
    // ���� ����
    char sqlstr[512] = "";
    sprintf_s(sqlstr, "select %s from %s", fields.c_str(), table.c_str());
    if (mysql_query(&mysql, sqlstr) != 0)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // ���� ��� ����
    MYSQL_RES* res = mysql_store_result(&mysql);
    if (!res)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // ���� ��� �ʵ� ��
    int fieldCount = mysql_num_fields(res);
    // �ʵ�� ��ºκ� �߰�

    // mysql_fetch_fields

    // ���⸸ �ϼ��ϸ� ���ư�.
    vector<int> fieldInfo;
    fieldInfo.push_back(1);
    fieldInfo.push_back(2);
    fieldInfo.push_back(3);

    SakilaDataU* sk = NULL;
    //SakilaData* sk = NULL;
    // ���� ������� ���ڵ�(row)���� �ϳ��� ���� ����
    MYSQL_ROW row;
    // ��� ���ڵ带 �� ���� ������ �ݺ� ����
    while (row = mysql_fetch_row(res))
    {
        //if (table == "actor")               sk = new Actor;
        //else if (table == "address")        sk = new Address;
        //else if (table == "category")       sk = new Category;
        //else if (table == "city")           sk = new City;
        //else if (table == "country")        sk = new Country;
        //else if (table == "customer")       sk = new Customer;
        //else if (table == "film")           sk = new Film;

        //sk->save(row);
        ////sk = new SakilaData;
        ////sk->save(row, fieldCount);
        //vec.push_back(sk);

        SakilaDataU* sdu = new SakilaDataU;
        sdu->save(row, fieldInfo);
    }
    // ���� ��� �޸� ����
    mysql_free_result(res);

    return TRUE;
}

int main(int argc, char* argv[])
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

    vector<SakilaDataU*> vec;
    //vector<SakilaData*> vec;
    FILE* fp = NULL;
    char filename[256];
    for (int i = 5; i < TABLECOUNT; i++)
    {
        if (!queryData(mysql, tables[i], fields[i], vec)) return 1;

        sprintf_s(filename, "./sakila/%s.txt", tables[i].c_str());
        errno_t err = fopen_s(&fp, filename, "wt");
        if (err != 0)
        {
            char errmsg[512];
            sprintf_s(errmsg, "Fail create file![%s]", filename);
            printf("%s\n", errmsg);
            break;
        }

        // ��ü���� ȭ�鿡 ����ϰ� ���Ͽ� ������ �޸𸮿��� �����Ѵ�.
        for (auto pc : vec)
        {
            printf("-----------------------------------------\n");
            pc->print(fp);
            delete pc;
        }
        vec.clear();
        fclose(fp);

        printf("========================================================\n");
    }
    // �����ͺ��̽� �ݱ�
    mysql_close(&mysql);
}