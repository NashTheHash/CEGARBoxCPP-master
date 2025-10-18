#ifndef DISTRIBUTED_H
#define DISTRIBUTED_H

#include "../../Defines/Defines.h"
#include "../NotKnow/NotKnow.h"
#include "../FEnum/FEnum.h"
#include "../Formula/Formula.h"
#include "../True/True.h"
#include "../NotDistributed/NotDistributed.h"
#include "../../GroupScanner/GroupScanner.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Distributed : public Formula, public enable_shared_from_this<Distributed> {
private:
  int modality_, power_;
  shared_ptr<Formula> subformula_;
  int distributedHash_;

public:
  Distributed(int modality, int power, shared_ptr<Formula> subformula);
  ~Distributed();

  int getModality() const;
  string getModalityListString() const;
  int getPower() const;
  shared_ptr<Formula> getSubformula() const;

  void incrementPower();

  string toString() const;
  FormulaType getType() const;

  shared_ptr<Formula> negatedNormalForm();
  shared_ptr<Formula> tailNormalForm();
  shared_ptr<Formula> negate();
  shared_ptr<Formula> simplify();
  shared_ptr<Formula> modalFlatten();
  shared_ptr<Formula> axiomSimplify(int axiom, int depth);

  shared_ptr<Formula> clone() const;

  shared_ptr<Formula> constructDistributedReduced() const;

  static shared_ptr<Formula> create(int modality, int power,
                                    const shared_ptr<Formula> &subformula);
  static shared_ptr<Formula> create(vector<int> modality,
                                    const shared_ptr<Formula> &subformula);

  bool operator==(const Formula &other) const;
  bool operator!=(const Formula &other) const;

  size_t hash() const;
};

#endif
