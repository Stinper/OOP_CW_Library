#pragma once

#include <iostream>
#include "IssuedBook.h"
#include "DataBase.h"
#include "Utils.h"
#include "Repositories.h"


class TableManager {
public:
	TableManager() { repository = nullptr; model = nullptr; }

	TableManager(IModel* model) : model(model) {
		this->repository = RepositoryFactory::createRepository(model);
	}

	void showListAllRecordsDialog() {
		auto recordSet = repository->selectAll();

		if (recordSet.empty()) {
			std::cout << "Не найдено ни одной записи\n";
			return;
		}

		repository->print(recordSet);
	}

	void showCreateRecordDialog() {
		repository->setModel(repository->input()); //Запрос на ввод полей модели (изначально она пустая)

		int status = repository->create();

		if (status == SQLITE_OK) {
			std::cout << "Новая запись успешно добавлена \n";
		}
	}

	void showUpdateRecordDialog(int ID) {
		auto recordSet = repository->findByID(ID);

		if (recordSet.empty()) {
			std::cout << "Запись с таким ID не найдена\n";
			return;
		}

		int action = showConfirmDialog("ВНИМАНИЕ! Вы собираетесь редактировать следующую запись: \n", recordSet);

		if (action == 1) {
			std::cout << "Выберите поле по его номеру и введите новое значение\n";
			IModel* updatedModel = repository->inputUpdates(recordSet.front());
			repository->setModel(updatedModel);
			int status = repository->update(ID);

			if (status == SQLITE_OK) {
				std::cout << "Запись успешно обновлена \n";
			}
		}

	}

	void showFindRecordDialog(const std::string& searchQuery) {
		auto recordSet = repository->find(searchQuery);

		if (recordSet.empty()) {
			std::cout << "По данному запросу не найдено ни одной записи\n";
			return;
		}

		repository->print(recordSet);
	}

	void showFindRecordByIdDialog(int ID) {
		auto recordSet = repository->findByID(ID);

		if (recordSet.empty()) {
			std::cout << "Запись с таким ID не найдена\n";
			return;
		}

		repository->print(recordSet);
	}

	void showDeleteRecordDialog(int ID) {
		auto recordSet = repository->findByID(ID);

		if (recordSet.empty()) {
			std::cout << "Запись с таким ID не найдена\n";
			return;
		}

		int action = showConfirmDialog("ВНИМАНИЕ! Вы собираетесь удалить следующую запись: \n", recordSet);

		if (action == 1) {
			repository->destroy(ID);
			std::cout << "Запись успешно удалена \n";
		}
	}

	void setRepository(BaseRepository* repository) {
		this->repository = repository;
	}

	void setModel(IModel* model) {
		this->model = model;

		clear_repository();
		repository = RepositoryFactory::createRepository(this->model);
	}

	int showConfirmDialog(const std::string& message, std::vector<std::vector<std::string>> recordSet) {
		std::cout << message;
		repository->print(recordSet);

		std::cout << "Вы уверены?\n[0] - Нет, отменить действие\n[1] - Да, продолжить выполнение\n";
		int action = Scanner::inputNumeric<int>("Действие: ");

		return action;
	}

	~TableManager() {
		clear_repository();
	}

private:
	BaseRepository* repository;
	IModel* model;

	void clear_repository() {
		if (this->repository != nullptr) {
			delete this->repository;
			this->repository = nullptr;
		}
	}
};


