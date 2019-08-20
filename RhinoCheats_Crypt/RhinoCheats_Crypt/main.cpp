#include <string>
#include <fstream>
#include <iostream>

#include "Encryption/Encryption.h"

int main(int argc, char* argv[])
{
	bool mode;
	std::string input, output;

	if (argc > 3) {
		mode = (bool)atoi(argv[1]);
		input = argv[2];
		output = argv[3];
	}

	else {
		return -1;
	}

	if (mode)
		encrypt(input, output);

	if (!mode)
		decrypt(input, output);

	return 0;
}