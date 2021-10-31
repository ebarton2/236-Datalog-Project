#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include "Relation.h"
//#include "Parser.h"
#include "Graph.h"


using namespace std;
class Database {
private:
	map<string, Relation> theDatabase; 
	vector<string> tempName;
public:
	Database() {};
	~Database() {};

	void setData(DatalogProgram& myDatalog) {
		for (unsigned int i = 0; i < myDatalog.getSchemeCount(); ++i) 
		{
			tempName.push_back(myDatalog.getSchemes().at(i).getName());
		}

		for (unsigned int i = 0; i < tempName.size(); ++i) 
		{
			Relation tempRelation;
			tempRelation.setName(tempName.at(i));
			tempRelation.setHeader(myDatalog.getSchemes().at(i).fullList());
			tempRelation.printHeaders();
			theDatabase.insert(std::make_pair(tempName.at(i), tempRelation)); // Insert relation
		}

		for (unsigned int i = 0; i < tempName.size(); ++i) 
		{
			for (unsigned int j = 0; j < myDatalog.getFactCount(); ++j) 
			{
				if (myDatalog.getFacts().at(j).getName() == tempName.at(i)) 
				{
					// This sets a tuple with a vector<string> that is a list of facts for a given name
					theDatabase[tempName.at(i)].setTuple(myDatalog.getFacts().at(j).fullList());
				}
			}
		}
	}

	string toString() {
		ostringstream os;
		for (unsigned int i = 0; i < tempName.size(); ++i)
		{
			os << tempName.at(i) << "(";
			os << theDatabase[tempName.at(i)].printHeaders();
			os << ")" << endl;
			os << theDatabase[tempName.at(i)].printTuples();
			os << endl;
		}
		return os.str();
	}

	map<string, Relation> getDatabase() { return theDatabase; }

	void setDatabaseAt(const string& nameKey, const Relation& kush) 
	{ 
		theDatabase[nameKey] = kush; 
	}

};
#endif