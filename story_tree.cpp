#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>

enum StepAction
{
	SINGLE_PATH,
	TWIN_PATH,
	RAND_PATH
};

struct UserOptions
{
	StepAction chooseType;
	char letter[3];
	std::string description[2];
	int healthShift[2];
};

bool isAlive(const int&);
int makeMove(int&, std::string, const UserOptions*, int);

int main()
{
	int player1 = 100;
	int treeCode = 0;
	UserOptions optObj;

	srand(time(NULL));


	while (isAlive(player1))
	{
		//printf("\n%d (%d)\n", treeCode, player1);
		switch (treeCode)
		{
		case 0x0:
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'r'; optObj.letter[1] = 'l';
			optObj.description[0] = "Choose cave"; optObj.description[1] = "Choose merchant road";
			optObj.healthShift[0] = 0; optObj.healthShift[1] = 0;
			treeCode |= makeMove(player1, "You are an adventurer", &optObj, 0);
			break;
		case 0x1:
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'f'; optObj.letter[1] = 'r';
			optObj.description[0] = "fight"; optObj.description[1] = "run";
			optObj.healthShift[0] = -50; optObj.healthShift[1] = -10;
			treeCode |= makeMove(player1, "An injured wolf is following you, what to do?", &optObj, 1);
			break;
		case 0x5:
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'd'; optObj.letter[1] = 'n';
			optObj.description[0] = "Take Detour"; optObj.description[1] = "follow narrow passage";
			optObj.healthShift[0] = -10; optObj.healthShift[1] = -40;
			treeCode |= makeMove(player1, "Choose path?", &optObj, 2);
			break;
		case 0x9:
			optObj.chooseType = RAND_PATH; optObj.letter[0] = 'd'; optObj.letter[1] = 'n';
			optObj.description[0] = "You've taken a detour"; optObj.description[1] = "You followed a narrow passage";
			optObj.healthShift[0] = -10; optObj.healthShift[1] = -40;
			treeCode |= makeMove(player1, "A random path has been chosen", &optObj, 2);
			break;
		case 0x2:
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'r'; optObj.letter[1] = 'l';
			optObj.description[0] = "Accept"; optObj.description[1] = "Decline";
			optObj.healthShift[0] = -50; optObj.healthShift[1] = -10;
			treeCode |= makeMove(player1, "Merchant offers you travel with him", &optObj, 1);
			break;
		default:
			player1 = 0;
			break;
		}

		std::cout << std::endl << "--------------------------------------------------" << std::endl;
	}

	//treeCode = makeMove(&player1, "Choose a path", "", TWIN_PATH, "-50,-20", 1);

	printf("%d", treeCode);

	system("pause");
	return EXIT_SUCCESS;
}

int makeMove(int &health, std::string prompt, const UserOptions *options, int bitShift)
{
	char letterSel;
	int newBit;
	
	std::cout << prompt << std::endl;

	switch (options->chooseType)
	{
	case SINGLE_PATH:
		std::cout << options->description[0];
		system("pause");
		//thisPlayer->health += std::stoi(parseStr);

		break;
	case TWIN_PATH:
		std::cout << options->letter[0] << ": " << options->description[0] << std::endl;
		std::cout << options->letter[1] << ": " << options->description[1] << std::endl;
		std::cout << "What do you choose? ";

		while (!(std::cin >> letterSel)
			  || (letterSel != options->letter[0]
			  && letterSel != options->letter[1]))
		{
			std::cout << "That is not a valid command. Try again: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits< std::streamsize>::max(), '\n');
		}

		if (letterSel == options->letter[0])
		{
			health += options->healthShift[0];
			newBit = 1;
		}
		else //is option 2
		{
			health += options->healthShift[1];
			newBit = 2;
		}

		break;
	case RAND_PATH:
		std::cout << options->description[0];

		if (rand() % 2)
		{
			health += options->healthShift[0];
			newBit = 1;
		}
		else
		{
			health += options->healthShift[1];
			newBit = 2;
		}

		break;
	default:
		break;
	}

	return newBit << bitShift*2;
}

bool isAlive(const int &health)
{
	return health > 0;
}