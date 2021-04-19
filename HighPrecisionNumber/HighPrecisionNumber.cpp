//
// Created by GeorgeHu on 2021/4/16.
//

#include "../include/HighPrecisionNumber.h"

//use vector to initialize
//inherit from vector<int>
HighPrecisionNumber::HighPrecisionNumber(vector<int> &x):vector<int>(x)
{
    this->length = x.size();
}

//default constructor, set length to 1
HighPrecisionNumber::HighPrecisionNumber():vector<int>(1, 0)
{
    this->length = 1;
}

//HighPrecisionNumber plus HighPrecisionNumber
HighPrecisionNumber HighPrecisionNumber::operator+(HighPrecisionNumber other)
{
    int max_length = max(other.length, this->length);
    if (other.length < max_length)
        other.resize(max_length, 0);
    else if (this->length < max_length)
        this->resize(max_length, 0);

    vector<int> ans(max_length+1, 0);
    for (int i = 0; i < max_length; i++)
    {
        ans[i] += ((*this)[i]+other[i])%10;
        ans[i+1] += ((*this)[i]+other[i])/10;
    }

    while (max_length > 1 && ans[max_length] == 0)
        max_length--;

    this->resize(this->length);
    ans.resize(max_length+1);

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator*(int other)
{
    int jw=0, num_length=0, factor=other;
    int max_length;
    while (factor > 0)
    {
        num_length++;
        factor /= 10;
    }

    max_length = this->length + num_length;
    vector<int> ans(max_length, 0);
    this->resize(max_length, 0);

    for (int i = 0; i < max_length; i++)
    {
        ans[i] += (*this)[i]*other + jw;
        jw = ans[i] / 10;
        ans[i] %= 10;
    }

    this->resize(this->length);
    while (max_length > 1 && ans[max_length] == 0)
        max_length--;
    ans.resize(max_length+1);

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator*(HighPrecisionNumber other)
{
    int max_length= this->length + other.length;
    HighPrecisionNumber ans;
    ans.resize(max_length, 0);

    for (int i = 0; i < other.getLength(); i++)
    {
        int j = i;
        HighPrecisionNumber tmp;
        tmp.resize(i+this->length+1, 0);
        tmp = (*this)*other[i];
        while (j > 0)
        {
            j--;
            tmp = tmp*10;
        }
        ans = ans + tmp;
    }

    while (max_length > 1 && ans[max_length] == 0)
        max_length--;
    ans.resize(max_length+1);

    return ans;
}

void HighPrecisionNumber::show()
{
    for (int i = this->length - 1; i >= 0; i--)
        cout << (*this)[i];
}

int HighPrecisionNumber::getLength() const { return this->length; }
