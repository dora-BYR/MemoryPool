//
// Created by 龙辉 on 16/4/9.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LMEMORYPOOL_LMEMORYBLOCK_H
#define LMEMORYPOOL_LMEMORYBLOCK_H

#include "define_l.h"
#include "LLinkList.h"

NS_LONG_BEGIN

    // 内存块 memory block
    class LMemoryBlock
    {
    public:
        LMemoryBlock();
        virtual ~LMemoryBlock();

        // 创建内存块 create memory block
        // @param nSize: the size of block
        static LMemoryBlock * create(void * pPoolStart, unsigned long nSize, unsigned long nOffset);

        // 释放使用权
        void release();

    public:
        // 创建内存块 create memory block
        // @param nSize: the size of block
        void setSize(unsigned long nSize);

        // 获取大小 get the size of block
        unsigned long getSize();

        // 获取块中剩余空闲可用的大小 get the free size in a block
        unsigned long getFreeSize();

        // 锁定 if need blocked
        void setIsFree(bool bIsFree);

        // 获取块是否可用 get if the block can be used
        bool getIsFree();

        // 获取空闲开始地址
        void * getFreeStart();

        // 设置当前偏移量
        void setOffset(unsigned long nOffset);
    private:
        // 初始化
        virtual bool initialize(void * pPoolStart);

    private:
        void * m_pPoolStart;
        unsigned long m_nSize;
        unsigned long m_nCurPos;
        bool m_bIsFree;
        unsigned long m_nOffset;
};

NS_LONG_END

#endif //LMEMORYPOOL_LMEMORYBLOCK_H