class Library {
public:
	static void start() {
		std::cout << "[----------СИСТЕМА УПРАВЛЕНИЯ ЭЛЕКТРОННОЙ БИБЛИОТЕКОЙ----------]" << std::endl;
		std::cout << "|------------------------------КНИГИ----------------------------\n" <<
					 "| [1]:\tПосмотреть список всех книг\n" <<
					 "| [2]:\tДобавить книгу\n" <<
					 "| [3]:\tНайти книгу\n" << 
					 "| [4]:\tНайти книгу по ID\n" << 
					 "| [5]:\tРедактировать книгу\n" << 
					 "| [6]:\tУдалить книгу\n" << std::endl;

		std::cout << "|------------------------------ЧИТАТЕЛИ----------------------------\n" <<
					 "| [7]:\tПосмотреть список всех читателей\n" <<
					 "| [8]:\tДобавить читателя\n" <<
					 "| [9]:\tНайти читеталя\n" <<
					 "| [10]:\tНайти читателя по ID\n" <<
					 "| [11]:\tРедактировать читателя\n" <<
					 "| [12]:\tУдалить читателя\n" << std::endl;

		std::cout << "|------------------------------ВЫДАЧА КНИГ----------------------------\n" <<
					 "| [13]:\tПосмотреть список всех выданных книг\n" <<
					 "| [14]:\tВыдать книгу\n" <<
					 "| [15]:\tНайти запись о выдаче\n" <<
					 "| [16]:\tРедактировать запись о выдаче\n" <<
					 "| [17]:\tВернуть книгу\n" << std::endl;

		std::cout << "|----------------------------ОЧЕРЕДЬ НА КНИГИ--------------------------\n" <<
					 "| [18]:\tПосмотреть очередь на книги\n" <<
					 "| [19]:\tДобавить читателя в очередь\n" <<
					 "| [20]:\tНайти запись\n" <<
					 "| [21]:\tУдалить очередь\n" << std::endl;

		IModel* model = nullptr;
		BaseRepository* repository = nullptr;
		IssuedBookRepository* issuedBookRepository = nullptr;

		std::string searchQuery;
		int ID = 0;
		int status = -1;

		int action = 0;

		while (action != -1) {
			try {
				action = Scanner::inputNumeric<int>("|--> Действие: ");
				TableManager tableManager;

				switch (action) {
				case 1:
					model = new Book();
					tableManager.setModel(model);
					tableManager.showListAllRecordsDialog();
					break;

				case 2:
					model = new Book();
					tableManager.setModel(model);
					tableManager.showCreateRecordDialog();
					break;

				case 3:
					searchQuery = Scanner::inputString("Введите поисковый запрос: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 4:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showFindRecordByIdDialog(ID);
					break;

				case 5:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showUpdateRecordDialog(ID);
					break;
				case 6:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showDeleteRecordDialog(ID);
					break;

				case 7:
					model = new Reader();
					tableManager.setModel(model);
					tableManager.showListAllRecordsDialog();
					break;

				case 8:
					model = new Reader();
					tableManager.setModel(model);
					tableManager.showCreateRecordDialog();
					break;

				case 9:
					searchQuery = Scanner::inputString("Введите поисковый запрос: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 10:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showFindRecordByIdDialog(ID);
					break;

				case 11:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showUpdateRecordDialog(ID);
					break;
					break;

				case 12:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showDeleteRecordDialog(ID);
					break;

				case 13:
					model = new IssuedBook();
					tableManager.setModel(model);
					tableManager.showListAllRecordsDialog();
					break;

				case 14:
					model = new IssuedBook();
					tableManager.setModel(model);
					tableManager.showCreateRecordDialog();
					break;

				case 15:
					searchQuery = Scanner::inputString("Введите поисковый запрос: ");

					model = new IssuedBook();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 16:
					ID = Scanner::inputNumeric<int>("Введите ID: ");

					model = new IssuedBook();
					tableManager.setModel(model);
					tableManager.showUpdateRecordDialog(ID);
					break;

				case 17: {
					ID = Scanner::inputNumeric<int>("Введите ID записи: ");
					issuedBookRepository = new IssuedBookRepository(new IssuedBook());

					auto recordSet = issuedBookRepository->findByID(ID);

					if (recordSet.empty()) {
						std::cout << "Запись не найдена";
						continue;
					}

					tableManager.setRepository(issuedBookRepository);

					status = tableManager.showConfirmDialog("ВНИМАНИЕ! Вы хотите отметить следующую книгу как возвращенную.\n", recordSet);

					if (status == 1) {
						status = issuedBookRepository->returnBook(ID);
						std::cout << "Книга успешно возвращена \n";
					}

					break;
				}
				case 18:
					model = new Queue();
					tableManager.setModel(model);
					tableManager.showListAllRecordsDialog();
					break;

				case 19:
					model = new Queue();
					tableManager.setModel(model);
					tableManager.showCreateRecordDialog();
					break;

				case 20:
					searchQuery = Scanner::inputString("Введите поисковый запрос: ");

					model = new Queue();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 21:
					ID = Scanner::inputNumeric<int>("Введите ID записи: ");
					model = new Queue();
					tableManager.setModel(model);
					tableManager.showDeleteRecordDialog(ID);
					break;
				}

				if (model != nullptr) {
					delete model;
					model = nullptr;
				}

				if (repository != nullptr) {
					delete repository;
					repository = nullptr;
				}
			}
			catch (std::exception e) {
				std::cerr << "Ошибка: " << e.what() << std::endl;
				continue;
			}
		}
	}
};
