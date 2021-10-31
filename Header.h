#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;
class Header {
private:
	vector<string> attributeNames;

	void add(string head) 
	{
		attributeNames.push_back(head);
	}

public:
	Header() {}
	~Header() {}

	void setHeader(vector<string> list) 
	{
		attributeNames = list;
	}

	void renameHeaderAt(const string& newHeader, const int& index)
	{
		attributeNames.at(index) = newHeader;
	}

	string getHeaderAt(const int& index) { return attributeNames.at(index); }

	vector<string> getHeader() { return attributeNames; }

	unsigned int headerSize()
	{
		return attributeNames.size();
	}

	string toString() 
	{
		string temp = "(";
		for (unsigned int i = 0; i < attributeNames.size() - 1; ++i) 
		{
			temp += attributeNames.at(i) + ", ";
		}
		temp += attributeNames.at(attributeNames.size() - 1) + ")";
		return temp;
	}

	bool isThere(const string& temp)
	{
		for (unsigned int i = 0; i < attributeNames.size(); ++i)
		{
			if (attributeNames.at(i) == temp) return true;
		}
		return false;
	}

	Header joinH(Header thisHeader, Header& oldHeader) // FIXME: Try to implement pass by reference
	{
		for (unsigned int i = 0; i < thisHeader.headerSize(); ++i)
		{
			if (!isThere(thisHeader.getHeaderAt(i)))
			{
				oldHeader.add(thisHeader.getHeaderAt(i));
			}
		}
		//cout << oldHeader.toString(); // Testing code
		return oldHeader;
	}

};
#endif
