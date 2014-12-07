#include "BigInteger.h"
#include <sstream>
using namespace std;

BigInteger::BigInteger():
    m_uiDecimalPrecision(g_uiDecimalPrecision)
{
}

BigInteger::BigInteger(const string& num):
	m_uiDecimalPrecision(g_uiDecimalPrecision)
{
    if (IsNumber(num))
    {
        m_strNumber = num;
    }
    TrimUselessZeros(m_strNumber);
}

BigInteger::BigInteger(const INT8* num):
	m_uiDecimalPrecision(g_uiDecimalPrecision)
{
    if (IsNumber(num))
    {
        m_strNumber = string(num);
    }
    TrimUselessZeros(m_strNumber);
}

string BigInteger::GetValue()
{
    return m_strNumber;
}

bool BigInteger::SetValue(const INT8* num)
{
    if (!IsNumber(num))
    {
        return false;
    }

    m_strNumber = num;
    TrimUselessZeros(m_strNumber);

    return true;
}

bool BigInteger::SetValue(const string& num)
{
    if (!IsNumber(num))
    {
        return false;
    }

    m_strNumber = num;
    TrimUselessZeros(m_strNumber);

    return true;
}

bool BigInteger::SetValue(const UINT32 num)
{
    ostringstream oss;

    oss << num;

    m_strNumber = oss.str();
    oss.str("");

    return true;
}

UINT32 BigInteger::GetDecimalPrecision()
{
    return m_uiDecimalPrecision;
}

void BigInteger::SetDecimalPrecision(const UINT32 precision)
{
    m_uiDecimalPrecision = 
        (precision < g_uiDecimalPrecisionLimit) ? precision : g_uiDecimalPrecisionLimit;
}

bool BigInteger::IsNumber(const string& num)
{
	int size = num.size();
	if (0 >= size)
	{
		return false;
	}

	for (INT32 i = 0; i < size; ++i)
	{
		if (('0' > num[i]) || ('9' < num[i]) )
		{
			return false;
		}
	}

	return true;
}

bool BigInteger::IsNumber(const INT8* num)
{
    int size = strlen(num);
    if (0 >= size)
    {
        return false;
    }

    for (INT32 i = 0; i < size; ++i)
    {
        if (('0' > num[i]) || ('9' < num[i]) )
        {
            return false;
        }
    }

    return true;
}

bool BigInteger::ComputePartForAdd(string& num, INT32 x, INT32 pos)
{ 
    INT32 i, tmp, size;
    INT32 carry; //进位
    ostringstream oss;
    INT8 ch;

    size = num.size();
    if (!IsNumber(num) || ((size - 1) < pos) || (0 > pos))
    {
        return false;
    }

    //特殊情况优先处理
    if (0 == x)
    {
        return true;
    }

    carry = 0;
    for (i = pos; i >= 0; --i)
    {
        if (i == pos)
        {
            tmp = (num[i] - '0') + x; // 只指定位上需要加上数x
        }
        else
        {
            tmp = (num[i] - '0') + carry;
        }
        //carry用完立即置0
        carry = 0;

        if (10 <= tmp)
        {
            num[i] = (tmp % 10) + '0';
            carry = tmp / 10;
        }
        else
        {
            num[i] = tmp + '0';
        }

        //考虑最后加完还有进位的情况
        if ((0 == i) && (0 < carry))
        {
            while(0 < carry)
            {
                ch = carry % 10 + '0';
                oss << ch;
                num.insert(0, oss.str());
                carry /= 10;
                oss.str("");
            }
        }

        if (0 >= carry) //无进位，则不用继续加法
        {
            break;
        }
    }

    return true;
}

bool BigInteger::ComputePartForMinus(string& num, INT32 x, INT32 pos)
{
    INT32 i, tmp, size;
    INT32 carry; //借位
    ostringstream oss;

    size = num.size();
    if (!IsNumber(num) || ((size - 1) < pos) || (0 > pos))
    {
        return false;
    }

    //特殊情况优先处理
    if (0 == x)
    {
        return true;
    }

    carry = 0;
    for (i = pos; i >= 0; --i)
    {
        if (i == pos)
        {
            tmp = (num[i] - '0') - x;
        }
        else
        {
            tmp = (num[i] - '0') - carry;
        }
        //carry用完立即置0
        carry = 0;

        if (tmp < 0) //说明不够减，则要借位
        {
            num[i] = tmp + 10 + '0';
            carry = 1;
        }
        else
        {
            num[i] = tmp + '0';
        }

        //减到后面不够减，则不计算了
        if ((0 == i) && (0 < carry))
        {
            return false;
        }

        if (0 >= carry)
        {
            break;
        }
    }

    return true;
}

