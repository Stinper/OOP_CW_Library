#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <regex>
#include "sqlite3.h"
#include "IModel.h"
#include "Utils.h"


namespace fs = std::filesystem;
void regexp_function(sqlite3_context*, int, sqlite3_value**);


class DataBase {
private:
	static sqlite3* db;

public:
	static void initialize(const std::string& dbName, std::vector<IModel*> tables) {
		if (!fs::exists(dbName)) {
			int status = open(dbName);

			if (status != SQLITE_OK) {
				std::cerr << "Ошибка инициализации базы данных" << std::endl;
				return;
			}

			for (IModel* table : tables)
				runSQL(table->сreateTableQuery());

			close();
		}
	}

	static int open(const std::string& dbName) {
		int status = sqlite3_open(dbName.c_str(), &db);
		sqlite3_create_function(db, "REGEXP", 2, SQLITE_UTF8, nullptr, regexp_function, nullptr, nullptr);
		return status;
	}

	static int close() {
		if (db != nullptr) {
			return sqlite3_close(db);
		}
	}

	static bool isOpen() {
		return db != nullptr;
	}

	static int runSQL(const std::string& sql) {
		char* errMsg = nullptr;

		int status = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

		if (status != SQLITE_OK) {
			std::cerr << "Ошибка выполнени¤ SQL-запроса: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}

		return status;
	}

	static int runTransaction(const std::vector<std::string> queries) {
		runSQL("BEGIN TRANSACTION");

		int status = -1;
		bool allOperationsSuccessful = true;

		for (const std::string& query : queries) {
			status = runSQL(query);
			
			if (status != SQLITE_OK) {
				allOperationsSuccessful = false;
				break;
			}
		}

		if (allOperationsSuccessful) {
			runSQL("COMMIT");
			return SQLITE_OK;
		}

		runSQL("ROLLBACK");
		return status;
	}

	static std::vector<std::vector<std::string>> select(const std::string& selectQuery) {
		std::vector<std::vector<std::string>> values;
		std::vector<std::string> record;

		if (!isOpen())
			return values;

		sqlite3_stmt* stmt;
		int status = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr);

		if (status != SQLITE_OK) {
			std::cerr << "Ошибка при подготовке запроса: " << sqlite3_errmsg(db) << std::endl;
			return values;
		}

		int columnsCount = sqlite3_column_count(stmt);

		while ((status = sqlite3_step(stmt)) == SQLITE_ROW) {
			for (int i = 0; i < columnsCount; i++) {
				record.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i))));
			}

			values.push_back(record);
			record.clear();
		}

		return values;

	}
};

sqlite3* DataBase::db = nullptr;

void regexp_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
	if (argc != 2) {
		return;
	}

	std::string text((const char*)sqlite3_value_text(argv[0]));
	std::string pattern((const char*)sqlite3_value_text(argv[1]));

	std::regex regex(StringHelper::toLowerCase(pattern));
	bool match = std::regex_search(StringHelper::toLowerCase(text), regex);

	sqlite3_result_int(context, match ? 1 : 0);
}