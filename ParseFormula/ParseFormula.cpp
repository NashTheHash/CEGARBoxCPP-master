#include "ParseFormula.h"
#include <string.h>
#include <string>
#include <sstream>

ParseFormula::ParseFormula(string *str) {
  ifstream formulaFile(*str);

  getline(formulaFile, s);
  file = &s;
  file->erase(
    remove_if(file->begin(), file->end(), ::isspace),
    file->end());
  GroupScanner::scanForK(*file);
  GroupScanner::scanForD(*file);

  formulaFile.close();
}

ParseFormula::ParseFormula(string str) {
  file = &str;
  file->erase(
    remove_if(file->begin(), file->end(), ::isspace),
    file->end());
  GroupScanner::scanForK(*file);
  GroupScanner::scanForD(*file);
}

ParseFormula::~ParseFormula() { // Deletion happens in main
  file = nullptr;
}

char ParseFormula::getChar() {
  if (index < file->size()) {
    return file->at(index);
  }
  return '%';
}

vector<int> ParseFormula::split(string &s, char delimiter) {     
    vector<int> tokens;     
    string token;     
    istringstream tokenStream(s);     
    while (getline(tokenStream, token, delimiter)) {      
        tokens.push_back(stoi(token));     
    }     
    return tokens;  
}

