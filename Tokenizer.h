#ifndef TOKENIZER_H
#define TOKENIZER_H
using namespace std;
#include "Token.h"
#include <string>
#include <iostream>
#include <fstream>

class Tokenizer
{
private:
	std::string inputString;
	int line = 0;
public:
	Tokenizer() {};
	~Tokenizer() {};

	Token getNextToken(string& input, int& lineNum) {
		inputString = input;
		line = lineNum;
		Token tempToken;
		//char tempChar;
		string tokenString;
		//int code;

		if (input[0] == ',') { // COMMA 0
			tokenString = ",";
			tempToken.setToken(tokenString, lineNum, 0);
			input = input.substr(1, input.size() - 1);
		}

		else if (input[0] == '.') { // PERIOD 1
			tokenString = ".";
			tempToken.setToken(tokenString, lineNum, 1);
			input = input.substr(1, input.size() - 1);
		}

		else if (input[0] == '?') { // Q_MARK 2
			tokenString = "?";
			tempToken.setToken(tokenString, lineNum, 2);
			input = input.substr(1, input.size() - 1);
		}

		else if (input[0] == '(') { // LEFT_PAREN 3
			tokenString = "(";
			tempToken.setToken(tokenString, lineNum, 3);
			input = input.substr(1, input.size() - 1);
		}

		else if (input[0] == ')') { // RIGHT_PAREN 4
			tokenString = ")";
			tempToken.setToken(tokenString, lineNum, 4);
			input = input.substr(1, input.size() - 1);
		}

		else if (input[0] == ':') { // COLON and COLON_DASH
			if (input[1] == '-') { // COLON_DASH 6
				tokenString = ":-";
				tempToken.setToken(tokenString, lineNum, 6);
				input = input.substr(2, input.size() - 2);
			}
			else {
				tokenString = ":"; // COLON 5
				tempToken.setToken(tokenString, lineNum, 5);
				input = input.substr(1, input.size() - 1);
			}
		}

		else if (input[0] == '*') { // MULTIPLY 7
			tokenString = "*";
			tempToken.setToken(tokenString, lineNum, 7);
			input = input.substr(1, input.size() - 1);
		}

		else if (input[0] == '+') { // ADD 8
			tokenString = "+";
			tempToken.setToken(tokenString, lineNum, 8);
			input = input.substr(1, input.size() - 1);
		}

		else if (input.substr(0, 7) == "Schemes" && !isalpha(input[7]) && !isdigit(input[7])) { // Checks for Schemes 9
			tokenString = "Schemes";
			tempToken.setToken(tokenString, lineNum, 9);
			input = input.substr(7, input.size() - 7);
		}

		else if (input.substr(0, 5) == "Facts" && !isalpha(input[5]) && !isdigit(input[5])) { // Checks for Facts 10
			tokenString = "Facts";
			tempToken.setToken(tokenString, lineNum, 10);
			input = input.substr(5, input.size() - 5);
		}

		else if (input.substr(0, 5) == "Rules" && !isalpha(input[5]) && !isdigit(input[5])) { // Checks for Rules 11
			tokenString = "Rules";
			tempToken.setToken(tokenString, lineNum, 11);
			input = input.substr(5, input.size() - 5);
		}

		else if (input.substr(0, 7) == "Queries" && !isalpha(input[7]) && !isdigit(input[7])) { // Checks for Queries 12
			tokenString = "Queries";
			tempToken.setToken(tokenString, lineNum, 12);
			input = input.substr(7, input.size() - 7);
		}

		else if (isalpha(input[0])) { // Checks for ID 13
			string tempStr;
			unsigned int i;
			for (i = 0; i < input.size(); ++i) {
				if ((!isalpha(input[i]) && !isdigit(input[i])) || (isspace(input[i]))) {
					break;
				}
				else {
					tempStr.push_back(input[i]);
					continue;
				}
			}
			tokenString = tempStr;
			tempToken.setToken(tokenString, lineNum, 13);
			input = input.substr(i, input.size() - i);
		}

		else if (input[0] == '\'') { // Checks for Strings 14
			string tempStr;
			unsigned int i;
			size_t strCounter = 0; // Counts apostrophies for determining string input
			int tempLine = lineNum;
			int code = 17;
			for (i = 0; i < input.size(); ++i) {
				if (input[i] == EOF) { // anything other than '
					code = 17;
					input = input.substr(i, input.size() - i);
					line = lineNum;
					break;
				}
				else if (input[i] != '\'') {
					if (input[i] == '\n') {
						++lineNum;
						line = lineNum;
					}
					tempStr.push_back(input[i]);
				}
				else if (input[i] == '\'') { // When we hit a '
					++strCounter;
					if (strCounter > 1) {
						if (input[i + 1] == '\'') {
							tempStr.push_back(input[i]);
							tempStr.push_back(input[i + 1]);
							++i;
						}
						else if (input[i + 1] != '\'') {
							tempStr.push_back(input[i]);
							code = 14;
							break;
						}
					}
					else {
						tempStr.push_back(input[i]);
					}
				}
			}
			tokenString = tempStr;
			tempToken.setToken(tokenString, tempLine, code);
			line = lineNum;
			if (input[i] != '\0') {  // THIS HAS BEEN THROWING AN EXCEPTION
				input = input.substr(i + 1, input.size() - i);  // SWAP THIS <----   
			}
			else {
				input = input.substr(i, input.size() - i); // WITH THIS <----
			}
		}
		// ----------------------------------------------------------------------------------
		else if (input[0] == '#') { // Checks for comments 15
			if (input[1] == '|') {
				int tempNum = lineNum;
				string tempString;
				tempString += input[0];
				tempString += input[1];
				for (unsigned int i = 2; i < input.size(); ++i) {
					if (input[i] == '\n') {
						++lineNum;
					}
					if (i == input.size() - 1) {
						int code = 17;
						tempToken.setToken(input, tempNum, code);
						input = "";
						break;
					}
					else if (input[i] == '|') {
						if (input[i + 1] == '#') {
							tempString = tempString + input[i] + input[i + 1];
							int code = 15;
							tempToken.setToken(tempString, tempNum, code);
							if (input[i + 2] == EOF) {
								input = "";
								break;
							}
							else {
								input = input.substr(i + 2, input.size() - (i + 2));
								break;
							}
						}
					}
					tempString += input[i];
				}
			}
			else {
				string tempString;
				unsigned int i;
				for (i = 0; i < input.size(); ++i) {
					if (input[i] == '\n') {
						break;
					}
					else {
						tempString += input[i];
					}
				}
				int code = 15;
				tempToken.setToken(tempString, lineNum, code);
				input = input.substr(i, input.size() - i);
			}

		}

		// Not a Token Identifier
		else if (isspace(input[0])) { // COMMENTED OUT TO SAVE THE CODE ITSELF
			input = input.substr(1, input.size() - 1);
		}

		// Not a Token Identifier
		else if (input[0] == '\n') {
			do {
				++lineNum;
				input = input.substr(1, input.size() - 1);
			} while (input[0] == '\n');
		}

		else { // Undefined 17
			string tempString(1, input[0]);
			int code = 17;
			int test = input.size();
			tempToken.setToken(tempString, lineNum, code);
			if (test > 0) {
				input = input.substr(1, input.size() - 1);
			}
		}
		return tempToken;
	}

	Token endOfTimes(int& line) {
		Token endToken;
		endToken.endOfFile(line);
		return endToken;
	}
};
#endif