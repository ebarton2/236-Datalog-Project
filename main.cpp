#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include "Parser.h"
//#include "Lexer.h"
#include "Interpreter.h"
using namespace std;
int main(int argc, char* argv[])
{
	ifstream in(argv[1]);
	if (!in) 
	{
		cerr << "Unable to open " << argv[1] << " for input";
		return 2;
	}
	int lineNum = 1;
	Lexer myLexer;
	string line;
	for (string tempLine; getline(in, tempLine);) 
	{
		if (in.eof()) 
		{
			line = line + tempLine;
		}
		else 
		{
			line = line + tempLine + '\n';
		}
	}
	myLexer.getTokens(line, lineNum);
	myLexer.endOfFile();

	Parser myDatalogProgram(myLexer);
	myDatalogProgram.parse(); 

	Interpreter myUnderBase(myDatalogProgram); //Transformers Marvel Comics Reference ftw
	myUnderBase.stData();

	cout << "Dependency Graph" << endl;
	myUnderBase.createGraphs();
	//cout << endl;
	//myUnderBase.evalGraphs();

	//cout << endl << endl;


	cout << "Rule Evaluation" << endl;
	myUnderBase.evalGraphs();
	cout << endl;
	cout << "Query Evaluation" << endl;
	myUnderBase.queryEvalMass();
	return 0;
}