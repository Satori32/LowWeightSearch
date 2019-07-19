#pragma once
#include <iostream>
#include <cstdint>

#if defined(_WIN32_)

//https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
//http://7ujm.net/etc/esc.html

/**/
#include <windows.h>

bool SetDisplayMode() {//only after Windows10 Th2?
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD M=0;
	if (GetConsoleMode(H, &M) == FALSE) return false;

	if (SetConsoleMode(H, M | ENABLE_VIRTUAL_TERMINAL_PROCESSING) == FALSE) return false;

	return true;
}
/**/
#else

bool SetDisplayMode() { return true; }

#endif

namespace  ESS {//EscapeSequenceSpecial
	bool ClearShow() {
		std::cout << "\033[2J";
		return true;
	}
	bool ClearSetting() {
		std::cout << "\033[!p";
		return true;
	}
	bool ClearLine() {
		std::cout << "\033[2K";
		return true;
	}
	bool Locate(const std::uint_fast32_t& X, const std::int_fast32_t Y) {
		std::cout << "\033[" << Y << ";" << X << "H";
		return true;
	}
	bool LocateUp(const std::uint_fast32_t& N) {
		std::cout << "\033[" << N << "A";
		return true;
	}
	bool LocateDown(const std::uint_fast32_t& N) {
		std::cout << "\033[" << N << "B";
		return true;
	}
	bool LocateRight(const std::uint_fast32_t& N) {
		std::cout << "\033[" << N << "C";
		return true;
	}
	bool LocateLwft(const std::uint_fast32_t& N) {
		std::cout << "\033[" << N << "D";
		return true;
	}
	bool SetFontColor(const std::uint_fast8_t& N) {//0<->9?
		std::cout << "\033[" << 30 + N << "m";
		return true;
	}
	bool SetBackgroundColor(const std::uint_fast8_t& N) {//0<->9?
		std::cout << "\033[" << 40 + N << "m";
		return true;
	}
	bool SetForegroundColor(const std::uint_fast8_t& N) {//0<->17?
		std::cout << "\033[" << 90 + N << "m";
		return true;
	}
	bool SetForegroundRGB(const std::uint_fast8_t& R, const std::uint_fast8_t& G, const std::uint_fast8_t& B) {
		std::cout << "\033[" << "38;" << "2;" << R << ";" << G << ";" << B << "m";
		return true;
	}
	bool SetBackgroundRGB(const std::uint_fast8_t& R, const std::uint_fast8_t& G, const std::uint_fast8_t& B) {
		std::cout << "\033[" << "48;" << "2;" << R << ";" << G << ";" << B << "m";
		return true;
	}
	bool SetUnderLine(const bool& b) {
		if (b) {
			std::cout << "\033[" << 4 << "m";
		}
		else {
			std::cout << "\033[" << 24 << "m";
		}
		return true;
	}
	bool HideCursor() {
		std::cout << "\033[?" << 25 << "h";
		return true;
	}
	bool ShowCursor() {
		std::cout << "\033[?" << 25 << "l";
		return true;
	}
	bool SetWindowTitle(const char* S) {
		std::cout << "\033]" << 0 << ";" << S << "\x07";
		return true;
	}
	
	//bool SetWindowSize(std::uint_fast16_t Width, std::uint_fast16_t Height) {
	//	std::cout << "\033[?" << 3 << "h";
	//	std::cout << "\033[?" << 3 << "l";
	//	return true;
	//}
	bool WriteLineDraw(std::uint_fast8_t N) {//element in 11
		if (N > 6)N += 3;
		std::cout << "\033(0" << static_cast<char>(0x6a + N) << "\033(B";
		return true;
	}
}