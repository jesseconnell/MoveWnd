// MoveWnd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>

std::vector<std::tuple<HWND, std::string>> possibles;
char target[1000];

BOOL CALLBACK enumWindowsProc(
	__in  HWND hWnd,
	__in  LPARAM lParam) 
{
	char text[1000];
	::GetWindowText(hWnd, text, 1000);
	if (strlen(text) >= strlen(target))
	{
		bool match = true;
		for (int i = 0; i < strlen(target); ++i)
		{
			if (target[i] != text[i])
			{
				match = false;
				break;
			}
		}
		if (match)
		{
			possibles.emplace_back(hWnd, std::string(text));
		}
	}
	return TRUE;
}

int main(int argc, char** argv)
{
	strncpy_s(target, argv[1], 1000);
	::EnumWindows(enumWindowsProc, NULL);
	int counter = 0;
	for (auto& p : possibles)
	{
		HWND hwnd;
		std::string text;
		std::tie(hwnd, text) = p;
		std::cout << ++counter << ": " << std::hex << (int)hwnd << "     '" << text << std::dec << "'\n";
	}
	if (!possibles.empty())
	{
		std::cout << "Move: ";
		int chosen = 0;
		std::cin >> chosen;
		if (chosen >= 1 && chosen <= possibles.size())
		{
			HWND hwnd;
			std::string text;
			std::tie(hwnd, text) = possibles[chosen - 1];
			RECT rect;
			::GetWindowRect(hwnd, &rect);
			auto&& logRect = [](auto s, RECT& r) {
				std::cout << s << ":  (" << r.left << ", " << r.top << ", " << r.right << ", " << r.bottom << ")" << std::endl;
			};
			logRect("Old rect", rect);
			::OffsetRect(&rect, -rect.left, -rect.top);
			logRect("New rect", rect);
			int height = rect.bottom - rect.top;
			int width = rect.right - rect.left;
			::MoveWindow(hwnd, rect.left, rect.top, width, height, TRUE);
		}
	}
    return 0;
}

