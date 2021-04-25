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
    //flag is true if sign is the different, else false.
    if (other == 0)
        return *this;
    else if ((*this) == 0)
        return other;

    bool flag=this->sign^other.sign;
    HighPrecisionNumber abs_x, abs_y;
    abs_x.setNumber(*this, true);
    abs_y.setNumber(other, true);

    if (flag)
    {
        if (this->sign)
            return abs_x-abs_y;
        else
            return abs_y-abs_x;
    }
    int max_length = max(other.length, this->length);
    if (abs_y.length < max_length)
    {
        abs_y.resize(max_length, 0);
        abs_y.length = max_length;
    }
    else if (abs_x.length < max_length)
    {
        abs_x.resize(max_length, 0);
        abs_x.length = max_length;
    }

    HighPrecisionNumber ans;
    ans.resize(max_length+1, 0);
    ans.length = max_length + 1;

    for (int i = 0; i < max_length; i++)
    {
        ans[i+1] += (ans[i]+abs_x[i]+abs_y[i])/10;
        ans[i] = (ans[i]+abs_x[i]+abs_y[i])%10;
    }

    ans.sign = other.sign;

    while (max_length > 1 && ans[max_length-1] == 0)
        max_length--;

    this->resize(this->length);
    ans.resize(max_length+1);
    ans.length = max_length+1;

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator+(int other)
{
    HighPrecisionNumber oth, ans;
    oth.setNumber(other, other >= 0);

    ans = (*this)+oth;

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator-(int other)
{
    HighPrecisionNumber other_number;
    other_number.setNumber(other, other >= 0);

    return (*this)-other_number;
}

HighPrecisionNumber HighPrecisionNumber::operator-(HighPrecisionNumber other)
{
    bool flag=this->sign^other.sign;
    HighPrecisionNumber ans;
    HighPrecisionNumber abs_x, abs_y;

    if ((*this)==other)
    {
        ans.setNumber(0);
        return ans;
    }

    if (this->sign && !other.sign)
    {
        abs_x.setNumber(*this, true);
        abs_y.setNumber(other, true);
        return abs_x+abs_y;
    }
    else if (!this->sign && other.sign)
    {
        abs_x.setNumber(*this, true);
        abs_y.setNumber(other, true);
        ans = abs_x + abs_y;
        ans.sign = false;
        return ans;
    }

    int max_length=max(other.length, this->length);
    HighPrecisionNumber *bigger, *smaller;
    abs_x.setNumber(*this, true);
    abs_y.setNumber(other, true);

    if (abs_x < abs_y)
    {
        smaller = &abs_x;
        bigger = &abs_y;
        ans.sign = false;
    }
    else
    {
        bigger = &abs_x;
        smaller = &abs_y;
        ans.sign = true;
    }

    ans.resize(max_length+1, 0);

    (*smaller).resize(max_length, 0);
    (*bigger).resize(max_length, 0);

    for (int i = 0; i < max_length; i++)
    {
        ans[i] += 10 + (*bigger)[i] - (*smaller)[i];
        ans[i+1] += ans[i]/10 - 1;
        ans[i] %= 10;
    }

    while (max_length > 0 && ans[max_length] == 0)
        max_length--;

    ans.resize(max_length+1);
    ans.length = max_length+1;

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator*(int other)
{
    int jw=0, num_length=0, factor=other >= 0 ? other:-other;
    int max_length;
    while (factor > 0)
    {
        num_length++;
        factor /= 10;
    }

    max_length = this->length + num_length;
    HighPrecisionNumber ans;
    ans.resize(max_length, 0);
    ans.assign(max_length, 0);
    ans.length = max_length;

    this->resize(max_length, 0);

    for (int i = 0; i < max_length; i++)
    {
        ans[i] += (*this)[i]*other + jw;
        jw = ans[i] / 10;
        ans[i] %= 10;
    }

    this->resize(this->length);
    while (max_length > 1 && ans[max_length-1] == 0)
        max_length--;
    ans.resize(max_length+1);
    ans.length = max_length;
    ans.sign = !(this->sign^(other>=0));

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator*(HighPrecisionNumber other)
{
    int max_length= this->length + other.length;
    HighPrecisionNumber ans;
    ans.resize(max_length+1, 0);
    ans.assign(max_length+1, 0);
    ans.length = max_length+1;

    for (int i = 0; i < other.getLength(); i++)
    {
        int j = i;
        HighPrecisionNumber tmp;
        tmp.resize(ans.length, 0);
        tmp.assign(ans.length, 0);
        tmp.length = ans.length;
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
    ans.sign = !(this->sign^other.sign);

    return ans;
}

HighPrecisionNumber HighPrecisionNumber::operator/(int other)
{
    int x=0, max_length, other_len=0;
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
    if (!(ans == 0))
        ans.sign = !(this->sign^(other >= 0));

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
    if (!(ans == 0))
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

void HighPrecisionNumber::setNumber(HighPrecisionNumber &t, bool flag)
{
    this->resize(t.length, 0);
    this->assign(t.begin(), t.end());
    this->length = t.length;
    this->sign = flag;
}

void HighPrecisionNumber::setNumber(int x, bool flag)
{
    int len=0, x_copy;
    x_copy = x >= 0 ? x:-x;
    this->sign = flag;
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
        x_copy = x >= 0 ? x:-x;
        for (int i = 0; i < len; i++)
        {
            (*this)[i] = x_copy % 10;
            x_copy /= 10;
        }
    }
}


bool HighPrecisionNumber::operator<(HighPrecisionNumber &other)
{
    //flag set to true if sign is the same, else is false.
    bool flag;
    flag = !(this->sign^other.sign);

    if (!flag)
    {
        if (!this->sign && other.sign)
            return true;
        else
            return false;
    }

    if (this->length > other.length)
        return !flag;
    else if (this->length < other.length)
        return flag;
    else
    {
        for (int i = this->length-1; i >= 0; i--)
        {
            if ((*this)[i] > other[i])
                return !flag;
            else if ((*this)[i] < other[i])
                return flag;
        }
        return false;
    }
}

bool HighPrecisionNumber::operator<(int other)
{
    HighPrecisionNumber other_num;
    other_num.setNumber(other, other >= 0);

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
    other_num.setNumber(other, other >= 0);

    return (*this)>other_num;
}

bool HighPrecisionNumber::operator==(HighPrecisionNumber &other)
{
    if (this->length == other.length && this->sign == other.sign)
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
    other_number.setNumber(other, other >= 0);

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
