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
using std::cerr;

int Library::callback(void* data, int argc, char** argv, char** az_col_name) {       
    for (int i = 0; i < argc; i++) {
        cout << az_col_name[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "----------------------------" << endl;
    return 0;
}

int Library::callbackCount(void* data, int argc, char** argv, char** az_col_name) {       
    if (argc > 0 && argv[0]) {
        cout << endl << argv[0] << endl; 
    }
    return 0;
}

int Library::sort_callback(void* data, int argc, char** argv, char** az_col_name) {
    if (argc >= 6) { 
        cout << "Title: " << (argv[1] ? argv[1] : "NULL") << ", ";
        cout << "Pages: " << (argv[5] ? argv[5] : "NULL") << endl;
    } else {
        cout << "Unexpected number of columns." << endl;
    }
    cout << "----------------------------" << endl;
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

    char* message_error;
    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &message_error);
    if (exit != SQLITE_OK) {
        string error_msg = "Error create table (books): " + string(message_error);
        sqlite3_free(message_error);
        throw DatabaseException(error_msg);
    }

    sql = "CREATE TABLE IF NOT EXISTS BORROWED_BOOKS ("
          "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
          "BOOK_ID INTEGER NOT NULL, "
          "DUE_DATE DATE NOT NULL, "
          "EMAIL TEXT NOT NULL,"
          "RETURNED INTEGER DEFAULT 0, "
          "FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID)"
          ");";

    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &message_error);
    if (exit != SQLITE_OK) {
        string error_msg = "Error create table (borrowed_books): " + string(message_error);
        sqlite3_free(message_error);
        throw DatabaseException(error_msg);
    }

    string trigger_sql = R"(
        CREATE TRIGGER IF NOT EXISTS check_due_date
        BEFORE INSERT ON BORROWED_BOOKS
        FOR EACH ROW
        BEGIN
            -- Sprawdzanie, czy data jest w formacie YYYY-MM-DD
            SELECT
            CASE
                WHEN LENGTH(NEW.DUE_DATE) != 10 OR
                    SUBSTR(NEW.DUE_DATE, 5, 1) != '-' OR
                    SUBSTR(NEW.DUE_DATE, 8, 1) != '-' OR
                    NOT (NEW.DUE_DATE BETWEEN '2024-01-01' AND '2026-12-31') THEN
                    RAISE (ABORT, 'DUE_DATE must be in the format YYYY-MM-DD and between 2024-01-01 and 2026-12-31')
            END;
        END;
    )";

    exit = sqlite3_exec(db, trigger_sql.c_str(), NULL, 0, &message_error);
    if (exit != SQLITE_OK) {
        string error_msg = "Error create trigger (check_due_date): " + string(message_error);
        sqlite3_free(message_error);
        throw DatabaseException(error_msg);
    } 
}

Library::~Library() {
    sqlite3_close(db);
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

void Library::addBook(const Book& book) {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    try {
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

        int book_id = -1;
        int current_amount = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            book_id = sqlite3_column_int(stmt, 0);
            current_amount = sqlite3_column_int(stmt, 1);
        }

        sqlite3_finalize(stmt);

        if (book_id != -1) {
            string update_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT + ? WHERE ID = ?;";
            
            exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
            if (exit != SQLITE_OK) {
                throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
            }
            
            sqlite3_bind_int(stmt, 1, book.getAmount());
            sqlite3_bind_int(stmt, 2, book_id);

            exit = sqlite3_step(stmt);
            if (exit != SQLITE_DONE) {
                throw DatabaseException("Error during step UPDATE: " + string(sqlite3_errmsg(db)));
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
                throw DatabaseException("Error during step INSERT: " + string(sqlite3_errmsg(db)));
            }

            sqlite3_finalize(stmt);
        }
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        cout << "Book added successfully." << endl;

    } catch (const DatabaseException& e) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Failed to add book: " << e.what() << endl;
        throw; 
    }
}

