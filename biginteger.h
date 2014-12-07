/********************************************
File:BigInteger.h
Author:guiwanli
Date:2014-09-30
Description: 该大整数内部用一个string类型变量存储字符串
History:
1. Create 2014-09-30
2. Rebuild 2014-12-04
********************************************/

#ifndef BIGINTEGE_H
#define BIGINTEGE_H

#include <string>
using namespace std;

typedef unsigned int   UINT32;
typedef unsigned short UINT16;
typedef unsigned char  UINT8;
typedef int   INT32;
typedef short INT16;
typedef char  INT8;

//默认保留4位小数
const UINT32 g_uiDecimalPrecision = 4;
//最多允许保留100位小数
const UINT32 g_uiDecimalPrecisionLimit = 100;

enum DIRECTION
{
    LEFT, //向左
    RIGHT //向右
};

//无符号大数类
class BigInteger
{
public:
    BigInteger(); //默认值为空
    BigInteger(const string& num); //通过给定数构造
    BigInteger(const INT8* num); //通过给定数构造
	BigInteger(const UINT32 num); //通过给定数构造

    BigInteger(const BigInteger& bigint); //拷贝构造

public:
    //返回原始字符串值
    string GetValue();
    //设定原始字符串的值
    bool SetValue(const string& num);
    bool SetValue(const INT8* num);
    bool SetValue(const UINT32 num);

    //查询小数精度值
    UINT32 GetDecimalPrecision();
    //设定小数精度值
    void SetDecimalPrecision(const UINT32 precision);

    //查询是否已赋值,true-已赋值,false-未赋值
    bool IsAssigned();

    //得到数的位数
    UINT32 Length();

    //运算类函数
    //1. 重载运算符，单目
    //重载=
    BigInteger operator= (const BigInteger& num);
    BigInteger operator= (const string& num);
    BigInteger operator= (const INT8* num);
    BigInteger operator= (const UINT32 num);

    //重载前置++,--
    BigInteger& operator++ ();
    BigInteger& operator-- ();

    //重载后置++,--
    BigInteger operator++ (INT32);
    BigInteger operator-- (INT32);

    //2. 重载运算符，双目
    //重载常规运算符，用于该对象的运算
    friend BigInteger operator+ (const BigInteger& num1, const BigInteger& num2);
    friend BigInteger operator- (const BigInteger& num1, const BigInteger& num2);
    friend BigInteger operator* (const BigInteger& num1, const BigInteger& num2);
    friend BigInteger operator/ (const BigInteger& num1, const BigInteger& num2);
    friend BigInteger operator% (const BigInteger& num1, const BigInteger& num2);

    friend bool operator< (const BigInteger& num1, const BigInteger& num2);
    //friend bool operator< (const BigInteger& num1, const INT32 num2);
    //friend bool operator< (const INT32 num2, const BigInteger& num1);
    friend bool operator<= (const BigInteger& num1, const BigInteger& num2);

    friend bool operator> (const BigInteger& num1, const BigInteger& num2);

    friend bool operator== (const BigInteger& num1, const BigInteger& num2);
    //friend bool operator== (const BigInteger& num1, const INT32 num2);
    //friend bool operator== (const INT32 num2, const BigInteger& num1);

    friend bool operator!= (const BigInteger& num1, const BigInteger& num2);

    //3. 常用有用函数
    //大数阶乘
    BigInteger BigIntegerFact();

private:
    //普通运算接口实现
    //大数加法，num1 + num2
    string BigIntegerAdd(const string& num1, const string& num2);
    //大数减法，num1 - num2
    string BigIntegerMinus(const string& num1, const string& num2);
    //大数乘法，num1 * num2
    string BigIntegerMultiply(const string& num1, const string& num2);
    //大数除法，num1 / num2
    string BigIntegerDivide(const string& num1, const string& num2);
    //大数求模，num1 % num2
    string BigIntegerMod(const string& num1, const string& num2);

    //计算阶乘
    string BigIntegerFact(const string& num);

    bool IsNumber(const string& num);
    bool IsNumber(const INT8* num);

    //在数num上的第pos位置加上x，"12345678987654321"，个位数索引最大
    bool ComputePartForAdd(string& num, INT32 x, INT32 pos);
    //在数num上的第pos位置减去x，"12345678987654321"，个位数索引最大
    bool ComputePartForMinus(string& num, INT32 x, INT32 pos);
    //在数num上乘上x，"12345678987654321"，个位数索引最大
    bool ComputePartForMulti(string& num, INT32 x);

    //将数num进行左移右移
    bool MoveNumber(string& num, INT32 count, DIRECTION dir);

    //把num2进行调整使接近num1的位数，但值仍小于num1，cnt是num2左移了多少位
    //例如 156和2，把2先调为200，但又要保证不大于num1，则调整为20
    //这是为了除法快速作差
    bool ChangeNumber(string& num1, string& num2, INT32 cnt);

    //该数是否是0
    bool IsZero();

    //是否是负数
    bool IsNegative();

    //去掉高位无用0
    void TrimUselessZeros(string& num);

private:
    string m_strNumber; //存放整数的字符串
    UINT32 m_uiDecimalPrecision; //小数的精度，保留多少位小数
};


/*************************************************************************
Rebuild dividing line 2014-12-04
*************************************************************************/
//整数转字符串
string Int2Str(const UINT32 num);



#endif
