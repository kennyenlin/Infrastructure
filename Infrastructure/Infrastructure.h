#pragma once

#include "sqlite3.h"
#include <list>
#include <fstream>

#define TOKENS_SIZE 8

bool CreateDatabase(sqlite3 **pp_sqlite_module);

bool ParseLine(char *p_cstr, std::list<std::string> &tokens, bool is_null_element[TOKENS_SIZE]);

void ExecuteStatement(char *p_sql_statement, sqlite3 *p_sqlite_module);

void ParseData(std::ifstream &file_stram, sqlite3 *p_sqlite_module, int &element_index);

bool isDouble(char *p_string);