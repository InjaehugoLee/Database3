// dbstudy.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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

// 참 / 거짓을 판명하는 형식으로 queryData라는 이름으로 생성 - 받는 데이터는 Mysql을 참고, 테이블, 필드, 클래스 sakila
BOOL queryData(MYSQL& mysql, string tables, string fields, vector<sakila*> &vec)
{
    //질의 전달
    char sql[512] = ""; // 문자형 형식으로 이름 sql을 만들고 초기화
    sprintf_s(sql, "select %s from %s", fields.c_str(), tables.c_str()); // 문자열을 불러오는 함수로 사용
    if (mysql_query(&mysql, sql) != 0) // 쿼리 오류시 오류 안내문 프린트
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // 질의 결과 저장
    MYSQL_RES* res = mysql_store_result(&mysql); //위에랑 달리 res 라는 변수에 넣었으므로 오류 발생시 밑의 내용과 같이 코드해야 함.
    if (res == NULL) // if(!res) 같은 표현 
    {
        printf("%s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        return FALSE;
    }

    // 질의 결과 필드 수
    int fieldCount = mysql_num_fields(res);  

    // 필드명 출력부분 추가
    sakila* sk = NULL; // 클래스 sakila 형태의 포인터 sk를 초기화

    // 질의 결과에서 레코드(row)별로 하나씩 담을 변수
    MYSQL_ROW row;

    // 모든 레코드를 다 읽을 때까지 반복 수행
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
    // 질의 결과 메모리 해제
    mysql_free_result(res);

    return TRUE;
}


int main()
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
    
    vector<sakila*> vec;
    FILE* fp = NULL;
    char filename[256];
    for ( int i = 0; i < TABLECOUNT; i++)
    { 
        if (!queryData(mysql, tables[i], fields[i], vec)) return 1;  // 위에서 false 리턴 시, 

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