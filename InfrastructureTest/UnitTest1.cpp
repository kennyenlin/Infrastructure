#include "stdafx.h"
#include <sstream>
#include "../Infrastructure/Infrastructure.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace InfrastructureTest
{
	[TestClass]
	public ref class UnitTest1
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		[TestMethod]
		void TestMethod_CreateDatabase()
		{
            sqlite3 *p_sqlite_module = NULL;
            Assert::IsTrue(CreateDatabase(&p_sqlite_module));
            Assert::IsTrue(CreateDatabase(&p_sqlite_module));
		};

        [TestMethod]
        void TestMethod_ParseLine()
        {
            char *p_cstr = "-37.863216,145.077914,\"Milano Espresso Bar\",,\"170 High St.\",\"Ashburton\",\"VIC\",\"Australia\"";
            std::list<std::string> tokens;
            bool is_null_element[TOKENS_SIZE] = {false};

            Assert::IsTrue(ParseLine(p_cstr, tokens, is_null_element));

            Assert::IsTrue(strcmp("-37.863216", tokens.front().c_str()) == 0);
            tokens.pop_front();
            Assert::IsTrue(strcmp("145.077914", tokens.front().c_str()) == 0);
            tokens.pop_front();
            Assert::IsTrue(strcmp("Milano Espresso Bar", tokens.front().c_str()) == 0);
            tokens.pop_front();
            Assert::IsTrue(strcmp("170 High St.", tokens.front().c_str()) == 0);
            tokens.pop_front();
            Assert::IsTrue(strcmp("Ashburton", tokens.front().c_str()) == 0);
            tokens.pop_front();
            Assert::IsTrue(strcmp("VIC", tokens.front().c_str()) == 0);
            tokens.pop_front();
            Assert::IsTrue(strcmp("Australia", tokens.front().c_str()) == 0);
            tokens.pop_front();

            Assert::IsTrue(is_null_element[0] == false &&
                           is_null_element[1] == false &&
                           is_null_element[2] == false &&
                           is_null_element[3] == true &&
                           is_null_element[4] == false &&
                           is_null_element[5] == false &&
                           is_null_element[6] == false &&
                           is_null_element[7] == false );
        };

        [TestMethod]
        void TestMethod_ExecuteStatement()
        {
            sqlite3 *p_sqlite_module = NULL;
            Assert::IsTrue(CreateDatabase(&p_sqlite_module));

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
            Assert::IsTrue(ExecuteStatement(p_sql_statement, p_sqlite_module));

            char *p_insert_statement = "INSERT INTO COFFEE_SHOP (ID, LAT, LNG, NAME, PHONE, ADDRESS, CITY, STATE, COUNTRY) VALUES (11, -37.863216, 145.077914, \"Milano Espresso Bar\", NULL, \"170 High St.\", \"Ashburton\", \"VIC\", \"Australia\");";

            Assert::IsTrue(ExecuteStatement(p_insert_statement, p_sqlite_module));

            sqlite3_close(p_sqlite_module);
        };

        [TestMethod]
        void TestMethod_ParseData()
        {
            sqlite3 *p_sqlite_module = NULL;
            Assert::IsTrue(CreateDatabase(&p_sqlite_module));

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

            Assert::IsTrue(ExecuteStatement(p_sql_statement, p_sqlite_module));

            std::string line;
            int element_index = 0;

            std::ifstream file_stream_1("coffee_shops-1.csv");

            getline(file_stream_1, line, '\n');

            Assert::IsTrue(ParseData(file_stream_1, p_sqlite_module, element_index));
        };
	};
}
