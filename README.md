# LIBRARY MANAGEMENT SYSTEM

## 1. Project Description

The **Library Management System** is a C++ application integrated with SQLite to manage a library of books. The system allows users to:

- Add, borrow, and return books.
- Remove books from the library.
- View available and borrowed books.
- Sort books by length.
- Search for books by title, author, or genre.
- Generate library statistics (in CSV and TXT formats).
- Manage library data with various sorting and counting features.

## 2. Technologies

- **C++**: Version 14.2.0
- **SQLite**: Version 3.46.0
- **SQL**

## 3. Installation

a) Download `Library-master.zip` from my repository.  
b) Extract the files to the selected folder.  
c) Open the `Library.exe` file.

*Note: No additional libraries or dependencies are required to run the application. Simply download the entire project folder and run the executable (`Library.exe`). Ensure all files from the zip are present in the extracted folder for the system to work correctly.*

If you see a warning that the executable is from an "unknown publisher" when trying to open `Library.exe`, you can bypass this warning by following these steps:
1. Click **"More info"** on the warning screen.
2. Click **"Run anyway"** to continue running the application.

This is a standard Windows security feature for files downloaded from the internet, and it can be ignored in this case since the application is safe to run.

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