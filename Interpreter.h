#pragma once
#include "Database.h"
//#include "Parser.h"
//#include "Graph.h"

using namespace std;

class Interpreter
{
private:
	Database theData;
	DatalogProgram theLog;

	Graph forwardGraph;
	Graph reverseGraph;

	void queryEval(int i) {
		string queryName = theLog.getQueries().at(i).getName();
		Relation currRelation = theData.getDatabase()[queryName];
		vector<int> varPos;
		// for i will deal with each query, the following for j will deal with each
		// item in the given query
		for (unsigned int j = 0; j < theLog.getQueries().at(i).paraSize(); ++j) 
		{
			if (theLog.getQueries().at(i).paraList().at(j).getType() == 1)
			{
				currRelation = currRelation.selectString(j, theLog.getQueries().at(i).paraList().at(j).toString());
				//currRelation.printRelation();
			}
			else if (theLog.getQueries().at(i).paraList().at(j).getType() == 0)
			{ // This is if it is a variable (ID = variable)
				if (!currRelation.isThere(theLog.getQueries().at(i).paraList().at(j).toString()))
				{
					// FIXME: the following assignment is probably way too redundant
					//variables.push_back(theLog.getQueries().at(i).paraList().at(j).toString());
			     varPos.push_back(j);
					currRelation = currRelation.rename(j, theLog.getQueries().at(i).paraList().at(j).toString(), currRelation);
				}
				else
				{
					currRelation = currRelation.rename(j, theLog.getQueries().at(i).paraList().at(j).toString(), currRelation);
					currRelation = currRelation.selectID(j);
					//currRelation.printRelation();
				}
			}
		}

		if (currRelation.empty()) { cout << theLog.getQueries().at(i).toStringPara() << "? No" << endl; }
		else
		{
			cout << theLog.getQueries().at(i).toStringPara() << "? Yes(" << currRelation.tupleSize() << ")" << endl;
			if (!varPos.empty())
			{
				//currRelation.printRelation();
				Relation something = currRelation.project(varPos);
				cout << something.printProjects();
			}
		}
	}

	bool evaluateRule(unsigned int i)
	{
		string ruleName = theLog.getRules().at(i).getPred().getName();
		
		Relation currRelation = theData.getDatabase()[ruleName];
		Relation tempRelation;
		tempRelation.setName(ruleName);
		tempRelation.setHeader(theLog.getRules().at(i).getPred().fullList());

		Relation aRel = EvaluatePredicate(theLog.getRules().at(i).getList().at(0)); // First Rule Predicate

		for (unsigned int j = 1; j < theLog.getRules().at(i).getList().size(); ++j)
		{
			Relation eRel = EvaluatePredicate(theLog.getRules().at(i).getList().at(j));
			aRel = aRel.join(eRel);
		}

		vector<int> varPosDos;
		for (unsigned int x = 0; x < tempRelation.headerSize(); ++x)
		{
			for (unsigned int y = 0; y < aRel.headerSize(); ++y)
			{
				if (tempRelation.getHeader().getHeaderAt(x) == aRel.getHeader().getHeaderAt(y)) 
				{ 
					varPosDos.push_back(y); 
				}
			}
		}
		aRel = aRel.project(varPosDos);

		for (unsigned int i = 0; i < tempRelation.headerSize(); ++i)
		{
			tempRelation = tempRelation.rename(i, currRelation.getHeader().getHeaderAt(i), tempRelation);
		}

		if (!tempRelation.unify(aRel)) 
		{
			//cout << tempRelation.printProjects();
		}

		bool returnVal = currRelation.unifyPrint(tempRelation);

		theData.setDatabaseAt(ruleName, currRelation);

		return returnVal;
	}

	Relation EvaluatePredicate(Predicate& rulePredicate)
	{
		vector<int> varPosition;
		Relation aRel = theData.getDatabase()[rulePredicate.getName()];

		for (unsigned int k = 0; k < rulePredicate.paraSize(); ++k)
		{
			if (rulePredicate.paraList().at(k).getType() == 1)
			{
				aRel = aRel.selectString(k, rulePredicate.paraList().at(k).toString());
			}
			else if (rulePredicate.paraList().at(k).getType() == 0)
			{ // This is if it is a variable (ID = variable)
				if (!aRel.isThere(rulePredicate.paraList().at(k).toString()))
				{
					// FIXME: the following assignment is probably way too redundant
					varPosition.push_back(k);
					aRel = aRel.rename(k, rulePredicate.paraList().at(k).toString(), aRel);
				}
				else
				{
					aRel = aRel.rename(k, rulePredicate.paraList().at(k).toString(), aRel);
					aRel = aRel.selectID(k);
				}
			}
		}
		aRel = aRel.project(varPosition);
		return aRel;
	}


public:
	Interpreter(Parser base) { theLog = base.getDatalog(); };
	~Interpreter() {};
	void stData() { theData.setData(theLog); }
	string toString() { return theData.toString(); }

