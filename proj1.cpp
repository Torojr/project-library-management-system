#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>

// Forward declarations
class Book;
class Member;
class Transaction;
class LibraryManagementSystem;

// Book class definition
class Book {
public:
    int bookID;
    std::string title;
    std::string author;
    std::string genre;
    std::string status;

    Book(int id, std::string t, std::string a, std::string g, std::string s)
        : bookID(id), title(t), author(a), genre(g), status(s) {}

    void displayBook() {
        std::cout << "ID: " << bookID << " | Title: " << title << " | Author: " << author << " | Genre: " << genre << " | Status: " << status << std::endl;
    }
};

// Member class definition
class Member {
public:
    int memberID;
    std::string name;
    std::string address;
    std::string phoneNumber;

    Member(int id, std::string n, std::string addr, std::string phone)
        : memberID(id), name(n), address(addr), phoneNumber(phone) {}

    void displayMember() {
        std::cout << "ID: " << memberID << " | Name: " << name << " | Address: " << address << " | Phone: " << phoneNumber << std::endl;
    }
};

// Transaction class definition
class Transaction {
public:
    int transactionID;
    int memberID;
    int bookID;
    std::string borrowDate;
    std::string returnDate;

    Transaction(int tID, int mID, int bID, std::string bDate, std::string rDate)
        : transactionID(tID), memberID(mID), bookID(bID), borrowDate(bDate), returnDate(rDate) {}

    void displayTransaction() {
        std::cout << "Transaction ID: " << transactionID << " | Member ID: " << memberID << " | Book ID: " << bookID << " | Borrow Date: " << borrowDate << " | Return Date: " << returnDate << std::endl;
    }
};

// Library Management System class definition
class LibraryManagementSystem {
public:
    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Transaction> transactions;

    void addBook(Book book) {
        books.push_back(book);
    }

    void addMember(Member member) {
        members.push_back(member);
    }

    void borrowBook(int memberID, int bookID) {
        for (auto &book : books) {
            if (book.bookID == bookID && book.status == "Available") {
                book.status = "Borrowed";
                std::time_t t = std::time(nullptr);
                char date[10];
                strftime(date, sizeof(date), "%Y-%m-%d", std::localtime(&t));
                Transaction transaction(transactions.size() + 1, memberID, bookID, date, "");
                transactions.push_back(transaction);
                std::cout << "Book borrowed successfully." << std::endl;
                return;
            }
        }
        std::cout << "Book not available for borrowing." << std::endl;
    }

    void returnBook(int memberID, int bookID) {
        for (auto &book : books) {
            if (book.bookID == bookID && book.status == "Borrowed") {
                book.status = "Available";
                for (auto &transaction : transactions) {
                    if (transaction.bookID == bookID && transaction.memberID == memberID && transaction.returnDate.empty()) {
                        std::time_t t = std::time(nullptr);
                        char date[10];
                        strftime(date, sizeof(date), "%Y-%m-%d", std::localtime(&t));
                        transaction.returnDate = date;
                        std::cout << "Book returned successfully." << std::endl;
                        return;
                    }
                }
            }
        }
        std::cout << "Book not found in borrowed transactions." << std::endl;
    }

    void displayAllBooks() {
        for (auto &book : books) {
            book.displayBook();
        }
    }

    void displayAllMembers() {
        for (auto &member : members) {
            member.displayMember();
        }
    }

    void displayAllTransactions() {
        for (auto &transaction : transactions) {
            transaction.displayTransaction();
        }
    }

    void saveToFile() {
        std::ofstream bookFile("books.dat", std::ios::out | std::ios::binary);
        std::ofstream memberFile("members.dat", std::ios::out | std::ios::binary);
        std::ofstream transactionFile("transactions.dat", std::ios::out | std::ios::binary);

        for (auto &book : books) {
            bookFile.write((char*)&book, sizeof(Book));
        }

        for (auto &member : members) {
            memberFile.write((char*)&member, sizeof(Member));
        }

        for (auto &transaction : transactions) {
            transactionFile.write((char*)&transaction, sizeof(Transaction));
        }

        bookFile.close();
        memberFile.close();
        transactionFile.close();
    }

    void loadFromFile() {
        std::ifstream bookFile("books.dat", std::ios::in | std::ios::binary);
        std::ifstream memberFile("members.dat", std::ios::in | std::ios::binary);
        std::ifstream transactionFile("transactions.dat", std::ios::in | std::ios::binary);

        Book book(0, "", "", "", "");
        while (bookFile.read((char*)&book, sizeof(Book))) {
            books.push_back(book);
        }

        Member member(0, "", "", "");
        while (memberFile.read((char*)&member, sizeof(Member))) {
            members.push_back(member);
        }

        Transaction transaction(0, 0, 0, "", "");
        while (transactionFile.read((char*)&transaction, sizeof(Transaction))) {
            transactions.push_back(transaction);
        }

        bookFile.close();
        memberFile.close();
        transactionFile.close();
    }
};

int main() {
    LibraryManagementSystem lms;
    lms.loadFromFile();

    // Add sample data
    lms.addBook(Book(1, "1984", "George Orwell", "Dystopian", "Available"));
    lms.addMember(Member(1, "John Doe", "123 Elm St", "555-1234"));

    // Perform operations
    lms.borrowBook(1, 1);
    lms.returnBook(1, 1);

    // Display data
    lms.displayAllBooks();
    lms.displayAllMembers();
    lms.displayAllTransactions();

    lms.saveToFile();
    return 0;
}
