#ifndef GROUPSCANNER_H
#define GROUPSCANNER_H

#include <unordered_map>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <algorithm>

#include <set>
#include <utility>
#include <vector>

using namespace std;

class GroupScanner {
	static unordered_map<int, string> modalityMap;
	static unordered_map<string, int> distMap;

	//static int maxKModality;
	//static int maxDModality;
	static int maxModality;

	static bool existsLoneDOperator;

	static set<int> agentsSet;

	static char getChar(int index, const string &formula);
	static bool isStringAnInt(const string &formula);
	static string normaliseGroupModalities(const string& modalityString);
	static vector<int> rearrangeAgents(const string &modalityString);

public:
	GroupScanner();
	~GroupScanner();

	//static int getMaxKModality();
	//static int getMaxDModality();
	static unordered_map<int, string> getModalityMap();
	static unordered_map<string, int> getDistMap();
	static string getTotalGroupString();
	//static bool getExistsLoneDOperator();

	static string getDistributedString(int modality);

	static void scanForD(const string& formula);
	static void scanForK(const string& formula);
	//static void setMaxKModality(int newMaxKModality);
	//static void setMaxDModality(int newMaxDModality);


	//static bool setExistsLoneDOperator(bool newExistsLoneDOperator);
};

#endif //GROUPSCANNER_H
