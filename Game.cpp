#include "Game.h"

void Game::start() {

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif // _WIN32

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned short> d_c(0, columns - 1);
	std::uniform_int_distribution<unsigned short> d_r(0, rows - 1);

	for (unsigned short i = 0; i < 60; ++i)
		table[d_c(gen)][d_r(gen)] = true;

	print();

	do {
		std::array<std::array<bool, columns>, rows> new_table{};
		std::copy(table.begin(), table.end(), new_table.begin());

		for (unsigned short x = 0; x < columns; ++x)
			for (unsigned short y = 0; y < rows; ++y) {
				bool field = table[x][y];
				std::array<std::array<bool, 3>, 3> sub = get_sub_field(x, y);

				if (field) {
					size_t alive =
							   std::count_if(sub.begin(), sub.end(), [](const std::array<bool, 3> r) { return r[0]; }) +
							   std::count_if(sub.begin(), sub.end(), [](const std::array<bool, 3> r) { return r[1]; }) +
							   std::count_if(sub.begin(), sub.end(), [](const std::array<bool, 3> r) { return r[2]; }) -
							   1,
						   dead = 9 - alive;

					if (alive == 2 || alive == 3)
						continue;
					else
						new_table[x][y] = false;
				}
				else {
					size_t alive =
							   std::count_if(sub.begin(), sub.end(), [](const std::array<bool, 3> r) { return r[0]; }) +
							   std::count_if(sub.begin(), sub.end(), [](const std::array<bool, 3> r) { return r[1]; }) +
							   std::count_if(sub.begin(), sub.end(), [](const std::array<bool, 3> r) { return r[2]; }),
						   dead = 8 - alive;

					if (alive == 3)
						new_table[x][y] = true;
				}
			}

		std::copy(new_table.begin(), new_table.end(), table.begin());

		std::this_thread::sleep_for(500ms);
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif // _WIN32

		print();
	} while (!std::all_of(table.begin(), table.end(), [](const std::array<bool, rows>& b) {
		return std::all_of(b.begin(), b.end(), [](const bool& b) { return !b; });
	}));

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif // _WIN32

	std::cout << "\u001b[47m\u001b[30;1m\u001b[1m\u001b[4m Game is finished \u001b[0m";
}

std::array<std::array<bool, 3>, 3> Game::get_sub_field(unsigned short x, unsigned short y) {
	std::array<std::array<bool, 3>, 3> sub{};

	for (unsigned short i = x + 1; i >= clamp(static_cast<int>(x - 1), 0, x + 1) && i < 3; --i)
		for (unsigned short j = y + 1; j >= clamp(static_cast<int>(y - 1), 0, x + 1) && j < 3; --j)
			sub[i][j] = table[i][j];

	return sub;
}

void Game::print() {

#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(hConsole, &consoleMode);
	consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, consoleMode);

	std::stringstream ss{};
	ss << ' ' << "\u001b[34m" << std::string(columns * 3 + 4, '=') << "\u001b[0m" << '\n';
	for (const auto& row : table) {
		ss << "\u001b[34m"
		   << " | "
		   << "\u001b[0m";
		for (const bool& field : row)
			ss << "\u001b[32m" << (field ? " O " : "   ") << "\u001b[0m";
		ss << "\u001b[34m"
		   << " | "
		   << "\u001b[0m" << '\n';
	}

	ss << ' ' << "\u001b[34m" << std::string(columns * 3 + 4, '=') << "\u001b[0m";
	std::cout << ss.str();
#else
	std::stringstream ss{};
	ss << ' ' << std::string(columns * 3 + 4, '=') << '\n';
	for (const auto& row : table) {
		ss << " | ";
		for (const bool& field : row)
			ss << (field ? " O " : "   ");
		ss << " | " << '\n';
	}

	ss << ' ' << std::string(columns * 3 + 4, '=');
	std::cout << ss.str();
#endif // _WIN32
}
