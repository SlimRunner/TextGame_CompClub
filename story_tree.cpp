#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>

///<summary>Enumerates the possible options that can be used in a move</summary>
enum StepAction
{
	SINGLE_PATH,
	TWIN_PATH,
	RAND_PATH
};

///<summary>Contains a set of values used to determine a move</summary>
struct UserOptions
{
	StepAction chooseType; //type of options
	char letter[3]; //contains the two leters that will be given as choice. mind that there must be room for the null character.
	std::string description[2]; //parallel array to letter that will contain descriptive text of each choice.
	int healthShift[2]; //parallel array to letter determines how a related value of health will change given the option chosen
};

bool isAlive(const int&);
int makeMove(int&, std::string, const UserOptions*, int);

int main()
{
	int player1 = 100; //users life
	int treeCode = 0; //bitfield that uniquely identifies the tree of choices
	UserOptions optObj; //object that contains the options that will display

	srand(time(NULL)); //seeds random value to the current time

	while (isAlive(player1))
	{
		//printf("\n%d (%d)\n", treeCode, player1);
		switch (treeCode)
		{
		case 0x0: //00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'r'; optObj.letter[1] = 'l';
			optObj.description[0] = "Choose cave"; optObj.description[1] = "Choose merchant road";
			optObj.healthShift[0] = 0; optObj.healthShift[1] = 0;
			treeCode |= makeMove(player1, "You are an adventurer", &optObj, 0);
			break;
		case 0x1: //00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 01
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'f'; optObj.letter[1] = 'r';
			optObj.description[0] = "fight"; optObj.description[1] = "run";
			optObj.healthShift[0] = -50; optObj.healthShift[1] = -10;
			treeCode |= makeMove(player1, "An injured wolf is following you, what to do?", &optObj, 1);
			break;
		case 0x5: //00 00 00 00  00 00 00 00  00 00 00 00  00 00 01 01
			optObj.chooseType = TWIN_PATH; optObj.letter[0] = 'd'; optObj.letter[1] = 'n';
			optObj.description[0] = "Take Detour"; optObj.description[1] = "follow narrow passage";
			optObj.healthShift[0] = -10; optObj.healthShift[1] = -40;
			treeCode |= makeMove(player1, "Choose path?", &optObj, 2);
			break;
		case 0x9: //00 00 00 00  00 00 00 00  00 00 00 00  00 00 10 01
			optObj.chooseType = RAND_PATH; optObj.letter[0] = 'd'; optObj.letter[1] = 'n';
			optObj.description[0] = "You've taken a detour"; optObj.description[1] = "You followed a narrow passage";
			optObj.healthShift[0] = -10; optObj.healthShift[1] = -40;
			treeCode |= makeMove(player1, "A random path has been chosen", &optObj, 2);
			break;
		case 0x2: //00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 10
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

///<summary>Prints data passed into the struct and returns a bit shifted bit pair that determines the option chosen.</summary>
///<param name="health">Numeric value passed by reference that will be used to punish or reward a choice</param>
///<param name="prompt">This text will describe the premise or a question that needs to be resolved</param>
///<param name="options">Structure that contains the options that will be given as resolution to the prompt</param>
///<param name="level">This the level of the current path. It determines the return value by shifting to the left its value twice to a pair of bits.</param>
///<returns>A unique identifier to a given tree of choices that works by accumulating every return for a level into a bitfield using logical disjuntion.</returns>
int makeMove(int &health, std::string prompt, const UserOptions *options, int level)
{
	char letterSel;
	int newBit;
	
	//prints the premise or question
	std::cout << prompt << std::endl;

	//determines the type of path
	switch (options->chooseType)
	{
	case SINGLE_PATH:
		std::cout << options->description[0];
		system("pause");
		//thisPlayer->health += std::stoi(parseStr);

		break;
	case TWIN_PATH:
		//prints the two option letters along with their respective description in the standard output device
		std::cout << options->letter[0] << ": " << options->description[0] << std::endl;
		std::cout << options->letter[1] << ": " << options->description[1] << std::endl;
		std::cout << "What do you choose? ";

		//repeat while input is invalid and user didn't entered either letter 1 or letter 2
		while (!(std::cin >> letterSel)				//cin return is null when input is not valid. Negated to force repetition under failure
			  || (letterSel != options->letter[0]	//determines if the user didn't chose the first option
			  && letterSel != options->letter[1]))	//determines if the user didn't chose the second option
		{
			std::cout << "That is not a valid command. Try again: "; //prints error message
			std::cin.clear(); //clears the error in the standard I/O device
			std::cin.ignore(std::numeric_limits< std::streamsize>::max(), '\n'); //standard way of clearing new line characters
		}

		//determines the user choice
		if (letterSel == options->letter[0])
		{//user chose option 1
			health += options->healthShift[0]; //deducts health according to this option
			newBit = 1; //binary value that determines that this option was chosen (01 in binary)
		}
		else
		{//user chose option 2
			health += options->healthShift[1]; //deducts health according to this option
			newBit = 2; //binary value that determines that this option was chosen (10 in binary)
		}
		break;

	case RAND_PATH:
		std::cout << options->description[0];

		//chooses randomly with a 50/50 chance either option
		if (rand() % 2)
		{
			health += options->healthShift[0]; //deducts health according to this option
			newBit = 1; //binary value that determines that this option was chosen (01 in binary)
		}
		else
		{
			health += options->healthShift[1]; //deducts health according to this option
			newBit = 2; //binary value that determines that this option was chosen (10 in binary)
		}
		break;

	default:
		break;
	}

	//returns the binary value determined the the choice the user made and bitshifts the value twice by the level
	return newBit << level*2;

	//successive levels can be accumulated in duplets (eg. 10 10 01 10 01 10) to create a unique identifier for any give tree of paths
}

///<summary>Determines if <code>health</code> is greater than zero</summary>
bool isAlive(const int &health)
{
	return health > 0;
}