#include "GroupScanner.h"

//int GroupScanner:: maxDModality = 0;
//int GroupScanner::maxKModality = 0;
int GroupScanner:: maxModality = 0;

bool GroupScanner::existsLoneDOperator = false;

unordered_map<int, string>GroupScanner::modalityMap;
unordered_map<string, int>GroupScanner:: distMap;

set<int> GroupScanner::agentsSet;

GroupScanner::GroupScanner() =default;
GroupScanner::~GroupScanner()= default;

char GroupScanner::getChar(const int index, const string &formula) {
	if (index < formula.size()) {
		return formula.at(index);
	}
	return '%';
}

string GroupScanner::getDistributedString(int modality) {
	return modalityMap[modality];
}

/*int GroupScanner::getMaxDModality(){
	return maxDModality;
}

int GroupScanner::getMaxKModality(){
	return maxKModality;
}*/

unordered_map<int, string> GroupScanner::getModalityMap() {
	return modalityMap;
}

unordered_map<string, int> GroupScanner::getDistMap() {
	return distMap;
}

string GroupScanner::getTotalGroupString() {
	vector<int> agentsVector;
	agentsVector.assign(agentsSet.begin(), agentsSet.end());

	sort(agentsVector.begin(), agentsVector.end());
	string everyoneModality;
	for(auto agent : agentsVector) {
		everyoneModality += to_string(agent) + ",";
	}
	everyoneModality = everyoneModality.substr(0,everyoneModality.size()-1);
	return everyoneModality;
}

bool GroupScanner::isStringAnInt(const string &formula) {
	//to handle D[]p
	if(formula.empty()) {
		return false;
	}
	for(const char idx : formula) {
		if(!isdigit(idx)) {
			return false;
		}
	}
	return true;
}

void GroupScanner::scanForD(const string& formula){
	int index = 0;
	++maxModality;
	//maxModality = maxKModality + 1;
	while(index < formula.size()) {
		if (getChar(index, formula) == 'D') {
			index+=2;
			string modality;

			while (isdigit(getChar(index, formula)) ||
				isspace(getChar(index, formula))  ||
				getChar(index, formula) == '-' ||
				getChar(index, formula) == ',') {
				if(!isspace(getChar(index, formula))) {
					modality += getChar(index, formula);
					++index;
				}
			}

			if (getChar(index, formula) != ']') {
				if (getChar(index, formula) == '%') {
					throw runtime_error("Unexpected character at position " +
							    to_string(index) +
							    " got end of file but expected ]");
				}
				throw runtime_error("Unexpected character at position " +
						    to_string(index) + " got " + getChar(index, formula) +
						    " but expected ]");
			}

			if(modality.empty()) {
				existsLoneDOperator = true;
				++index;
				continue;
			}
			modality = normaliseGroupModalities(modality);

			if(distMap.find(modality) == distMap.end()) {
				//modalityMap[maxDModality] = modality;
				//distMap[modality] = maxDModality;
				//++maxDModality;
				modalityMap[maxModality] = modality;
				distMap[modality] = maxModality;
				++maxModality;
			}
			++index;
		}
		else {
			++index;
		}
	}

	if(existsLoneDOperator) {
		string everyOneModality = getTotalGroupString();
		if(distMap.find(everyOneModality) != distMap.end()) {
			//modalityMap[maxDModality] = everyOneModality;
			modalityMap[maxModality] = everyOneModality;
		}
	}
}

void GroupScanner::scanForK(const string& formula){
	int index = 0;
	while(index < formula.size()) {
		if (getChar(index, formula) == 'K' ||getChar(index, formula) == 'D') {
			index+=2;
			string modality;
			int commaCounter = 0;
			while (isdigit(getChar(index, formula)) || getChar(index, formula) == ',') {

				modality += getChar(index, formula);
				++index;
				commaCounter++;
			}

			if (getChar(index, formula) != ']') {
				if (getChar(index, formula) == '%') {
					throw runtime_error("Unexpected character at position " +
							    to_string(index) +
							    " got end of file but expected ]");
				}
				throw runtime_error("Unexpected character at position " +
						    to_string(index) + " got " + getChar(index, formula) +
						    " but expected ]");
			}
			//if the D operator has a group of size >1, don't add it to K modalities
			if(!isStringAnInt(modality)) {
				vector<int> tempListOfModalities = rearrangeAgents(modality);
				//maxDModality = tempListOfModalities[tempListOfModalities.size()-1];
				maxModality = tempListOfModalities[tempListOfModalities.size()-1];
				continue;
			}
			++index;

			const int intModality = stoi(modality);
			modalityMap[intModality] = "";
			/*if(intModality > maxKModality) {
				maxKModality = intModality;
			}*/
			if(intModality > maxModality) {
				maxModality = intModality;
			}
			agentsSet.insert(intModality);
		}
		else {
			++index;
		}
	}
}

/*void GroupScanner::setMaxDModality(const int newMaxDModality){
	maxDModality = newMaxDModality;
}

void GroupScanner::setMaxKModality(const int newMaxKModality){
	maxKModality = newMaxKModality;
}*/

string GroupScanner::normaliseGroupModalities(const string &modalityString) {
	vector<int> newAgents = rearrangeAgents(modalityString);
	string finalAgentString;

	for(const auto& newAgent : newAgents) {
		finalAgentString += to_string(newAgent) + ',';
	}

	finalAgentString = finalAgentString.substr(0, finalAgentString.size()-1);

	return finalAgentString;
}

vector<int> GroupScanner::rearrangeAgents(const string &modalityString) {
	vector<int> newAgents;
	int stringStart = 0, stringEnd = 0, iter = 0, agent;
	while (iter <= modalityString.size()) {
		if(modalityString[iter] == ',' || iter == modalityString.size()) {
			stringEnd = iter;
			agent = stoi(modalityString.substr(stringStart,stringEnd-stringStart));
			newAgents.push_back(agent);
			agentsSet.insert(agent);
			stringStart = stringEnd+1;
		}
		++iter;
	}
	sort(newAgents.begin(), newAgents.end());
	return newAgents;
}