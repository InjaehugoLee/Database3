//#define BINARYMODE  //파일을 read하는 모드     >>writemode하고 싶으면 주석    or  text파일로 만들고 싶으면 주석
#define WRITEMODE   //파일에 write하는 모드    >>binarymode하고 싶으면 주석

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
    //질의전달
    char sqlstr[512] = "";
    sprintf_s(sqlstr, "select %s from %s", fields.c_str(), tables.c_str());
    if (mysql_query(&mysql, sqlstr) != 0)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    //질의 결과 저장
    MYSQL_RES* res = mysql_store_result(&mysql);
    if (!res)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    //질의 결과 필드 수
    int fieldCount = mysql_num_fields(res);

    //필드명 출력부분 추가
    Sakila* sk = NULL;

    //질의 결과에서 레코드(row)별로 하나씩 담을 변수
    MYSQL_ROW row;

    //모든 레코드를 다 읽을 때까지 반복 수행
    while (row = mysql_fetch_row(res))
    {
        sk = new Sakila;
        sk->save(row, fieldCount);
        vec.push_back(sk);
    }

    //질의 결과 메모리 해제
    mysql_free_result(res);

    return TRUE;
}

int main(int argc, char* argv[])
{
    // mysql db를 사용하기 위한 변수 선언
    MYSQL mysql;

    // 변수 초기화
    if (mysql_init(&mysql) == NULL)
    {
        printf("Fail to initialize mysql \n");
        return 1;
    }

    // 데이터베이스 연결
    if (mysql_real_connect(&mysql, "localhost", "scott", "tiger", "sakila", 0, 0, 0) == NULL)  //아이디랑 비번 변경해주세요
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
        //객체들을 화면에 출력하고 파일에 적은뒤 메모리에서 제거한다.
        for (auto pc : vec)
        {
            pc->write(fp);
            //pc->print(fp);    //이 친구는 txt파일을 print해주는 것.
            delete pc;
        }
        vec.clear();
        fclose(fp);
    }

    //데이터베이스 닫기
    mysql_close(&mysql);

}
#else   //이 메인 함수가 binarymode를 실행하는 주석
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