void Library::borrowBook(const BorrowedBook& borrowed_book) {
    const Book& book = borrowed_book.getBook();
    string title = book.getTitle();
    string due_date = borrowed_book.getDueDate();
    string email = borrowed_book.getEmail();
    bool is_returned = borrowed_book.isReturned();

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    try {
        Book existing_book = getBookByTitle(title);

        string sql = "SELECT ID, AMOUNT FROM BOOKS WHERE TITLE = ?;";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);

        int book_id = -1;
        int amount = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            book_id = sqlite3_column_int(stmt, 0);
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
            sqlite3_bind_int(stmt, 1, book_id);

            exit = sqlite3_step(stmt);
            if (exit != SQLITE_DONE) {
                string error_msg = "Error during step UPDATE: " + string(sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                throw DatabaseException(error_msg);
            }
            sqlite3_finalize(stmt);

            string insert_sql = "INSERT INTO BORROWED_BOOKS (BOOK_ID, DUE_DATE, EMAIL, RETURNED) VALUES (?, ?, ?, ?);";
            exit = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, NULL);
            if (exit != SQLITE_OK) {
                throw DatabaseException("Error during INSERT statement: " + string(sqlite3_errmsg(db)));
            }
            sqlite3_bind_int(stmt, 1, book_id);
            sqlite3_bind_text(stmt, 2, due_date.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 4, is_returned ? 1 : 0);

            exit = sqlite3_step(stmt);
            if (exit != SQLITE_DONE) {
                string error_msg = "Error during step INSERT: " + string(sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                throw DatabaseException(error_msg);
            }
            sqlite3_finalize(stmt);

        } else {
            throw DatabaseException("Book is not available.");
        }

        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        cout << "Book borrowed successfully." << endl;

    } catch (const DatabaseException& e) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Failed to borrow book: " << e.what() << endl;
        throw; 
    }
}

void Library::returnBook(int book_id, const string& due_date, const string& email) {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    try {
        string select_sql = "SELECT ID FROM BORROWED_BOOKS WHERE BOOK_ID = ? AND DUE_DATE = ? AND EMAIL = ? AND RETURNED = 0;";
        
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, select_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
        }
        
        sqlite3_bind_int(stmt, 1, book_id);
        sqlite3_bind_text(stmt, 2, due_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
        
        int borrowed_book_id = -1;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            borrowed_book_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
        
        if (borrowed_book_id == -1) {
            throw DatabaseException("No borrowed book found with the provided details.");
        }
        
        string update_sql = "UPDATE BORROWED_BOOKS SET RETURNED = 1 WHERE ID = ?;";
        exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
        }
        
        sqlite3_bind_int(stmt, 1, borrowed_book_id);
        
        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            throw DatabaseException("Error during step UPDATE: " + string(sqlite3_errmsg(db)));
        }
        sqlite3_finalize(stmt);
        
        string update_amount_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT + 1 WHERE ID = ?;";
        
        exit = sqlite3_prepare_v2(db, update_amount_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
        }
        
        sqlite3_bind_int(stmt, 1, book_id);
        
        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            throw DatabaseException("Error during step UPDATE: " + string(sqlite3_errmsg(db)));
        }
        
        sqlite3_finalize(stmt);

        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        cout << "Borrowed book returned successfully." << endl;

    } catch (const DatabaseException& e) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Failed to return book: " << e.what() << endl;
        throw; 
    }
}

