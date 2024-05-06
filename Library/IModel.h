#pragma once

#include <string>
#include <sstream>
#include <vector>

class IModel {
public:
	virtual std::string ñreateTableQuery() const = 0;
	virtual std::string insertQuery() const = 0;
	virtual std::string updateQuery(int ID) const = 0;
	virtual std::string deleteQuery(int ID) const = 0;
	virtual std::string selectAllQuery() const = 0;
	virtual std::string findQuery(const std::string& toFind) const = 0;

	virtual std::string findByIDQuery(int ID) const {
		std::stringstream ss;
		ss << selectAllQuery() << " WHERE ID = " << ID;

		return ss.str();
	}
};

