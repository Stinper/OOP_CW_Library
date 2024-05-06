#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include "IModel.h"
#include "Utils.h"

class IssuedBook : public IModel {
private:
	int readerID = 0;
	int bookID = 0;
	std::string issueDate;
	std::string returnDate;

public:

	IssuedBook() {}

	IssuedBook(int readerID, int bookID, const std::string& issueDate, const std::string& returnDate)
		: readerID(readerID), bookID(bookID)
	{
		this->issueDate = DateHelper::toDataBaseFormat(issueDate);
		this->returnDate = DateHelper::toDataBaseFormat(returnDate);
	}

	std::string сreateTableQuery() const override {
		return std::string("CREATE TABLE IF NOT EXISTS issued_books ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"ReaderID INTEGER NOT NULL,"
			"BookID INTEGER NOT NULL,"
			"IssueDate DATE NOT NULL,"
			"ReturnDate DATE NOT NULL,"
			"Returned INTEGER DEFAULT 0,"
			"FOREIGN KEY (ReaderID) REFERENCES readers(ID),"
			"FOREIGN KEY (BookID) REFERENCES books(ID),"
			"CHECK (IssueDate <= ReturnDate)"
			");"
		);
	}

	std::string insertQuery() const override {
		std::stringstream ss;

		ss << "INSERT INTO issued_books (ReaderID, BookID, IssueDate, ReturnDate) VALUES (" <<
			readerID << ", " << bookID << ", '" << issueDate << "', '" << returnDate << "');";

		return ss.str();
	}

	std::string updateQuery(int ID) const override {
		std::stringstream ss;

		ss << "UPDATE issued_books SET "
			"ReaderID = " << readerID << ","
			"BookID = " << bookID << ","
			"IssueDate = '" << issueDate << "',"
			"ReturnDate = '" << returnDate << "' "
			"WHERE ID = " << ID;

		return ss.str();
	}

	std::string deleteQuery(int ID) const override {
		std::stringstream ss;
		ss << "DELETE FROM issued_books WHERE ID = " << ID;
		return ss.str();
	}

	std::string selectAllQuery() const override {
		return std::string("SELECT ID, ReaderID, BookID, IssueDate, ReturnDate, Returned FROM issued_books");
	}

	std::string selectAllVerboseQuery() const {
		return std::string("SELECT ib.ID, r.FIO AS Reader, b.title as Book, ib.IssueDate, ib.ReturnDate, ib.Returned "
			"FROM issued_books ib "
			"INNER JOIN readers r ON (ib.ReaderID = r.id) "
			"INNER JOIN books b ON (ib.BookID = b.id)"
		);
	}

	std::string findQuery(const std::string& toFind) const override {
		std::stringstream ss;

		ss << selectAllVerboseQuery() <<
			" WHERE REGEXP(Reader, '.*" << toFind << ".*')"
			" OR REGEXP(Book, '.*" << toFind << ".*')"
			" OR REGEXP(CAST(IssueDate AS TEXT), '.*" << toFind << ".*')"
			" OR REGEXP(CAST(ReturnDate AS TEXT), '.*" << toFind << ".*');";

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

	void setIssueDate(const std::string& issueDate) {
		if (!DateHelper::isCorrectDate(issueDate)) {
			throw std::exception("Дата введена некорректно");
		}

		this->issueDate = DateHelper::toDataBaseFormat(issueDate);
	}

	void setReturnDate(const std::string& returnDate) {
		if (!DateHelper::isCorrectDate(returnDate)) {
			throw std::exception("Дата введена некорректно");
		}

		this->returnDate = DateHelper::toDataBaseFormat(returnDate);
	}

};
