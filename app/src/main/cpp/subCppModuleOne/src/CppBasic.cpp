//
// Created by Administrator on 2019/2/16.
//

#include "CppBasic.h"
#include <iostream>
#include "Person.h"

/**
 * 1、Cpp 基本数据类型
 *short int	2	-32,768 to 32,767
 *unsigned short int	2	0 to 65,535
 *unsigned int	4	0 to 4,294,967,295
 *int	4	-2,147,483,648 to 2,147,483,647
 *long int	4	-2,147,483,648 to 2,147,483,647
 *unsigned long int	4	0 to 4,294,967,295
 *long long int	8	-(2^63) to (2^63)-1
 *unsigned long long int	8	0 to 18,446,744,073,709,551,615
 *signed char	1	-128 to 127
 *unsigned char	1	0 to 255
 *float	4
 *double	8
 *long double	12
 *wchar_t	2 or 4	1 wide character
 *
 * 2、Cpp 基本输入输出
 * 输入流（input stream）:从设备（比如键盘）到内存中
 * 输出流（output stream）:从内存中输出到设备（比如屏幕）
 * 有关输入输出流的库：
 * iostream 标准的输入输出流库
 * iomanip 手持设备的输入输出流库
 * fstream 主要是文件流
 *
 * 在C++中cout cin 经常用来进行输入输出打印
 *
 * 3、C/C++中的预处理器机制
 *
 * 4、操作符
 * OPERATOR	DESCRIPTION	ASSOCIATIVITY
        ()	Parentheses (function call)	left-to-right
        []	Brackets (array subscript)
        .	Member selection via object name
        ->	Member selection via pointer
        ++/–	Postfix increment/decrement
        ++/–	Prefix increment/decrement	right-to-left
        +/-	Unary plus/minus
        !~	Logical negation/bitwise complement
        (type)	Cast (convert value to temporary value of type)
        *	Dereference
        &	Address (of operand)
        sizeof	Determine size in bytes on this implementation
        *,/,%	Multiplication/division/modulus	left-to-right
        +/-	Addition/subtraction	left-to-right
        << , >>	Bitwise shift left, Bitwise shift right	left-to-right
        < , <=	Relational less than/less than or equal to	left-to-right
        > , >=	Relational greater than/greater than or equal to	left-to-right
        == , !=	Relational is equal to/is not equal to	left-to-right
        &	Bitwise AND	left-to-right
        ^	Bitwise exclusive OR	left-to-right
        |	Bitwise inclusive OR	left-to-right
        &&	Logical AND	left-to-right
        ||	Logical OR	left-to-right
        ?:	Ternary conditional	right-to-left
        =	Assignment	right-to-left
        += , -=	Addition/subtraction assignment
        *= , /=	Multiplication/division assignment
        %= , &=	Modulus/bitwise AND assignment
        ^= , |=	Bitwise exclusive/inclusive OR assignment
        <>=	Bitwise shift left/right assignment
        ,	expression separator	left-to-right
 *
 * 4、指针
 * 为了访问指针变量的地址 需要用到&变量名
 * 声明指针，必须在变量前面加上*
 * 为了访问指针变量指向的值，需要在指针变量前面加上*
 *
 * 5 Cpp 中的引用
 * 需要在类型后面加上&
 * int x = 10;

  // ref is a reference to x.
  int& ref = x;

 *
 * 6 面向对象编程（OOP）
 *
 * 定义类成员函数的方法有两种：
 * 1、在类定义之内定义
 * 2、在类定义之外定义
 * */

int calculate() {
    short _short = 3;
    int _int = 4;
    long _long = 5L;
    char _char = 'a';
    signed long _signedlong = 100L;

    //调用标准库的方法需要使用std:: 或者在全局声明 using namespace std;
    std::cout << "在Console 中输出的文字"; //在Console中输出

    return 0;
}

/**
 * 如何确定内建数据类型的范围
 * */
void exceedValidRangeForBuildInDataType() {
    for (char i = 0; i <= 255; ++i) {
        std::cout << i;
    }
}

/**
 * Cpp 中的 预处理器 的机制
 * */
#define LIMIT = 5

void preproccor() {
    for (int i = 0; i < LIMIT; i++) {
        std::cout << i;
    }
}

void useOOP() {
    //声明
    Person person;


    person.name = 'a';
    //访问数据成员
    person.getScore();
}


