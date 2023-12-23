#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>

enum Symbol {
	CROSS = 'X',
	ZERO = '0',
	EMPTY = '-'
};

enum gameProgress
{
	IN_PROGRESS,
	PLAYER_WON,
	AI_WON,
	DRAW
};

struct Coord
{
	size_t y{ 0 };
	size_t x{ 0 };
};
#pragma pack(push, 1)
struct myGame
{
	Symbol** ppField{ nullptr };
	gameProgress progress = IN_PROGRESS;
	const size_t SIZE = 3;
	size_t turn = 0;
	Coord playerCord;
	Coord AI_Cord;
	Symbol player;
	Symbol ai;
};
#pragma pack(pop)


void __fastcall destructGame(myGame& rGame) {
	for (size_t i = 0; i < rGame.SIZE; i++)
	{
		delete[] rGame.ppField[i];
	}
	delete[] rGame.ppField;
	rGame.ppField = nullptr;
}

void inline clearScr() {
	system("cls");
}

int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::mt19937_64 generator(seed);
	std::uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

void __fastcall initGame(myGame& rGame) {
	rGame.ppField = new Symbol * [rGame.SIZE];

	for (size_t i = 0; i < rGame.SIZE; i++)
	{
		rGame.ppField[i] = new Symbol[rGame.SIZE];
	}

	if (getRandomNum(0, 100) >= 51)
	{
		rGame.player = CROSS;
		rGame.ai = ZERO;
		rGame.turn = 0;
	}
	else if (getRandomNum(0, 100) < 51)
	{
		rGame.player = ZERO;
		rGame.ai = CROSS;
		rGame.turn = 1;
	}

	for (size_t y = 0; y < rGame.SIZE; y++)
	{
		for (size_t x = 0; x < rGame.SIZE; x++)
		{
			rGame.ppField[y][x] = EMPTY;
		}
	}
}

void __fastcall paintField(myGame& rGame) {
	std::cout << "  ";
	for (size_t x = 0; x < rGame.SIZE; x++)
	{
		std::cout << "   " << x + 1;
	}
	std::cout << std::endl;

	for (size_t y = 0; y < rGame.SIZE; y++)
	{
		std::cout << " " << y + 1 << " | ";
		for (size_t x = 0; x < rGame.SIZE; x++)
		{
			std::cout << (char)rGame.ppField[y][x] << " | ";
		}
		std::cout << std::endl;
	}
	std::cout << "Player symbol: " << (char)rGame.player << "\n" << "AI symbol: " << (char)rGame.ai << "\n" << "Turn: "
		<< rGame.turn << "\n";
}

void __fastcall playerTurn(myGame& rGame) {
	do{
	std::cout << "Enter Y coord: ";
	std::cin >> rGame.playerCord.y;
	std::cout << "Enter X coord: ";
	std::cin >> rGame.playerCord.x;

	} while (rGame.ppField[rGame.playerCord.y - 1][rGame.playerCord.x - 1] != EMPTY);
	rGame.ppField[rGame.playerCord.y - 1][rGame.playerCord.x - 1] = rGame.player;
}

void __fastcall checkWinner(myGame& rGame) {
	Symbol check = EMPTY;
	bool flag = false;
	for (size_t y = 0; y < rGame.SIZE; y++)
	{
		for (size_t x = 0; x < rGame.SIZE; x++)
		{
			if (rGame.ppField[0][0] == rGame.ppField[0][1] && rGame.ppField[0][1] == rGame.ppField[0][2])
			{
				check = rGame.ppField[0][0];
			}

			else if (rGame.ppField[1][0] == rGame.ppField[1][1] && rGame.ppField[1][1] == rGame.ppField[1][2])
			{
				check = rGame.ppField[1][0];
			}

			else if (rGame.ppField[2][0] == rGame.ppField[2][1] && rGame.ppField[2][1] == rGame.ppField[2][2])
			{
				check = rGame.ppField[2][0];
			}

			else if (rGame.ppField[0][0] == rGame.ppField[1][0] && rGame.ppField[1][0] == rGame.ppField[2][0])
			{
				check = rGame.ppField[0][0];
			}

			else if (rGame.ppField[0][1] == rGame.ppField[1][1] && rGame.ppField[1][1] == rGame.ppField[2][1])
			{
				check = rGame.ppField[0][1];
			}

			else if (rGame.ppField[0][2] == rGame.ppField[1][2] && rGame.ppField[1][2] == rGame.ppField[2][2])
			{
				check = rGame.ppField[0][2];
			}
			else if (rGame.ppField[0][0] == rGame.ppField[1][1] && rGame.ppField[1][1] == rGame.ppField[2][2])
			{
				check = rGame.ppField[0][0];
			}

			else if (rGame.ppField[0][2] == rGame.ppField[1][1] && rGame.ppField[1][1] == rGame.ppField[2][0])
			{
				check = rGame.ppField[0][2];
			}

			if (rGame.ppField[y][x] == EMPTY)
			{
				flag = true;
			}

		}
	}

	if (check == rGame.player)
	{
		rGame.progress = PLAYER_WON;
	}
	else if (check == rGame.ai)
	{
		rGame.progress = AI_WON;
	}
	else if (!flag) rGame.progress = DRAW;

}