bool BigInteger::ComputePartForMulti(string& num, INT32 x)
{
    if (!IsNumber(num))
    {
        return false;
    }

    //特殊情况优先处理
    if (0 == x)
    {
        num.clear();
        num.insert(0, "0");
        return true;
    }
    if (1 == x)
    {
        return true;
    }

    INT32 i, size, tmp;
    INT32 carry; //进位
    ostringstream oss;
    INT8 ch;

    size = num.size();
    carry = 0;
    for (i = size - 1; i >= 0; --i)
    {
        tmp = (num[i] - '0') * x + carry;
        //carry用完了立即置0
        carry = 0;

        if (10 <= tmp)
        {
            num[i] = (tmp % 10) + '0';
            carry = tmp / 10;
        }
        else
        {
            num[i] = tmp + '0';
        }

        if ((0 == i) && (0 < carry))
        {
            while(0 < carry)
            {
                ch = carry % 10 + '0';
                oss << ch;
                num.insert(0, oss.str());
                carry /= 10;
                oss.str("");
            }
        }
    }

    return true;
}

string BigInteger::BigIntegerAdd(const string& num1, const string& num2)
{
	string result;

	if (!IsNumber(num1) || !IsNumber(num2))
	{
		return result;
	}

    //特殊情况优先处理
    if ("0" == num1)
    {
        return num2;
    }
    if ("0" == num2)
    {
        return num1;
    }

	INT32 i, size1, size2;

    result = num1;
    size1 = num1.size();
    size2 = num2.size();

    //被加数位数不够，则先补成和加数一样长
    if (size1 < size2)
    {
        for (i = 1; i <= (size2 - size1); ++i)
        {
            result.insert(0, "0");
        }
    }
		
    //加数位数不够时
    if (size1 > size2)
    {
        for (i = size2 - 1; i >= 0; --i)
        {
            if (!ComputePartForAdd(result, num2[i] - '0', (size1 - 1) - (size2 - 1 - i)))
            {
                result.clear();
                break;
            }
        }
    }
    else //两数位数一样长
    {
        for (i = size2 - 1; i >= 0; --i)
        {
            if (!ComputePartForAdd(result, num2[i] - '0', i))
            {
                result.clear();
                break;
            }
        }
    }

    return result;
}

string BigInteger::BigIntegerMinus(const string& num1, const string& num2)
{
    string result;

    if (!IsNumber(num1) || !IsNumber(num2))
    {
        return result;
    }

    //特殊情况优先处理
    if ("0" == num1)
    {
        result = num2;
        result.insert(0, "-");

        return result;
    }
    if ("0" == num2)
    {
        return num1;
    }
    if (num1 == num2)
    {
        result.insert(0, "0");
        return result;
    }

    INT32 i, size1, size2;
    string tmp;
    bool isNegative = false;

    size1 = num1.size();
    size2 = num2.size();

    //被减数位数不够，则用减数减去被减数，最后带上负号
    if (size1 < size2)
    {
        isNegative = true;
        tmp = num1;
        for (i = 1; i <= (size2 - size1); ++i)
        {
            tmp.insert(0, "0");
        }

        result = num2;
        size1 = tmp.size();
    } 
    //减数位数不够，先补齐
    else if (size1 > size2)
    {
        tmp = num2;
        for (i = 1; i <= (size1 - size2); ++i)
        {
            tmp.insert(0, "0");
        }

        result = num1;
        size2 = tmp.size();
    }
    else
    {
        result = num1;
        tmp = num2;

        //位数相等时，有可能被减数小于减数
        for (i = 0; i < size1; ++i)
        {
            if (num1[i] > num2[i])
            {
                break;
            }

            if (num1[i] < num2[i]) //被减数小于减数，则将两者交换
            {
                isNegative = true;
                result = num2;
                tmp = num1;
                break;
            }
        }
    }
    
    for (i = size2 - 1; i >= 0; --i)
    {
        if (!ComputePartForMinus(result, tmp[i] - '0', i))
        {
            result.clear();
            break;
        }
    }

    TrimUselessZeros(result);

    if (isNegative)
    {
        result.insert(0, "-");
    }

    return result;
}

