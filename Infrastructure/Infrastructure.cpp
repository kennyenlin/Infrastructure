// Infrastructure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Infrastructure.h"
#include <string>
#include <sstream>

using namespace std;

#pragma comment(lib, "../lib/sqlite3.lib")

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char* argv[])
{
    char *p_search_latitude[2] = {NULL};
    char *p_search_longtitude[2] = {NULL};
    char *p_search_partial_name[2] = {NULL};

    switch(argc)
    {
        case 2:
        {
            p_search_partial_name[0] = strtok(argv[1], "=");
            p_search_partial_name[1] = strtok(NULL, "=");

            if (strcmp(p_search_partial_name[0], "NAME") != 0 ||
                p_search_partial_name[1] == NULL)
            {
                printf("parameter NAME is not correct\n");
                return 0;
            }
        }
        break;
        case 3:
        {
            p_search_latitude[0] = strtok(argv[1], "=");
            p_search_latitude[1] = strtok(NULL, "=");

            p_search_longtitude[0] = strtok(argv[2], "=");
            p_search_longtitude[1] = strtok(NULL, "=");

            if (strcmp(p_search_latitude[0], "LAT") != 0 ||
                strcmp(p_search_longtitude[0], "LNG") != 0 ||
                p_search_latitude[1] == NULL ||
                p_search_longtitude[1] == NULL ||
                !isDouble(p_search_latitude[1]) ||
                !isDouble(p_search_longtitude[1]))
            {
                printf("parameter is not correct\n");
                return 0;
            }
        }
        break;
        case 4:
        {
            p_search_latitude[0] = strtok(argv[1], "=");
            p_search_latitude[1] = strtok(NULL, "=");

            p_search_longtitude[0] = strtok(argv[2], "=");
            p_search_longtitude[1] = strtok(NULL, "=");

            p_search_partial_name[0] = strtok(argv[3], "=");
            p_search_partial_name[1] = strtok(NULL, "=");

            if (strcmp(p_search_latitude[0], "LAT") != 0 ||
                strcmp(p_search_longtitude[0], "LNG") != 0 ||
                strcmp(p_search_partial_name[0], "NAME") != 0 ||
                p_search_latitude[1] == NULL ||
                p_search_longtitude[1] == NULL ||
                p_search_partial_name[1] == NULL||
                !isDouble(p_search_latitude[1]) ||
                !isDouble(p_search_longtitude[1]))
            {
                printf("parameter is not correct\n");
                return 0;
            }
        }
        break;
        default:
        {
            printf("too many parameters");
            return 0;
        }
    }

	sqlite3 *p_sqlite_module = NULL;

    CreateDatabase(&p_sqlite_module);

    char *p_sql_statement = "CREATE TABLE COFFEE_SHOP("  \
                            "ID INT PRIMARY KEY       NOT NULL," \
                            "LAT            REAL      NOT NULL," \
                            "LNG            REAL      NOT NULL," \
                            "NAME           CHAR(50)  NOT NULL," \
                            "PHONE          CHAR(50)          ," \
                            "ADDRESS        CHAR(200)         ," \
                            "CITY           CHAR(50)          ," \
                            "STATE          CHAR(50)  NOT NULL," \
                            "COUNTRY        CHAR(50)  NOT NULL);";

    ExecuteStatement(p_sql_statement, p_sqlite_module);
    
    string line;
    int element_index = 0;

    ifstream file_stream_1("coffee_shops-1.csv");

    getline(file_stream_1, line, '\n');

    ParseData(file_stream_1, p_sqlite_module, element_index);

    ifstream file_stream_2("coffee_shops-2.csv");

    getline(file_stream_2, line, '\n');

    ParseData(file_stream_2, p_sqlite_module, element_index);

    char select_statement[600] = {0};

    if (argc == 2)
        sprintf(select_statement, "SELECT * FROM COFFEE_SHOP WHERE NAME LIKE %s%s%s", "'%"
                                                                                    , p_search_partial_name[1]
                                                                                    , "%'");
    else if (argc == 3)
        sprintf(select_statement, "SELECT * FROM COFFEE_SHOP ORDER BY ((%s - LAT) * (%s - LAT) + (%s - LNG) * (%s - LNG)) ASC", p_search_latitude[1]
                                                                                                                              , p_search_latitude[1]
                                                                                                                              , p_search_longtitude[1]
                                                                                                                              , p_search_longtitude[1]);
    else if (argc == 4)
        sprintf(select_statement, "SELECT * FROM COFFEE_SHOP WHERE NAME LIKE %s%s%s ORDER BY ((%s - LAT) * (%s - LAT) + (%s - LNG) * (%s - LNG)) ASC", "'%"
                                                                                                                                                     , p_search_partial_name[1]
                                                                                                                                                     , "%'"
                                                                                                                                                     , p_search_latitude[1]
                                                                                                                                                     , p_search_latitude[1]
                                                                                                                                                     , p_search_longtitude[1]
                                                                                                                                                     , p_search_longtitude[1]);
    ExecuteStatement(select_statement, p_sqlite_module);

	sqlite3_close(p_sqlite_module);

	return 0;
}

