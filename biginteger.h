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
    BigInteger(string& num); //通过给定数构造
    BigInteger(const string& num); //通过给定数构造
    BigInteger(INT8* num); //通过给定数构造
    BigInteger(const INT8* num); //通过给定数构造
    BigInteger(UINT32 num); //通过给定数构造
    BigInteger(const UINT32 num); //通过给定数构造

public:
    //返回内置成员
    string GetValue();
    //设定内置成员的值
    bool SetValue(char* num);
    bool SetValue(const char* num);
    bool SetValue(string& num);
    bool SetValue(const string& num);
    //查询精度值
    UINT32 GetPrecision();
    //设定精度值
    void SetPrecision(UINT32 pre);

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

    //大数阶乘，普通接口和专用接口
    string BigIntegerFact(const string& num);
    BigInteger BigIntegerFact();

    //重载运算符，双目
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

    //重载运算符，单目
    //重载=
    BigInteger operator= (const BigInteger& num);
    BigInteger operator= (const char* num);
    BigInteger operator= (const UINT8 num);
    BigInteger operator= (const UINT16 num);
    BigInteger operator= (const UINT32 num);
    BigInteger operator= (const unsigned long num);

    //重载前置++
    BigInteger& operator++ ();

private:
    bool IsNumber(const string& num);

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
    bool ChangeNumber(string& num1, string& num2, INT32& cnt);

private:
    string m_strNumber; //存放整数的字符串
    UINT32 m_uiPrecision; //小数的精度，多少位小数
};


//*************************************************************************//
//Rebuild dividing line 2014-12-04
//*************************************************************************//
//整数转字符串
string Int2Str(const UINT32 num);



#endif
