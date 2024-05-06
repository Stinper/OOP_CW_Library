#pragma once
#include <iostream>
#include <iomanip>
#include "DataBase.h"
#include "IModel.h"
#include "Book.h"
#include "Reader.h"
#include "Queue.h"
#include "Utils.h"

class BaseRepository {
protected:
	IModel* model;

	static int inputForeignKey(const std::string& message, const std::string& tableName) {
		int ID = Scanner::inputNumeric<int>(message);
		std::string query = "SELECT ID FROM " + tableName + " WHERE ID = " + std::to_string(ID);

		if (DataBase::select(query).empty()) {
			throw std::exception("Записи с таким ID не существует");
		}

		return ID;
	}

public:
	BaseRepository() {
		this->setModel(nullptr);
	}

	BaseRepository(IModel* model) {
		this->setModel(model);
	}

	virtual void print(const std::vector<std::vector<std::string>>& recordSet) const = 0;
	virtual IModel* input() const = 0;
	virtual IModel* toModel(std::vector<std::string> record) const = 0;
	virtual IModel* inputUpdates(std::vector<std::string> recordToUpdate) const = 0;

	virtual int create() {
		return DataBase::runSQL(model->insertQuery());
	}

	virtual int update(int ID) {
		return DataBase::runSQL(model->updateQuery(ID));
	}

	virtual int destroy(int ID) {
		return DataBase::runSQL(model->deleteQuery(ID));
	}

	virtual std::vector<std::vector<std::string>> selectAll() {
		return DataBase::select(model->selectAllQuery());
	}

	virtual std::vector<std::vector<std::string>> findByID(int id) {
		return DataBase::select(model->findByIDQuery(id));
	}

	virtual std::vector<std::vector<std::string>> find(const std::string& query) {
		return DataBase::select(model->findQuery(query));
	}

	void setModel(IModel* model) {
		this->model = model;
	}
};


class BookRepository : public BaseRepository {
public:
	BookRepository() : BaseRepository() {}
	BookRepository(Book* book) : BaseRepository(book) {}

	void print(const std::vector<std::vector<std::string>>& recordSet) const override {
		if (recordSet.empty()) {
			std::cout << "Не найдено ни одной книги" << std::endl;
			return;
		}

		std::string line(170, '-');

		std::cout << line << std::endl;

		std::cout << std::left << std::setw(5) << "ID" << " | ";
		std::cout << std::left << std::setw(20) << "Автор" << " | ";
		std::cout << std::left << std::setw(30) << "Название" << " | ";
		std::cout << std::left << std::setw(20) << "Издательство" << " | ";
		std::cout << std::left << std::setw(20) << "Категория" << " | ";
		std::cout << std::left << std::setw(15) << "Год выпуска" << " | ";
		std::cout << std::left << std::setw(13) << "Всего копий" << " | ";
		std::cout << std::left << std::setw(10) << "Доступно" << " | ";
		std::cout << std::left << std::setw(5) << "Рейтинг" << std::endl;

		std::cout << line << std::endl;

		for (const auto& record : recordSet) {
			std::cout << std::left << std::setw(5) << record[0] << " | ";
			std::cout << std::left << std::setw(20) << record[1] << " | ";
			std::cout << std::left << std::setw(30) << record[2] << " | ";
			std::cout << std::left << std::setw(20) << record[3] << " | ";
			std::cout << std::left << std::setw(20) << record[4] << " | ";
			std::cout << std::left << std::setw(15) << record[5] << " | ";
			std::cout << std::left << std::setw(13) << record[6] << " | ";
			std::cout << std::left << std::setw(10) << record[7] << " | ";
			std::cout << std::left << std::setw(5) << record[8] << std::endl;
		}

		std::cout << line << std::endl;
	}

	IModel* toModel(std::vector<std::string> record) const override {
		return new Book(record[1], record[2], record[3], record[4], stoi(record[5]), stoi(record[6]), stoi(record[7]), stoi(record[8]));
	}

