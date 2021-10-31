#ifndef RELATION_H
#define RELATION_H
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <set>
#include <list>

#include "Header.h"

// Declaration of class Tuple
class Tuple : public std::vector<string> {};
// End declaration of class Tuple

using namespace std;
class Relation {
private:
	//int _tuples;
	string relationName;
	Header headerList;
	set<Tuple> relationTuples;

public:
	Relation() {}
	~Relation() {}

	bool empty() { return relationTuples.empty(); }

	unsigned int tupleSize() { return relationTuples.size(); }

	void setName(const string& name) 
	{
		relationName = name;
	}

	void setHeader(const vector<string>& header) { headerList.setHeader(header); } // CASE 1

	void setHeader(const Header& header) { headerList = header; } // CASE 2

	void renameHeaderAt(const int& index, const string& newHeader) { headerList.renameHeaderAt(newHeader, index); }

	void setTuple(const vector<string>& tuples) 
	{
		Tuple temp;
		for (unsigned int i = 0; i < tuples.size(); ++i) 
		{
			temp.push_back(tuples.at(i));
		}
		relationTuples.insert(temp);
	}

	void addTuple(const Tuple& nt) { relationTuples.insert(nt); }

	Header getHeader() { return headerList; }

	set<Tuple> getTuples() { return relationTuples; }

	set<Tuple>::iterator tupIterBegin() { set<Tuple>::iterator it = relationTuples.begin(); return it; }

	set<Tuple>::iterator tupIterEnd() { set<Tuple>::iterator it = relationTuples.end(); return it; }

	unsigned int headerSize()
	{
		return headerList.headerSize();
	}

	string printName() 
	{
		return relationName;
	}

    string printHeaders() 
	{
		return headerList.toString();
	}

	string printTuples() {
		ostringstream os;
		set<Tuple>::iterator it = relationTuples.begin();
		while (it != relationTuples.end()) 
		{
			os << "(";
			for(unsigned int i = 0; i < it->size() - 1; ++i) 
			{
				os << it->at(i) << ",";
			}
			os << it->at(it->size() - 1) << ")" << endl;
			it++;
		}	
		return os.str();
	}

	void printRelation()
	{
		cout << printName() << " " << printHeaders() << endl;
		cout << printTuples() << endl;
	}

	string printProjects()
	{
		ostringstream os;
		set<Tuple>::iterator it = relationTuples.begin();
		while (it != relationTuples.end())
		{
			os << "  " << headerList.getHeaderAt(0) << "=" << it->at(0);
			if (headerList.headerSize() == 1) os << endl;
			for (unsigned int i = 1; i < headerList.headerSize(); ++i)
			{
				os << ", " << headerList.getHeaderAt(i) << "=" << it->at(i);
				if (i == headerList.headerSize() - 1) os << endl;
			}
			//os << endl;
			++it;
		}
		return os.str();
	}

	Relation selectString(const int& location, const string& item) 
	{
		Relation temp;
		temp.setName(relationName); // Sets name of relation
		
		vector<string> atempt;
		atempt = headerList.getHeader();
		temp.setHeader(atempt); // Sets header(s) of relation;

		set<Tuple>::iterator it = relationTuples.begin();
		while (it != relationTuples.end()) 
		{
			if (it->at(location) == item) 
			{
				temp.setTuple(*it);
			}
			++it;
		}
		return temp;
	}

	Relation selectID(const int& location)
	{
		Relation temp;

		temp.setName(relationName); // Sets name of relation

		vector<string> atempt;
		atempt = headerList.getHeader();
		temp.setHeader(atempt); // Sets header(s) of relation;

		set<Tuple>::iterator it = relationTuples.begin();
		while (it != relationTuples.end())
		{
			for (int i = 0; i < location; ++i)
			{
				if (headerList.getHeaderAt(i) == headerList.getHeaderAt(location))
				{
					if (it->at(i) == it->at(location))
					{
						temp.setTuple(*it);
					}
				}
			}
			++it;
		}
		return temp;
	}

	Relation project(const vector<int>& vars) // Not needed for Lab 3; changes order of columns in a relation to evaluate rules
	{
		Relation temp;
		temp.setName(relationName); // Sets name of relation

		vector<string> atempt;

		for (unsigned int i = 0; i < vars.size(); ++i)
		{
			atempt.push_back(headerList.getHeaderAt(vars.at(i)));
		}
		temp.setHeader(atempt); // Sets header(s) of relation;

		set<Tuple>::iterator it2 = relationTuples.begin();
		while (it2 != relationTuples.end())
		{
			vector<string> tempVec;
			for (unsigned int i = 0; i < temp.getHeader().headerSize(); ++i)
			{
				for (unsigned int j = 0; j < headerList.headerSize(); ++j)
				{
						tempVec.push_back(it2->at(vars.at(i)));
						break;
				}
			}
			temp.setTuple(tempVec);
			++it2;
		}
		return temp;
	}

