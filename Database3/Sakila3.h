#pragma once
#include <vector>
#include <string>
#include "mysql.h"

using namespace std;

//==============================================================================
// 무식하게 한개의 데이터 타입으로 저장하는 형태

class SakilaData
{
public:
    vector<string> values;
public:
    SakilaData() {}
    ~SakilaData() {}

    void print(FILE* fp = NULL)
    {
        for (auto t : values)
        {
            if (fp) fprintf(fp, "%s|", t.c_str());
            printf("%s\n", t.c_str());
        }
    }
    void save(MYSQL_ROW& row, int count)
    {
        for (int i = 0; i < count; i++)
        {
            if (row[i])     values.push_back(row[i]);
            else            values.push_back("");
        }
    }
};

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

class SakilaDataU
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

//==============================================================================

class Sakila // 추상 클래스
{
public:
    Sakila() {}
    ~Sakila() {}

    virtual void print(FILE* fp) = 0;   // 순수 가상 함수
    virtual void save(MYSQL_ROW& row) = 0;

    void writeString(FILE* fp, string& str)
    {
        int len = (int)str.size();
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(str.c_str(), len, 1, fp);
    }
};

class Actor : public Sakila
{
public:
    int actor_id;
    string first_name, last_name, last_update;
public:
    Actor()
    {
        actor_id = -1;
        first_name = last_name = last_update = "";
    }
    ~Actor() {}

    void print(FILE* fp = NULL)
    {
        if (fp)
        {
#ifdef BINARYMODE
            fwrite(&actor_id, sizeof(actor_id), 1, fp);
            writeString(fp, first_name);
            writeString(fp, last_name);
            writeString(fp, last_update);
#else
            fprintf(fp, "%d|", actor_id);
            fprintf(fp, "%s|", first_name.c_str());
            fprintf(fp, "%s|", last_name.c_str());
            fprintf(fp, "%s\n", last_update.c_str());
#endif
        }
        printf("actor_id   :%d\n", actor_id);
        printf("first_name :%s\n", first_name.c_str());
        printf("last_name  :%s\n", last_name.c_str());
        printf("last_update:%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx])   actor_id = atoi(row[idx]);   idx++;
        if (row[idx])   first_name = row[idx];         idx++;
        if (row[idx])   last_name = row[idx];         idx++;
        if (row[idx])   last_update = row[idx];
    }
};
class Address : public Sakila
{
public:
    int address_id;
    string address, address2, district;
    int city_id;
    string gpostal_code, location, last_update;
public:
    Address()
    {
        address_id = 0;
        address = address2 = district = "";
        city_id = 0;
        gpostal_code = location = last_update = "";
    }
    ~Address() {}

    void print(FILE* fp)
    {
        if (fp)
        {
            fprintf(fp, "%d|", address_id);
            fprintf(fp, "%s|", address.c_str());
            fprintf(fp, "%s|", address2.c_str());
            fprintf(fp, "%s|", district.c_str());
            fprintf(fp, "%d|", city_id);
            fprintf(fp, "%s|", gpostal_code.c_str());
            fprintf(fp, "%s|", location.c_str());
            fprintf(fp, "%s\n", last_update.c_str());
        }
        printf("address_id  :%d\n", address_id);
        printf("address     :%s\n", address.c_str());
        printf("address2    :%s\n", address2.c_str());
        printf("district    :%s\n", district.c_str());
        printf("city_id     :%d\n", city_id);
        printf("gpostal_code:%s\n", gpostal_code.c_str());
        printf("location    :%s\n", location.c_str());
        printf("last_update :%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx])   address_id = atoi(row[idx]); idx++;
        if (row[idx])   address = row[idx];         idx++;
        if (row[idx])   address2 = row[idx];         idx++;
        if (row[idx])   district = row[idx];         idx++;
        if (row[idx])   city_id = atoi(row[idx]);    idx++;
        if (row[idx])   gpostal_code = row[idx];     idx++;
        if (row[idx])   location = row[idx];         idx++;
        if (row[idx])   last_update = row[idx];
    }
};
class Category : public Sakila
{
public:
    int category_id;
    string name, last_update;
public:
    Category()
    {
        category_id = 0;
        name = last_update = "";
    }
    ~Category() {}

