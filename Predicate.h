#ifndef PREDICATE_H
#define PREDICATE_H

#include <iostream>
#include <string>
#include <vector>
#include "Parameter.h"
//#include "Token.h"

using namespace std;

class Predicate 
{
private:
	string _name;
	vector<string> _list; // For Schemes and Facts
	vector<Parameter> parameterList;   // For Rules and Queries

	void setIDList(unsigned int& tracker, vector<Token> tokenList, bool& pass) 
	{
		for (unsigned int i = tracker; i < tokenList.size(); ++i) 
		{
			if (tracker >= tokenList.size()) 
			{
				pass = false;
				break;
			}

			if (tokenList.at(tracker).getTokenCode() == 0) 
			{ // COMMA
				++tracker;
				if ((&tokenList.at(tracker) == NULL) || (tokenList.at(tracker).getTokenCode() != 13)) 
				{
					pass = false;
					break;
				}
				//setIDList(tracker, tokenList, pass);
			}
			else if (tokenList.at(tracker).getTokenCode() == 13) 
			{ // ID
				_list.push_back(tokenList.at(tracker).getTokenValue());
				++tracker;
				//setIDList(tracker, tokenList, pass);
			}
			else 
			{
				break;
			}
		}
	}

	void setStringList(unsigned int& tracker, vector<Token> tokenList, bool& pass) 
	{
		for (unsigned int i = tracker; i < tokenList.size(); ++i) 
		{
			if (tracker >= tokenList.size()) 
			{
				pass = false;
				break;
			}

			if (tokenList.at(tracker).getTokenCode() == 0) 
			{ // COMMA
				++tracker;
				if ((&tokenList.at(tracker) == NULL) || (tokenList.at(tracker).getTokenCode() != 14)) 
				{
					pass = false;
					break;
				}
			}
			else if (tokenList.at(tracker).getTokenCode() == 14) 
			{ // STRING
				_list.push_back(tokenList.at(tracker).getTokenValue());
				++tracker;
			}
			else 
			{
				break;
			}
		}
	}

	void setPredicateList(unsigned int& tracker, vector<Token> tokenList, bool& pass) 
	{
		for (unsigned int i = tracker; i < tokenList.size(); ++i) 
		{
			if (tracker >= tokenList.size()) 
			{
				pass = false;
				break;
			}

			if (tokenList.at(tracker).getTokenCode() == 0) 
			{ // COMMA
				++tracker;
				if ((&tokenList.at(tracker) != NULL)
					|| ((tokenList.at(tracker).getTokenCode() == 13) // ID
					|| (tokenList.at(tracker).getTokenCode() == 14) // STRING
					|| (tokenList.at(tracker).getTokenCode() == 3))) // LEFT_PAREN
				{
					continue;
				}
				else 
				{
					pass = false;
					break;
				}
			}

			else if ((tokenList.at(tracker).getTokenCode() == 13) // ID
				|| (tokenList.at(tracker).getTokenCode() == 14) // STRING
				|| (tokenList.at(tracker).getTokenCode() == 3)) // LEFT_PAREN
			{
			
				Parameter tempPara;
				tempPara.setParameter(tracker, tokenList, pass);
				parameterList.push_back(tempPara);
				if (pass == false) {
					break;
				}
				//++tracker;
			}

			else {
				break;
			}
		}
	}

public:
	Predicate(void) {};
	~Predicate(void) {};

