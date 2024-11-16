#ifndef DATABASE_STATISTICS_H
#define DATABASE_STATISTICS_H

#include <string>
#include <map>
#include <sqlite3.h>
#include <fstream>
#include <iostream>

using std::map;
using std::string;

class DatabaseStatistics {
private:
    sqlite3* db;
public:
    DatabaseStatistics(sqlite3* db);
    void generateStatistics(const string& filename, bool isCSV = false);
    void generateBooksDetails(const string& txt_filename, const string& csv_filename);
private:
    void getCountByAuthor(map<std::string, int>& author_count);
    void getCountByGenre(map<std::string, int>& genre_count);
    int getTotalBooks();
};

#endif // DATABASE_STATISTICS_H