void Library::removeBook(const string& title, int amount) {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    try {
        string select_sql = "SELECT ID, AMOUNT FROM BOOKS WHERE TITLE = ?;";
        
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, select_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            throw DatabaseException("Error during SELECT statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);

        int book_id = -1;
        int current_amount = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            book_id = sqlite3_column_int(stmt, 0);
            current_amount = sqlite3_column_int(stmt, 1);
        } else {
            sqlite3_finalize(stmt);
            throw DatabaseException("Book not found.");
        }
        sqlite3_finalize(stmt);

        if (current_amount < amount) {
            throw DatabaseException("Not enough books to remove.");
        }

        if (current_amount == amount) {
            string delete_sql = "DELETE FROM BOOKS WHERE ID = ?;";
            exit = sqlite3_prepare_v2(db, delete_sql.c_str(), -1, &stmt, NULL);
            if (exit != SQLITE_OK) {
                throw DatabaseException("Error during DELETE statement: " + string(sqlite3_errmsg(db)));
            }
            sqlite3_bind_int(stmt, 1, book_id);
        } else {
            string update_sql = "UPDATE BOOKS SET AMOUNT = AMOUNT - ? WHERE ID = ?;";
            exit = sqlite3_prepare_v2(db, update_sql.c_str(), -1, &stmt, NULL);
            if (exit != SQLITE_OK) {
                throw DatabaseException("Error during UPDATE statement: " + string(sqlite3_errmsg(db)));
            }
            sqlite3_bind_int(stmt, 1, amount);
            sqlite3_bind_int(stmt, 2, book_id);
        }

        exit = sqlite3_step(stmt);
        if (exit != SQLITE_DONE) {
            string error_msg = "Error during step DELETE/UPDATE: " + string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw DatabaseException(error_msg);
        }

        sqlite3_finalize(stmt);

        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        cout << "Book removed successfully." << endl;

    } catch (const DatabaseException& e) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Failed to remove book: " << e.what() << endl;
        throw; 
    }
}

void Library::clearDatabase() {
    string sql = "DELETE FROM BOOKS;";
    char* message_error;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &message_error);
    if (exit != SQLITE_OK) {
        string error_msg = "Error during database clearing: " + string(message_error);
        sqlite3_free(message_error);
        throw DatabaseException(error_msg);
    }
}

void Library::displayAllBooks() {
    cout << "All available books in library: " << endl;
    cout << "----------------------------" << endl;
    string sql = "SELECT * FROM BOOKS;";
    
    char* message_error;
    int exit = sqlite3_exec(db, sql.c_str(), callback, 0, &message_error);
    if (exit != SQLITE_OK) {
        string error_msg = "Error during displaying books: " + string(message_error);
        sqlite3_free(message_error);
        throw DatabaseException(error_msg);
    }
}

void Library::displayBorrowedBooks() {
    cout << "All borrowed books: " << endl;
    cout << "----------------------------" << endl;
    
    string sql = "SELECT B.ID, BK.TITLE, BK.AUTHOR, BK.YEAR, BK.PAGES, B.DUE_DATE, B.EMAIL FROM BORROWED_BOOKS B "
                 "INNER JOIN BOOKS BK ON B.BOOK_ID = BK.ID "
                 "WHERE B.RETURNED = 0;";
    
    char* message_error;
    int exit = sqlite3_exec(db, sql.c_str(), callback, 0, &message_error);
    if (exit != SQLITE_OK) {
        string error_msg = "Error during displaying borrowed books: " + string(message_error);
        sqlite3_free(message_error);
        throw DatabaseException(error_msg);
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
    cout << "Books counted by kind: " << p_kind.getSelectedKind();
    string sql = "SELECT SUM(AMOUNT) FROM BOOKS WHERE KIND = '" + p_kind.getSelectedKind() + "';";
    sqlite3_exec(db, sql.c_str(), callbackCount, 0, NULL);
}

void Library::countByAuthor(const string& p_author) const {                
    cout << "Books counted by author: " << p_author;
    string sql = "SELECT SUM(AMOUNT) FROM BOOKS WHERE AUTHOR = '" + p_author + "';";
    sqlite3_exec(db, sql.c_str(), callbackCount, 0, NULL);
}

void Library::sortByLength(const string& p_choice) const {
    if (p_choice == "ascending") {
        string sql = "SELECT * FROM BOOKS ORDER BY PAGES;";
        sqlite3_exec(db, sql.c_str(), sort_callback, 0, NULL);
    } else if (p_choice == "descending") {
        string sql = "SELECT * FROM BOOKS ORDER BY PAGES DESC;";
        sqlite3_exec(db, sql.c_str(), sort_callback, 0, NULL);
    } else {
        cout << "Try again." << endl;
    }
}