string BigInteger::BigIntegerMultiply(const string& num1, const string& num2)
{
    string result;

    if (!IsNumber(num1) || !IsNumber(num2))
    {
        return result;
    }

    //特殊情况优先处理
    if (("1" == num1))
    {
        return num2;
    }
    if ( ("1" == num2))
    {
        return num2;
    }
    if (("0" == num1) || ("0" == num2))
    {
        result.insert(0, "0");
        return result;
    }

    INT32 i, size;
    INT32 move; //要移动的位数
    string tmp;

    size = num2.size(); 
    result.insert(0, "0");
    for (i = size - 1; i >= 0; --i)
    {
        tmp = num1;
        move = size - 1 - i;
        MoveNumber(tmp, move, LEFT); //比如 23*5000，则先把23移位成23000，再乘以5
        ComputePartForMulti(tmp, num2[i] - '0');
        result = BigIntegerAdd(result, tmp);
    }

    return result;
}

string BigInteger::BigIntegerDivide(const string& num1, const string& num2)
{
    BigInteger result;

    if (!IsNumber(num1) || !IsNumber(num2))
    {
        return result.m_strNumber;
    }

    //用0作除数，则不计算了
    if ("0" == num2)
    {
        return result.m_strNumber;
    }
    //被除数是0，则结果为0
    if ("0" == num1)
    {
        result.SetValue("0");
        return result.m_strNumber;
    }
    if ("1" == num2)
    {
        return num1;
    }
    if (num1 == num2)
    {
        result.SetValue("1");
        return result.m_strNumber;
    }

    BigInteger bi1(num1);
    BigInteger bi2(num2);
    BigInteger tmp;
    BigInteger decimal; //临时存放小数的一位
    BigInteger tmpResult;//结果的整数部分

    UINT32 cnt; //计数小数位数，以计算到需要的精度
    INT32  shift; //减数被扩大了多少倍

    cnt = 0;
    decimal.m_strNumber.clear(); //清空初始值
    result.SetValue("0");
    tmpResult.SetValue("0");
    shift = 0;
    while (cnt <= m_uiDecimalPrecision)
    {
        if (0 == cnt) //开始进行小数部分计算时，不执行这一步，即无需扩大减数
        {
            ChangeNumber(bi1.m_strNumber, bi2.m_strNumber, shift);
        }
        
        tmp = bi1 - bi2;

        if (bi1 < bi2) //说明不够除了
        {
            //1. 准备进行小数部分计算时，bi2必定恢复了原始值
            //2. tmpResult为0说明整数部分已计算完毕，开始计算小数部分了
            if ((num2 == bi2.m_strNumber) && (BigInteger("0") == tmpResult))
            {
                if (0 != decimal.m_strNumber.size())
                {
                    result.m_strNumber.insert(result.m_strNumber.size(), decimal.m_strNumber);
                }
                decimal.m_strNumber.clear(); //清空重新计算某个小数的值
                decimal.m_strNumber.insert(0, "0");

                if (0 == cnt) //放小数点的时机是除数恢复了正常并且cnt有计数
                {
                    result.m_strNumber.insert(result.m_strNumber.size(), ".");
                }

                ++cnt;

                if ("0" != bi1.m_strNumber) //不等于0才进行扩大，等于0则不处理，保持为0即可
                {
                    MoveNumber(bi1.m_strNumber, 1, LEFT); //令被减数变大10倍，再尝试去减减数
                }
            }
            else
            {
                //若有扩大过除数，则此时需扩大商值
                MoveNumber(tmpResult.m_strNumber, shift, LEFT);
                result = result + tmpResult;
                tmpResult.SetValue("0");//整数部分已计算完，将该变量清零

                if (0 != shift)
                {
                    //缩小扩大过的除数
                    MoveNumber(bi2.m_strNumber, 1, RIGHT);
                    --shift;
                }
            }
        }
        else
        {
            if (0 == cnt) //还没开始计算到小数时候的计算
            {
                ++tmpResult;
                bi1 = tmp;
            }
            else
            {
                ++decimal;
                bi1 = tmp;
            }
        }
    }

    return result.m_strNumber;
}

