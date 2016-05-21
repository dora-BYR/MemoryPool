//
// Created by 龙辉 on 16/4/9.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LMemoryBlock.h"

#include <string>

NS_LONG_BEGIN

    LMemoryBlock::LMemoryBlock():
            m_pPoolStart(nullptr),
        m_nSize(NULL),
        m_nCurPos(NULL),
        m_bIsFree(true)
    {

    }

    LMemoryBlock::~LMemoryBlock()
    {

    }

    bool LMemoryBlock::initialize(void *pPool)
    {
        m_pPoolStart = pPool;
        return true;
    }

    LMemoryBlock* LMemoryBlock::create(void * pPool, unsigned long nSize, unsigned long nOffset)
    {
        auto memBlock = new LMemoryBlock();
        memBlock->setSize(nSize);
        memBlock->setOffset(nOffset);
        memBlock->initialize(pPool);
        return memBlock;
    }

    void LMemoryBlock::setSize(unsigned long nSize)
    {
        m_nSize = nSize;
    }

    void LMemoryBlock::setIsFree(bool bIsFree)
    {
        m_bIsFree = bIsFree;
    }

    bool LMemoryBlock::getIsFree()
    {
        return m_bIsFree;
    }

    unsigned long LMemoryBlock::getFreeSize()
    {
        unsigned long nFreeSize = m_nSize - m_nCurPos;
        return nFreeSize;
    }

    void LMemoryBlock::release()
    {
        if (m_pPoolStart)
        {
            memset(m_pPoolStart, NULL, m_nSize);
        }
        m_nCurPos = NULL;
        m_bIsFree = true;
    }

    void* LMemoryBlock::getFreeStart()
    {
        return (void *)((unsigned char *)m_pPoolStart + m_nOffset);
    }

    void LMemoryBlock::setOffset(unsigned long nOffset) {
       m_nOffset  = nOffset;
    }

NS_LONG_END