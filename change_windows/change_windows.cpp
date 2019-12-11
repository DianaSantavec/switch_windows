#include "pch.h"
#include <iostream>
#include <windows.h>
#include <future>
#include <list>
#include <string>

//const int DELAY = 5000;

std::list<std::string> windows_names;
std::list <std::string> split_string(std::string user_input);
std::list <HWND> list_of_HWNDs;
void show_windows(std::list<std::string> user_input, std::list <HWND> list_of_HWNDs);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char WinText[256] = { NULL };

	if (!hwnd || !IsWindowVisible(hwnd)) // if hwnd == NULL || if the window is not visible
		return 1; // continue recursion
	else {
		GetWindowTextA(hwnd, WinText, sizeof(WinText));
		if (strlen(WinText) != 0) {
			windows_names.push_back(WinText);
			list_of_HWNDs.push_back(hwnd);
		}
		return 1; // continue recursion
	}
}
int main()
{
	int counter = 0;
	std::string user_input;
	EnumWindows(EnumWindowsProc, 0);
	for (auto const& window_name : windows_names) {
		std::cout << ++counter << ") " << window_name << "\n";
	}
	std::cout << "\n\n" << "MENI" << "Unesite brojeve aplikacija koje zelite da se smenjuju razdvojene zarezima: ";
	std::cin >> user_input;
	std::list<std::string> user_input_splitted = split_string(user_input);

	//for (auto const& one : user_input_splitted) {
	//	std::cout << one;
	//}

	while (1) {
		show_windows(user_input_splitted, list_of_HWNDs);
	}
	return 0;
}

std::list <std::string> split_string(std::string user_input) {
	std::list <std::string> result;
	std::string temp = "";
	int i = 0;
	while (user_input[i] != '\0') {
		if (user_input[i] != ',') {
			temp += user_input[i];
		}
		else {
			result.push_back(temp);
			temp = "";
		}
		i++;
	}
	result.push_back(temp);
	return result;
}


void show_windows(std::list<std::string> user_input, std::list <HWND> list_of_HWNDs) {
	int counter_int;
	for (auto const& counter : user_input) {
		counter_int = stoi(counter);
		std::list<HWND>::iterator iterator = list_of_HWNDs.begin();
		std::advance(iterator, counter_int - 1);
		SetForegroundWindow(*iterator);
		Sleep(5000);
	}
}