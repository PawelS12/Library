#include "DatabaseStatistics.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sqlite3.h>
#include <codecvt>
#include <locale>

using std:: cerr;
using std::endl;
using std::string;
using std::ofstream;
using std::map;

DatabaseStatistics::DatabaseStatistics(sqlite3* db) : db(db) {}

void DatabaseStatistics::generateStatistics(const string& filename, bool isCSV) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file: " << filename << endl;
        return;
    }

    map<string, int> authorCount;
    getCountByAuthor(authorCount);

    map<string, int> genreCount;
    getCountByGenre(genreCount);

    int totalBooks = getTotalBooks();

    if (isCSV) {
        outputFile << "Total Books, " << totalBooks << "\n";
        outputFile << "Books by Author:\n";
        for (const auto& entry : authorCount) {
            outputFile << entry.first << ", " << entry.second << "\n";
        }
        outputFile << "Books by Genre:\n";
        for (const auto& entry : genreCount) {
            outputFile << entry.first << ", " << entry.second << "\n";
        }
    } else {
        outputFile << "Total Books: " << totalBooks << endl;
        outputFile << "Books by Author:" << endl;
        for (const auto& entry : authorCount) {
            outputFile << entry.first << ": " << entry.second << endl;
        }
        outputFile << "Books by Genre:" << endl;
        for (const auto& entry : genreCount) {
            outputFile << entry.first << ": " << entry.second << endl;
        }
    }

    outputFile.close();
}

void DatabaseStatistics::generateBooksDetails(const string& txt_filename, const string& csv_filename) {
    // std::locale utf8Locale(std::locale(), new std::codecvt_utf8<char>());

    ofstream txt_file(txt_filename);
    ofstream csv_File(csv_filename);

    // txt_file.imbue(utf8Locale);
    // csv_File.imbue(utf8Locale);


    if (!txt_file.is_open() || !csv_File.is_open()) {
        cerr << "Failed to open the files: " << txt_filename << " or " << csv_filename << endl;
        return;
    }

    csv_File << "Title, Author, Genre, Year, Amount\n";

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

        txt_file << "Title: " << title << ", Author: " << author << ", Genre: " << genre
                << ", Year: " << year << ", Amount: " << amount << endl;

        csv_File << title << "," << author << "," << genre << "," << year << "," << amount << "\n";
    }

    sqlite3_finalize(stmt);

    txt_file.close();
    csv_File.close();
}

void DatabaseStatistics::getCountByAuthor(map<string, int>& authorCount) {
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
        authorCount[author] = amount;
    }

    sqlite3_finalize(stmt);
}

void DatabaseStatistics::getCountByGenre(map<string, int>& genreCount) {
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
        genreCount[genre] = amount;
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

    int totalBooks = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        totalBooks = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return totalBooks;
}