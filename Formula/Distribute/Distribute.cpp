#include "Distribute.h"


Distribute::Distribute(vector<int> modality, int power, shared_ptr<Formula> subformula) {

    modalityList_ = modality;
    power_ = power;

    Distribute *DistributeFormula = dynamic_cast<Distribute *>(subformula.get());

    if (DistributeFormula) {
        if (DistributeFormula->getModalityList() == modalityList_) {

            power_ += DistributeFormula->getPower();
            subformula_ = DistributeFormula->getSubformula();
        }
        else subformula_ = subformula;
    }
    else subformula_ = subformula;

    std::hash<FormulaType> ftype_hash;
    std::hash<int> int_hash;
    size_t totalHash = ftype_hash(getType());
    size_t vectorHash = 0;

    modalityListString_ = "";


    for(auto item: modalityList_){

        vectorHash += int_hash(item);
        modalityListString_ += to_string(item) + ",";
        //cout << "Item: " << item<< '\n';
    }

    modalityListString_ = modalityListString_.substr(0, modalityListString_.length() - 1);
    //cout << "MOdalityString: " << modalityListString_ << '\n';

    DistributeHash_ = totalHash + int_hash(vectorHash) + int_hash(power_) + subformula_->hash();
}


Distribute::~Distribute() {

#if DEBUG_DESTRUCT
  cout << "DESTRUCTING Distribute" << endl;
#endif
}


vector<int> Distribute::getModalityList() const{
    return modalityList_;
}


string Distribute::getModalityListString() const{
    return modalityListString_;
}


int Distribute::getPower() const{
    return power_;
}


shared_ptr<Formula> Distribute::getSubformula() const{
    return subformula_;
}


void Distribute::incrementPower(){
    power_++;
}


string Distribute::toString() const {

    string ret = "";
    for (int i = 0; i < power_; ++i) ret += "D[" + modalityListString_ + "]";

    return ret + subformula_->toString();
}


FormulaType Distribute::getType() const{
    return FDistribute;
}


shared_ptr<Formula> Distribute::negatedNormalForm() {

  subformula_ = subformula_->negatedNormalForm();
  return shared_from_this();
}


shared_ptr<Formula> Distribute::tailNormalForm() {

    assert (1 == 0);

}


shared_ptr<Formula> Distribute::negate() {
  return NotDistribute::create(modalityList_, power_, subformula_->negate());
}


shared_ptr<Formula> Distribute::simplify() {

    subformula_ = subformula_->simplify();

    switch (subformula_->getType()) {

        case FTrue: return True::create();

        case FDistribute: {
            Distribute *DistributeFormula = dynamic_cast<Distribute *>(subformula_.get());

            if (DistributeFormula->getModalityListString() == modalityListString_) {

                power_ += DistributeFormula->getPower();
                subformula_ = DistributeFormula->getSubformula();
            }
            return shared_from_this();
        } 
        default: return shared_from_this();
    }
}


shared_ptr<Formula> Distribute::modalFlatten() {

    subformula_ = subformula_->modalFlatten();

    if (subformula_->getType() == FDistribute) {
        Distribute *k = dynamic_cast<Distribute *>(subformula_.get());

        if (k->getModalityListString() == modalityListString_) {
                power_ += k->getPower();
                subformula_ = k->getSubformula();
        }
    }
    return shared_from_this();
}


shared_ptr<Formula> Distribute::axiomSimplify(int axiom, int depth) { 

    subformula_ = subformula_->axiomSimplify(axiom, depth+power_);

    if (depth > 0) power_ = 1;
    else power_ = min(power_, 2);


    return shared_from_this(); 
}


shared_ptr<Formula> Distribute::create(vector<int> modality, int power, const shared_ptr<Formula> &subformula) {
    
    if (power == 0) return subformula;
    return shared_ptr<Formula>(new Distribute(modality, power, subformula));
}


shared_ptr<Formula> Distribute::create(vector<vector<int>> modality, const shared_ptr<Formula> &subformula) {

    if (modality.size() == 0) return subformula;

    shared_ptr<Formula> formula = Distribute::create(modality[modality.size() - 1], 1, subformula);


    for (size_t i = modality.size() - 1; i > 0; i--) {
        formula = Distribute::create(modality[i - 1], 1, formula);
    }

    return formula;
}


shared_ptr<Formula> Distribute::constructDistributeReduced() const {
  return Distribute::create(modalityList_, power_ - 1, subformula_);
}


shared_ptr<Formula> Distribute::clone() const {
    return create(modalityList_, power_, subformula_->clone());
}


bool Distribute::operator==(const Formula &other) const {

    if (other.getType() != getType()) return false;

    const Distribute *otherDistribute = dynamic_cast<const Distribute *>(&other);

    return modalityListString_ == otherDistribute->modalityListString_ &&
        power_ == otherDistribute->power_ &&
        *subformula_ == *(otherDistribute->subformula_);
}


bool Distribute::operator!=(const Formula &other) const {
    return !(operator==(other));
}


size_t Distribute::hash() const {
  return DistributeHash_;
}