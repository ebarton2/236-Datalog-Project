#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
class Token
{
private:
	string value;
	int line;
	string token;
	int tokenCode;
public:
	Token(void) { line = 1; tokenCode = 0; };
	~Token(void) {};

	void setToken(std::string& input, int& line, int code) {
		value = input; this->line = line; tokenCode = code;
		if (code == 0) 
		{
			token = "COMMA";
		}
		else if (code == 1) 
		{
			token = "PERIOD";
		}
		else if (code == 2) 
		{
			token = "Q_MARK";
		}
		else if (code == 3) 
		{
			token = "LEFT_PAREN"; // (
		}
		else if (code == 4) 
		{
			token = "RIGHT_PAREN"; // )
		}
		else if (code == 5) 
		{
			token = "COLON";
		}
		else if (code == 6) 
		{
			token = "COLON_DASH";
		}
		else if (code == 7) 
		{
			token = "MULTIPLY";
		}
		else if (code == 8) 
		{
			token = "ADD";
		}
		else if (code == 9) 
		{
			token = "SCHEMES";
		}
		else if (code == 10) 
		{
			token = "FACTS";
		}
		else if (code == 11) 
		{
			token = "RULES";
		}
		else if (code == 12) 
		{
			token = "QUERIES";
		}
		else if (code == 13) 
		{ // ID token
			token = "ID";
		}
		else if (code == 14) 
		{ // STRING token
			token = "STRING";
		}
		else if (code == 15) 
		{
			token = "COMMENT";
		}
		else if (code == 16) 
		{ // End of File
			token = "EOF";
		}
		else if (code == 17) 
		{
			token = "UNDEFINED";
		}
	};

	string getTokenType() 
	{
		return token;
	}

	int getTokenCode() 
	{
		return tokenCode;
	}

	string getTokenValue() 
	{
		return value;
	}

	void endOfFile(int& lineNum)
	{
		value = "";
		line = lineNum;
		token = "EOF";
		tokenCode = 16;
	}

	bool isComment() 
	{
		if (token == "COMMENT") 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	std::string toString() 
	{
		return "(" + token + ",\"" + value + "\"," + to_string(line) + ")";
	};
};

enum TokenTypes { COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, ENDOF };

#endif
