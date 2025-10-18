#include "FormulaTriple.h"

FormulaTriple::FormulaTriple() {}

FormulaTriple::~FormulaTriple() {}

void FormulaTriple::setClauses(clause_set newClauses) { clauses = newClauses; }
void FormulaTriple::setBoxClauses(modal_clause_set newBoxClauses) {
  boxClauses = newBoxClauses;
}
void FormulaTriple::setDiamondClauses(modal_clause_set newDiamondClauses) {
  diamondClauses = newDiamondClauses;
}
void FormulaTriple::setKnowClauses(modal_clause_set newKnowClauses) {
  knowClauses = newKnowClauses;
}
void FormulaTriple::setNotKnowClauses(modal_clause_set newNotKnowClauses) {
  notKnowClauses = newNotKnowClauses;
}
void FormulaTriple::setDistributedClauses(modal_clause_set newDistributedClauses) {
  distributedClauses = newDistributedClauses;
}
void FormulaTriple::setNotDistributedClauses(modal_clause_set newNotDistributedClauses) {
  notDistributedClauses = newNotDistributedClauses;
}


void FormulaTriple::eraseClauses() { clauses.clear(); }
void FormulaTriple::eraseBoxClauses() { boxClauses.clear(); }
void FormulaTriple::eraseDiamondClauses() { diamondClauses.clear(); }
void FormulaTriple::eraseKnowClauses() { knowClauses.clear(); }
void FormulaTriple::eraseNotKnowClauses() { notKnowClauses.clear(); }
void FormulaTriple::eraseDistributedClauses() { distributedClauses.clear(); }
void FormulaTriple::eraseNotDistributedClauses() { notDistributedClauses.clear(); }

const clause_set &FormulaTriple::getClauses() const { return clauses; }
const modal_clause_set &FormulaTriple::getBoxClauses() const {
  return boxClauses;
}
const modal_clause_set &FormulaTriple::getDiamondClauses() const {
  return diamondClauses;
}
const modal_clause_set &FormulaTriple::getKnowClauses() const {
  return knowClauses;
}
const modal_clause_set &FormulaTriple::getNotKnowClauses() const {
  return notKnowClauses;
}
const modal_clause_set &FormulaTriple::getDistributedClauses() const {
  return distributedClauses;
}
const modal_clause_set &FormulaTriple::getNotDistributedClauses() const {
  return notDistributedClauses;
}

void FormulaTriple::addClause(shared_ptr<Formula> formula) {
  clauses.insert({formula});
}

void FormulaTriple::addBoxClause(int modality, shared_ptr<Formula> left,
                                 shared_ptr<Formula> right) {
  boxClauses.insert({modality, left, right});
}

void FormulaTriple::addDiamondClause(int modality, shared_ptr<Formula> left,
                                     shared_ptr<Formula> right) {
  diamondClauses.insert({modality, left, right});
}

void FormulaTriple::addKnowClause(int modality, shared_ptr<Formula> left,
                                 shared_ptr<Formula> right) {
  knowClauses.insert({modality, left, right});
}

void FormulaTriple::addNotKnowClause(int modality, shared_ptr<Formula> left,
                                     shared_ptr<Formula> right) {
  notKnowClauses.insert({modality, left, right});
}

void FormulaTriple::addDistributedClause(int modality, shared_ptr<Formula> left,
                                 shared_ptr<Formula> right) {
  distributedClauses.insert({modality, left, right});
}

void FormulaTriple::addNotDistributedClause(int modality, shared_ptr<Formula> left,
                                     shared_ptr<Formula> right) {
  notDistributedClauses.insert({modality, left, right});
}

void FormulaTriple::addClause(Clause clause) { clauses.insert(clause); }

void FormulaTriple::addBoxClause(ModalClause boxClause) {
  boxClauses.insert(boxClause);
}

void FormulaTriple::addDiamondClause(ModalClause diamondClause) {
  diamondClauses.insert(diamondClause);
}

void FormulaTriple::addKnowClause(ModalClause knowClause) {
  knowClauses.insert(knowClause);
}

void FormulaTriple::addNotKnowClause(ModalClause notKnowClause) {
  notKnowClauses.insert(notKnowClause);
}

void FormulaTriple::addDistributedClause(ModalClause distributedClause) {
  distributedClauses.insert(distributedClause);
}

void FormulaTriple::addNotDistributedClause(ModalClause notDistributedClause) {
  notDistributedClauses.insert(notDistributedClause);
}

void FormulaTriple::extendBoxClauses(const modal_clause_set &otherBoxClauses) {
  boxClauses.insert(otherBoxClauses.begin(), otherBoxClauses.end());
}

void FormulaTriple::extendKnowClauses(const modal_clause_set &otherKnowClauses) {
  knowClauses.insert(otherKnowClauses.begin(), otherKnowClauses.end());
}

void FormulaTriple::extendDistributedClauses(const modal_clause_set 
                                              &otherDistributedClauses) {
  distributedClauses.insert(otherDistributedClauses.begin(), otherDistributedClauses.end());
}

void FormulaTriple::extendClauses(const FormulaTriple &otherClauses) {
  clauses.insert(otherClauses.clauses.begin(), otherClauses.clauses.end());
  boxClauses.insert(otherClauses.boxClauses.begin(),
                    otherClauses.boxClauses.end());
  diamondClauses.insert(otherClauses.diamondClauses.begin(),
                        otherClauses.diamondClauses.end());
  knowClauses.insert(otherClauses.knowClauses.begin(),
                    otherClauses.knowClauses.end());
  notKnowClauses.insert(otherClauses.notKnowClauses.begin(),
                        otherClauses.notKnowClauses.end());
  distributedClauses.insert(otherClauses.distributedClauses.begin(),
                    otherClauses.distributedClauses.end());
  notDistributedClauses.insert(otherClauses.notDistributedClauses.begin(),
                        otherClauses.notDistributedClauses.end());
}

