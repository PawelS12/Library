#include <iostream>
#include <string>
#include <vector>
#include "library.h"
#include "Book.h"
#include "BorrowedBook.h"
#include "Kind.h"
#include "DatabaseException.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int Library::callback(void* data, int argc, char** argv, char** azColName) {       
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "----------------------------" << endl;
    return 0;
}

int Library::callbackCount(void* data, int argc, char** argv, char** azColName) {       
    if (argc > 0 && argv[0]) {
        cout << "Count: " << argv[0] << endl; 
    }
    return 0;
}

Library::Library(const string& db_name) {
    int exit = sqlite3_open(db_name.c_str(), &db);
    if (exit) {
        throw DatabaseException("Error open data base: " + string(sqlite3_errmsg(db)));
    }

    string sql = "CREATE TABLE IF NOT EXISTS BOOKS ("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "TITLE TEXT NOT NULL, "
                 "AUTHOR TEXT NOT NULL, "
                 "KIND TEXT NOT NULL, "
                 "YEAR INTEGER NOT NULL, "
                 "PAGES INTEGER NOT NULL,"
                 "AMOUNT INTEGER NOT NULL"
                 ");";

    char* messageError;
    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error create table (books): " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    }

    sql = "CREATE TABLE IF NOT EXISTS BORROWED_BOOKS ("
          "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
          "BOOK_ID INTEGER NOT NULL, "
          "DUE_DATE DATE NOT NULL, "
          "RETURNED INTEGER DEFAULT 0, "
          "FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID)"
          ");";

    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error create table (borrowed_books): " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    }

    string triggerSql = R"(
        CREATE TRIGGER IF NOT EXISTS check_due_date
        BEFORE INSERT ON BORROWED_BOOKS
        FOR EACH ROW
        BEGIN
            -- Check if the DUE_DATE is between January 1, 2024 and December 31, 2026
            SELECT
                CASE
                    WHEN NEW.DUE_DATE < '2024-01-01' OR NEW.DUE_DATE > '2026-12-31' THEN
                        RAISE (ABORT, 'DUE_DATE must be between 2024-01-01 and 2026-12-31')
                END;
        END;
    )";

    exit = sqlite3_exec(db, triggerSql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error create trigger (check_due_date): " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    } 
}

Library::~Library() {
    sqlite3_close(db);
}

void Library::addBook(const Book& book) {                                           
    string check_sql = "SELECT ID, AMOUNT FROM BOOKS WHERE TITLE = ? AND AUTHOR = ? AND KIND = ? AND YEAR = ? AND PAGES = ?;";
    
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, check_sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, book.getAuthor().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, book.getKind().getSelectedKind().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, book.getYear());
    sqlite3_bind_int(stmt, 5, book.getPages());

    int bookId = -1;
    int currentAmount = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bookId = sqlite3_column_int(stmt, 0);
        currentAmount = sqlite3_column_int(stmt, 1);
    }

    sqlite3_finalize(stmt);

    if (bookId != -1) {
        string update_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT + ? WHERE ID = ?;";
        
        exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
        }
        
        sqlite3_bind_int(stmt, 1, book.getAmount());
        sqlite3_bind_int(stmt, 2, bookId);

        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            string errorMsg = "Error during step UPDATE: " + string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw DatabaseException(errorMsg);
        }

        sqlite3_finalize(stmt);
    } else {
        string insert_sql = "INSERT INTO BOOKS (TITLE, AUTHOR, KIND, YEAR, PAGES, AMOUNT) VALUES (?, ?, ?, ?, ?, ?);";
        
        exit = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during INSERT prepare statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, book.getAuthor().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, book.getKind().getSelectedKind().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, book.getYear());
        sqlite3_bind_int(stmt, 5, book.getPages());
        sqlite3_bind_int(stmt, 6, book.getAmount());

        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            string errorMsg = "Error during step INSERT: " + string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw DatabaseException(errorMsg);
        }

        sqlite3_finalize(stmt);
    }
}

Book Library::getBookByTitle(const string& title) const {
    string sql = "SELECT * FROM BOOKS WHERE TITLE = ?;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
    }
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);


    Book book; 
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int kindType = sqlite3_column_int(stmt, 3);
        int year = sqlite3_column_int(stmt, 4);
        int pages = sqlite3_column_int(stmt, 5);
        int amount = sqlite3_column_int(stmt, 6);

        Kind book_kind = Kind::fromInt(kindType); 
        book = Book(id, title, author, book_kind, year, pages, amount);
    } else {
        sqlite3_finalize(stmt);
        throw DatabaseException("Book not found.");
    }
    sqlite3_finalize(stmt);

    return book;
}

