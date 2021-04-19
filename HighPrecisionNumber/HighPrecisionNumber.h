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

//start from low ones place
class HighPrecisionNumber: vector<int>
{
private:
    int length;
public:
    HighPrecisionNumber(vector<int> &x);
    HighPrecisionNumber();
    HighPrecisionNumber operator+(HighPrecisionNumber other);
    HighPrecisionNumber operator*(int other);
    HighPrecisionNumber operator*(HighPrecisionNumber other);
    void show();
    int getLength() const;
};


#endif //LUOGU2_HIGHPRECISIONNUMBER_H
