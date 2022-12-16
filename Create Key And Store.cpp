#define _CRT_SECURE_NO_WARNINGS //disable build error caused by std::localtime

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <cstdlib> 
#include <ctime> 
#include <Windows.h>

const std::string fileName = "\\KEYS"; //used later on to create our keys folder
const std::string iniName = "\\KEYS.ini"; //used later on to store our keys

const int alphabetLettersCount = 26;

std::string getDate()
{
	std::time_t t = std::time(nullptr); //get the time
	std::tm tm = *std::localtime(&t); //convert time into std::tm struct
	std::stringstream ss; //temp

	//format the date
	ss << (1900 + tm.tm_year) << "-"
		<< std::setfill('0') << std::setw(2) << (1 + tm.tm_mon) << "-"
		<< std::setfill('0') << std::setw(2) << tm.tm_mday;

	return ss.str();
}


std::string getFileLocation()
{
	char buffer[MAX_PATH]; //temp
	GetModuleFileNameA(NULL, buffer, MAX_PATH); //get our module
	auto loc = std::filesystem::path(buffer).parent_path(); //get our exe's file location
	return loc.u8string(); //return as a string
}


void checkFileExists()
{
	std::string tempLocation = getFileLocation() + fileName; //store our file location

	if (!std::filesystem::exists(tempLocation)) //check if file exists or not
	{
		std::cout << "File has been created to " << tempLocation << ", first run? \n";

		std::filesystem::create_directory(tempLocation); //if not create a file in our exe's loation
	}
	else
	{
		std::cout << "Seems like our file exists at " << tempLocation << " \n";
	}
}

void write2File(const char* input_text)
{
	if (!input_text) return; //just to be sure...

	std::string fileLocation = getFileLocation() + fileName + iniName; //get our ini locaction
	std::string currentDate = getDate(); //store the date in a temp variable

	std::ostringstream tempkeyStringStream; tempkeyStringStream << "KEY[" << input_text << "(" << std::string(input_text).length() << ")" << "]"; //messy but does the job.


	WritePrivateProfileStringA(currentDate.c_str(), tempkeyStringStream.str().c_str(), input_text, fileLocation.c_str()); //write to file
}

std::vector<std::string> makeKeys()
{

	int keyCount; //temp
	int keyLength; //temp
	bool useNumbers; //temp

LABEL_0:
	{
		std::cout << "Hello there, how many keys would you want? \n"; //prompt user
		std::cin >> keyCount;
	}

	if (!keyCount)
		goto LABEL_0; //if key count is 0 go back

LABEL_1:
	{
		std::cout << "And what length would you like? \n"; //prompt user
		std::cin >> keyLength;
	}

	if (!keyLength)
		goto LABEL_1; //if key length is 0 go back


	std::cout << "Should i use numbers? (0 = false, 1 = true) \n"; //prompt user
	std::cin >> useNumbers;


	std::vector<std::string> tempKeys; //temp
	std::string tempKey; //temp

	for (int i = 0; i < keyCount; i++)//execution times depending on keyCount
	{
		for (int j = 0; j < keyLength; j++) //execution times depending on keyLength
		{
			int current_char = rand() % alphabetLettersCount; //make a random number depending on the size of our alphabet
			int current_number = rand() % 10; //make a random number from 0-9

			bool number_or_char = rand() % 2; //make a true(1) or false(0) statement randomly

			if (number_or_char)
			{
				tempKey += std::to_string(0 + current_number); //0 being our start number we add our random number on top of it to get our final number and convert it to a string
			}
			else
			{
				tempKey += 'A' + current_char; //the ASCII codes for the uppercase alphabetical characters are in the range 65 to 90, so we start from A which is 65 and add our random number(which at max is 25) on top of it
			}
		}
		tempKeys.push_back(tempKey); //push our temp key into a vector
		tempKey.clear(); //clear our temp key so we don't overwrite it
	}

	return tempKeys; //return the vector
}

int main()
{
	srand(time(static_cast<time_t*>(0)));  //seed for rand

	checkFileExists(); //check if our file that we are going to write our keys to exists

	for (std::string vec : makeKeys()) //make our keys and parse them to write in our file
	{
		write2File(vec.c_str()); //write to file
		std::cout << vec << "\n"; //print the keys to console
	}

	std::cout << "Created your keys boss, want me to open the location? (0 = no, 1 = yes) \n";

	bool openLocation;

	std::cin >> openLocation; //prompt user

	if (!openLocation)
		return 0;

	std::string filePath = getFileLocation() + fileName;
	ShellExecuteA(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOWDEFAULT); //open our file location

	return 0;
}

