//
// Created by Administrator on 2019/2/16.
//

#ifndef NATIVE_GO_PERSON_H
#define NATIVE_GO_PERSON_H

/**
 * 面向对象编程，类的编写，.h的声明 以及方法的实现
 *
 *声明函数
 * */
class Person {

private:
    int age;
private:
    long number;

public:
    char name;

public:
    int getScore();//在类定义中没有定义成员函数的实现，需要在外部使用::操作符实现定义
public:
    char guessMyName() { //在定义之内定义了成员函数的实现
        return name;
    }

};


#endif //NATIVE_GO_PERSON_H