	IModel* input() const override {
		std::string author = Scanner::inputString("Автор: ");
		std::string title = Scanner::inputString("Название: ");
		std::string publisher = Scanner::inputString("Издательство: ");
		std::string category = Scanner::inputString("Категория: ");
		int year = Scanner::inputNumeric<int>("Год выпуска: ");
		int copiesCount = Scanner::inputNumeric<int>("Всего копий: ");
		int availableCopiesAmount = Scanner::inputNumeric<int>("Доступно копий для выдачи: ");
		int rating = Scanner::inputNumeric<int>("Рейтинг: ");

		return new Book(author, title, publisher, category, year, copiesCount, availableCopiesAmount, rating);
	}

	IModel* inputUpdates(std::vector<std::string> recordToUpdate) const override {
		Book* book = dynamic_cast<Book*>(toModel(recordToUpdate));

		std::cout << "Выберите поле для изменения: \n"
			"[1] Автор\n"
			"[2] Название\n"
			"[3] Издательство\n"
			"[4] Категория\n"
			"[5] Год выпуска\n"
			"[6] Всего копий\n"
			"[7] Доступно копий\n"
			"[8] Рейтинг\n";

		int fieldToUpdate = Scanner::inputNumeric<int>("Поле: ");
		std::string newStringValue;
		int newIntValue;

		switch (fieldToUpdate) {

		case 1:
			newStringValue = Scanner::inputString("Автор: ");
			book->setAuthor(newStringValue);
			break;

		case 2:
			newStringValue = Scanner::inputString("Название: ");
			book->setTitle(newStringValue);
			break;

		case 3:
			newStringValue = Scanner::inputString("Издательство: ");
			book->setPublisher(newStringValue);
			break;

		case 4:
			newStringValue = Scanner::inputString("Категория: ");
			book->setCategory(newStringValue);
			break;

		case 5:
			newIntValue = Scanner::inputNumeric<int>("Год выпуска: ");
			book->setYear(newIntValue);
			break;

		case 6:
			newIntValue = Scanner::inputNumeric<int>("Всего копий: ");
			book->setCopiesCount(newIntValue);
			break;

		case 7:
			newIntValue = Scanner::inputNumeric<int>("Доступно копий: ");
			book->setAvailableCopiesAmount(newIntValue);
			break;

		case 8:
			newIntValue = Scanner::inputNumeric<int>("Рейтинг: ");
			book->setRating(newIntValue);
			break;

		default:
			std::cout << "Неверное значение\n";
		}

		return book;
	}
};


class ReaderRepository : public BaseRepository {
public:
	ReaderRepository() : BaseRepository() {}
	ReaderRepository(Reader* reader) : BaseRepository(reader) {}

	IModel* toModel(std::vector<std::string> record) const override {
		return new Reader(record[1], record[2], stoi(record[3]));
	}

	void print(const std::vector<std::vector<std::string>>& recordSet) const override {
		if (recordSet.empty()) {
			std::cout << "Не найдено ни одного читателя" << std::endl;
			return;
		}

		std::string line(80, '-');

		std::cout << line << std::endl;

		std::cout << std::left << std::setw(5) << "ID" << " | ";
		std::cout << std::left << std::setw(40) << "ФИО" << " | ";
		std::cout << std::left << std::setw(15) << "Номер телефона" << " | ";
		std::cout << std::left << std::setw(10) << "Возраст" << std::endl;

		std::cout << line << std::endl;

		for (const auto& record : recordSet) {
			std::cout << std::left << std::setw(5) << record[0] << " | ";
			std::cout << std::left << std::setw(40) << record[1] << " | ";
			std::cout << std::left << std::setw(15) << record[2] << " | ";
			std::cout << std::left << std::setw(10) << record[3] << std::endl;
		}

		std::cout << line << std::endl;
	}

	IModel* input() const override {
		std::string FIO = Scanner::inputString("ФИО: ");
		std::string phoneNumber = Scanner::inputString("Номер телефона: ");
		int age = Scanner::inputNumeric<int>("Возраст: ");

		return new Reader(FIO, phoneNumber, age);
	}