string BigInteger::BigIntegerMod(const string& num1, const string& num2)
{
    string result;

    if (!IsNumber(num1) || !IsNumber(num2))
    {
        return result;
    }

    //用0作除数，则不计算了
    if ("0" == num2)
    {
        return result;
    }

    //被除数是0则结果为0，被1整除返回0
    if (("0" == num1) || ("1" == num2))
    {
        result.insert(0, "0");
        return result;
    }

    //位数比除数少，则余数是自己
    if (num1.size() < num2.size())
    {
        return num1;
    }

    BigInteger bi1(num1);
    BigInteger bi2(num2);
    BigInteger tmp;
    INT32 shift;

    shift = 0;
    while(1)
    {
        ChangeNumber(bi1.m_strNumber, bi2.m_strNumber, shift);
        tmp = bi1 - bi2;

        if (bi1 < bi2)
        {
            if (num2 == bi2.m_strNumber)
            {
                result = bi1.m_strNumber;
                break;
            }

            if (0 != shift)
            {
                //缩小扩大过的除数
                MoveNumber(bi2.m_strNumber, 1, RIGHT);
                --shift;
            }
        }
        else
        {
            bi1 = tmp;
        }
    }

    return result;
}

string BigInteger::BigIntegerFact(const string& num)
{
    BigInteger result;

    if (!IsNumber(num))
    {
        return result.m_strNumber;
    }

    //要计算的阶乘太大，则不计算
    if (g_uiFactorialLimit < GetValue(0))
    {
        return result.m_strNumber;
    }

    BigInteger tmp(num);
    BigInteger i("2");
    result.SetValue("1");
    for (; i <= tmp; ++i)
    {
        result.m_strNumber = BigIntegerMultiply(result.m_strNumber, i.m_strNumber);
    }

    return result.m_strNumber;
}

BigInteger BigInteger::BigIntegerFact()
{
    m_strNumber = BigIntegerFact(m_strNumber);

    return *this;
}

BigInteger operator+ (const BigInteger& num1, const BigInteger& num2)
{
    BigInteger result;

    result.m_strNumber = result.BigIntegerAdd(num1.m_strNumber, num2.m_strNumber);

    return result;
}

BigInteger operator- (const BigInteger& num1, const BigInteger& num2)
{
    BigInteger result;

    result.m_strNumber = result.BigIntegerMinus(num1.m_strNumber, num2.m_strNumber);

    return result;
}

BigInteger operator* (const BigInteger& num1, const BigInteger& num2)
{
    BigInteger result;

    result.m_strNumber = result.BigIntegerMultiply(num1.m_strNumber, num2.m_strNumber);

    return result;
}

BigInteger operator/ (const BigInteger& num1, const BigInteger& num2)
{
    BigInteger result;

    result.m_strNumber = result.BigIntegerDivide(num1.m_strNumber, num2.m_strNumber);

    return result;
}

BigInteger operator% (const BigInteger& num1, const BigInteger& num2)
{
    BigInteger result;

    result.m_strNumber = result.BigIntegerMod(num1.m_strNumber, num2.m_strNumber);

    return result;
}

bool operator< (const BigInteger& num1, const BigInteger& num2)
{
    int size1, size2;

    size1 = num1.m_strNumber.size();
    size2 = num2.m_strNumber.size();

    //<运算时，相等视为false
    if (num1 == num2)
    {
        return false;
    }

    if (size1 < size2)
    {
        return true;
    }

    else if (size1 > size2)
    {
        return false;
    }
    else
    {
        for (int i = 0 ; i < size1; ++i)
        {
            if (num1.m_strNumber[i] < num2.m_strNumber[i])
            {
                return true;
            }
            else if (num1.m_strNumber[i] > num2.m_strNumber[i])
            {
                return false;
            }
        }
    }

    return false;
}
//friend bool operator< (const BigInteger& num1, const INT32 num2);
//friend bool operator< (const INT32 num2, const BigInteger& num1);

bool operator<= (const BigInteger& num1, const BigInteger& num2)
{
    return ((num1 < num2) || (num1 == num2));
}

bool operator> (const BigInteger& num1, const BigInteger& num2)
{
    //>运算时，相等视为false
    if (num1 == num2)
    {
        return false;
    }

    return !(num1 < num2);
}

bool operator== (const BigInteger& num1, const BigInteger& num2)
{
    if (num1.m_strNumber == num2.m_strNumber)
    {
        return true;
    }

    return false;
}
//friend bool operator== (const BigInteger& num1, const INT32 num2);
//friend bool operator== (const INT32 num2, const BigInteger& num1);

bool operator!= (const BigInteger& num1, const BigInteger& num2)
{
    return !(num1 == num2);
}