bool CreateDatabase(sqlite3 **pp_sqlite_module)
{
    int error_code = 0;

    system("del coffee_shop.db");
    sqlite3_open("coffee_shop.db", pp_sqlite_module);

    if (error_code)
    {
        fprintf(stderr, "cannot open database: %s\n", sqlite3_errmsg(*pp_sqlite_module));
        return false;
    }else if (NULL  == *pp_sqlite_module)
    {
        fprintf(stderr, "no instance for database");
        return false;
    }

    return true;
}

bool ParseLine(char *p_cstr, std::list<string> &tokens, bool is_null_element[TOKENS_SIZE])
{
    if (p_cstr == NULL)
        return false;

    char *p_local = NULL;

    p_local = new char[strlen(p_cstr) + 1];

     strncpy(p_local, p_cstr, strlen(p_cstr) + 1);

    int token_index = 0;
    char *p_temp = NULL;

    p_temp = strtok(p_local, ",");
    tokens.push_back(p_temp);

    p_temp = strtok(NULL, ",\"");
    tokens.push_back(p_temp);

    token_index += 2;

    p_temp = strtok(NULL, "\"");
    while(p_temp != NULL)
    {
        if (p_temp[0] != ',')
        {
            tokens.push_back(p_temp);
            token_index++;
        }
        else
        {
            for (int i = 1; i < strlen(p_temp) + 1; i++)
            {
                if(p_temp[i] == ',')
                {
                    is_null_element[token_index] = true;
                    token_index++;
                }
            }

        }
        p_temp = strtok(NULL, "\"");
    }

    return true;
}

bool ExecuteStatement(char *p_sql_statement, sqlite3 *p_sqlite_module)
{
    int error_code = 0;

    char *p_error_message = NULL;

    error_code = sqlite3_exec(p_sqlite_module, p_sql_statement, callback, 0, &p_error_message);
    if (error_code != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", p_error_message);
        sqlite3_free(p_error_message);
        return false;
    }

    return true;
}

bool ParseData(std::ifstream &file_stram, sqlite3 *p_sqlite_module, int &element_index)
{
    string line;

    while (getline(file_stram, line, '\n'))
    {
        element_index++;

        char *p_cstr = new char[line.length() + 1];
        strcpy(p_cstr, line.c_str());

        bool is_null_element[TOKENS_SIZE] = {false};

        list<string> tokens;

        if (!ParseLine(p_cstr, tokens, is_null_element))
        {
            delete [] p_cstr;
            return false;
        }

        char *p_insert_statement = "INSERT INTO COFFEE_SHOP (ID, LAT, LNG, NAME, PHONE, ADDRESS, CITY, STATE, COUNTRY)";

        char insert_statement_buffer[600] = {0};
        sprintf(insert_statement_buffer, "%s VALUES (%d", p_insert_statement, element_index);

        for (int i = 0; i < TOKENS_SIZE; i++)
        {
            if (!is_null_element[i])
            {
                if (i == 0 || i == 1)
                    sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ", %f", atof(tokens.front().c_str()));
                else
                    sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ", \"%s\"", tokens.front().c_str());

                tokens.pop_front();
            }
            else
            {
                sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ", NULL");
            }
        }

        sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ");");

        if (!ExecuteStatement(insert_statement_buffer, p_sqlite_module))
        {
            delete [] p_cstr;
            return false;
        }

        delete [] p_cstr;
    }

    return true;
}

bool isDouble(char *p_string)
{
    istringstream prepared_stream(p_string);
    double conversion = 0.0;

    prepared_stream >> noskipws >> conversion;

    return prepared_stream.eof() && !prepared_stream.fail();
}