void Library::borrowBook(const BorrowedBook& borrowedBook) {
    const Book& book = borrowedBook.getBook();
    string title = book.getTitle();
    string dueDate = borrowedBook.getDueDate();
    bool isReturned = borrowedBook.isReturned();

    Book existingBook = getBookByTitle(title);

    string sql = "SELECT ID, AMOUNT FROM BOOKS WHERE TITLE = ?;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
    }
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);

    int bookId = -1;
    int amount = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bookId = sqlite3_column_int(stmt, 0);
        amount = sqlite3_column_int(stmt, 1);
    } else {
        sqlite3_finalize(stmt);
        throw DatabaseException("Book not found.");
    }
    sqlite3_finalize(stmt);

    if (amount > 0) {
        string update_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT - 1 WHERE ID = ?;";
        exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_bind_int(stmt, 1, bookId);

        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            string errorMsg = "Error during step UPDATE: " + string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw DatabaseException(errorMsg);
        }
        sqlite3_finalize(stmt);

        string insert_sql = "INSERT INTO BORROWED_BOOKS (BOOK_ID, DUE_DATE, RETURNED) VALUES (?, ?, ?);";
        exit = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during INSERT statement: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_bind_int(stmt, 1, bookId);
        sqlite3_bind_text(stmt, 2, dueDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, isReturned ? 1 : 0);

        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            string errorMsg = "Error during step INSERT: " + string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw DatabaseException(errorMsg);
        }
        sqlite3_finalize(stmt);
    } else {
        throw DatabaseException("Book is not available.");
    }
}

void Library::returnBook(int bookId, const string& dueDate) {
    string select_sql = "SELECT ID FROM BORROWED_BOOKS WHERE BOOK_ID = ? AND DUE_DATE = ? AND RETURNED = 0;";
    
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, select_sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
    }
    
    sqlite3_bind_int(stmt, 1, bookId);
    sqlite3_bind_text(stmt, 2, dueDate.c_str(), -1, SQLITE_STATIC);
    
    int borrowedBookId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        borrowedBookId = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    
    if (borrowedBookId == -1) {
        throw DatabaseException("No borrowed book found with the provided details.");
    }
    
    string update_sql = "UPDATE BORROWED_BOOKS SET RETURNED = 1 WHERE ID = ?;";
    exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
    }
    
    sqlite3_bind_int(stmt, 1, borrowedBookId);
    
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        string errorMsg = "Error during step UPDATE: " + string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseException(errorMsg);
    }
    sqlite3_finalize(stmt);
    
    string update_amount_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT + 1 WHERE ID = ?;";
    
    exit = sqlite3_prepare_v2(db, update_amount_sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
    }
    
    sqlite3_bind_int(stmt, 1, bookId);
    
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        string errorMsg = "Error during step UPDATE: " + string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseException(errorMsg);
    }
    
    sqlite3_finalize(stmt);
}

void Library::removeBook(const string& title, int amount) {
    string select_sql = "SELECT ID, AMOUNT FROM BOOKS WHERE TITLE = ?;";
    
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, select_sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);

    int bookId = -1;
    int currentAmount = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bookId = sqlite3_column_int(stmt, 0);
        currentAmount = sqlite3_column_int(stmt, 1);
    } else {
        sqlite3_finalize(stmt);
        throw DatabaseException("Book not found.");
    }
    sqlite3_finalize(stmt);

    if (currentAmount < amount) {
        throw DatabaseException("Not enough books to remove.");
    }

    if (currentAmount == amount) {
        string delete_sql = "DELETE FROM BOOKS WHERE ID = ?;";
        exit = sqlite3_prepare_v2(db, delete_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during DELETE statement: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_bind_int(stmt, 1, bookId);
    } else {
        string update_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT - ? WHERE ID = ?;";
        exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_bind_int(stmt, 1, amount);
        sqlite3_bind_int(stmt, 2, bookId);
    }

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        string errorMsg = "Error during step DELETE/UPDATE: " + string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseException(errorMsg);
    }

    sqlite3_finalize(stmt);
}

