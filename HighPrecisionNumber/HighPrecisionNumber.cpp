//
// Created by GeorgeHu on 2021/4/16.
//

#include "../include/HighPrecisionNumber.h"

//use vector to initialize
//inherit from vector<int>
HighPrecisionNumber::HighPrecisionNumber(vector<int> &x, bool sign):vector<int>(x)
{
    this->length = x.size();
    this->sign = sign;
}

//default constructor, set length to 1
HighPrecisionNumber::HighPrecisionNumber():vector<int>(1, 0)
{
    this->length = 1;
    this->sign = true;
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

    return HighPrecisionNumber(ans);
}

HighPrecisionNumber HighPrecisionNumber::operator+(int other)
{
    int num_len=0, other_copy=other;
    vector<int> oth;

    while (other_copy > 0)
    {
        num_len++;
        other_copy /= 10;
    }
    while (other > 0)
    {
        oth.push_back(other % 10);
        other /= 10;
    }

    HighPrecisionNumber high_oth(oth);

    return (*this)+high_oth;
}

HighPrecisionNumber HighPrecisionNumber::operator-(int other)
{
    HighPrecisionNumber other_number;
    other_number.setNumber(other);

    return (*this)-other_number;
}

HighPrecisionNumber HighPrecisionNumber::operator-(HighPrecisionNumber other)
{
    int max_length;
    max_length = max(other.length, this->length);
    HighPrecisionNumber ans;
    ans.resize(max_length+1, 0);

    if (other < (*this))
    {
        other.resize(max_length, 0);
        for (int i = 0; i < max_length; i++)
        {
            ans[i] += 10 + (*this)[i] - other[i];
            ans[i+1] += ans[i]/10 - 1;
            ans[i] %= 10;
        }
        ans.sign = true;
    }
    else if (other > (*this))
    {
        this->resize(max_length, 0);
        for (int i = 0; i < max_length; i++)
        {
            ans[i] += 10 + other[i] - (*this)[i];
            ans[i+1] += ans[i]/10 - 1;
            ans[i] %= 10;
        }
        this->resize(this->length);
        ans.sign = false;
    }
    else
    {
        ans.setNumber(0);
        ans.sign = true;
        return ans;
    }

    while (max_length > 0 && ans[max_length] == 0)
        max_length--;

    ans.resize(max_length+1);
    ans.length = max_length+1;

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

    return HighPrecisionNumber(ans);
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

    max_length--;
    while (max_length > 0 && ans[max_length] == 0)
        max_length--;
    ans.resize(max_length+1);
    ans.length = max_length + 1;

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator/(int other)
{
    int x=0, max_length, other_copy=other, other_len=0;
    HighPrecisionNumber other_number;
    other_number.setNumber(other);

    if (other == 0)
        throw invalid_argument("DIV0");
    else if ((*this)<other)
    {
        other_number.setNumber(0);
        return other_number;
    }
    else if ((*this)==other)
    {
        other_number.setNumber(1);
        return other_number;
    }
    else
        other_len = other_number.length;

    max_length = max(this->length, other_len);
    HighPrecisionNumber ans;
    ans.resize(max_length, 0);

    for (int i = this->length-1; i >= 0; i--)
    {
        ans[i] = (x*10+(*this)[i])/other;
        x = (x*10 + (*this)[i])%other;
    }

    while (max_length > 1 && ans[max_length-1] == 0)
        max_length--;

    ans.resize(max_length);
    ans.length = max_length;
    ans.sign = !((other > 0)^((*this).sign));

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator/(HighPrecisionNumber other)
{
    HighPrecisionNumber temp, copy, ans;
    copy.resize(this->length);
    copy.assign(this->begin(), this->end());
    copy.length = this->length;

    int other_len, max_length;
    if (other == 0)
        throw invalid_argument("DIV0");
    else if ((*this)<other)
    {
        other.setNumber(0);
        return other;
    }
    else if ((*this)==other)
    {
        other.setNumber(1);
        return other;
    }
    else
        other_len = other.length;

    ans.resize(this->length);
    max_length = this->length + other.length - 1;
    temp.resize(max_length, 0);
    temp.length = max_length+1;

    for (int i = 0; i < this->length; i++)
    {
        temp.length--;
        temp.resize(temp.length);
        temp.assign(temp.length, 0);
        for (int j = 0; j < other.length; j++)
            temp[max_length-other_len+j-i] = other[j];

        while (copy > temp)
        {
            ans[max_length-other_len-i]++;
            copy = copy - temp;
            if (copy[copy.length-1] == 0)
            {
                copy.length--;
                copy.resize(copy.length);
            }
        }
    }
    max_length = this->length;
    while (max_length > 1 && ans[max_length-1] == 0)
        max_length--;
    ans.resize(max_length);
    ans.length = max_length;
    ans.sign = !(this->sign^other.sign);

    return ans;
}

void HighPrecisionNumber::show()
{
    if (!this->sign)
        cout << "-";
    for (int i = this->length - 1; i >= 0; i--)
        cout << (*this)[i];
}

int HighPrecisionNumber::getLength() const { return this->length; }

void HighPrecisionNumber::setNumber(int x)
{
    int x_copy=x, len=0;
    if (x == 0)
    {
        this->resize(1);
        this->assign(1, 0);
        this->length = 1;
    }
    else
    {
        while (x_copy > 0)
        {
            len++;
            x_copy /= 10;
        }
        this->resize(len);
        this->length = len;
        for (int i = 0; i < len; i++)
        {
            (*this)[i] = x % 10;
            x /= 10;
        }
    }
}


bool HighPrecisionNumber::operator<(HighPrecisionNumber &other)
{
    if (this->length > other.length)
        return false;
    else if (this->length < other.length)
        return true;
    else
    {
        for (int i = this->length-1; i >= 0; i--)
        {
            if ((*this)[i] > other[i])
                return false;
            else if ((*this)[i] < other[i])
                return true;
        }
        return false;
    }
}

bool HighPrecisionNumber::operator<(int other)
{
    HighPrecisionNumber other_num;
    other_num.setNumber(other);

    return (*this)<other_num;
}

bool HighPrecisionNumber::operator>(HighPrecisionNumber &other)
{
    if (other < (*this))
        return true;
    else
        return false;
}

bool HighPrecisionNumber::operator>(int other)
{
    HighPrecisionNumber other_num;
    other_num.setNumber(other);

    return (*this)>other_num;
}

bool HighPrecisionNumber::operator==(HighPrecisionNumber &other)
{
    if (this->length == other.length)
    {
        for (int i = this->length-1; i >= 0; i--)
        {
            if ((*this)[i] != other[i])
                return false;
        }
        return true;
    }
    else
        return false;
}

bool HighPrecisionNumber::operator==(int other)
{
    HighPrecisionNumber other_number;
    other_number.setNumber(other);

    return other_number==(*this);
}

//this > other return 1, this < other return -1, this == other return 0
int HighPrecisionNumber::compare(const HighPrecisionNumber &other) const
{
    if (this->length > other.length)
        return 1;
    else if (this->length < other.length)
        return -1;
    else
    {
        for (int i = this->length-1; i >= 0; i--)
        {
            if ((*this)[i] > other[i])
                return 1;
            else if ((*this)[i] < other[i])
                return -1;
        }
        return 0;
    }
}