BigInteger BigInteger::operator= (const BigInteger& num)
{
    m_strNumber = num.m_strNumber;

    return *this;
}

BigInteger BigInteger::operator= (const string& num)
{
    if (!IsNumber(num))
    {
        return *this; //参数不合法，构造空值
    }

    m_strNumber = num;
    TrimUselessZeros(m_strNumber);

    return *this;
}

BigInteger BigInteger::operator= (const INT8* num)
{
    if (!IsNumber(num))
    {
        return *this; //参数不合法，构造空值
    }

    m_strNumber = num;
    TrimUselessZeros(m_strNumber);

    return *this;
}

BigInteger BigInteger::operator= (const UINT32 num)
{
    m_strNumber = Int2Str(num);

    return *this;
}

BigInteger& BigInteger::operator++ ()
{
    string strOne("1");

    m_strNumber = BigIntegerAdd(m_strNumber, strOne);

    return *this;
}

bool BigInteger::MoveNumber(string& num, INT32 count, DIRECTION dir)
{
    if (!IsNumber(num))
    {
        return false;
    }
    
    int i;

    for (i = 1; i <= count; ++i)
    {
        if (LEFT == dir)
        {
            num.insert(num.size(), "0");
        }
        else
        {
            num = num.substr(0, num.size() - i);
        }
    }
    
    return true;
}

bool BigInteger::ChangeNumber(string& num1, string& num2, INT32& cnt)
{
    if (!IsNumber(num1) || !IsNumber(num2))
    {
        return false;
    }

    INT32 i, size1, size2, tmp;

    size1 = num1.size();
    size2 = num2.size();

    if (size1 <= size2) //这种情况不改变num2
    {
        return false;
    }

    tmp = cnt;//保存旧值
    cnt = size1 - size2;   //例如 987 和 2，直接把2变为200
    if (num1[0] < num2[0]) //例如 189 和 3，则只把3变为30
    {
        --cnt;
    }

    if (0 == cnt) //这种情况看起来无需移动，恢复cnt的原值
    {
        cnt = tmp;
        return false;
    }

    for (i = 0; i < cnt; ++i)
    {
        num2.insert(num2.size(), "0");
    }

    return true;
}

//*************************************************************************//
//Rebuild dividing line 2014-12-04
//*************************************************************************//
string Int2Str(const UINT32 num)
{
	ostringstream oss;

	oss << num;

	return oss.str();
}

BigInteger::BigInteger(const UINT32 num):
	m_uiDecimalPrecision(g_uiDecimalPrecision)
{
	m_strNumber = Int2Str(num);
}

bool BigInteger::IsAssigned()
{
    return !m_strNumber.empty();
}

BigInteger::BigInteger(const BigInteger& bigint)
{
    m_strNumber = bigint.m_strNumber;
    m_uiDecimalPrecision = bigint.m_uiDecimalPrecision;
}

UINT32 BigInteger::Length()
{
    return m_strNumber.size();
}

BigInteger BigInteger::operator++ (INT32)
{
    BigInteger old = *this;

    string strOne("1");
    m_strNumber = BigIntegerAdd(m_strNumber, strOne);

    return old;
}

bool BigInteger::IsZero()
{
    return ("0" == m_strNumber);
}

bool BigInteger::IsNegative()
{
    return ('-' == m_strNumber[0]);
}

void BigInteger::TrimUselessZeros(string& num)
{
    //去掉前面的0，例如 “0023445”
    INT32 i;
    INT32 size = num.size();

    for (i = 0; i < size; ++i)
    {
        if ('0' != num[i])
        {
            break;
        }
    }

    if (size == i) //说明全为0
    {
        num.clear();
        num.insert(0, "0");
    }
    else
    {
        num = num.substr(i); //去掉高位无用的0
    }
}

BigInteger& BigInteger::operator-- ()
{
    string strOne("1");

    m_strNumber = BigIntegerMinus(m_strNumber, strOne);

    return *this;
}

BigInteger BigInteger::operator-- (INT32)
{
    BigInteger old = *this;

    string strOne("1");
    m_strNumber = BigIntegerMinus(m_strNumber, strOne);

    return old;
}

UINT32 BigInteger::GetValue(UINT32)
{
    UINT32 result = 0;
    if (g_strMaxInt >= m_strNumber)
    {
        result = atol(m_strNumber.c_str());
    }

    return result;
}