	IModel* inputUpdates(std::vector<std::string> recordToUpdate) const override {
		Reader* reader = dynamic_cast<Reader*>(toModel(recordToUpdate));

		std::cout << "Выберите поле для изменения: \n"
			"[1] ФИО\n"
			"[2] Номер телефона\n"
			"[3] Возраст\n";

		int fieldToUpdate = Scanner::inputNumeric<int>("Поле: ");
		std::string newStringValue;
		int newIntValue;

		switch (fieldToUpdate) {

		case 1:
			newStringValue = Scanner::inputString("ФИО: ");
			reader->setFIO(newStringValue);
			break;

		case 2:
			newStringValue = Scanner::inputString("Номер телефона: ");
			reader->setPhoneNumber(newStringValue);
			break;

		case 3:
			newIntValue = Scanner::inputNumeric<int>("Возраст: ");
			reader->setAge(newIntValue);
			break;

		default:
			std::cout << "Неверное значение\n";
		}

		return reader;
	}
};


class IssuedBookRepository : public BaseRepository {
public:
	IssuedBookRepository() : BaseRepository() {}

	IssuedBookRepository(IssuedBook* issuedBook) : BaseRepository(issuedBook) {}

	IModel* toModel(std::vector<std::string> record) const override {
		return new IssuedBook(stoi(record[1]), stoi(record[2]), record[3], record[4]);
	}

	int create() override {
		IssuedBook* issuedBook = dynamic_cast<IssuedBook*>(model);
		int bookID = issuedBook->getBookID(), readerID = issuedBook->getReaderID();

		auto queue = getBookQueue(bookID);

		if (!isBookAvailable(bookID, readerID, queue))
			throw std::exception("Невозможно выдать книгу этому читателю, т.к. она забронирована другим читателем");

		if (isEnoughBooksToIssue(bookID)) {
			std::vector<std::string> queries;
			queries.push_back("UPDATE books SET AvailableCopiesCount = AvailableCopiesCount - 1 WHERE ID = " + std::to_string(bookID));

			if (!queue.empty()) {
				int queueRecordID = stoi(queue[0][0]);
				queries.push_back("UPDATE queue SET BookIssued = 1 WHERE ID = " + std::to_string(queueRecordID));
			}

			queries.push_back(model->insertQuery());

			return DataBase::runTransaction(queries);
		}

		throw std::exception("Недостаточно книг для выдачи.");
	}

	int returnBook(int issueRecordID) {
		auto record = this->findByID(issueRecordID);

		if (record.empty()) {
			throw std::exception("Записи с таким ID не существует");
		}

		std::string bookID = DataBase::select("SELECT BookID FROM issued_books WHERE ID = " + std::to_string(issueRecordID))[0][0];

		if (!isBookReturned(issueRecordID)) {
			std::vector<std::string> queries;
			queries.push_back("UPDATE books SET AvailableCopiesCount = AvailableCopiesCount + 1 WHERE ID = " + bookID);
			queries.push_back("UPDATE issued_books SET ReturnDate = DATE('now'), returned = 1 WHERE ID = " + std::to_string(issueRecordID));

			return DataBase::runTransaction(queries);
		}

		throw std::exception("Книга уже была возвращена");
	}

	void print(const std::vector<std::vector<std::string>>& recordSet) const override {
		if (recordSet.empty()) {
			std::cout << "Не найдено ни одной записи" << std::endl;
			return;
		}

		std::string line(160, '-');

		std::cout << line << std::endl;

		std::cout << std::left << std::setw(15) << "ID записи" << " | ";
		std::cout << std::left << std::setw(40) << "ФИО читателя" << " | ";
		std::cout << std::left << std::setw(30) << "Название книги" << " | ";
		std::cout << std::left << std::setw(25) << "Дата выдачи" << " | ";
		std::cout << std::left << std::setw(20) << "Дата возврата" << " | ";
		std::cout << std::left << std::setw(15) << "Возвращено" << std::endl;

		std::cout << line << std::endl;

		for (const auto& record : recordSet) {
			std::cout << std::left << std::setw(15) << record[0] << " | ";
			std::cout << std::left << std::setw(40) << record[1] << " | ";
			std::cout << std::left << std::setw(30) << record[2] << " | ";
			std::cout << std::left << std::setw(25) << record[3] << " | ";
			std::cout << std::left << std::setw(20) << record[4] << " | ";
			std::cout << std::left << std::setw(15) << (stoi(record[5]) == 1 ? "Да" : "Нет") << std::endl;
		}

		std::cout << line << std::endl;
	}

