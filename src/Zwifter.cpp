/*
 * Zwifter.cpp
 *
 *  Created on: 5 déc. 2019
 *      Author: vgol
 */

#include <iostream>
#include <fstream>

#include "Zwifter.hpp"


/*
 * It will iterate through all the lines in file and
 * put them in given vector
 */
bool getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs)
{

	// Open the File
	std::ifstream in(fileName.c_str());

	// Check if object is valid
	if (!in)
	{
		std::cerr << "Cannot open the File : " << fileName << std::endl;
		return false;
	}

	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
			vecOfStrs.push_back(str);
	}
	//Close The File
	in.close();
	return true;
}

void parseAndConvert(Zwifter &zwifter_, std::vector<std::string> input, std::vector<std::string> output) {

	bool is_repet = false;
	std::string repet1;
	std::string line;

	for (auto raw_line : input) {

		if (raw_line.find("FTP") == std::string::npos) {

			// add single line
			zwifter_.addSingle(raw_line);

		} else {

			line = raw_line.substr(0, raw_line.find("FTP") + 3);

			if (is_repet) {
				is_repet = false;

				// handle second part of repetition
				zwifter_.addRepetition(repet1, line);
			}

			if (line.at(1) == 'x' || line.at(2) == 'x') {
				// we have a repetition
				is_repet = true;
				repet1 = line;

				continue;
			}
			else {

				// add single line
				zwifter_.addSingle(line);
			}

		}

	}

	zwifter_.finish();

	return;
}

int main(int argc, char *argv[]) {

	assert(argc == 2);

	std::vector<std::string> vecOfStr;
	std::vector<std::string> output;

	const char * fname = argv[1];

	// Get the contents of file in a vector
	bool result = getFileContent(fname, vecOfStr);

	if (result) {
		Zwifter myZwifter(fname);

		parseAndConvert(myZwifter, vecOfStr, output);

		myZwifter.print();
	} else {

		return -1;
	}

	return 0;
}

