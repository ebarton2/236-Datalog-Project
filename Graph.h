#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stack>

#include "Parser.h"

using namespace std;

class SCC
{
private:
	set<int> components;
	bool selfDependent;
public:
	SCC() { selfDependent = false; }
	~SCC() {}
	void addComponent(const int& x) { components.insert(x); }
	void setComponents(const set<int>& mySCC) { components = mySCC; }
	void setBool(const bool& depend) { selfDependent = depend; }

	void combine(set<int>& anSCC)
	{
		for (set<int>::const_iterator iter = anSCC.begin(); iter != anSCC.end();)
		{
			addComponent(*iter);
			++iter;
		}
	}

	set<int> getComp() { return components; }
	bool getDependence() { return selfDependent; }
	unsigned int size() { return components.size(); }

	string toString()
	{
		ostringstream os;
		set<int>::const_iterator iter = components.begin();
		os << "R" << *iter;
		++iter;
		while (iter != components.end())
		{
			os << ",R" << *iter;
			++iter;
		}
		os << endl;
		return os.str();
	}
};

class Graph // Look at this Gra-aph
{
private:
	map<int, set<int>> adjacencyList;

public:
	Graph() {}
	~Graph() {}

	void initGraph(const int& i)
	{
		set<int> numbers;
		adjacencyList.insert(std::make_pair(i, numbers));
	}

	void addEdge(const int& x, const int& y)
	{
		adjacencyList[x].insert(y);
	}

	string toString()
	{
		ostringstream os;
		for (map<int, set<int>>::const_iterator it1 = adjacencyList.begin(); it1 != adjacencyList.end();)
		{
			os << "R" << it1->first << ":";
			for (set<int>::const_iterator it2 = it1->second.begin(); it2 != it1->second.end();)
			{
				if (it2 == it1->second.begin()) os << "R" << *it2;
				else { os << "," << "R" << *it2 ; }
				++it2;
			}
			os << endl;
			++it1;
		}
		return os.str();
	}


	void DFSPost(unsigned int& x, vector<int>& visited, stack<int>& order) {
		set<int>::const_iterator it = adjacencyList[x].begin();
		int y = (int)x; // Keep an eye on this
		visited.push_back(y);

		while (it != adjacencyList[x].end()) 
		{
			bool notIn = true;

			for (unsigned int i = 0; i < visited.size(); i++) 
			{
				if (visited.at(i) == *it) 
				{
					notIn = false;
				}
			}

			if (notIn) 
			{
				unsigned int yolo = *it;
				DFSPost(yolo, visited, order);
			}
			++it;
		}
		int yo = (int)x;
		//cout << "Stack: " << yo << endl;
		order.push(yo);
	}

	unsigned int size() { return adjacencyList.size(); }

	SCC DFSForest(unsigned int& x, stack<int>& order, vector<int>& visited) {
		set<int>::const_iterator it = adjacencyList[x].begin();
		int y = (int)x; // Keep an eye on this
		visited.push_back(y);
		SCC myCCS;

		while (it != adjacencyList[x].end())
		{
			bool notIn = true;

			for (unsigned int i = 0; i < visited.size(); i++)
			{
				if (visited.at(i) == *it)
				{
					notIn = false;
				}
				if (x == (unsigned int)*it) { myCCS.setBool(true); }
			}

			if (notIn)
			{
				unsigned int yolo = *it;
				SCC anotherOne = DFSForest(yolo, order, visited);
				set<int> setter = anotherOne.getComp();
				myCCS.combine(setter);
			}
			++it;
		}
		int yo = (int)x;
		//cout << "Stack: " << yo << endl;
		myCCS.addComponent(yo);
		return myCCS;
	}
};