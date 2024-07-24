#include <iostream>
#include "Kind.h"
#include "Book.h"
#include "BorrowedBook.h"
#include "Library.h"

using std::cout;
using std::endl;

int main() {
    Kind crime(Kind::KindType::CRIME);
    Kind horror(Kind::KindType::HORROR);
    Kind fantasy(Kind::KindType::FANTASY);
    Kind fairy_tale(Kind::KindType::FAIRY_TALE);
    Kind reportage(Kind::KindType::REPORTAGE);
    Kind contemporary_literature(Kind::KindType::CONTEMPORARY_LITERATURE);
    Kind romance(Kind::KindType::ROMANCE);
    Kind scientific_literature(Kind::KindType::SCIENTIFIC_LITERATURE);
    Kind thriller(Kind::KindType::THRILLER);
    Kind poetry(Kind::KindType::POETRY);
    Kind science_fiction(Kind::KindType::SCI_FI);

    Library library_1("library.db");

    // Book book1(1, "Hobbit", "J.R.R Tolkien", fantasy, 1900, 400, 5);
    // Book book2(2, "The Lord of the rings", "J.R.R Tolkien", fantasy, 1901, 1000, 3);
    // Book book3(3, "Eden", "Stanislaw Lem", science_fiction, 1970, 350, 1);

    // library_1.addBook(book1);
    // library_1.addBook(book2);
    // library_1.addBook(book3); 

    // BorrowedBook borrowed_1(1, book1, "2024-07-03", false); 
    // library_1.borrowBook(borrowed_1); 

    // library_1.displayAllBooks();
    // cout << endl;
    // library_1.displayBorrowedBooks();
    // cout << endl;
    // library_1.displayBooksByLength();
    // cout << endl;
    // library_1.countBooks();
    // cout << endl;
    // library_1.searchByTitle("Hobbit");
    // cout << endl;
    // library_1.searchByAuthor("J.R.R Tolkien");
    // cout << endl;
    // library_1.searchByKind(fantasy);
    // cout << endl;
    // library_1.countByKind(science_fiction);
    // cout << endl;
    // library_1.countByAuthor("Stanislaw Lem");
    // library_1.clearDatabase();
    // cout << endl;
    // library_1.displayAllBooks();

    return 0;
}