    void print(FILE* fp)
    {
        if (fp)
        {
            fprintf(fp, "%d|", category_id);
            fprintf(fp, "%s|", name.c_str());
            fprintf(fp, "%s\n", last_update.c_str());
        }
        printf("%d\n", category_id);
        printf("%s\n", name.c_str());
        printf("%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx])   category_id = atoi(row[idx]); idx++;
        if (row[idx])   name = row[idx];              idx++;
        if (row[idx])   last_update = row[idx];
    }
};
class City : public Sakila
{
public:
    int city_id;
    string city;
    int country_id;
    string last_update;
public:
    City()
    {
        city_id = 0;
        city = "";
        country_id = 0;
        last_update = "";

    }
    ~City() {}

    void print(FILE* fp)
    {
        if (fp)
        {
            fprintf(fp, "%d|", city_id);
            fprintf(fp, "%s|", city.c_str());
            fprintf(fp, "%d|", country_id);
            fprintf(fp, "%s\n", last_update.c_str());
        }
        printf("%d\n", city_id);
        printf("%s\n", city.c_str());
        printf("%d\n", country_id);
        printf("%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx])   city_id = atoi(row[idx]);       idx++;
        if (row[idx])   city = row[idx];                idx++;
        if (row[idx])   country_id = atoi(row[idx]);    idx++;
        if (row[idx])   last_update = row[idx];
    }
};

class Country : public Sakila
{
public:
    int country_id;
    string country, last_update;
public:
    Country()
    {
        country_id = 0;
        country = last_update = "";
    }
    ~Country() {}

    void print(FILE* fp)
    {
        if (fp)
        {
            fprintf(fp, "%d|", country_id);
            fprintf(fp, "%s|", country.c_str());
            fprintf(fp, "%s\n", last_update.c_str());
        }
        printf("%d\n", country_id);
        printf("%s\n", country.c_str());
        printf("%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx])   country_id = atoi(row[idx]);       idx++;
        if (row[idx])   country = row[idx];                idx++;
        if (row[idx])   last_update = row[idx];
    }
};

class Customer : public Sakila
{
public:
    int customer_id, store_id;
    string first_name, last_name, email;
    int address_id, active;
    string create_date, last_update;
public:
    Customer()
    {
        customer_id = store_id = 0;
        first_name = last_name = email = "";
        address_id = active = 0;
        create_date = last_update = "";

    }
    ~Customer() {}

    void print(FILE* fp)
    {
        if (fp)
        {
            fprintf(fp, "%d|", customer_id);
            fprintf(fp, "%d|", store_id);
            fprintf(fp, "%s|", first_name.c_str());
            fprintf(fp, "%s|", last_name.c_str());
            fprintf(fp, "%s|", email.c_str());
            fprintf(fp, "%d|", address_id);
            fprintf(fp, "%d|", active);
            fprintf(fp, "%s|", create_date.c_str());
            fprintf(fp, "%s\n", last_update.c_str());
        }
        printf("%d\n", customer_id);
        printf("%d\n", store_id);
        printf("%s\n", first_name.c_str());
        printf("%s\n", last_name.c_str());
        printf("%s\n", email.c_str());
        printf("%d\n", address_id);
        printf("%d\n", active);
        printf("%s\n", create_date.c_str());
        printf("%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx]) customer_id = atoi(row[idx]);     idx++;
        if (row[idx]) store_id = atoi(row[idx]);        idx++;
        if (row[idx]) first_name = row[idx];            idx++;
        if (row[idx]) last_name = row[idx];             idx++;
        if (row[idx]) email = row[idx];                 idx++;
        if (row[idx]) address_id = atoi(row[idx]);      idx++;
        if (row[idx]) active = atoi(row[idx]);          idx++;
        if (row[idx]) create_date = row[idx];           idx++;
        if (row[idx]) last_update = row[idx];
    }
};
//film_id smallint UN AI PK
//title varchar(255)
//description text
//release_year year
//language_id tinyint UN
//original_language_id tinyint UN
//rental_duration tinyint UN
//rental_rate decimal(4, 2)
//length smallint UN
//replacement_cost decimal(5, 2)
//rating enum('G', 'PG', 'PG-13', 'R', 'NC-17')
//special_features set('Trailers', 'Commentaries', 'Deleted Scenes', 'Behind the Scenes')
//last_update timestamp
class Film : public Sakila
{
public:
    int film_id;
    string title, description, release_year;
    int language_id, original_language_id, rental_duration, rental_rate, length, replacement_cost;
    string rating, special_features, last_update;
public:
    Film() {}
    ~Film() {}

