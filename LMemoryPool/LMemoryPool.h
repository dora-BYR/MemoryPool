//
// Created by 龙辉 on 16/4/9.
// Copyright (c) 2016 yuemo studio. All rights reserved.
// buptlong@126.com
//

/*
    Copyright (C) 2016 longhui all right reserved.

　　 Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is furnished to do
    so, subject to the following conditions:
　　
　　 The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
// 内存/对象池功能实现
// 1. 支持固定块大小的内存池/对象池
// 2. 支持不同块大小的内存池/对象池
// 3. 支持内存池/对象池大小动态变化, 内存池容量自动智能扩展大小(消耗少量性能)
// 4. 支持对象回收
//
// 对象池性能测试结果
// 1. 单个对象的大小越大, 内存池与系统性能对比, 优势越大
// 2. 需要分配生成的对象数量越多, 内存池与系统性能对比, 优势越大
// 3. 内存池初始化趋于足够使用时, 也即减少内存池容量中途自动扩展次数, 可以明显提高内存池性能
// 4. 减少或不用对象回收利用机制, 可以极大提高内存池性能
//


#ifndef LMEMORYPOOL_LMEMORYPOOL_H
#define LMEMORYPOOL_LMEMORYPOOL_H

#include "define_l.h"
#include "LMemoryBlock.h"

#include "math.h"
#include <vector>
#include <unordered_map>
#include <stdlib.h>

#define SIZE_RESERVED_SPACE     (sizeof(unsigned long))             // 块内部预留字段的缺省大小
#define COUNT_OBJECT_INIT       (1000)          // 默认初始化时预分配的缺省对象数量

NS_LONG_BEGIN
    // 双向链表 double-linklist
    template <typename T>
    class LLinkList;

    // 内存块 memory block
    class LMemoryBlock;

    // 存储每种大小块的内存区首地址,当前区长度,初始化数量和块链表等信息
    typedef struct{
        unsigned char * pStartPos;
        unsigned int nSize;
        unsigned int nCount;
        unsigned int nBlockSize;
        LLinkList<LMemoryBlock *> blockList;
    } MemTrunck;

    // 内存池 memory pool
    //template <typename T>
    class LMemoryPool
    {
    public:
        LMemoryPool();
        virtual ~LMemoryPool();

        // 创建内存池 create memoryPool.
        // 每个内存block会额外预留四个字节,保存其所属链表节点的内存地址,以便后续处理,例如回收.
        // 避免查找链表的时间消耗.但这样会占用一定的内存空间.
        // @param nSize: 池大小 the size of pool, 池数量 the count of pool
        // @param nBlockSize: 池中单位内存块的大小 the block size of the pool.
        // @param nCount: count of block when initialize, this will auto increase when pool-area is full.
        static LMemoryPool * create(unsigned long nxBlockSize,                // 块大小最大值
                                    unsigned int nCount = COUNT_OBJECT_INIT,    // 预分配的块数量
                                    bool bIsOptimize = false);

        // 追加块大小为nSize的内存区.如果存在nSize大笑的内存区,则忽略.
        // append a mempool trunk by nSize (object size)
        // @param nSize: the size of object
        // @param nCount: the default count of object to be created when initialize
        void addPoolTrunk(unsigned int nSize, unsigned nCount = COUNT_OBJECT_INIT);


        // 从池中获取对象
        // @param pObject: pointer-ref of the object to be created
        template <typename T>
        T * getObject(T * &pObject = NULL) {
            auto pMemTrunk = m_BlockSizeHashMap[sizeof(T) + SIZE_RESERVED_SPACE];
            pMemTrunk = (!!pMemTrunk) ? pMemTrunk : reAllocate(sizeof(T) + SIZE_RESERVED_SPACE, COUNT_OBJECT_INIT);
            if (!pMemTrunk) {
                return nullptr;
            }
            LLinkNode<LMemoryBlock *> * pNode = (LLinkNode<LMemoryBlock *> *)pMemTrunk->blockList.getFront();
            while (!pNode || !pNode->getValue()->getIsFree()) {
                reAllocate(sizeof(T) + SIZE_RESERVED_SPACE, 2 * pMemTrunk->nCount);
                pNode = (LLinkNode<LMemoryBlock *> *)pMemTrunk->blockList.getFront();
            }
            LMemoryBlock * pBlock = pMemTrunk->blockList.getFront()->getValue();
            if (pBlock) {
                if (pBlock->getIsFree() && pBlock->getFreeSize() >= sizeof(T)) {
                    pBlock->setIsFree(false);
                    // pMemTrunk->blockList.removeFront();
                    pMemTrunk->blockList.moveToTail(pNode);
                    pObject = (T *)(pBlock->getFreeStart());
                    // pObject->T::T();
                    return pObject;
                }
            }
            return nullptr;
        }

        // 回收已分配的内存对象. recycle object memory
        // 调用回收过程会导致一定的性能下降. this may leads to low performance
        // @param pObject: the object pointer to be free
        template <typename T>
        void recycleObject(T* pObject) {
            if (!pObject){
                return;
            }
            auto pMemTrunk = m_BlockSizeHashMap[sizeof(T) + SIZE_RESERVED_SPACE];
            if (!pMemTrunk) {
                return;
            }
            pObject->T::~T();
            unsigned long nSize = sizeof(T);
            memset((void *)pObject, NULL, nSize);
            auto pNode = *((LLinkNode<LMemoryBlock *> **)((unsigned char *)pObject + nSize));
            auto pBlock = pNode->getValue();
            pBlock->setIsFree(true);
            // pMemTrunk->blockList.push_back(pNode);
            pMemTrunk->blockList.moveToHead(pNode);
        }

        // 非对象方式获取一段内存使用
        void * getBuffer(unsigned long nSize);

        // 回收一段非对象方式获取的内存, 需要指定原分配长度
        void recycleBuffer(void * pStart, unsigned long nSize);

        // 打印内存整体使用情况
        void printStatus();

    protected:
        // 初始化/重新分配大小的池 malloc/re-allocate
        // @param nBlockSize: 内部计算过后的块大小, 包含了额外的预留字段部分.
        // @param nCount: 初始化预分配的数量
        MemTrunck * reAllocate(unsigned nBlockSize, unsigned int nCount);

        // 初始化
        virtual bool initialize(unsigned long nBlockSize,
                                unsigned int nCount = COUNT_OBJECT_INIT,
                                bool bIsOptimize = false);

        // 生成块大小序列数组 generate the block size-list
        // 这里设为2的幂 size value will be 2^x
        void generateBlockSizeList(unsigned long nMaxBlockSize, unsigned long nMinBlockSize = 0, bool bIsOptimize = false);

    protected:
        // 块大小索引哈希表
        std::unordered_map<unsigned int, MemTrunck *> m_BlockSizeHashMap;
        // 存储块大小值
        std::vector<unsigned long> m_BlockSizeArray;
    };

NS_LONG_END

#endif //LMEMORYPOOL_LMEMORYPOOL_H