	IModel* input() const override {
		int readerID = inputForeignKey("ID читателя: ", "readers");
		int bookID = inputForeignKey("ID книги: ", "books");
		std::string issueDate = Scanner::inputDate("Дата выдачи книги (в формате дд.мм.гггг): ");
		std::string returnDate = Scanner::inputDate("Дата возврата книги (в формате дд.мм.гггг): ");

		return new IssuedBook(readerID, bookID, issueDate, returnDate);
	}

	IModel* inputUpdates(std::vector<std::string> recordToUpdate) const override {
		IssuedBook* issuedBook = dynamic_cast<IssuedBook*>(toModel(recordToUpdate));

		std::cout << "Выберите поле для изменения: \n"
			"[1] ID читателя\n"
			"[2] ID книги\n"
			"[3] Дата выдачи книги\n"
			"[4] Дата возврата книги\n";

		int fieldToUpdate = Scanner::inputNumeric<int>("Поле: ");
		std::string newStringValue;
		int newIntValue;

		switch (fieldToUpdate) {

		case 1:
			newIntValue = inputForeignKey("ID читателя: ", "readers");
			issuedBook->setReaderID(newIntValue);
			break;

		case 2:
			newIntValue = inputForeignKey("ID книги: ", "books");
			issuedBook->setBookID(newIntValue);
			break;

		case 3:
			newStringValue = Scanner::inputDate("Дата выдачи книги (в формате дд.мм.гггг): ");
			issuedBook->setIssueDate(newStringValue);
			break;

		case 4:
			newStringValue = Scanner::inputDate("Дата возврата книги (в формате дд.мм.гггг): ");
			issuedBook->setReturnDate(newStringValue);
			break;

		default:
			std::cout << "Неверное значение\n";
		}

		return issuedBook;
	}

	std::vector<std::vector<std::string>> selectAll() override {
		IssuedBook* issuedBook = dynamic_cast<IssuedBook*>(model);
		
		if(issuedBook)
			return DataBase::select(issuedBook->selectAllVerboseQuery());

		return BaseRepository::selectAll();
	}

private:
	std::vector<std::vector<std::string>> getBookQueue(int bookID) {
		std::string query = "SELECT ID, BookID, ReaderID FROM queue WHERE BookID = " + std::to_string(bookID) + " AND BookIssued = 0 ORDER BY dateOfQueuing DESC;";
		return DataBase::select(query);
	}

	bool isBookAvailable(int bookID, int readerID, std::vector<std::vector<std::string>> queue) {
		if (queue.empty())
			return true;

		const auto& firstRecord = queue.front();
		return stoi(firstRecord[2]) == readerID;
	}

	bool isEnoughBooksToIssue(int bookID) {
		std::string query = "SELECT AvailableCopiesCount FROM books WHERE ID = " + std::to_string(bookID);
		auto recordSet = DataBase::select(query);

		if (recordSet.empty())
			return false;

		return stoi(recordSet[0][0]) > 0;
	}

	bool isBookReturned(int issueRecordID) {
		std::string query = "SELECT returned FROM issued_books WHERE ID = " + std::to_string(issueRecordID);
		auto recordSet = DataBase::select(query);

		return stoi(recordSet[0][0]) == 1;
	}
};


class QueueRepository : public BaseRepository {
public:
	QueueRepository() : BaseRepository() {}
	QueueRepository(Queue* queue) : BaseRepository(queue) {}

	IModel* toModel(std::vector<std::string> record) const override {
		std::string date = DateHelper::toDataBaseFormat(record[3]);

		return new Queue(stoi(record[1]), stoi(record[2]), date);
	}

	int create() override {
		Queue* queue = dynamic_cast<Queue*>(model);

		if (getBookAvailableCopiesCount(queue->getBookID()) != 0) {
			throw std::exception("Невозможно добавить читателя в очередь, существует как минимум 1 копия этой книги, доступная для выдачи.");
		}

		if (isReaderInQueue(queue->getReaderID(), queue->getBookID())) {
			throw std::exception("Невозможно добавить читателя в очередь, т.к. он уже находится в очереди на эту книгу.");
		}

		return BaseRepository::create();
	}

