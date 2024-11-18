# LIBRARY MANAGEMENT SYSTEM

## 1. Project Description

This project is a library management system written using C++, SQLite, and SQL. The main features of the system include:

- Adding new books to the database.
- Borrowing books.
- Returning borrowed books.
- Removing books from the library.
- Displaying a list of all available books.
- Displaying a list of all borrowed books.
- Displaying books divided into long and short.
- Counting the total number of books.
- Counting books by author or genre.
- Searching for books by author, title or genre.
- Sorting books by length.
- Generating library statistics in csv and txt files.
- Clearing the entire database.

## 2. Technologies

- **C++**: Version 14.2.0
- **SQLite**: Version 3.46.0
- **SQL**

## 3. Installation

a) Download `Library-master.zip` from my repository.  
b) Extract the files to the selected folder.  
c) Open the `Library.exe` file.

*Note: To ensure that the application functions correctly, make sure that all files from the `Library-master.zip` are present in the extracted folder.**

## 4. Project structure
``` 
Library/
├── database/                  # SQLite database file
│   └── library.db            
├── include/                   # Header files for the project
│   ├── Book.h
│   ├── BorrowedBook.h
│   ├── DatabaseException.h
│   ├── DatabaseStatistics.h
│   ├── Genre.h
│   ├── Library.h
│   └── LibraryMenu.h
├── src/                       # Source files implementing the project logic
│   ├── Book.cpp
│   ├── BorrowedBook.cpp
│   ├── DatabaseException.cpp
│   ├── DatabaseStatistics.cpp
│   ├── Genre.cpp
│   ├── Library.cpp
│   ├── LibraryMenu.cpp
│   ├── main.cpp           
│   └── sqlite3.c               
├── statistics/                # Generated statistics files
│   ├── books_details.csv
│   ├── books_details.txt
│   ├── library_stats.csv
│   └── library_stats.txt
├── Library.exe                # Executable file
├── libgcc_s_seh-1.dll         # Required DLLs for runtime
├── libsqlite3-0.dll
├── libstdc++-6.dll
├── libwinpthread-1.dll
├── sqlite3.def                # SQLite linker definition file
└── sqlite3.o                  # SQLite object file
``` 

## License
The project is licensed under the terms of the MIT License. See the `LICENSE` file for details.