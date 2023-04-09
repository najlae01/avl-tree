// avl-library.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*

Tests for the AVLTree class.=
*/

#include "library.h"
#include <fstream>

int main() {
	int error = 0;
	std::cout << "Unit Test #3" << std::endl;
	Library lib;
	std::ifstream lib_txt("exemple_librairie_a.txt");
	std::string line;
	while (std::getline(lib_txt, line)) {
		Book book(line);
		lib.insert(book);
	};
	std::string book_str_1 = "The broom of the system;9784062061612;David Foster Wallace;19";
	Book book_1(book_str_1);
	std::string book_str_2 = "The Promise;9780399161490;Robert Crais;17";
	Book book_2(book_str_2);
	std::string book_str_3 = "The Broken Crown;9780886777401;Michelle West;12";
	Book book_3(book_str_3);
	std::string book_str_4 = "The Last Painting Of Sara De Vos;9781410490636;Dominic Smith;27";
	Book book_4(book_str_4);
	std::string book_str_5 = "Sunshine;9780425191781;Robin McKinley;13";
	Book book_5(book_str_5);
	Library lib_bis = lib;
	lib.merge(lib_bis);
	if (!(lib == lib_bis)) {
		std::cerr << "FAILURE - I" << std::endl;
		error++;
	}
	if (!lib.contains(book_1) ||
		!lib.contains(book_2) ||
		!lib.contains(book_3) ||
		!lib.contains(book_4) ||
		!lib.contains(book_5)) {
		std::cerr << "FAILURE - II" << std::endl;
		error++;
	}
	if (lib.total(book_1) != 38 ||
		lib.total(book_2) != 34 ||
		lib.total(book_3) != 24 ||
		lib.total(book_4) != 54 ||
		lib.total(book_5) != 26) {
		std::cerr << "FAILURE - III" << std::endl;
		error++;
	}
	lib.merge(lib_bis);
	lib.merge(lib_bis);
	if (lib.total(book_1) != 76 ||
		lib.total(book_2) != 68 ||
		lib.total(book_3) != 48 ||
		lib.total(book_4) != 108 ||
		lib.total(book_5) != 52) {
		std::cerr << "FAILURE - IV" << std::endl;
		error++;
	}
	if (!(lib == lib_bis)) {
		std::cerr << "FAILURE - V" << std::endl;
		error++;
	}
	if (error == 0)
		std::cout << "\t==> OK" << std::endl;
	return error;
}