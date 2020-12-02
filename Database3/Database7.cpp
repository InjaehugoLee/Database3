//#define BINARYMODE  //������ read�ϴ� ���     >>writemode�ϰ� ������ �ּ�    or  text���Ϸ� ����� ������ �ּ�
#define WRITEMODE   //���Ͽ� write�ϴ� ���    >>binarymode�ϰ� ������ �ּ�

#include <iostream>
#include "Sakila5.h"

#define TABLECOUNT 16

string tables[16] = {
    "actor", "address", "category", "city", "country",
    "customer", "film", "film_actor", "film_category", "film_text",
    "inventory", "language", "payment", "rental", "staff",
    "store"
};

#ifdef WRITEMODE

string fields[16] = {
    "actor_id, first_name, last_name, last_update",
    "address_id, address, address2, district, city_id, postal_code, phone, location, last_update",
    "category_id, name, last_update",
    "city_id, city, country_id, last_update",
    "country_id, country, last_update",
    "customer_id, store_id, first_name, last_name, email, address_id, active, create_date, last_update",
    "film_id, title, description, release_year, language_id, original_language_id, rental_duration, rental_rate, length, replacement_cost, rating, special_features, last_update",
    "actor_id, film_id, last_update",
    "film_id, category_id, last_update",
    "film_id, title, description",
    "inventory_id, film_id, store_id, last_update",
    "language_id, name, last_update",
    "payment_id, customer_id, staff_id, rental_id, amount, payment_date, last_update",
    "rental_id, rental_date, inventory_id, customer_id, return_date, staff_id, last_update",
    "staff_id, first_name, last_name, address_id, email, store_id, active, username, password, last_update",
    "store_id, manager_staff_id, address_id, last_update"
};



BOOL queryData(MYSQL& mysql, string tables, string fields, vector<Sakila*>& vec)
{
    //��������
    char sqlstr[512] = "";
    sprintf_s(sqlstr, "select %s from %s", fields.c_str(), tables.c_str());
    if (mysql_query(&mysql, sqlstr) != 0)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    //���� ��� ����
    MYSQL_RES* res = mysql_store_result(&mysql);
    if (!res)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    //���� ��� �ʵ� ��
    int fieldCount = mysql_num_fields(res);

    //�ʵ�� ��ºκ� �߰�
    Sakila* sk = NULL;

    //���� ������� ���ڵ�(row)���� �ϳ��� ���� ����
    MYSQL_ROW row;

    //��� ���ڵ带 �� ���� ������ �ݺ� ����
    while (row = mysql_fetch_row(res))
    {
        sk = new Sakila;
        sk->save(row, fieldCount);
        vec.push_back(sk);
    }

    //���� ��� �޸� ����
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
        printf("Fail to initialize mysql \n");
        return 1;
    }

    // �����ͺ��̽� ����
    if (mysql_real_connect(&mysql, "localhost", "scott", "tiger", "sakila", 0, 0, 0) == NULL)  //���̵�� ��� �������ּ���
    {
        printf("%s\n", mysql_error(&mysql));
        return 1;
    }

    CreateDirectory("sakila", NULL);

    vector<Sakila*> vec;
    FILE* fp = NULL;
    char filename[256];
    for (int i = 0; i < TABLECOUNT; i++)
    {
        if (!queryData(mysql, tables[i], fields[i], vec)) return 1;

#ifdef BINARYMODE       //
        sprintf_s(filename, "./sakila/%s.bin", tables[i].c_str());
        errno_t err = fopen_s(&fp, filename, "wb");
#else
        sprintf_s(filename, "./sakila/%s.txt", tables[i].c_str());
        errno_t err = fopen_s(&fp, filename, "wt");
#endif

        if (err != 0)
        {
            char errmsg[512];
            sprintf_s(errmsg, "Fail create file![%s]", filename);
            printf("%s", errmsg);
            break;
        }

        printf("%02d-%s\n", i, tables[i].c_str());
        //��ü���� ȭ�鿡 ����ϰ� ���Ͽ� ������ �޸𸮿��� �����Ѵ�.
        for (auto pc : vec)
        {
            pc->write(fp);
            //pc->print(fp);    //�� ģ���� txt������ print���ִ� ��.
            delete pc;
        }
        vec.clear();
        fclose(fp);
    }

    //�����ͺ��̽� �ݱ�
    mysql_close(&mysql);

}
#else   //�� ���� �Լ��� binarymode�� �����ϴ� �ּ�
int main()
{
    FILE* fp;
    errno_t err;
    char fname[256];
    for (int i = 0; i < TABLECOUNT; i++)
    {
        sprintf_s(fname, "./sakila/%s.bin", tables[i].c_str());
        err = fopen_s(&fp, fname, "rb");
        if (err != 0)
        {
            printf("[%s]fail to open!\n", fname);
            break;
        }

        int cnt, len;
        char str[256];
        while (fread(&cnt, sizeof(int), 1, fp)) // record count
        {
            for (int i = 0; i < cnt; i++)
            {
                fread(&len, sizeof(int), 1, fp);    // len
                fread(str, len, 1, fp);             // string
                str[len] = '\0';

                if (i != 0) printf(",");
                printf("%s", str);
            }
            printf("\n");
        }
        fclose(fp);
    }
}

#endif