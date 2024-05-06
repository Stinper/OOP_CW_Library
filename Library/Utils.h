#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

#undef max

class DateHelper {
public:
    static bool isCorrectDate(const std::string& date) {
        return toDataBaseFormat(date) != "";
    }

    static std::string toDataBaseFormat(const std::string& date) {
		if (isDateInDataBaseFormat(date))
			return date;

        std::tm tm = {};
        std::stringstream ss(date);
        ss >> std::get_time(&tm, "%d.%m.%Y");

        if (ss.fail())
            return "";

        std::stringstream result;
        result << std::put_time(&tm, "%Y-%m-%d");
        return result.str();
    }
	
private:
	static bool isDateInDataBaseFormat(const std::string& date) {
		std::tm tm = {};
		std::stringstream ss(date);
		ss >> std::get_time(&tm, "%Y-%m-%d");

		return !ss.fail();
	}
};

class Scanner {
public:
	static std::string inputString(const std::string& message) {
		std::string str;

		std::cout << message;
		std::getline(std::cin >> std::ws, str);

		return str;
	}

	static std::string inputDate(const std::string& message) {
		std::string date;

		while (!DateHelper::isCorrectDate(date)) {
			std::cout << message;
			std::getline(std::cin >> std::ws, date);
		}

		return date;
	}

	template<class T>
	static T inputNumeric(const std::string& message) {
		T var = T();

		while (true)
		{
			std::cout << message;
			std::cin >> var;

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(32000, '\n');
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				return var;
			}

		}
	}
};

class StringHelper {
public:
	static std::string toLowerCase(const std::string& initialString) {
		std::string lowerString;
		lowerString.resize(initialString.length());

		std::transform(initialString.begin(), initialString.end(), lowerString.begin(), ::tolower);
		return lowerString;
	}
};