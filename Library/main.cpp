#include <iostream>
#include <Windows.h>
#include "Library.h"
#include "Book.h"
#include "Reader.h"
#include "Queue.h"
#include "IssuedBook.h"

int main()
{
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const std::string dbName = "library.db";
    std::vector<IModel*> tables{new Book(), new Reader(), new IssuedBook(), new Queue()};

    DataBase::initialize(dbName, tables);
    DataBase::open(dbName);

    Library::start();

    DataBase::close();

    for (IModel* table : tables) {
        delete table;
    }

}

