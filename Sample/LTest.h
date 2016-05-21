//
// Created by 龙辉 on 16/4/12.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LMEMORYPOOL_LTEST_H
#define LMEMORYPOOL_LTEST_H

#include "define_l.h"

typedef struct {
    int nCount;
    const char * name;
    int nPick;
    int nPick1;
    int nPick2;
    int nPick3;
    int nPick4;
    int nPick5;
    int nPick6;
    int nPick7;
    int nPick8;
    int nPick9;
    int nPick10;
    int nPick11;
    int nPick12;
    int nPick13;
    int nPick15;
    int nPick16;
    int nPick17;
    int nPick18;
} NS_P;

class LTestObject
{
public:
    LTestObject():
            m_nId(0),
            m_pName("class LTestObject")
    {}
    virtual ~LTestObject(){};

    void setID(int nId){ m_nId = nId;}
    int getID(){return m_nId;};
    void setName(const char * pName){ m_pName = pName;}
    const char * getName(){return m_pName;}

protected:
    int m_nId;
    const char * m_pName;
    NS_P m_mNsp;
    NS_P m_mNsp1;
    NS_P m_mNsp2;
    NS_P m_mNsp3;
    NS_P m_mNsp4;
    NS_P m_mNsp5;
    NS_P m_mNsp6;
    NS_P m_mNsp7;
    NS_P m_mNsp8;
    NS_P m_mNsp9;
    NS_P m_mNsp10;
    NS_P m_mNsp11;
    NS_P m_mNsp12;
    NS_P m_mNsp13;
    NS_P m_mNsp14;
    NS_P m_mNsp15;
    NS_P m_mNsp16;
    NS_P m_mNsp17;
    NS_P m_mNsp18;
    NS_P m_mNsp19;
    NS_P m_mNsp20;
    NS_P m_mNsp21;
    NS_P m_mNsp22;
};

class LTestObject1
{
public:
    LTestObject1():
            m_nId(0),
            m_pName("class LTestObject1")
    {}
    virtual ~LTestObject1(){};

    void setID(int nId){ m_nId = nId;}
    int getID(){return m_nId;};
    void setName(const char * pName){ m_pName = pName;}
    const char * getName(){return m_pName;}

protected:
    int m_nId;
    const char * m_pName;
    NS_P m_mNsp;
    NS_P m_mNsp1;
    NS_P m_mNsp2;
    NS_P m_mNsp3;
//    NS_P m_mNsp4;
//    NS_P m_mNsp5;
//    NS_P m_mNsp6;
//    NS_P m_mNsp7;
//    NS_P m_mNsp8;
//    NS_P m_mNsp9;
//    NS_P m_mNsp10;
//    NS_P m_mNsp11;
//    NS_P m_mNsp12;
//    NS_P m_mNsp13;
//    NS_P m_mNsp14;
//    NS_P m_mNsp15;
//    NS_P m_mNsp16;
//    NS_P m_mNsp17;
//    NS_P m_mNsp18;
//    NS_P m_mNsp19;
//    NS_P m_mNsp20;
//    NS_P m_mNsp21;
//    NS_P m_mNsp22;
};

class LTestObject2 : public LTestObject
{
public:
    LTestObject2(): LTestObject(), m_szSex("male")

    {}
    virtual ~LTestObject2(){};

    void setSex(const char * szSex){ m_szSex = szSex;}
    const char * getSex(){return m_szSex;};

protected:
    const char * m_szSex;
    static int m_sTag;
};

class LTest
{
public:
    LTest(){}
    virtual ~LTest();

    static void test_memPool();
    static void test_performance();
};

#endif //LMEMORYPOOL_LTEST_H
