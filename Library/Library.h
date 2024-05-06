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
			std::cout << "�� ������� �� ����� ������\n";
			return;
		}

		repository->print(recordSet);
	}

	void showCreateRecordDialog() {
		repository->setModel(repository->input()); //������ �� ���� ����� ������ (���������� ��� ������)

		int status = repository->create();

		if (status == SQLITE_OK) {
			std::cout << "����� ������ ������� ��������� \n";
		}
	}

	void showUpdateRecordDialog(int ID) {
		auto recordSet = repository->findByID(ID);

		if (recordSet.empty()) {
			std::cout << "������ � ����� ID �� �������\n";
			return;
		}

		int action = showConfirmDialog("��������! �� ����������� ������������� ��������� ������: \n", recordSet);

		if (action == 1) {
			std::cout << "�������� ���� �� ��� ������ � ������� ����� ��������\n";
			IModel* updatedModel = repository->inputUpdates(recordSet.front());
			repository->setModel(updatedModel);
			int status = repository->update(ID);

			if (status == SQLITE_OK) {
				std::cout << "������ ������� ��������� \n";
			}
		}

	}

	void showFindRecordDialog(const std::string& searchQuery) {
		auto recordSet = repository->find(searchQuery);

		if (recordSet.empty()) {
			std::cout << "�� ������� ������� �� ������� �� ����� ������\n";
			return;
		}

		repository->print(recordSet);
	}

	void showFindRecordByIdDialog(int ID) {
		auto recordSet = repository->findByID(ID);

		if (recordSet.empty()) {
			std::cout << "������ � ����� ID �� �������\n";
			return;
		}

		repository->print(recordSet);
	}

	void showDeleteRecordDialog(int ID) {
		auto recordSet = repository->findByID(ID);

		if (recordSet.empty()) {
			std::cout << "������ � ����� ID �� �������\n";
			return;
		}

		int action = showConfirmDialog("��������! �� ����������� ������� ��������� ������: \n", recordSet);

		if (action == 1) {
			repository->destroy(ID);
			std::cout << "������ ������� ������� \n";
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

		std::cout << "�� �������?\n[0] - ���, �������� ��������\n[1] - ��, ���������� ����������\n";
		int action = Scanner::inputNumeric<int>("��������: ");

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
		std::cout << "[----------������� ���������� ����������� �����������----------]" << std::endl;
		std::cout << "|------------------------------�����----------------------------\n" <<
					 "| [1]:\t���������� ������ ���� ����\n" <<
					 "| [2]:\t�������� �����\n" <<
					 "| [3]:\t����� �����\n" << 
					 "| [4]:\t����� ����� �� ID\n" << 
					 "| [5]:\t������������� �����\n" << 
					 "| [6]:\t������� �����\n" << std::endl;

		std::cout << "|------------------------------��������----------------------------\n" <<
					 "| [7]:\t���������� ������ ���� ���������\n" <<
					 "| [8]:\t�������� ��������\n" <<
					 "| [9]:\t����� ��������\n" <<
					 "| [10]:\t����� �������� �� ID\n" <<
					 "| [11]:\t������������� ��������\n" <<
					 "| [12]:\t������� ��������\n" << std::endl;

		std::cout << "|------------------------------������ ����----------------------------\n" <<
					 "| [13]:\t���������� ������ ���� �������� ����\n" <<
					 "| [14]:\t������ �����\n" <<
					 "| [15]:\t����� ������ � ������\n" <<
					 "| [16]:\t������������� ������ � ������\n" <<
					 "| [17]:\t������� �����\n" << std::endl;

		std::cout << "|----------------------------������� �� �����--------------------------\n" <<
					 "| [18]:\t���������� ������� �� �����\n" <<
					 "| [19]:\t�������� �������� � �������\n" <<
					 "| [20]:\t����� ������\n" <<
					 "| [21]:\t������� �������\n" << std::endl;

		IModel* model = nullptr;
		BaseRepository* repository = nullptr;
		IssuedBookRepository* issuedBookRepository = nullptr;

		std::string searchQuery;
		int ID = 0;
		int status = -1;

		int action = 0;

		while (action != -1) {
			try {
				action = Scanner::inputNumeric<int>("|--> ��������: ");
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
					searchQuery = Scanner::inputString("������� ��������� ������: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 4:
					ID = Scanner::inputNumeric<int>("������� ID: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showFindRecordByIdDialog(ID);
					break;

				case 5:
					ID = Scanner::inputNumeric<int>("������� ID: ");

					model = new Book();
					tableManager.setModel(model);
					tableManager.showUpdateRecordDialog(ID);
					break;
				case 6:
					ID = Scanner::inputNumeric<int>("������� ID: ");

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
					searchQuery = Scanner::inputString("������� ��������� ������: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 10:
					ID = Scanner::inputNumeric<int>("������� ID: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showFindRecordByIdDialog(ID);
					break;

				case 11:
					ID = Scanner::inputNumeric<int>("������� ID: ");

					model = new Reader();
					tableManager.setModel(model);
					tableManager.showUpdateRecordDialog(ID);
					break;
					break;

				case 12:
					ID = Scanner::inputNumeric<int>("������� ID: ");

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
					searchQuery = Scanner::inputString("������� ��������� ������: ");

					model = new IssuedBook();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 16:
					ID = Scanner::inputNumeric<int>("������� ID: ");

					model = new IssuedBook();
					tableManager.setModel(model);
					tableManager.showUpdateRecordDialog(ID);
					break;

				case 17: {
					ID = Scanner::inputNumeric<int>("������� ID ������: ");
					issuedBookRepository = new IssuedBookRepository(new IssuedBook());

					auto recordSet = issuedBookRepository->findByID(ID);

					if (recordSet.empty()) {
						std::cout << "������ �� �������";
						continue;
					}

					tableManager.setRepository(issuedBookRepository);

					status = tableManager.showConfirmDialog("��������! �� ������ �������� ��������� ����� ��� ������������.\n", recordSet);

					if (status == 1) {
						status = issuedBookRepository->returnBook(ID);
						std::cout << "����� ������� ���������� \n";
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
					searchQuery = Scanner::inputString("������� ��������� ������: ");

					model = new Queue();
					tableManager.setModel(model);
					tableManager.showFindRecordDialog(searchQuery);
					break;

				case 21:
					ID = Scanner::inputNumeric<int>("������� ID ������: ");
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
				std::cerr << "������: " << e.what() << std::endl;
				continue;
			}
		}
	}
};
