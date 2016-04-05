// Infrastructure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <list>
#include <fstream>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void ParseLine(char *p_cstr, char **pp_tokens, int token_number)
{
    pp_tokens[0] = strtok(p_cstr, ",\"");

    for (int token_index = 1; token_index < token_number; token_index++)
        pp_tokens[token_index] = strtok(NULL, ",\"");

//     char *p_token = strtok(p_cstr, ",\"");
// 
//     while (p_token != NULL)
//     {
//         tokens.push_back(p_token);
//         p_token = strtok (NULL, ",\"");
//     }
}

#define TOKENS_SIZE 8

void IsNullElement(char *p_cstr, int length, bool is_null_element[TOKENS_SIZE])
{
    int seperation_location[TOKENS_SIZE - 1] = {0};

    int seperation_index = 0;
    for (int i = 0; i < length; i++)
    {
        if (p_cstr[i] == ',')
        {
            seperation_location[seperation_index] = i;
            seperation_index ++;
        }
    }

    int seperation_difference[TOKENS_SIZE] = {0};

    seperation_difference[0] = seperation_location[0] - (-1);

    for (int i = 1; i < TOKENS_SIZE; i++)
        seperation_difference[i] = seperation_location[i] - seperation_location[i - 1];

    for (int i = 0; i < TOKENS_SIZE; i++)
        if (seperation_difference[i] == 1)
            is_null_element[i] = true;
}

void ExecuteStatement(char *p_sql_statement, sqlite3 *p_sqlite_module)
{
    int error_code = 0;

    char *p_error_message = NULL;

    error_code = sqlite3_exec(p_sqlite_module, p_sql_statement, callback, 0, &p_error_message);
    if (error_code != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", p_error_message);
        sqlite3_free(p_error_message);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
}

void ParseData(std::ifstream &file_stram, sqlite3 *p_sqlite_module)
{
    string line;
//    list<char *> tokens;

    int data_index = 0;
    //int tokens_size = 8;

    while (getline(file_stram, line, '\n'))
    {
        data_index++;

        char *p_cstr = new char[line.length() + 1];
        strcpy(p_cstr, line.c_str());

        bool is_null_element[TOKENS_SIZE] = {false};

        IsNullElement(p_cstr, line.length() + 1, is_null_element);

        char **pp_tokens = NULL;
        pp_tokens = new char*[TOKENS_SIZE];
        for (int i = 0; i < TOKENS_SIZE; i++)
            pp_tokens[i] = NULL;

        ParseLine(p_cstr, pp_tokens, TOKENS_SIZE);

        char *p_insert_statement = "INSERT INTO COFFEE_SHOP (ID, LAT, LNG, NAME, PHONE, ADDRESS, CITY, STATE, COUNTRY)";


//         for (int i = 0; i < tokens_size; i++)
//             printf("%s\n", pp_tokens[i]);

//         int token_index = 0;
// 
//         while (!tokens.empty())
//         {
//             pp_tokens[token_index] = tokens.front();
//             tokens.pop_front();
//             token_index++;
//         }
// 
//         printf("token_index = %d\n", token_index);

        char insert_statement_buffer[600] = {0};
        sprintf(insert_statement_buffer, "%s VALUES (%d", p_insert_statement, data_index);

        int valid_token_index = 0;
        for (int i = 0; i < TOKENS_SIZE; i++)
        {
            if (!is_null_element[i])
            {
                if (i == 0 || i == 1)
                    sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ", %f", atof(pp_tokens[valid_token_index]));
                else
                    sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ", '%s'", pp_tokens[valid_token_index]);

                valid_token_index ++;
            }
            else
            {
                sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ", NULL");
            }
        }

        sprintf(insert_statement_buffer + strlen(insert_statement_buffer), ");");

//         sprintf(insert_statement_buffer, "%s VALUES (%d, %f, %f, %s, %s, %s, %s, %s, %s);", p_insert_statement
//                                                                                           , data_index
//                                                                                           , atof(pp_tokens[0])
//                                                                                           , atof(pp_tokens[1])
//                                                                                           , pp_tokens[2]
//                                                                                           , pp_tokens[3]
//                                                                                           , pp_tokens[4]
//                                                                                           , pp_tokens[5]
//                                                                                           , pp_tokens[6]
//                                                                                           , pp_tokens[7]);

        ExecuteStatement(insert_statement_buffer, p_sqlite_module);

        printf("%s\n", insert_statement_buffer);

        delete [] p_cstr;
        delete [] pp_tokens;

    }
}

