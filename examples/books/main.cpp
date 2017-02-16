#include <fstream>
#include <steeljson/reader.h>
#include <steeljson/value.h>

void print_books(const steeljson::array& books) {
	for (std::size_t i = 0; i < books.size(); ++i) {
		const steeljson::object& book = books.at(i).as<const steeljson::object&>();
		std::cout << "Title: " << book.at("title").as<const std::string&>() << std::endl;
		std::cout << "Author: " << book.at("author").as<const std::string&>() << std::endl;
		std::cout << "Publisher: " << book.at("publisher").as<const std::string&>() << std::endl;
		std::cout << "Edition: " << book.at("edition").as<const std::string&>() << std::endl;
		std::cout << "Language: " << book.at("language").as<const std::string&>() << std::endl;
		std::cout << "ISBN-10: " << book.at("ISBN-10").as<const std::string&>() << std::endl;
		std::cout << "Pages: " << book.at("pages").as<std::int16_t>() << std::endl;
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[]) {
	try {
		std::ifstream ifs{ "books.json" };
		const steeljson::value val{ steeljson::read_document(ifs) };
		const steeljson::object& doc{ val.as<const steeljson::object&>() };
		const steeljson::array& books{ doc.at("books").as<const steeljson::array&>() };
		print_books(books);
	} catch (const std::runtime_error& e) {
		std::cout << e.what();
	}

	return 0;
}