	void queryEvalMass() {
		for (unsigned int i = 0; i < theLog.getQueryCount(); ++i)
		{
			queryEval(i);
		}
	}

	void evaluateRulesPoint() // Uses fixed-point algorithm to brute-force your way through
	{
		bool heyheyhey = true;
		int passes = 0;
		while (heyheyhey == true) 
		{
			heyheyhey = false;
			for (unsigned int i = 0; i < theLog.getRuleCount(); ++i)
			{
				cout << theLog.getRules().at(i).toString() << endl;
				if (evaluateRule(i) == true) { 
					heyheyhey = true; 
				}
			}
			++passes;
		}
		cout << endl << "Schemes populated after " << passes << " passes through the Rules." << endl << endl;
	}	

	void createGraphs()
	{
		for (unsigned int i = 0; i < theLog.getRuleCount(); ++i)
		{
			forwardGraph.initGraph(i);
			reverseGraph.initGraph(i);
		}

		for (unsigned int i = 0; i < theLog.getRuleCount(); ++i) 
		{
			//forwardGraph.initGraph(i);

			//reverseGraph.initGraph(i)
			for (unsigned int j = 0; j < theLog.getRules().at(i).getList().size(); ++j)
			{
				for (unsigned int k = 0; k < theLog.getRuleCount(); ++k)
				{
					if (theLog.getRules().at(i).getList().at(j).getName() == theLog.getRules().at(k).getPred().getName())
					{
						forwardGraph.addEdge(i, k);
						reverseGraph.addEdge(k, i);
					}
				}
			}
		}

		cout << forwardGraph.toString(); //Prints off the dependency graph
		cout << endl;
		//cout << reverseGraph.toString();
		//cout << endl;
	}

	void evalGraphs()
	{
		vector<int> visited;
		stack<int> postOrder;
		for (unsigned int i = 0; i < reverseGraph.size(); ++i)
		{
			if (visited.empty()) { reverseGraph.DFSPost(i, visited, postOrder); }
			else {
				bool visit = false;
				for (unsigned int j = 0; j < visited.size(); ++j)
				{
					if (visited.at(j) == (int)i) { visit = true; }
				}
				if (visit) { continue; }
				else { reverseGraph.DFSPost(i, visited, postOrder); }
			}
		}

		visited.clear();
		vector<SCC> allSCC;
		while (!postOrder.empty())
		{
			unsigned int index = postOrder.top();
			postOrder.pop();
			if (visited.empty()) { allSCC.push_back(forwardGraph.DFSForest(index, postOrder, visited)); }
			else {
				bool visit = false;
				for (unsigned int i = 0; i < visited.size(); ++i)
				{
					if (visited.at(i) == (int)index) { visit = true; }
				}
				if (visit) { continue; }
				else { allSCC.push_back(forwardGraph.DFSForest(index, postOrder, visited)); }
			}
		}

		for (unsigned int i = 0; i < allSCC.size(); ++i)
		{
			if (allSCC.at(i).size() > 1) { allSCC.at(i).setBool(true); }
		}

		// Now for the fun part

		for (unsigned int v = 0; v < allSCC.size(); ++v)
		{
			bool heyheyhey = true;
			int passes = 0;
			set<int> setter = allSCC.at(v).getComp();

			cout << "SCC: " << allSCC.at(v).toString();
			while (heyheyhey == true)
			{
				heyheyhey = false;
				
				if (allSCC.at(v).getDependence() == true)
				{
					for (set<int>::const_iterator iter = setter.begin(); iter != setter.end(); ++iter)
					{
						cout << theLog.getRules().at(*iter).toString() << endl;
						if (evaluateRule(*iter) == true) {
							heyheyhey = true;
						}						
					}
					++passes;
					
				}
				else { // Case for a single SCC
					set<int>::const_iterator iter = setter.begin();
					cout << theLog.getRules().at(*iter).toString() << endl;
					evaluateRule(*iter);
					heyheyhey = false;
					++passes;
					//cout << passes << " passes: " << allSCC.at(v).toString();
				}
				
			}
			cout << passes << " passes: " << allSCC.at(v).toString();
		}

		
		//cout << endl << "Schemes populated after " << passes << " passes through the Rules." << endl << endl;
	}
};