void FormulaTriple::removeTrueAndFalse() {
  clause_set newClauses;
  for (Clause clause : clauses) {
    if (clause.formula->getType() == FTrue) {
      clause.formula = Not::create(Atom::create("$false"));
    } else if (clause.formula->getType() == FFalse) {
      clause.formula = Atom::create("$false");
    }
    newClauses.insert(clause);
  }
  clauses = newClauses;

  modal_clause_set newBoxClauses;
  for (ModalClause clause : boxClauses) {
    if (clause.left->getType() == FTrue) {
      clause.left = Not::create(Atom::create("$false"));
    } else if (clause.left->getType() == FFalse) {
      clause.left = Atom::create("$false");
    }
    if (clause.right->getType() == FTrue) {
      clause.right = Not::create(Atom::create("$false"));
    } else if (clause.right->getType() == FFalse) {
      clause.right = Atom::create("$false");
    }
    newBoxClauses.insert(clause);
  }
  boxClauses = newBoxClauses;

  modal_clause_set newDiamondClauses;
  for (ModalClause clause : diamondClauses) {
    if (clause.left->getType() == FTrue) {
      clause.left = Not::create(Atom::create("$false"));
    } else if (clause.left->getType() == FFalse) {
      clause.left = Atom::create("$false");
    }
    if (clause.right->getType() == FTrue) {
      clause.right = Not::create(Atom::create("$false"));
    } else if (clause.right->getType() == FFalse) {
      clause.right = Atom::create("$false");
    }
    newDiamondClauses.insert(clause);
  }
  diamondClauses = newDiamondClauses;

  modal_clause_set newKnowClauses;
  for (ModalClause clause : knowClauses) {
    if (clause.left->getType() == FTrue) {
      clause.left = Not::create(Atom::create("$false"));
    } else if (clause.left->getType() == FFalse) {
      clause.left = Atom::create("$false");
    }
    if (clause.right->getType() == FTrue) {
      clause.right = Not::create(Atom::create("$false"));
    } else if (clause.right->getType() == FFalse) {
      clause.right = Atom::create("$false");
    }
    newKnowClauses.insert(clause);
  }
  knowClauses = newKnowClauses;

  modal_clause_set newNotKnowClauses;
  for (ModalClause clause : notKnowClauses) {
    if (clause.left->getType() == FTrue) {
      clause.left = Not::create(Atom::create("$false"));
    } else if (clause.left->getType() == FFalse) {
      clause.left = Atom::create("$false");
    }
    if (clause.right->getType() == FTrue) {
      clause.right = Not::create(Atom::create("$false"));
    } else if (clause.right->getType() == FFalse) {
      clause.right = Atom::create("$false");
    }
    newKnowClauses.insert(clause);
  }
  notKnowClauses = newNotKnowClauses;

  modal_clause_set newDistributedClauses;
  for (ModalClause clause : distributedClauses) {
    if (clause.left->getType() == FTrue) {
      clause.left = Not::create(Atom::create("$false"));
    } else if (clause.left->getType() == FFalse) {
      clause.left = Atom::create("$false");
    }
    if (clause.right->getType() == FTrue) {
      clause.right = Not::create(Atom::create("$false"));
    } else if (clause.right->getType() == FFalse) {
      clause.right = Atom::create("$false");
    }
    newDistributedClauses.insert(clause);
  }
  distributedClauses = newDistributedClauses;

  modal_clause_set newNotDistributedClauses;
  for (ModalClause clause : notKnowClauses) {
    if (clause.left->getType() == FTrue) {
      clause.left = Not::create(Atom::create("$false"));
    } else if (clause.left->getType() == FFalse) {
      clause.left = Atom::create("$false");
    }
    if (clause.right->getType() == FTrue) {
      clause.right = Not::create(Atom::create("$false"));
    } else if (clause.right->getType() == FFalse) {
      clause.right = Atom::create("$false");
    }
    newDistributedClauses.insert(clause);
  }
  notDistributedClauses = newNotDistributedClauses;
}

vector<string> FormulaTriple::toStringComponents() const {
  vector<string> components;
  for (Clause clause : clauses) {
    components.push_back(clause.formula->toString());
  }

  for (ModalClause boxClause : boxClauses) {
    components.push_back(boxClause.left->toString() + " -> [" +
                         to_string(boxClause.modality) + "]" +
                         boxClause.right->toString());
  }

  for (ModalClause diamondClause : diamondClauses) {
    components.push_back(diamondClause.left->toString() + " -> <" +
                         to_string(diamondClause.modality) + ">" +
                         diamondClause.right->toString());
  }
  for (ModalClause knowClause : knowClauses) {
    components.push_back(knowClause.left->toString() + " -> K[" +
                         to_string(knowClause.modality) + "]" +
                         knowClause.right->toString());
  }

  for (ModalClause notKnowClause : notKnowClauses) {
    components.push_back(notKnowClause.left->toString() + " -> ?[" +
                         to_string(notKnowClause.modality) + "]" +
                         notKnowClause.right->toString());
  }
  for (ModalClause distributedClause : distributedClauses) {
    components.push_back(distributedClause.left->toString() + " -> D[" +
                        GroupScanner::getModalityMap()[distributedClause.modality]
                         + "]" + distributedClause.right->toString());
  }
  for (ModalClause notDistributedClause : notDistributedClauses) {
    components.push_back(notDistributedClause.left->toString() + " -> ?[" +
                         GroupScanner::getModalityMap()[notDistributedClause.modality]
                         + "]" + notDistributedClause.right->toString());
  }
  return components;
}