	void print(const std::vector<std::vector<std::string>>& recordSet) const override {
		if (recordSet.empty()) {
			std::cout << "Не найдено ни одной записи" << std::endl;
			return;
		}

		std::string line(150, '-');

		std::cout << line << std::endl;

		std::cout << std::left << std::setw(15) << "ID записи" << " | ";
		std::cout << std::left << std::setw(40) << "ФИО читателя" << " | ";
		std::cout << std::left << std::setw(30) << "Название книги" << " | ";
		std::cout << std::left << std::setw(25) << "Дата постановки в очередь" << " | ";
		std::cout << std::left << std::setw(15) << "Книга выдана" << std::endl;

		std::cout << line << std::endl;

		for (const auto& record : recordSet) {
			std::cout << std::left << std::setw(15) << record[0] << " | ";
			std::cout << std::left << std::setw(40) << record[1] << " | ";
			std::cout << std::left << std::setw(30) << record[2] << " | ";
			std::cout << std::left << std::setw(25) << record[3] << " | ";
			std::cout << std::left << std::setw(15) << (stoi(record[4]) == 1 ? "Да" : "Нет") << std::endl;
		}

		std::cout << line << std::endl;
	}

	IModel* input() const override {
		int readerID = inputForeignKey("ID читателя: ", "readers");
		int bookID = inputForeignKey("ID книги: ", "books");
		std::string date = Scanner::inputDate("Дата постановки в очередь (в формате дд.мм.гггг): ");
		return new Queue(readerID, bookID, date);
	}

	IModel* inputUpdates(std::vector<std::string> recordToUpdate) const override {
		Queue* queue = dynamic_cast<Queue*>(toModel(recordToUpdate));

		std::cout << "Выберите поле для изменения: \n"
			"[1] ID читателя\n"
			"[2] ID книги\n"
			"[3] Дата постановки в очередь\n";

		int fieldToUpdate = Scanner::inputNumeric<int>("Поле: ");
		std::string newStringValue;
		int newIntValue;

		switch (fieldToUpdate) {

		case 1:
			newIntValue = inputForeignKey("ID читателя: ", "readers");
			queue->setReaderID(newIntValue);
			break;

		case 2:
			newIntValue = inputForeignKey("ID книги: ", "books");
			queue->setBookID(newIntValue);
			break;

		case 3:
			newStringValue = Scanner::inputDate("Дата постановки в очередь (в формате дд.мм.гггг): ");
			queue->setDateOfQueuing(newStringValue);
			break;

		default:
			std::cout << "Неверное значение\n";
		}

		return queue;
	}

	std::vector<std::vector<std::string>> selectAll() override {
		Queue* queue = dynamic_cast<Queue*>(model);

		if (queue)
			return DataBase::select(queue->selectAllVerboseQuery());

		return BaseRepository::selectAll();
	}

private:
	int getBookAvailableCopiesCount(int bookID) {
		std::string query = "SELECT AvailableCopiesCount FROM books WHERE ID = " + std::to_string(bookID);
		auto recordSet = DataBase::select(query);

		return stoi(recordSet[0][0]);
	}

	bool isReaderInQueue(int readerID, int bookID) {
		std::string query = "SELECT ID FROM queue WHERE ReaderID = " + std::to_string(readerID) + " AND BookID = " + std::to_string(bookID);
		auto record = DataBase::select(query);

		return !record.empty();
	}
};


class RepositoryFactory {
public:
	static BaseRepository* createRepository(IModel* model) {
		if (dynamic_cast<Book*>(model)) {
			return new BookRepository(dynamic_cast<Book*>(model));
		}
		else if (dynamic_cast<Reader*>(model)) {
			return new ReaderRepository(dynamic_cast<Reader*>(model));
		}
		else if (dynamic_cast<IssuedBook*>(model)) {
			return new IssuedBookRepository(dynamic_cast<IssuedBook*>(model));
		}
		else if (dynamic_cast<Queue*>(model)) {
			return new QueueRepository(dynamic_cast<Queue*>(model));
		}
		else {
			return nullptr;
		}
	}
};