#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H


//#include "Predicate.h"
#include "Rule.h"
//#include "Token.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

class DatalogProgram {
private:
	unsigned int schemeCount = 0, factCount = 0, ruleCount = 0, queryCount = 0;
	vector<Predicate> schemePredicate, factPredicate, queryPredicate;
	vector<Rule> _rules;
	set<string> domain;

	void setDomain(vector<Predicate> theColdHardFacts) 
	{
		for (unsigned int i = 0; i < theColdHardFacts.size(); ++i) 
		{
			for (unsigned int j = 0; j < theColdHardFacts.at(i).listSize(); ++j) 
			{
				string temp;
				temp = theColdHardFacts.at(i).getList(j);
				domain.insert(temp);
			}
		}
	}

public:
	DatalogProgram(void) {};
	~DatalogProgram(void) {};

	bool parseSchemes(unsigned int& tracker, vector<Token> tokenList, bool& pass) 
	{
			Predicate temp;
			if (temp.setPredicateScheme(tracker, tokenList, pass)) 
			{
				schemePredicate.push_back(temp);
				++schemeCount;
			}
			else 
			{
				pass = false;
				return pass;
			}
			if (tokenList.at(tracker).getTokenCode() == 13) 
			{
				if (parseSchemes(tracker, tokenList, pass) == false)
				{
					pass = false;
					return pass;
				}
			}
		return pass;
	};

	bool parseFacts(unsigned int& tracker, vector<Token> tokenList, bool& pass) {
		while ((&tokenList.at(tracker) != NULL) //
			&& tokenList.at(tracker).getTokenCode() != 11) // NOT RULES FIXME
		{
			Predicate temp;
			if ((tokenList.at(tracker).getTokenCode() != 16) //FIXME EOF
				&& temp.setPredicateFact(tracker, tokenList, pass))
			{
				factPredicate.push_back(temp);
				++factCount;
			}
			else if (tokenList.at(tracker).getTokenCode() == 16) 
			{
				return pass;
			}
			else 
			{
				pass = false;
				return pass;
			}
		}
		return pass;
	};

	bool parseRules(unsigned int& tracker, vector<Token> tokenList, bool& pass) {
		while ((&tokenList.at(tracker) != NULL) //
			&& tokenList.at(tracker).getTokenCode() != 12) // NOT QUERIES FIXME
		{
			Rule tempRule;
			if ((tokenList.at(tracker).getTokenCode() != 16) //FIXME EOF
				&& tempRule.setRule(tracker, tokenList, pass))
			{
				_rules.push_back(tempRule);
				++ruleCount;
			}
			else if (tokenList.at(tracker).getTokenCode() == 16) 
			{
				return pass;
			}
			else 
			{
				pass = false;
				return pass;
			}
		}
		return pass;
	};

	bool parseQueries(unsigned int& tracker, vector<Token> tokenList, bool& pass) 
	{
		while ((&tokenList.at(tracker) != NULL) //
			&& tokenList.at(tracker).getTokenCode() != 16) // NOT RULES FIXME
		{
			Predicate temp;
			if ((tokenList.at(tracker).getTokenCode() != 16) //FIXME EOF
				&& temp.setPredicateQuery(tracker, tokenList, pass))
			{
				queryPredicate.push_back(temp);
				++queryCount;
			}
			else if (tokenList.at(tracker).getTokenCode() == 16) 
			{
				return pass;
			}
			else 
			{
				pass = false;
				return pass;
			}
		}
		return pass;
	};

	void print() 
	{
		cout << "Schemes(" << schemeCount << "):" << endl;
		for (unsigned int i = 0; i < schemePredicate.size(); ++i) 
		{
			cout << "  " << schemePredicate.at(i).toString() << endl;
		}
		cout << "Facts(" << factCount << "):" << endl;
		for (unsigned int i = 0; i < factPredicate.size(); ++i) 
		{
			cout << "  " << factPredicate.at(i).toString() << "." << endl;
		}
		cout << "Rules(" << ruleCount << "):" << endl;
		for (unsigned int i = 0; i < _rules.size(); ++i) 
		{
			cout << "  " << _rules.at(i).toString() << endl;
		}
		cout << "Queries(" << queryCount << "):" << endl;
		for (unsigned int i = 0; i < queryPredicate.size(); ++i) 
		{
			cout << "  " << queryPredicate.at(i).toStringPara() << "?" << endl;
		}
		setDomain(factPredicate);
		cout << "Domain(" << domain.size() << "):" << endl;
		set<string>::iterator it = domain.begin();
		while (it != domain.end()) 
		{
			cout << "  " << (*it) << endl;
			it++;
		}
	}

	vector<Predicate> getSchemes() 
	{
		return schemePredicate;
	}

	unsigned int getSchemeCount() 
	{
		return schemeCount;
	}

	vector<Predicate> getFacts() 
	{
		return factPredicate;
	}

	unsigned int getFactCount() 
	{
		return factCount;
	}

	vector<Predicate> getQueries() 
	{
		return queryPredicate;
	}

	unsigned int getQueryCount() 
	{
		return queryCount;
	}

	vector<Rule> getRules() 
	{
		return _rules;
	}

	unsigned int getRuleCount() 
	{
		return ruleCount;
	}

};
#endif // !DATALOGPROGAM_H