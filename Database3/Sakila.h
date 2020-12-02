#pragma once
#include <iostream>
#include <string>
#include "mysql.h"
using namespace std;

class sakila
{
public:
	sakila(){} // 생성자
	~sakila(){} // 소멸자

	virtual void print(FILE* fp) = 0; // 가상 함수, 프린트를 하면서 파일을 저장할 수 있게함.
	virtual void save(MYSQL_ROW& row) = 0; // 가사 함수, Mysql에서 행을 가져옴.
};

class actor : public sakila
{
public:
	int actor_id;
	string first_name, last_name, last_update;
public:
	actor() 
	{
		// 위 property에서 선언한 함수들을 생성자 안에서 초기화.
		actor_id = 0;
		first_name = last_name = last_update = "";
	
	}
	~actor(){}

	void print(FILE* fp = NULL) // fp 라는 이름의 FILE 포인터에 NULL 을 초기화.
	{
		if (fp)   // fp 가 NULL이면 문제가 없으니, 파일에 저장
		{
			fprintf(fp, "%d |", actor_id);
			fprintf(fp, "%s |", first_name.c_str());
			fprintf(fp, "%s |", last_name.c_str());
			fprintf(fp, "%s\n", last_update.c_str());
		}
		printf("%d\n", actor_id);
		printf("%s\n", first_name.c_str());
		printf("%s\n", last_name.c_str());
		printf("%s\n", last_update.c_str());
	}
	void save(MYSQL_ROW& row)
	{	
		int idx = 0;
		if (row[idx]) actor_id = atoi(row[idx]); idx++;
		if (row[idx]) first_name = (row[idx]); idx++;
		if (row[idx]) last_name = (row[idx]); idx++;
		if (row[idx]) last_update = (row[idx]); idx++;
	}
};

class actor_info : public sakila
{
public:
	int actor_id;
	string film_info, first_name, last_name;
public:
	actor_info()
	{
		actor_id = 0;
		film_info = first_name = last_name = "";

	}
	~actor_info() {}

	void print(FILE* fp)
	{
		if (fp)
		{
			fprintf(fp, "%d |", actor_id);
			fprintf(fp, "%s |", film_info.c_str());
			fprintf(fp, "%s |", first_name.c_str());
			fprintf(fp, "%s\n", last_name.c_str());

		}
		printf("actor_id	: %d\n", actor_id);
		printf("film_info	: %s\n", film_info.c_str());
		printf("first_name	: %s\n", first_name.c_str());
		printf("last_name	: %s\n", last_name.c_str());
	}
	void save(MYSQL_ROW& row)
	{
		int idx = 0;
		if (row[idx]) actor_id = atoi(row[idx]); idx++;
		if (row[idx]) film_info = (row[idx]); idx++;
		if (row[idx]) first_name = (row[idx]); idx++;
		if (row[idx]) last_name = (row[idx]); idx++;
	}
};

class film : public sakila
{
public:
	
	string film_id, title, description, release_year, language_id;
public:
	film()
	{
		
		film_id, title, description, release_year, language_id = "";

	}
	~film() {}

	void print(FILE* fp)
	{
		if (fp)
		{
			fprintf(fp, "%s |", film_id.c_str());
			fprintf(fp, "%s |", title.c_str());
			fprintf(fp, "%s |", description.c_str());
			fprintf(fp, "%s |", release_year.c_str());
			fprintf(fp, "%s\n", language_id.c_str());

		}
		printf("film_id		: %s\n", film_id.c_str());
		printf("title		: %s\n", title.c_str());
		printf("description	: %s\n", description.c_str());
		printf("release_year	: %s\n", release_year.c_str());
		printf("language_id	: %s\n", language_id.c_str());
	}
	void save(MYSQL_ROW& row)
	{
		int idx = 0;
		if (row[idx]) film_id = (row[idx]); idx++;
		if (row[idx]) title = (row[idx]); idx++;
		if (row[idx]) description = (row[idx]); idx++;
		if (row[idx]) release_year = (row[idx]); idx++;
		if (row[idx]) language_id = (row[idx]); 
	}
};

class customer : public sakila
{
public:
	string active, address_id, create_date;
public:
	customer()
	{
		active = address_id = create_date = "";
	}
	~customer(){}
	
	void print(FILE* fp)
	{
		if (fp)
		{
			fprintf(fp, "%s |", active.c_str());
			fprintf(fp, "%s |", address_id.c_str());
			fprintf(fp, "%s\n", create_date.c_str());
		}
		printf("active		: %s\n", active.c_str());
		printf("address_id	: %s\n", address_id.c_str());
		printf("create_id	: %s\n", create_date.c_str());
	}
	void save(MYSQL_ROW& row)
	{
		int idx = 0;
		if (row[idx]) active = (row[idx]); idx++;
		if (row[idx]) address_id = (row[idx]); idx++;
		if (row[idx]) create_date = (row[idx]);
	}
};

class payment : public sakila
{
public: 
	string payment_id, rental_id, amount;
public:
	payment() 
	{
		payment_id = rental_id = amount = "";
	}
	~payment() {}

	void print(FILE* fp)
	{
		if (fp)
		{
			fprintf(fp, "%s |", payment_id.c_str());
			fprintf(fp, "%s |", rental_id.c_str());
			fprintf(fp, "%s\n", amount.c_str());
		}
		printf("pamyment_id		: %s\n", payment_id.c_str());
		printf("rental_id		: %s\n", rental_id.c_str());
		printf("amount			: %s\n", amount.c_str());
	}

	void save(MYSQL_ROW& row)
	{
		int idx = 0;
		if (row[idx]) payment_id = (row[idx]); idx++;
		if (row[idx]) rental_id = (row[idx]); idx++;
		if (row[idx]) amount = (row[idx]); 
	}
};

class store : public sakila
{
public:
	string store_id, manager_staff_id, address_id, last_update;
public:
	store() 
	{
		store_id = manager_staff_id = address_id = last_update = "";
	}
	~store() {}

	void printf(FILE* fp)
	{
		if (fp)
		{
			fprintf(fp, "%s |", store_id.c_str());
			fprintf(fp, "%s |", manager_staff_id.c_str());
			fprintf(fp, "%s |", address_id.c_str());
			fprintf(fp, "%s\n", last_update.c_str());
		}
	}
	void save()
};
