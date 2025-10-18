#include "NotDistributed.h"

NotDistributed::NotDistributed(int modality, int power, shared_ptr<Formula> subformula) {
  modality_ = modality;
  power_ = power;

  NotDistributed *notDistributedFormula = dynamic_cast<NotDistributed *>(subformula.get());
  if (notDistributedFormula) {
    if (notDistributedFormula->getModality() == modality_) {
      power_ += notDistributedFormula->getPower();
      subformula_ = notDistributedFormula->getSubformula();
    } else {
      subformula_ = subformula;
    }
  } else {
    subformula_ = subformula;
  }
  std::hash<FormulaType> ftype_hash;
  std::hash<int> int_hash;
  size_t totalHash = ftype_hash(getType());
  notDistributedHash_ = totalHash + int_hash(modality_) + int_hash(power_) +
         subformula_->hash();
}

NotDistributed::~NotDistributed() {
#if DEBUG_DESTRUCT
  cout << "DESTRUCTING NOTDISTRIBUTED" << endl;
#endif
}

int NotDistributed::getModality() const { return modality_; }

string NotDistributed::getModalityListString() const {
  return GroupScanner::getModalityMap()[modality_];
}

int NotDistributed::getPower() const { return power_; }

shared_ptr<Formula> NotDistributed::getSubformula() const { return subformula_; }

void NotDistributed::incrementPower() { power_++; }

string NotDistributed::toString() const {
    string ret = "";
    for (int i = 0; i < power_; ++i) {
      ret += "@[" + GroupScanner::getModalityMap()[modality_] + "]";
    }
    return ret + subformula_->toString();
         
}

FormulaType NotDistributed::getType() const { return FNotDistributed; }

shared_ptr<Formula> NotDistributed::negatedNormalForm() {
  subformula_ = subformula_->negatedNormalForm();
  return shared_from_this();
}


shared_ptr<Formula> NotDistributed::tailNormalForm() {
    assert (1 == 0);
}

shared_ptr<Formula> NotDistributed::negate() {
  return Distributed::create(modality_, power_, subformula_->negate());
}

shared_ptr<Formula> NotDistributed::simplify() {
  subformula_ = subformula_->simplify();

  switch (subformula_->getType()) {
  case FTrue:
    return True::create();
  case FDistributed: {
    NotDistributed *notDistributedFormula = dynamic_cast<NotDistributed *>(subformula_.get());
    if (notDistributedFormula->getModality() == modality_) {
      power_ += notDistributedFormula->getPower();
      subformula_ = notDistributedFormula->getSubformula();
    }
    return shared_from_this();
  }

  default:
    return shared_from_this();
  }
}

shared_ptr<Formula> NotDistributed::modalFlatten() {
  subformula_ = subformula_->modalFlatten();
  if (subformula_->getType() == FDistributed) {
    NotDistributed *k = dynamic_cast<NotDistributed *>(subformula_.get());
    if (k->getModality() == modality_) {
      power_ += k->getPower();
      subformula_ = k->getSubformula();
    }
  }
  return shared_from_this();
}

shared_ptr<Formula> NotDistributed::axiomSimplify(int axiom, int depth) { 
    subformula_ = subformula_->axiomSimplify(axiom, depth+power_);
    if (depth > 0)
        power_ = 1;
    else
        power_ = min(power_, 2);
    return shared_from_this(); 
}

shared_ptr<Formula> NotDistributed::create(int modality, int power,
                                const shared_ptr<Formula> &subformula) {
  if (power == 0) {
    return subformula;
  }
  return shared_ptr<Formula>(new NotDistributed(modality, power, subformula));
}

shared_ptr<Formula> NotDistributed::create(vector<int> modality,
                                const shared_ptr<Formula> &subformula) {
  if (modality.size() == 0) {
    return subformula;
  }
  shared_ptr<Formula> formula =
      create(modality[modality.size() - 1], 1, subformula);
  for (size_t i = modality.size() - 1; i > 0; i--) {
    formula = create(modality[i - 1], 1, formula);
  }
  return formula;
}

shared_ptr<Formula> NotDistributed::constructNotDistributedReduced() const {
  return create(modality_, power_ - 1, subformula_);
}

shared_ptr<Formula> NotDistributed::clone() const {
  return create(modality_, power_, subformula_->clone());
}

bool NotDistributed::operator==(const Formula &other) const {
  if (other.getType() != getType()) {
    return false;
  }
  const NotDistributed *otherNotDistributed = dynamic_cast<const NotDistributed *>(&other);
  return modality_ == otherNotDistributed->modality_ && power_ ==
    otherNotDistributed->power_ && *subformula_ == *(otherNotDistributed->subformula_);
}

bool NotDistributed::operator!=(const Formula &other) const {
  return !(operator==(other));
}

size_t NotDistributed::hash() const {
  return notDistributedHash_;
}
