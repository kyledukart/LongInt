#include "longint.h"

  istream &operator>>( istream &in, LongInt &rhs ){
      string input;
      in >> input;
      rhs = LongInt(input);
      return in;
  }

  ostream &operator<<( ostream &out, const LongInt &rhs ){
      string output;
      LongInt temp = rhs;
      if (temp.digits.isEmpty() == true) {
        out << '0';
        return out;
      }
      if (temp.negative == true)
        output.push_back('-');
      while (temp.digits.isEmpty() == false){
        output.push_back(temp.digits.getFront());
        temp.digits.removeFront();
      }
      out << output;
      return out;
  }

  // Constructors
  LongInt::LongInt( const string str ){
    string temp = str;
    if (temp.front() == '-'){
        negative = true;
        temp.erase(0,1);
    }
    else
        negative = false;
    while (temp.empty() == false){
        digits.addFront(temp.back());
        temp.pop_back();
    }
    remove0s();
  }

  LongInt::LongInt( const LongInt &rhs ){
    *this = rhs;
    remove0s();
  }

  LongInt::LongInt( ){
//    digits = Deque<char>;
    negative = false;
  }

  // Destructor
  LongInt::~LongInt( ){
//    delete digits;
  }

  // Arithmetic binary operators
  LongInt LongInt::operator+( const LongInt &rhs ) const{
    LongInt answer = LongInt();
    LongInt tempthis = *this;
    LongInt rhstemp = rhs;
    int lhdigit, rhdigit, remainder = 0;
    if (tempthis.negative == true && rhstemp.negative == false) {
        tempthis.negative = false;
        return (rhstemp - tempthis);
    }
    if (tempthis.negative == false && rhstemp.negative == true) {
        rhstemp.negative = false;
        return (tempthis - rhstemp);
    }
    if (tempthis.negative == true && rhstemp.negative == true)
        answer.negative = true;
    while (tempthis.digits.isEmpty() == false || rhstemp.digits.isEmpty() == false || remainder == 1) {
        if (tempthis.digits.isEmpty() == false) {
            lhdigit = tempthis.digits.getBack() - '0';
        }
        else {
            lhdigit = 0;
        }
        if (rhstemp.digits.isEmpty() == false) {
            rhdigit = rhstemp.digits.getBack() - '0';
        }
        else {
            rhdigit = 0;
        }
        if ((lhdigit + rhdigit + remainder) >= 10){
            answer.digits.addFront('0' + (lhdigit + rhdigit + remainder - 10));
            remainder = 1;
            if (tempthis.digits.isEmpty() == false)
                tempthis.digits.removeBack();
            if (rhstemp.digits.isEmpty() == false)
                rhstemp.digits.removeBack();
        }
        else {
            answer.digits.addFront('0' + (lhdigit + rhdigit + remainder));
            remainder = 0;
            if (tempthis.digits.isEmpty() == false)
                tempthis.digits.removeBack();
            if (rhstemp.digits.isEmpty() == false)
                rhstemp.digits.removeBack();
        }
    }
    answer.remove0s();
    return answer;
  }

  LongInt LongInt::operator-( const LongInt &rhs ) const{
    LongInt answer = LongInt();
    LongInt tempthis = *this;
    LongInt rhstemp = rhs;
    int lhdigit, rhdigit, remainder = 0;
    if (tempthis.negative == true && rhstemp.negative == false) {
        rhstemp.negative = true;
        return (rhstemp + tempthis);
    }
    if (tempthis.negative == false && rhstemp.negative == true) {
        rhstemp.negative = false;
        return (tempthis + rhstemp);
    }
    if (tempthis.negative == true && rhstemp.negative == true) {
        rhstemp.negative = false;
        tempthis.negative = false;
        return (rhstemp - tempthis);
    }
    if (tempthis < rhstemp) {
        answer = rhstemp - tempthis;
        answer.negative = true;
        return answer;
    }
    while (tempthis.digits.isEmpty() == false || rhstemp.digits.isEmpty() == false) {
        if (tempthis.digits.isEmpty() == false) {
            lhdigit = tempthis.digits.getBack() - '0';
        }
        else {
            lhdigit = 0;
        }
        if (rhstemp.digits.isEmpty() == false) {
            rhdigit = rhstemp.digits.getBack() - '0';
        }
        else {
            rhdigit = 0;
        }
        if (lhdigit - rhdigit - remainder < 0) {
            answer.digits.addFront('0' + (lhdigit - rhdigit - remainder + 10));
            if (tempthis.digits.isEmpty() == false)
                tempthis.digits.removeBack();
            if (rhstemp.digits.isEmpty() == false)
                rhstemp.digits.removeBack();
            remainder = 1;
        }
        else {
            answer.digits.addFront('0' + (lhdigit - rhdigit - remainder));
            if (tempthis.digits.isEmpty() == false)
                tempthis.digits.removeBack();
            if (rhstemp.digits.isEmpty() == false)
                rhstemp.digits.removeBack();
            remainder = 0;
        }
    }
    answer.remove0s();
    return answer;
  }

  // assignment operators
  const LongInt& LongInt::operator=( const LongInt &rhs ){
    negative = rhs.negative;
    digits = rhs.digits;
    remove0s();
    return *this;
  }

  // Logical binary operators
  bool LongInt::operator< ( const LongInt & rhs ) const{
    if (*this == rhs || *this > rhs)
        return false;
    else
        return true;
  }

  bool LongInt::operator<=( const LongInt & rhs ) const{
    if (*this < rhs || *this == rhs)
        return true;
    else
        return false;
  }

  bool LongInt::operator> ( const LongInt & rhs ) const{
    if (digits.isEmpty() && rhs.digits.isEmpty())
        return false;
    if (negative == false && rhs.negative == true)
        return true;
    if (negative == true && rhs.negative == false)
        return false;
    if (digits.size() > rhs.digits.size()) {                        // length of first number is larger
        if (negative)
            return false;
        else
            return true;
    }
    else if (digits.size() < rhs.digits.size()) {                   // length of first number is smaller
        if (negative)
            return true;
        else
            return false;
    }
    else {
        LongInt temp = rhs;
        LongInt tempthis = *this;
        while (tempthis.digits.isEmpty() == false) {
            if (tempthis.digits.getFront() > temp.digits.getFront()) {       // value of first number is larger
                if (negative)
                    return false;
                else
                    return true;
            }
            else if (tempthis.digits.getFront() < temp.digits.getFront()) {  // value of first number is smaller
                if (negative)
                    return true;
                else
                    return false;
            }
            tempthis.digits.removeFront();
            temp.digits.removeFront();
        }
        return false;                                               // numbers are equal
    }
  }

  bool LongInt::operator>=( const LongInt & rhs ) const{
    if (*this > rhs || *this == rhs)
        return true;
    else
        return false;
  }

  bool LongInt::operator==( const LongInt & rhs ) const{
    if (digits.size() == rhs.digits.size()) {
        if (negative == rhs.negative) {
            LongInt temp = rhs;
            LongInt tempthis = *this;
            while (tempthis.digits.isEmpty() == false && temp.digits.isEmpty() == false) {
                if (tempthis.digits.getFront() == temp.digits.getFront()) {
                    tempthis.digits.removeFront();
                    temp.digits.removeFront();
                }
                else
                    return false;
            }
            if (tempthis.digits.isEmpty() == true && temp.digits.isEmpty() == true)
                return true;
        }
        else
            return false;
    }
    return false;
  }

  bool LongInt::operator!=( const LongInt & rhs ) const{
    return !(*this == rhs);
  }

  void LongInt::remove0s( ){
    if (digits.isEmpty() == false) {
        while (digits.getFront() == '0') {
            digits.removeFront();
            if (digits.isEmpty())
                break;
        }
    }
//    if (digits.isEmpty())
//        digits.addFront('0');
  }
