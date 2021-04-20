//
// Created by GeorgeHu on 2021/4/16.
//

#ifndef LUOGU2_HIGHPRECISIONNUMBER_H
#define LUOGU2_HIGHPRECISIONNUMBER_H

#include <vector>
#include <iostream>
#include <algorithm>

using std::vector;
using std::cout;
using std::max;
using std::invalid_argument;

//start from low ones place
class HighPrecisionNumber: vector<int>
{
private:
    int length;
    bool sign;//false for negative, true for positive or zero
public:
    HighPrecisionNumber(vector<int> &x, bool sign=true);
    void setNumber(int x);
    HighPrecisionNumber();
    HighPrecisionNumber operator+(HighPrecisionNumber other);
    HighPrecisionNumber operator+(int other);
    HighPrecisionNumber operator-(int other);
    HighPrecisionNumber operator-(HighPrecisionNumber other);
    HighPrecisionNumber operator*(int other);
    HighPrecisionNumber operator*(HighPrecisionNumber other);
    HighPrecisionNumber operator/(int other);
    HighPrecisionNumber operator/(HighPrecisionNumber other);
    bool operator<(HighPrecisionNumber &other);
    bool operator<(int other);
    bool operator>(HighPrecisionNumber &other);
    bool operator>(int other);
    bool operator==(HighPrecisionNumber &other);
    bool operator==(int other);
    int compare(const HighPrecisionNumber &other) const;
    void show();
    int getLength() const;

};


#endif //LUOGU2_HIGHPRECISIONNUMBER_H