shared_ptr<Formula> ParseFormula::parseRest() {
  while (isspace(getChar()))
    ++index;

  if (getChar() == '(') {
    ++index;
    shared_ptr<Formula> inside = parseIff();

    while (isspace(getChar()))
      ++index;

    if (getChar() != ')') {
      if (getChar() == '%') {
        throw runtime_error("Unexpected character at position " +
                            to_string(index) +
                            " got end of file but expected )");
      }
      throw runtime_error("Unexpected character at position " +
                          to_string(index) + " got " + getChar() +
                          " but expected )");
    }
    ++index;

    return inside;
  } 
  else if (getChar() == 'K') {
    index+=2;
    string modality;
    getChar();
    if (getChar() == '-') {
      modality = "-";
      ++index;
    } else {
      string modality = "";
    }

    while (isdigit(getChar())) {
      modality += getChar();
      ++index;
    }

    if (getChar() != ']') {
      if (getChar() == '%') {
        throw runtime_error("Unexpected character at position " +
                            to_string(index) +
                            " got end of file but expected ]");
      }
      throw runtime_error("Unexpected character at position " +
                          to_string(index) + " got " + getChar() +
                          " but expected ]");
    }
    ++index;

    shared_ptr<Formula> rest = parseRest();

    if (modality == "") {
      if (rest->getType() == FKnow) {
        Know *restKnow = dynamic_cast<Know *>(rest.get());
        if (restKnow->getModality() == 1) {
          restKnow->incrementPower();
          return rest;
        }
      }
      return Know::create(1, 1, rest);
    } else {
      if (rest->getType() == FKnow) {
        Know *restKnow = dynamic_cast<Know *>(rest.get());
        if (restKnow->getModality() == stoi(modality)) {
          restKnow->incrementPower();
          return rest;
        }
      }
      return Know::create(stoi(modality), 1, rest);
    }
  } 

  else if (getChar() == 'D') {
    index+=2;
    string modality;
    getChar();
    if (getChar() == '-') {
      modality = "-";
      ++index;
    } else {
      string modality = "";
    }

    while (isdigit(getChar()) || getChar() == ',') {
      
      modality += getChar();
      ++index;
    }

    if (getChar() != ']') {
      if (getChar() == '%') {
        throw runtime_error("Unexpected character at position " +
                            to_string(index) +
                            " got end of file but expected >");
      }
      throw runtime_error("Unexpected character at position " +
                          to_string(index) + " got " + getChar() +
                          " but expected >");
    }
    ++index;

    shared_ptr<Formula> rest = parseRest();

    if (modality == "") {
      if (rest->getType() == FDistributed) {
        Distributed *restDistributed = dynamic_cast<Distributed *>(rest.get());
        if (restDistributed->getModalityListString() == "1") {
          restDistributed->incrementPower();
          return rest;
        }
      }
      string groupString = GroupScanner::getTotalGroupString();
      int newModality = GroupScanner::getDistMap()[groupString];
      return Distributed::create(newModality, 1, rest);
    } else {
      if (rest->getType() == FDistributed) {
        Distributed *restDistributed = dynamic_cast<Distributed *>(rest.get());
        if (restDistributed->getModalityListString() == modality) {
          restDistributed->incrementPower();
          return rest;
        }
      }
      vector<int> newModalityList = split(modality, ',');

      //create a new K formula if the group size is 1
      if(newModalityList.size() == 1){
        return Know::create(newModalityList[0], 1, rest);
      }
      int newModality = GroupScanner::getDistMap()[modality];
      return Distributed::create(newModality, 1, rest);
    }
  } 
  else if (getChar() == '?') {
    index+=2;
    string modality;

    getChar();
    if (getChar() == '-') {
      modality = "-";
      ++index;
    } else {
      string modality = "";
    }

    while (isdigit(getChar())) {
      modality += getChar();
      ++index;
    }

    if (getChar() != ']') {
      if (getChar() == '%') {
        throw runtime_error("Unexpected character at position " +
                            to_string(index) +
                            " got end of file but expected >");
      }
      throw runtime_error("Unexpected character at position " +
                          to_string(index) + " got " + getChar() +
                          " but expected >");
    }
    ++index;

    shared_ptr<Formula> rest = parseRest();

    if (modality == "") {
      if (rest->getType() == FNotKnow) {
        NotKnow *restNotKnow = dynamic_cast<NotKnow *>(rest.get());
        if (restNotKnow->getModality() == 1) {
          restNotKnow->incrementPower();
          return rest;
        }
      }
      return NotKnow::create(1, 1, rest);
    } else {
      if (rest->getType() == FNotKnow) {
        NotKnow *restNotKnow = dynamic_cast<NotKnow *>(rest.get());
        if (restNotKnow->getModality() == stoi(modality)) {
          restNotKnow->incrementPower();
          return rest;
        }
      }
      return NotKnow::create(stoi(modality), 1, rest);
    }
  } 
  else if (file->substr(index, 3) == "~K[") {
    index+=3;
    string modality;
    
    getChar();
    if (getChar() == '-') {
      modality = "-";
      ++index;
    } else {
      string modality = "";
    }

    while (isdigit(getChar())) {
      modality += getChar();
      ++index;
    }

    if (getChar() != ']') {
      if (getChar() == '%') {
        throw runtime_error("Unexpected character at position " +
                            to_string(index) +
                            " got end of file but expected >");
      }
      throw runtime_error("Unexpected character at position " +
                          to_string(index) + " got " + getChar() +
                          " but expected >");
    }
    ++index;

    shared_ptr<Formula> rest = parseRest()->negate();

    if (modality == "") {
      if (rest->getType() == FNotKnow) {
        NotKnow *restNotKnow = dynamic_cast<NotKnow *>(rest.get());
        if (restNotKnow->getModality() == 1) {
          restNotKnow->incrementPower();
          return rest;
        }
      }
      return NotKnow::create(1, 1, rest);
    } else {
      if (rest->getType() == FNotKnow) {
        NotKnow *restNotKnow = dynamic_cast<NotKnow *>(rest.get());
        if (restNotKnow->getModality() == stoi(modality)) {
          restNotKnow->incrementPower();
          return rest;
        }
      }
      return NotKnow::create(stoi(modality), 1, rest);
    }
  } 
  else if (file->substr(index, 3) == "~D[") {
    index+=3;
    string modality;
    
    getChar();
    if (getChar() == '-') {
      modality = "-";
      ++index;
    } else {
      string modality = "";
    }

    while (isdigit(getChar())) {
      modality += getChar();
      ++index;
    }

    if (getChar() != ']') {
      if (getChar() == '%') {
        throw runtime_error("Unexpected character at position " +
                            to_string(index) +
                            " got end of file but expected >");
      }
      throw runtime_error("Unexpected character at position " +
                          to_string(index) + " got " + getChar() +
                          " but expected >");
    }
    ++index;

    shared_ptr<Formula> rest = parseRest()->negate();

    if (modality == "") {
      if (rest->getType() == FNotDistributed) {
        NotDistributed *restNotDistributed = dynamic_cast<NotDistributed *>(rest.get());
        if (restNotDistributed->getModalityListString() == "1") {
          restNotDistributed->incrementPower();
          return rest;
        }
      }
      int newModality = GroupScanner::getDistMap()[modality];
      return Distributed::create(newModality, 1, rest);
    } else {
      if (rest->getType() == FNotDistributed) {
        NotDistributed *restNotDistributed = dynamic_cast<NotDistributed *>(rest.get());
        if (restNotDistributed->getModalityListString() == (modality)) {
          restNotDistributed->incrementPower();
          return rest;
        }
      }

      vector<int> newModalityList = split(modality, ',');

      if(newModalityList.size() == 1){
        return NotKnow::create(newModalityList[0], 1, rest);
      }
      int newModality = GroupScanner::getDistMap()[modality];
      return NotDistributed::create(newModality, 1, rest);
    }
  } 
  else if (getChar() == '~') {
    ++index;
    return Not::create(parseRest());
  } 
  else if (file->substr(index, 5) == "$true") {
    index += 5;
    return True::create();
  } 
  else if (file->substr(index, 6) == "$false") {
    index += 6;
    return False::create();
  } 
  else if (isalnum(getChar()) || getChar() == '_') {
    string name = "";

    while (isalnum(getChar()) || getChar() == '_') {
      name += getChar();
      ++index;
    }
    return Atom::create(name);
  }

  if (getChar() == '%') {
    throw runtime_error("Unexpected end of file");
  }
  throw runtime_error("Unexpected character at position " + to_string(index) +
                      " got " + getChar());
}

