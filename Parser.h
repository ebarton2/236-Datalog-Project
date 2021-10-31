#pragma once
//#include "Token.h"
#include "Lexer.h"
#include "DatalogProgram.h"

#include <iostream>
#include <string>

class Parser {
private:
	DatalogProgram myDatalog;
	Lexer myLexer;
	vector<Token> myTokens;
public:
	Parser() {};
	Parser(Lexer theSauce) { myLexer = theSauce; myTokens = theSauce.returnTokenList(); };
	~Parser(void) {};

	bool parse() {
		unsigned int indexTracker = 0;
		bool passState = true;
		try
		{
			if (myTokens.size() >= indexTracker + 2 // Begin checks for Schemes
				&& (myLexer.getCodeFromToken(indexTracker) == 9
					&& myLexer.getCodeFromToken(indexTracker + 1) == 5))
			{
				indexTracker = indexTracker + 2;
				if (myTokens.at(indexTracker).getTokenCode() == 10) {
					throw (myTokens.at(indexTracker));
				}
				if (myDatalog.parseSchemes(indexTracker, myTokens, passState) != true) {
					throw (myTokens.at(indexTracker));
				}
			}
			else
			{
				throw (myTokens.at(indexTracker));
			} // End checks for Schemes
			// -----------------------------------------------------------
			if (myTokens.size() >= indexTracker + 2 // Checks for Facts
				&& (myLexer.getCodeFromToken(indexTracker) == 10
					&& myLexer.getCodeFromToken(indexTracker + 1) == 5))
			{
				indexTracker = indexTracker + 2;
				if (!(indexTracker >= myTokens.size()) // Checks for Rules
					&& (myTokens.at(indexTracker).getTokenCode() != 11))
				{
					if (myDatalog.parseFacts(indexTracker, myTokens, passState) != true) {
						throw (myTokens.at(indexTracker));
					}
				}
			}
			else
			{
				throw (myTokens.at(indexTracker));
			}
			//-------------------------------------------------------------
			if (myTokens.size() >= indexTracker + 2 // Checks for Rules
				&& (myLexer.getCodeFromToken(indexTracker) == 11
					&& myLexer.getCodeFromToken(indexTracker + 1) == 5))
			{
				indexTracker = indexTracker + 2;
				if (!(indexTracker >= myTokens.size()) // Checks for Queries
					&& (myTokens.at(indexTracker).getTokenCode() != 12))
				{
					if (myDatalog.parseRules(indexTracker, myTokens, passState) != true) {
						throw (myTokens.at(indexTracker));
					}
				}
			}
			else
			{
				throw (myTokens.at(indexTracker));
			}
			//------------------------------------------------------------
			if (myTokens.size() >= indexTracker + 2 // Checks for Queries
				&& (myLexer.getCodeFromToken(indexTracker) == 12
					&& myLexer.getCodeFromToken(indexTracker + 1) == 5))
			{
				indexTracker = indexTracker + 2;
				if (indexTracker >= myTokens.size() || myTokens.at(indexTracker).getTokenCode() == 16) {
					passState = false;
					throw (myTokens.at(indexTracker));
				}
				if (!(indexTracker >= myTokens.size())) // Checks for Queries FIXME
				//&& (myTokens.at(indexTracker).getTokenCode() == 16))
				{
					if (myDatalog.parseQueries(indexTracker, myTokens, passState) != true) {
						throw (myTokens.at(indexTracker));
					}
				}
			}
			else
			{
				throw (myTokens.at(indexTracker));
			}
		}
		catch (Token error)
		{
			passState = false;
			cout << "Failure!" << endl;
			cout << "  " << error.toString() << endl;
		}

		return passState;
	};

	void print() {
		myDatalog.print();
	}

	DatalogProgram getDatalog() {
		return myDatalog;
	}
};