void Library::removeBorrowedBook(const string& title, const string& dueDate) {
    string sql = "DELETE FROM BORROWED_BOOKS WHERE BOOK_ID IN (SELECT ID FROM BOOKS WHERE TITLE = ?) AND DUE_DATE = ?;";
    
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        throw DatabaseException("Error DELETE statement: " + string(sqlite3_errmsg(db)));
    }
    
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, dueDate.c_str(), -1, SQLITE_STATIC);
    
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        string errorMsg = "Error step DELETE: " + string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw DatabaseException(errorMsg);
    }
    
    sqlite3_finalize(stmt);
}

void Library::clearDatabase() {
    string sql = "DELETE FROM BOOKS;";
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error during database clearing: " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    }
}

void Library::displayAllBooks() {
    cout << "All available books in library: " << endl;
    cout << "----------------------------" << endl;
    string sql = "SELECT * FROM BOOKS;";
    
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error during displaying books: " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    }
}

void Library::displayBorrowedBooks() {
    cout << "All borrowed books: " << endl;
    cout << "----------------------------" << endl;
    
    string sql = "SELECT B.ID, BK.TITLE, BK.AUTHOR, BK.YEAR, BK.PAGES, B.DUE_DATE FROM BORROWED_BOOKS B "
                    "INNER JOIN BOOKS BK ON B.BOOK_ID = BK.ID "
                    "WHERE B.RETURNED = 0;";
    
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error during displaying borrowed books: " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    }
}

void Library::displayReturnedBooks() { //raczej nie potrzebne
    cout << "All returned books: " << endl;
    cout << "----------------------------" << endl;
    
    string sql = "SELECT B.ID, BK.TITLE, BK.AUTHOR, BK.YEAR, BK.PAGES, B.DUE_DATE FROM BORROWED_BOOKS B "
                    "INNER JOIN BOOKS BK ON B.BOOK_ID = BK.ID "
                    "WHERE B.RETURNED = 1;";
    
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        string errorMsg = "Error during displaying returned books: " + string(messageError);
        sqlite3_free(messageError);
        throw DatabaseException(errorMsg);
    }
}

void Library::displayBooksByLength() const {                                 
    cout << "Long books (>= 400 pages): " << endl;
    string sql = "SELECT * FROM BOOKS WHERE PAGES >= 400;";
    sqlite3_exec(db, sql.c_str(), callback, 0, NULL);
    cout << endl;
    cout << "Short books (< 400 pages): " << endl;
    sql = "SELECT * FROM BOOKS WHERE PAGES < 400;";
    sqlite3_exec(db, sql.c_str(), callback, 0, NULL);
}

void Library::countBooks() const {                                         
    cout << "Number of all books in library: ";
    string sql = "SELECT SUM(AMOUNT) FROM BOOKS;";
    sqlite3_exec(db, sql.c_str(), callbackCount, 0, NULL);
} 

void Library::searchByTitle(const string& p_title) const {                  
    cout << "Book searched by title: " << p_title << endl;
    string sql = "SELECT * FROM BOOKS WHERE TITLE = '" + p_title + "';";
    sqlite3_exec(db, sql.c_str(), callback, 0, NULL);
}

void Library::searchByAuthor(const string& p_author) const {               
    cout << "Books searched by author: " << p_author << endl;
    string sql = "SELECT * FROM BOOKS WHERE AUTHOR = '" + p_author + "';";
    sqlite3_exec(db, sql.c_str(), callback, 0, NULL);
}

void Library::searchByKind(const Kind& p_kind) const {                    
    cout << "Books searched by kind: " << p_kind.getSelectedKind() << endl;
    string sql = "SELECT * FROM BOOKS WHERE KIND = '" + p_kind.getSelectedKind() + "';";
    sqlite3_exec(db, sql.c_str(), callback, 0, NULL);
}

void Library::countByKind(const Kind& p_kind) const {                     
    cout << "Books counted by kind: " << p_kind.getSelectedKind() << endl;
    string sql = "SELECT COUNT(*) FROM BOOKS WHERE KIND = '" + p_kind.getSelectedKind() + "';";
    sqlite3_exec(db, sql.c_str(), callbackCount, 0, NULL);
}

void Library::countByAuthor(const string& p_author) const {                
    cout << "Books counted by author: " << p_author << endl;
    string sql = "SELECT COUNT(*) FROM BOOKS WHERE AUTHOR = '" + p_author + "';";
    sqlite3_exec(db, sql.c_str(), callbackCount, 0, NULL);
} 