shared_ptr<Formula> ParseFormula::parseAnd() {
  shared_ptr<Formula> left = parseRest();

  while (isspace(getChar()))
    ++index;

  if (getChar() == '&') {
    ++index;
    shared_ptr<Formula> right = parseAnd();

    if (left->getType() == FAnd && right->getType() == FAnd) {
      And *leftAnd = dynamic_cast<And *>(left.get());
      And *rightAnd = dynamic_cast<And *>(right.get());
      if (rightAnd->getLength() < leftAnd->getLength()) {
        for (auto rightSub : *rightAnd->getSubformulasReference()) {
          leftAnd->addSubformula(rightSub);
        }
        return left;
      } else {
        for (auto leftSub : *leftAnd->getSubformulasReference()) {
          rightAnd->addSubformula(leftSub);
        }
        return right;
      }
    } else if (left->getType() == FAnd) {
      And *leftAnd = dynamic_cast<And *>(left.get());
      leftAnd->addSubformula(right);
      return left;
    } else if (right->getType() == FAnd) {
      And *rightAnd = dynamic_cast<And *>(right.get());
      rightAnd->addSubformula(left);
      return right;
    } else {
      formula_set andSet;

      andSet.insert(left);
      andSet.insert(right);

      return And::create(andSet);
    }
  }
  return left;
}

shared_ptr<Formula> ParseFormula::parseOr() {
  shared_ptr<Formula> left = parseAnd();

  while (isspace(getChar()))
    ++index;

  if (getChar() == '|') {
    ++index;
    shared_ptr<Formula> right = parseOr();

    if (left->getType() == FOr && right->getType() == FOr) {
      Or *leftOr = dynamic_cast<Or *>(left.get());
      Or *rightOr = dynamic_cast<Or *>(right.get());
      if (rightOr->getLength() < leftOr->getLength()) {
        for (auto rightSub : *rightOr->getSubformulasReference()) {
          leftOr->addSubformula(rightSub);
        }
        return left;
      } else {
        for (auto leftSub : *leftOr->getSubformulasReference()) {
          rightOr->addSubformula(leftSub);
        }
        return right;
      }
    } else if (left->getType() == FOr) {
      Or *leftOr = dynamic_cast<Or *>(left.get());
      leftOr->addSubformula(right);
      return left;
    } else if (right->getType() == FOr) {
      Or *rightOr = dynamic_cast<Or *>(right.get());
      rightOr->addSubformula(left);
      return right;
    } else {
      formula_set orSet;

      orSet.insert(left);
      orSet.insert(right);

      return Or::create(orSet);
    }
  }
  return left;
}

shared_ptr<Formula> ParseFormula::parseImp() {
  shared_ptr<Formula> left = parseOr();
  while (isspace(getChar()))
    ++index;

  if (file->substr(index, 2) == "=>") {
    index += 2;
    shared_ptr<Formula> right = parseImp();

    formula_set orSet;

    if (right->getType() == FOr) {
      Or *rightOr = dynamic_cast<Or *>(right.get());
      rightOr->addSubformula(Not::create(left));
      return right;
    }
    orSet.insert(Not::create(left));
    orSet.insert(right);

    return Or::create(orSet);
  }
  return left;
}

shared_ptr<Formula> ParseFormula::parseIff() {
  shared_ptr<Formula> left = parseImp();

  while (isspace(getChar()))
    ++index;

  if (file->substr(index, 3) == "<=>") {
    index += 3;
    shared_ptr<Formula> right = parseIff();

    formula_set orSet;

    formula_set andSet;
    formula_set andNotSet;

    andSet.insert(left);
    andSet.insert(right);

    andNotSet.insert(Not::create(left));
    andNotSet.insert(Not::create(right));

    orSet.insert(And::create(andSet));
    orSet.insert(And::create(andNotSet));

    return Or::create(orSet);
  }
  return left;
}

shared_ptr<Formula> ParseFormula::parseFormula() {
  shared_ptr<Formula> formula = parseIff();

  while (isspace(getChar()))
    ++index;

  if (getChar() != '%') {
    throw runtime_error("Unexpected character at position " + to_string(index) +
                        " got " + getChar());
  }

  return formula;
}