void __fastcall AI_Turn(myGame& rGame) {
	bool flag = false;

	for (size_t y = 0; y < rGame.SIZE; y++)
	{
		for (size_t x = 0; x < rGame.SIZE; x++)
		{
			if (rGame.ppField[y][x] == EMPTY)
			{
				rGame.ppField[y][x] = rGame.ai;
				checkWinner(rGame);
				if (rGame.progress == AI_WON)
				{
					flag = true;
					break;
				}
				else {
					rGame.progress = IN_PROGRESS;
					rGame.ppField[y][x] = EMPTY;
				}
				break;
			}
		}
		if (flag)
		{
			break;
		}
	}

	if (!flag)
	{
		for (size_t y = 0; y < rGame.SIZE; y++)
		{
			for (size_t x = 0; x < rGame.SIZE; x++)
			{
				if (rGame.ppField[y][x] == EMPTY)
				{
					rGame.ppField[y][x] = rGame.player;
					checkWinner(rGame);
					if (rGame.progress == PLAYER_WON)
					{
						rGame.ppField[y][x] = rGame.ai;
						rGame.progress = IN_PROGRESS;
						checkWinner(rGame);
						flag = true;
						break;
					}
					else {
						rGame.progress = IN_PROGRESS;
						rGame.ppField[y][x] = EMPTY;
					}
				}
			}
			if (flag)
			{
				break;
			}
		}
	}

	if (!flag)
	{
		if (rGame.ppField[1][1] == EMPTY) {
			rGame.ppField[1][1] = rGame.ai;
			flag = true;
		}
		else if (rGame.ppField[0][0] == EMPTY)
		{
			rGame.ppField[0][0] = rGame.ai;
			flag = true;
		}
		else if (rGame.ppField[2][0] == EMPTY)
		{
			rGame.ppField[2][0] = rGame.ai;
			flag = true;
		}
		else if (rGame.ppField[0][2] == EMPTY)
		{
			rGame.ppField[0][2] = rGame.ai;
			flag = true;
		}
		else if (rGame.ppField[2][2] == EMPTY)
		{
			rGame.ppField[2][2] = rGame.ai;
			flag = true;
		}
	}
	
	if (!flag)
	{
		do
		{
			rGame.AI_Cord.y = getRandomNum(0, 2);
			rGame.AI_Cord.x = getRandomNum(0, 2);

		} while (rGame.ppField[rGame.AI_Cord.y][rGame.AI_Cord.x] != EMPTY);
		rGame.ppField[rGame.AI_Cord.y][rGame.AI_Cord.x] = rGame.ai;
	}
}


int main()
{
	myGame game;
	initGame(game);

	do
	{
		clearScr();
		paintField(game);
		if (game.turn % 2 != 0)
		{
			AI_Turn(game);
		}
		else
		{
			playerTurn(game);
		} 
		game.turn++;

		paintField(game);

		checkWinner(game);

	} while (game.progress == IN_PROGRESS);

		if (game.progress == AI_WON)
		{
			std::cout << "AI WON =(";
		}
		else if (game.progress == PLAYER_WON) {
			std::cout << "PLAYER WON =)";
		}
		else if (game.progress == DRAW)
		{
			std::cout << "It`s draw =/";
		}

	destructGame(game);
}