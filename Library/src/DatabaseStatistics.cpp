#include "DatabaseStatistics.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sqlite3.h>
#include <filesystem>

using std:: cerr;
using std::endl;
using std::string;
using std::ofstream;
using std::map;
using namespace std::filesystem;

DatabaseStatistics::DatabaseStatistics(sqlite3* db) : db(db) {}

void DatabaseStatistics::generateStatistics(const string& filename, bool is_CSV) {
    ofstream output_file(filename);
    if (!output_file.is_open()) {
        cerr << "Failed to open the file: " << filename << endl;
        return;
    }

    map<string, int> author_count;
    getCountByAuthor(author_count);

    map<string, int> genre_count;
    getCountByGenre(genre_count);

    int totalBooks = getTotalBooks();

    if (is_CSV) {
        output_file << "Total Books, " << totalBooks << "\n";
        output_file << "Books by Author:\n";
        for (const auto& entry : author_count) {
            output_file << entry.first << ", " << entry.second << "\n";
        }
        output_file << "Books by Genre:\n";
        for (const auto& entry : genre_count) {
            output_file << entry.first << ", " << entry.second << "\n";
        }
    } else {
        output_file << "Total Books: " << totalBooks << endl << endl;
        output_file << "Books by Author:" << endl;
        for (const auto& entry : author_count) {
            output_file << entry.first << ": " << entry.second << endl;
        }
        output_file << "\nBooks by Genre:" << endl;
        for (const auto& entry : genre_count) {
            output_file << entry.first << ": " << entry.second << endl;
        }
    }

    output_file.close();
}

void DatabaseStatistics::generateBooksDetails(const string& txt_filename, const string& csv_filename) {

    path txt_path(txt_filename);
    path csv_path(csv_filename); 

    ofstream txt_file(txt_path);
    ofstream csv_file(csv_path);

    if (!txt_file.is_open() || !csv_file.is_open()) {
        cerr << "Failed to open the files: " << txt_filename << " or " << csv_filename << endl;
        return;
    }

    csv_file << "Title, Author, Genre, Year, Amount\n";

    string sql = "SELECT TITLE, AUTHOR, GENRE, YEAR, AMOUNT FROM BOOKS;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        cerr << "Error during SELECT statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string genre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int year = sqlite3_column_int(stmt, 3);
        int amount = sqlite3_column_int(stmt, 4);

        txt_file << "Title: " << title << ", Author: " << author << ", Genre: " << genre << ", Year: " << year << ", Amount: " << amount << endl;

        csv_file << title << "," << author << "," << genre << "," << year << "," << amount << "\n";
    }

    sqlite3_finalize(stmt);

    txt_file.close();
    csv_file.close();
}

void DatabaseStatistics::getCountByAuthor(map<string, int>& author_count) {
    string sql = "SELECT AUTHOR, SUM(AMOUNT) FROM BOOKS GROUP BY AUTHOR;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        cerr << "Error during SELECT statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int amount = sqlite3_column_int(stmt, 1);
        author_count[author] = amount;
    }

    sqlite3_finalize(stmt);
}

void DatabaseStatistics::getCountByGenre(map<string, int>& genre_count) {
    string sql = "SELECT GENRE, SUM(AMOUNT) FROM BOOKS GROUP BY GENRE;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        cerr << "Error during SELECT statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string genre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int amount = sqlite3_column_int(stmt, 1);
        genre_count[genre] = amount;
    }

    sqlite3_finalize(stmt);
}

int DatabaseStatistics::getTotalBooks() {
    string sql = "SELECT SUM(AMOUNT) FROM BOOKS;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        cerr << "Error during SELECT statement: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    int total_books = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total_books = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total_books;
}