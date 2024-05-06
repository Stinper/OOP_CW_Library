#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include "IModel.h"
#include "Utils.h"


class Queue : public IModel {
private:
	int readerID = 0;
	int bookID = 0;
	std::string dateOfQueuing;

public:
	Queue () {}
	Queue(int readerID, int bookID, const std::string& date)
		: readerID(readerID), bookID(bookID)
	{
		this->dateOfQueuing = DateHelper::toDataBaseFormat(date);
	}

	std::string ñreateTableQuery() const override {
		return std::string("CREATE TABLE IF NOT EXISTS queue ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ReaderID INTEGER NOT NULL,"
			"BookID INTEGER NOT NULL,"
			"dateOfQueuing DATE NOT NULL,"
			"BookIssued INTEGER NOT NULL DEFAULT 0,"
			"FOREIGN KEY (ReaderID) REFERENCES readers(ID),"
			"FOREIGN KEY (BookID) REFERENCES books(ID)"
			");"
		);
	}

	std::string insertQuery() const override {
		std::stringstream ss;

		ss << "INSERT INTO queue (ReaderID, BookID, dateOfQueuing) VALUES (" <<
			readerID << ", " << bookID << ", '" << dateOfQueuing << "');";

		return ss.str();
	}

	std::string updateQuery(int ID) const override {
		std::stringstream ss;

		ss << "UPDATE queue SET "
			"ReaderID = " << readerID << ","
			"BookID = " << bookID << ","
			"IssueDate = '" << dateOfQueuing << "';";

		return ss.str();
	}

	std::string deleteQuery(int ID) const override {
		std::stringstream ss;
		ss << "DELETE FROM queue WHERE ID = " << ID;
		return ss.str();
	}

	std::string selectAllQuery() const override {
		return std::string("SELECT ID, ReaderID, BookID, dateOfQueuing, BookIssued FROM queue");
	}

	std::string selectAllVerboseQuery() const {
		return std::string("SELECT q.ID, r.FIO AS Reader, b.title as Book, q.dateOfQueuing, q.BookIssued "
			"FROM queue q "
			"INNER JOIN readers r ON (q.ReaderID = r.id) "
			"INNER JOIN books b ON (q.BookID = b.id)"
		);
	}

	std::string findQuery(const std::string& toFind) const override {
		std::stringstream ss;

		ss << selectAllVerboseQuery() <<
			" WHERE REGEXP(Reader, '.*" << toFind << ".*')"
			" OR REGEXP(Book, '.*" << toFind << ".*')"
			" OR REGEXP(CAST(dateOfQueuing AS TEXT), '.*" << toFind << ".*');";

		return ss.str();
	}

	std::string findByIDQuery(int ID) const {
		std::stringstream ss;
		ss << selectAllQuery() << " WHERE ID = " << ID;

		return ss.str();
	}

	int getBookID() const {
		return bookID;
	}

	int getReaderID() const {
		return readerID;
	}
	
	void setBookID(int bookID) {
		this->bookID = bookID;
	}

	void setReaderID(int readerID) {
		this->readerID = readerID;
	}

	void setDateOfQueuing(const std::string& date) {
		this->dateOfQueuing = date;
	}

};