// void ParseStatement(std::ifstream &file_stram, sqlite3 *p_sqlite_module)
// {
//     string line;
//     list<char *> tokens;
// 
//     if (getline(file_stram, line, '\n'))
//     {
//         char *p_cstr = new char[line.length() + 1];
//         strcpy(p_cstr, line.c_str());
// 
//         ParseLine(p_cstr, tokens);
// 
//         string sql_statement = "CREATE TABLE COFFEE_SHOPS(";
//         string latitude_postfix = " INT NOT NULL,";
//         string longtitude_postfix = " INT NOT NULL,";
//         string name_postfix = " CHAR[100] NOT NULL,";
//         string phone_postfix = " INT NOT NULL,";
//         string address_postfix = " CHAR[200] NOT NULL,";
//         string city_postfix = " CHAR[200] NOT NULL,";
//         string county_post_fix = " CHAR[200] NOT NULL";
// 
// 
//         //printf("tokens number = %d\n", tokens.size())
// 
//         while (!tokens.empty())
//         {
//             printf("%s@", tokens.front());
//             tokens.pop_front();
//         }
//         printf("\n");
// 
//         delete [] p_cstr;
// 
//     }
// }

int _tmain(int argc, _TCHAR* argv[])
{
	sqlite3 *p_sqlite_module = NULL;
	int error_code = 0;

	sqlite3_open("coffee_shop.db", &p_sqlite_module);

	if (error_code)
	{
		fprintf(stderr, "cannot open database: %s\n", sqlite3_errmsg(p_sqlite_module));
		return 0;
	}else if (NULL  == p_sqlite_module)
	{
		fprintf(stderr, "no instance for database");
		return 0;
	}

	std::ifstream file_stream("coffee_shops-1.csv");

    char *p_sql_statement = "CREATE TABLE COFFEE_SHOP("  \
                            "ID INT PRIMARY KEY       NOT NULL," \
                            "LAT            REAL      NOT NULL," \
                            "LNG            REAL      NOT NULL," \
                            "NAME           CHAR(50)  NOT NULL," \
                            "PHONE          CHAR(50)," \
                            "ADDRESS        CHAR(200) NOT NULL," \
                            "CITY           CHAR(50)  NOT NULL," \
                            "STATE          CHAR(50)  NOT NULL," \
                            "COUNTRY        CHAR(50)  NOT NULL);";

    ExecuteStatement(p_sql_statement, p_sqlite_module);
    
    string line;

    getline(file_stream, line, '\n');

    ParseData(file_stream, p_sqlite_module);

//     char *p_error_message = NULL;
// 
//     error_code = sqlite3_exec(p_sqlite_module, p_sql_statement, callback, 0, &p_error_message);
//     if (error_code != SQLITE_OK)
//     {
//         fprintf(stderr, "SQL error: %s\n", p_error_message);
//         sqlite3_free(p_error_message);
//     }
//     else
//     {
//         fprintf(stdout, "Table created successfully\n");
//     }

//    ParseStatement(file_stream, p_sqlite_module);

// 	std::string line;
// 
// 	std::list<char *> tokens;
// 
// 	char *p_tokens = NULL;
// 
// 	while(std::getline(file_stream, line, '\n'))
// 	{
// 		char *cstr = new char[line.length() + 1];
// 		strcpy(cstr, line.c_str());
// 
// 		p_tokens = strtok(cstr, ",\"");
// 
// 		while (p_tokens != NULL)
// 		{
// 			tokens.push_back(p_tokens);
// 			//printf ("%s", p_tokens);
// 			p_tokens = strtok (NULL, ",\"");
// 			//tokens.push_back(p_tokens);
// 		}
// 
// 		while (!tokens.empty())
// 		{
// 			printf("%s@", tokens.front());
// 			tokens.pop_front();
// 		}
// 		printf("\n");
// 
// 		delete [] cstr;
// 	}

	sqlite3_close(p_sqlite_module);

	system("pause");

	return 0;
}