    void print(FILE* fp)
    {
        if (fp)
        {
            fprintf(fp, "%d|", film_id);
            fprintf(fp, "%s|", title.c_str());
            fprintf(fp, "%s|", description.c_str());
            fprintf(fp, "%s|", release_year.c_str());
            fprintf(fp, "%d|", language_id);
            fprintf(fp, "%d|", original_language_id);
            fprintf(fp, "%d|", rental_duration);
            fprintf(fp, "%d|", rental_rate);
            fprintf(fp, "%d|", length);
            fprintf(fp, "%d|", replacement_cost);
            fprintf(fp, "%s|", rating.c_str());
            fprintf(fp, "%s|", special_features.c_str());
            fprintf(fp, "%s\n", last_update.c_str());
        }
        printf("%d\n", film_id);
        printf("%s\n", title.c_str());
        printf("%s\n", description.c_str());
        printf("%s\n", release_year.c_str());
        printf("%d\n", language_id);
        printf("%d\n", original_language_id);
        printf("%d\n", rental_duration);
        printf("%d\n", rental_rate);
        printf("%d\n", length);
        printf("%d\n", replacement_cost);
        printf("%s\n", rating.c_str());
        printf("%s\n", special_features.c_str());
        printf("%s\n", last_update.c_str());
    }
    void save(MYSQL_ROW& row)
    {
        int idx = 0;
        if (row[idx]) film_id = atoi(row[idx]);             idx++;
        if (row[idx]) title = row[idx];                     idx++;
        if (row[idx]) description = row[idx];               idx++;
        if (row[idx]) release_year = row[idx];              idx++;
        if (row[idx]) language_id = atoi(row[idx]);         idx++;
        if (row[idx]) original_language_id = atoi(row[idx]);idx++;
        if (row[idx]) rental_duration = atoi(row[idx]);     idx++;
        if (row[idx]) rental_rate = atoi(row[idx]);         idx++;
        if (row[idx]) length = atoi(row[idx]);              idx++;
        if (row[idx]) replacement_cost = atoi(row[idx]);    idx++;
        if (row[idx]) rating = row[idx];                    idx++;
        if (row[idx]) special_features = row[idx];          idx++;
        if (row[idx]) last_update = row[idx];
    }
};

class film_actor : public Sakila
{
public:
    film_actor() {}
    ~film_actor() {}
};

class film_category : public Sakila
{
public:
    film_category() {}
    ~film_category() {}
};

class film_text : public Sakila
{
public:
    film_text() {}
    ~film_text() {}
};

class inventory : public Sakila
{
public:
    inventory() {}
    ~inventory() {}
};

class language : public Sakila
{
public:
    language() {}
    ~language() {}
};

class payment : public Sakila
{
public:
    payment() {}
    ~payment() {}
};

class rental : public Sakila
{
public:
    rental() {}
    ~rental() {}
};

class staff : public Sakila
{
public:
    staff() {}
    ~staff() {}
};

class store : public Sakila
{
public:
    store() {}
    ~store() {}
};
