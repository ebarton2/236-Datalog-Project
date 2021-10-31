#ifndef PARAMETER_H
#define PARAMETER_H

//#include "Rule.h"
//#include "Predicate.h"
#include <iostream>
#include <string>

class Parameter {
private:
	string _para = "";
	int _type = 0; // 0 = ID, 1 = STRING, 2 = expression

	bool setExpression(unsigned int& tracker, vector<Token> tokenList, bool& pass) {
		if ((tokenList.at(tracker).getTokenCode() == 3) // LEFT_PAREN
			|| (tokenList.at(tracker).getTokenCode() == 4) // RIGHT_PAREN
			|| (tokenList.at(tracker).getTokenCode() == 13) // ID
			|| (tokenList.at(tracker).getTokenCode() == 14)) // STRING
		{
			// Note: This is OK
		}
		else {
			pass = false;
			return pass;
		}

		// FIRST CHECK -> ID | STRING | expression(LEFT_PAREN)
		if ((tokenList.at(tracker).getTokenCode() == 13)
			|| (tokenList.at(tracker).getTokenCode() == 14)) // Checks if current item is a string or ID
		{
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;
		}
		else if (tokenList.at(tracker).getTokenCode() == 3) {
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;
			if (!(setExpression(tracker, tokenList, pass))) {
				pass = false;
				return pass;
			}
		}

		// SECOND CHECK -> operators
		if ((tokenList.at(tracker).getTokenCode() == 7) // MULTIPLY
			|| (tokenList.at(tracker).getTokenCode() == 8)) // ADD
		{
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;
		}
		else {
			pass = false;
			return pass;
		}

		// THIRD CHECK -> ID STRING expression(LEFT_PAREN)
		if ((tokenList.at(tracker).getTokenCode() == 13)
			|| (tokenList.at(tracker).getTokenCode() == 14)) // Checks if current item is a string or ID
		{
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;
		}
		else if (tokenList.at(tracker).getTokenCode() == 3) {
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;
			if (!(setExpression(tracker, tokenList, pass))) {
				pass = false;
				return pass;
			}
		}

		// FINAL CHECK -> RIGHT_PAREN
		if (tokenList.at(tracker).getTokenCode() == 4) {
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;
			pass = true;
			return pass;
		}
		else {
			pass = false;
			return pass;
		}

		return pass;
	}

public:
	Parameter() {}
	~Parameter() {}

	bool setParameter(unsigned int& tracker, vector<Token> tokenList, bool& pass) {
		if (&tokenList.at(tracker) == NULL) {
			pass = false;
			return pass;
		}

		if (tokenList.at(tracker).getTokenCode() == 13) { // ID
			_type = 0;
			_para = tokenList.at(tracker).getTokenValue();
			++tracker;
		}
		else if (tokenList.at(tracker).getTokenCode() == 14) { // STRING
			_type = 1;
			_para = tokenList.at(tracker).getTokenValue();
			++tracker;
		}
		else if (tokenList.at(tracker).getTokenCode() == 3) { // LEFT_PAREN
			_type = 2;
			_para += tokenList.at(tracker).getTokenValue();
			++tracker;

			if (!setExpression(tracker, tokenList, pass)) { pass = false; return pass; }
		}
		return pass;
	}


	string toString() {
		return _para;
	}

	int getType() {
		return _type;
	}
};

#endif