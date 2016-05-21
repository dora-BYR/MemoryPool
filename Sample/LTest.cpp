//
// Created by 龙辉 on 16/4/12.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LTest.h"
#include "LMemoryPool.h"
#include <string>

void LTest::test_memPool() {
    printf("//////////////////////////////// 内存池功能测试 ////////////////////////////////\n");
    LTestObject1 * pObject = nullptr;
    auto pPool = NS_LONG::LMemoryPool::create(sizeof(LTestObject1), 200);
    pPool->getObject(pObject);
    pObject->setID(201604);
    printf("sizeof(LTestObject) = %d\n", sizeof(LTestObject1));
    printf("sizeof(* pObject) = %d\n", sizeof(* pObject));
    printf("1: object.m_nId = %d, object.m_nName = %s\n", pObject->getID(), pObject->getName());

    pPool->printStatus();
    pPool->recycleObject(pObject);
    pPool->printStatus();
    //
    LTestObject1 * pObject1 = nullptr;
    pPool->getObject(pObject1);
    pObject1->setID(201605);
    pObject1->setName("test object1");
    printf("2: object1.m_nId = %d, object1.m_nName = %s\n", pObject1->getID(), pObject1->getName());

    pPool->printStatus();

    // 增加新对象大小的内存区
    // add pool trunk with size of another new class object
    pPool->addPoolTrunk(sizeof(LTestObject2), 30);
    pPool->printStatus();
    LTestObject2 * pObject2 = nullptr;
    for (int i = 0; i < 22; ++i) {
        pPool->getObject(pObject2);
        pObject2->setID(i);
        pObject2->setSex("famale");
        pObject2->setName("test object2");
        printf("2: object2.m_nId = %d, object2.m_nName = %s, object2.m_szSex = %s\n", pObject2->getID(), pObject2->getName(), pObject2->getSex());
    }
    pPool->printStatus();

    // 内存池区自动智能扩展测试
    printf("Test for auto-reSize to pool (size: 30 -> 60)\n");
    for (int i = 0; i < 13; ++i) {
        pPool->getObject(pObject2);
        pObject2->setID(i);
        pObject2->setSex("famale");
        pObject2->setName("test object2");
        printf("2: object2.m_nId = %d, object2.m_nName = %s, object2.m_szSex = %s\n", pObject2->getID(), pObject2->getName(), pObject2->getSex());
    }
    pPool->printStatus();

    // 非对象方式获取和使用内存
    int nDataBufferLength = 512;
    int nBlockInitCount = 1;
    pPool->addPoolTrunk(nDataBufferLength, nBlockInitCount);
    pPool->printStatus();
    void * p = pPool->getBuffer(nDataBufferLength);
    const char * szName = "use memory as buffer";
    memcpy(p, szName, strlen(szName));
    printf("3: string = %s\n", p);
    pPool->printStatus();

    // 销毁内存池
    delete pPool;
    pPool = nullptr;
    printf("3: string = %s\n", p);
}

void LTest::test_performance() {
    printf("\n//////////////////////////////// 性能测试 ////////////////////////////////\n");
    unsigned long nCount = 1867284;
    unsigned long nInitCount = 2000000;

    // 系统方法创建对象
    clock_t tick1,tick2;
    tick1 = clock();
    // new
    for (int i = 0; i < nCount; ++i) {
        LTestObject * pObject = new LTestObject();
        // delete pObject1;
        // pObject1 = nullptr;
    }
    tick2 = clock();
    printf("create: pool unused = %.4f  ms\n", (tick2 - tick1)/1000.0);


    // 通过对象池创建对象
    auto pPool = NS_LONG::LMemoryPool::create(sizeof(LTestObject), nInitCount);

    tick1 = clock();
    for (int i = 0; i < nCount; ++i) {
        LTestObject * pObject = nullptr;
        pPool->getObject(pObject);
        //pObject->setID(i);
        //pObject->setName("longhui");
        //printf("pObject->m_nId = %d, pObject->m_nName = %s\n", pObject->getID(), pObject->getName());
        //pPool->recycleObject(pObject);
    }
    tick2 = clock();
    printf("create: pool used = %.4f ms\n", (tick2 - tick1)/1000.0);
    pPool->printStatus();
}