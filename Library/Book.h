#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include "IModel.h"


class Book : public IModel {
private:
	std::string author;
	std::string title;
	std::string publisher;
	std::string category;
	int year = 0;
	int copiesCount = 0;
	int availableCopiesAmount = 0;
	int rating = 0;

public:
	Book() {}

	Book(const std::string& author, const std::string& title, const std::string& publisher, std::string category, int year, int copiesCount, int availableCopiesCount, int rating = 0)
		: author(author), title(title), publisher(publisher), category(category), year(year), copiesCount(copiesCount), availableCopiesAmount(availableCopiesCount), rating(rating)
	{}

	std::string ñreateTableQuery() const override {
		return std::string("CREATE TABLE IF NOT EXISTS books ("
								"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
								"Author TEXT NOT NULL,"
								"Title TEXT NOT NULL,"
								"Publisher TEXT NOT NULL,"
								"Category TEXT NOT NULL,"
								"Year INTEGER NOT NULL,"
								"CopiesCount INTEGER NOT NULL,"
								"AvailableCopiesCount INTEGER NOT NULL,"
								"Rating INTEGER"
								");"
		);
	}

	std::string insertQuery() const override {
		std::stringstream ss;

		ss << "INSERT INTO Books (Author, Title, Publisher, Category, Year, CopiesCount, AvailableCopiesCount, Rating) VALUES ('"
			<< author << "', '" << title << "', '" << publisher << "', '" << category << "', " << year << ", " << copiesCount << ", " << availableCopiesAmount << ", " << rating << ");";

		return ss.str();
	}

	std::string updateQuery(int ID) const override {
		std::stringstream ss;
		
		ss << "UPDATE Books SET "
			"Author = '" << author << "',"
			"Title = '" << title << "',"
			"Publisher = '" << publisher << "',"
			"Category = '" << category << "',"
			"Year = " << year << ","
			"CopiesCount = " << copiesCount << ","
			"AvailableCopiesCount = " << availableCopiesAmount << ","
			"Rating = " << rating << " "
			"WHERE ID = " << ID;

		return ss.str();
	}

	std::string deleteQuery(int ID) const override {
		std::stringstream ss;
		ss << "DELETE FROM Books WHERE ID = " << ID;
		return ss.str();
	}

	std::string selectAllQuery() const override {
		return std::string("SELECT ID, Author, Title, Publisher, Category, Year, CopiesCount, AvailableCopiesCount, Rating FROM books");
	}

	std::string findQuery(const std::string& toFind) const override {
		std::stringstream ss;

		ss << selectAllQuery() <<
			" WHERE REGEXP(Author, '.*" << toFind << ".*')"
			" OR REGEXP(Title, '.*" << toFind << ".*')"
			" OR REGEXP(Publisher, '.*" << toFind << ".*')"
			" OR REGEXP(Category, '.*" << toFind << ".*')"
			" OR REGEXP(CAST(Year AS TEXT), '.*" << toFind << ".*')"
			" OR REGEXP(CAST(Rating AS TEXT), '" << toFind << "')";

		return ss.str();
	}

	std::string getAuthor() const {
		return this->author;
	}

	std::string getTitle() const {
		return this->title;
	}

	std::string getPublisher() const {
		return this->publisher;
	}

	std::string getCategory() const {
		return this->category;
	}

	int getRating() const {
		return this->rating;
	}

	int getYear() const {
		return this->year;
	}

	int getAvailableCopiesAmount() const {
		return this->availableCopiesAmount;
	}


	void setAuthor(const std::string& author) {
		this->author = author;
	}

	void setTitle(const std::string& title) {
		this->title = title;
	}

	void setPublisher(const std::string publisher) {
		this->publisher = publisher;
	}

	void setCategory(std::string category) {
		this->category = category;
	}

	void setRating(int rating) {
		this->rating = rating;
	}

	void setYear(int year) {
		this->year = year;
	}

	void setCopiesCount(int copiesCount) {
		this->copiesCount = copiesCount;
	}

	void setAvailableCopiesAmount(int availableCopiesAmount) {
		this->availableCopiesAmount = availableCopiesAmount;
	}
};