	//Also sets headPredicate because it's the same thing
	bool setPredicateScheme(unsigned int& tracker, vector<Token> tokenList, bool& pass) {
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 13) { // Checks for ID
			_name = tokenList.at(tracker).getTokenValue();
			++tracker;
		}
		else {
			pass = false;
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 3) { // Checks for LEFT_PAREN
			++tracker;
		}
		else {
			pass = false;
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 13) { // Checks for ID
			_list.push_back(tokenList.at(tracker).getTokenValue());
			++tracker;
		}
		else {
			pass = false;
			return pass;
		}
		setIDList(tracker, tokenList, pass); // SET THE ID LIST
		if (pass == false) {
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 4) { // checks for RIGHT_PAREN
			++tracker;
		}
		else {
			pass = false;
			return pass;
		}
		return pass;
	}
	// --------------------------------------
	bool setPredicateFact(unsigned int& tracker, vector<Token> tokenList, bool& pass) {
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 13) { // Checks for ID
			_name = tokenList.at(tracker).getTokenValue();
			++tracker;
			//continue;
		}
		else 
		{
			pass = false;
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 3) 
		{ // Checks for LEFT_PAREN
			++tracker;
			//continue;
		}
		else 
		{
			pass = false;
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 14) 
		{ // Checks for STRING
			_list.push_back(tokenList.at(tracker).getTokenValue());
			++tracker;
			//continue;
		}
		else {
			pass = false;
			return pass;
		}
		setStringList(tracker, tokenList, pass); // SET THE STRING LIST
		if (pass == false) {
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 4) { // checks for RIGHT_PAREN
			++tracker;
			//continue;
		}
		else {
			pass = false;
			return pass;
		}
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 1) { // checks for PERIOD
			++tracker;
			//continue;
		}
		else {
			pass = false;
			return pass;
		}
		return pass;
	}
	// --------------------------------------
	bool setPredicateQuery(unsigned int& tracker, vector<Token> tokenList, bool& pass) 
	{
		if (!(setPredicate(tracker, tokenList, pass))) 
		{
			pass = false;
			return pass;
		}
		if (tokenList.at(tracker).getTokenCode() == 2) 
		{
			++tracker;
			return pass;
		}
		else 
		{
			pass = false;
			return pass;
		}
		return pass;
	}

	bool setPredicate(unsigned int& tracker, vector<Token> tokenList, bool& pass) { // Calls with Rules and Queries
		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 13) {
			_name = tokenList.at(tracker).getTokenValue();
			++tracker; // Checks for ID
		}
		else {
			pass = false;
			return pass;
		}

		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 3) {
			++tracker;// Checks for LEFT_PAREN
		}
		else {
			pass = false;
			return pass;
		}

		if ((&tokenList.at(tracker) != NULL)) {	// && tokenList.at(tracker).getTokenCode() == 13) {
			if ((tokenList.at(tracker).getTokenCode() == 13) // ID
				|| (tokenList.at(tracker).getTokenCode() == 14)) // STRING
			{
				Parameter tempPara;
				if (!(tempPara.setParameter(tracker, tokenList, pass))) { return pass; }
				parameterList.push_back(tempPara);
			}
			else if (tokenList.at(tracker).getTokenCode() == 3) {  // LEFT_PAREN
				Parameter tempPara;
				if (!(tempPara.setParameter(tracker, tokenList, pass))) { return pass; }
				parameterList.push_back(tempPara);
			}
			else {
				pass = false;
				return pass;
			}

			//++tracker;  // Don't know if I need this still
		}
		else {
			pass = false;
			return pass;
		}

		setPredicateList(tracker, tokenList, pass); // SET THE PREDICATE LIST

		if (pass == false) {
			return pass;
		}

		if ((&tokenList.at(tracker) != NULL) && tokenList.at(tracker).getTokenCode() == 4) {
			++tracker;   // checks for RIGHT_PAREN
		}
		else {
			pass = false;
			return pass;
		}

		return pass;
	}

	string getName() { return _name; }

	string getList(unsigned int i) { return _list.at(i); }

	vector<string> fullList() {	return _list; }

	unsigned int listSize() { return _list.size(); }

	vector<Parameter> paraList() { return parameterList;  }

	unsigned int paraSize() { return parameterList.size(); }

	string toString() {
		string temp;
		temp += _name;
		temp += '(';
		for (unsigned int j = 0; j < _list.size() - 1; ++j) {
			temp += _list.at(j);
			temp += ',';
		}
		temp += _list.at(_list.size() - 1);
		temp += ')';
		return temp;
	}

	string toStringPara() {
		string temp = "";
		temp += _name;
		temp += '(';
		for (unsigned int j = 0; j < parameterList.size() - 1; ++j) {
			temp += parameterList.at(j).toString();
			temp += ',';
		}
		temp += parameterList.at(parameterList.size() - 1).toString();
		temp += ')';
		return temp;
	}
};
#endif