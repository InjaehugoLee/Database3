// dbstudy.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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
    // 질의 전달
    char sqlstr[512] = "";
    sprintf_s(sqlstr, "select %s from %s", fields.c_str(), table.c_str());
    if (mysql_query(&mysql, sqlstr) != 0)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // 질의 결과 저장
    MYSQL_RES* res = mysql_store_result(&mysql);
    if (!res)
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // 질의 결과 필드 수
    int fieldCount = mysql_num_fields(res);
    // 필드명 출력부분 추가

    // mysql_fetch_fields

    // 여기만 완성하면 돌아감.
    vector<int> fieldInfo;
    fieldInfo.push_back(1);
    fieldInfo.push_back(2);
    fieldInfo.push_back(3);

    SakilaDataU* sk = NULL;
    //SakilaData* sk = NULL;
    // 질의 결과에서 레코드(row)별로 하나씩 담을 변수
    MYSQL_ROW row;
    // 모든 레코드를 다 읽을 때까지 반복 수행
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
    // 질의 결과 메모리 해제
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
        printf("Fail to initialize mysql\n");
        return 1;
    }

    // 데이터베이스 연결
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

        // 객체들을 화면에 출력하고 파일에 적은뒤 메모리에서 제거한다.
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
    // 데이터베이스 닫기
    mysql_close(&mysql);
}