	Relation rename(const int& index, const string& newHeader, Relation& tempRel) 
	{  // changes the header of the relation, but has the same tuples
		tempRel.renameHeaderAt(index, newHeader);
		return tempRel;
	}

	bool isThere(const string& temp) { return headerList.isThere(temp); }

	Relation join(Relation& thisRelation)
	{
		vector<pair<unsigned int, unsigned int>> locDataPair;
		for (unsigned int x = 0; x < headerList.headerSize(); ++x)
		{
			for (unsigned int y = 0; y < thisRelation.getHeader().headerSize(); ++y)
			{
				if (headerList.getHeaderAt(x) == thisRelation.getHeader().getHeaderAt(y))
				{
					pair<unsigned int, unsigned int> pear;
					pear.first = x;
					pear.second = y;
					locDataPair.push_back(pear);
				}
			}
		} 

		Header newHeader = headerList.joinH(thisRelation.getHeader(), headerList);
		//cout << endl << newHeader.toString() << endl; // For Debugging

		Relation r;
		r.setName(relationName);
		r.setHeader(newHeader);

		//r.printRelation(); // For Debugging

		set<Tuple>::iterator it1 = relationTuples.begin();

		while (it1 != relationTuples.end())
		{
			set<Tuple>::iterator it2 = thisRelation.tupIterBegin();
			while (it2 != thisRelation.tupIterEnd())
			{
				if (isJoinable(*it1, *it2, locDataPair)) {
					//cout << "YESS" << endl;
					Tuple nt = combineTuples(*it1, *it2, locDataPair);
					r.addTuple(nt);
					//r.printRelation();
				}
				++it2;
			}
			++it1;
		}
		return r; // instead of thisRelation
	}

	bool isJoinable(const Tuple& t1, const Tuple& t2, const vector<pair<unsigned int, unsigned int>>& tracker) // FIXME ??
	{
		bool isJoin = true;
		for (unsigned int i = 0; i < tracker.size(); ++i)
		{
			if (t1.at(tracker.at(i).first) == t2.at(tracker.at(i).second))
			{
				isJoin = true;
			}
			else { return false; }
		}
		return isJoin;
	}

	Tuple combineTuples(const Tuple& t1, const Tuple& t2, const vector<pair<unsigned int, unsigned int>>& tracker)
	{
		Tuple nt = t1;
		bool stuff = true;
		
		for (unsigned int i = 0; i < t2.size(); ++i)
		{
			for (unsigned int j = 0; j < tracker.size(); ++j)
			{
				if (tracker.at(j).second == i)
				{
					stuff = false;
				}
			}
			if (stuff == true) { nt.push_back(t2.at(i)); }
			stuff = true;
		}
		return nt;
	}

	bool unify(Relation& aRel)
	{
		unsigned int size = relationTuples.size();
		set<Tuple>::iterator it = aRel.tupIterBegin();
		while (it != aRel.tupIterEnd())
		{
			Tuple aT = *it;
			relationTuples.insert(aT);
			++it;
		}
		
		if (relationTuples.size() == size) { return false; }
		else { return true; } // Returns true when something changed.
	}

	bool unifyPrint(Relation& aRel)
	{
		unsigned int size = relationTuples.size();
		unsigned int tempSize = relationTuples.size();
		set<Tuple>::iterator it = aRel.tupIterBegin();
		while (it != aRel.tupIterEnd())
		{
			Tuple aT = *it;
			relationTuples.insert(aT);
			if (relationTuples.size() > tempSize)
			{
				tempSize = relationTuples.size();
				cout << "  " << headerList.getHeaderAt(0) << "=" << aT.at(0);
				for (unsigned int i = 1; i < headerList.headerSize(); ++i)
				{
					cout << ", " << headerList.getHeaderAt(i) << "=" << aT.at(i);
				}
				//if (i == headerList.headerSize() - 1) 
				cout << endl;
			}
			++it;
		}

		if (relationTuples.size() == size) { return false; }
		else { return true; } // Returns true when something changed.
	}
};
#endif