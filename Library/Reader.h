#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include "IModel.h"


class Reader : public IModel {
private:
	std::string FIO;
	std::string phoneNumber;
	int age = 0;

public:
	Reader() {}

	Reader(const std::string& FIO, const std::string& phoneNumber, int age)
		: FIO(FIO), phoneNumber(phoneNumber), age(age)
	{}

	std::string ñreateTableQuery() const override {
		return std::string("CREATE TABLE IF NOT EXISTS readers ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"FIO TEXT NOT NULL,"
			"PhoneNumber TEXT NOT NULL,"
			"Age INTEGER NOT NULL);"
		);
	}

	std::string insertQuery() const override {
		std::stringstream ss;

		ss << "INSERT INTO readers (FIO, phoneNumber, age) VALUES ('"
			<< FIO << "', '" << phoneNumber << "', " << age << ");";

		return ss.str();
	}

	std::string updateQuery(int ID) const override {
		std::stringstream ss;

		ss << "UPDATE readers SET "
			"FIO = '" << FIO << "',"
			"phoneNumber = '" << phoneNumber << "',"
			"age = " << age << " "
			"WHERE ID = " << ID;

		return ss.str();
	}

	std::string deleteQuery(int ID) const override {
		std::stringstream ss;
		ss << "DELETE FROM readers WHERE ID = " << ID;
		return ss.str();
	}

	std::string selectAllQuery() const override {
		return std::string("SELECT ID, FIO, phoneNumber, age FROM readers");
	}

	std::string findQuery(const std::string& toFind) const override {
		std::stringstream ss;

		ss << selectAllQuery() <<
			" WHERE REGEXP(FIO, '.*" << toFind << ".*')"
			" OR REGEXP(phoneNumber, '.*" << toFind << ".*')"
			" OR REGEXP(CAST(Age AS TEXT), '" << toFind << "')";

		return ss.str();
	}

	std::string getFIO() const {
		return this->FIO;
	}

	std::string getPhoneNumber() const {
		return this->phoneNumber;
	}

	int getAge() const {
		return this->age;
	}


	void setFIO(const std::string& FIO) {
		this->FIO = FIO;
	}

	void setPhoneNumber(std::string& phoneNumber) {
		this->phoneNumber = phoneNumber;
	}

	void setAge(int age) {
		this->age = age;
	}
};
