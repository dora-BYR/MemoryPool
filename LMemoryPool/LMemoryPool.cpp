//
// Created by 龙辉 on 16/4/9.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LMemoryPool.h"

NS_LONG_BEGIN

    LMemoryPool::LMemoryPool() {
    }

    LMemoryPool::~LMemoryPool() {
        for (auto iter : m_BlockSizeHashMap) {
            MemTrunck * p = iter.second;
            if (p && p->pStartPos) {
                //memset(p->pStartPos, NULL, p->nSize);
                free((void *)(p->pStartPos));
            }
        }
    }

    LMemoryPool * LMemoryPool::create(unsigned long nBlockSize,
            unsigned int nCount,
            bool bIsOptimize) {
        auto memPool = new LMemoryPool();
        memPool->initialize(nBlockSize, nCount, bIsOptimize);
        return memPool;
    }

    bool LMemoryPool::initialize(unsigned long nBlockSize,
            unsigned int nCount,
            bool bIsOptimize)
    {
        generateBlockSizeList(nBlockSize + SIZE_RESERVED_SPACE, nBlockSize + SIZE_RESERVED_SPACE);

        unsigned int nArraySize = m_BlockSizeArray.size();
        for (int i = 0; i < nArraySize; ++i)
        {
            reAllocate(m_BlockSizeArray[i], nCount);
        }
        return true;
    }

    void LMemoryPool::addPoolTrunk(unsigned int nSize, unsigned nCount) {
        if (m_BlockSizeHashMap[nSize + SIZE_RESERVED_SPACE]) {
            return;
        }
        m_BlockSizeArray.push_back(nSize + SIZE_RESERVED_SPACE);
        reAllocate(nSize + SIZE_RESERVED_SPACE, nCount);
    }

    void LMemoryPool::printStatus() {
        printf("-------------------------- memory pool status -------------------------\n");
        for (auto iter : m_BlockSizeHashMap) {
            int nUnusedCount = 0;
            iter.second->blockList.iteratorList([&](LLinkNode<LMemoryBlock *> * pNode){
                if (pNode && pNode->getValue() && pNode->getValue()->getIsFree()) {
                    ++nUnusedCount;
                }
            });
            printf("| block(object) size = %d; blocks used/unused/total = %d/%d/%d\n", iter.first, iter.second->nCount - nUnusedCount, nUnusedCount, iter.second->nCount);
        }
        printf("-----------------------------------------------------------------------\n");

    }

    MemTrunck * LMemoryPool::reAllocate(unsigned nBlockSize, unsigned int nCount) {
        MemTrunck * pMemTrunck = nullptr;
        unsigned int nPoolSize = nBlockSize * nCount;
        if (nPoolSize > 0 && !m_BlockSizeArray.empty()){
            unsigned int reAllocSize = nPoolSize;
            bool isRealloc = false;
            if (!m_BlockSizeHashMap[nBlockSize]) {
                pMemTrunck = new MemTrunck();
                pMemTrunck->pStartPos = (unsigned char *)malloc(nPoolSize);
                memset(pMemTrunck->pStartPos, NULL, nPoolSize);
                pMemTrunck->nSize = nPoolSize;
                pMemTrunck->nCount = nCount;
                pMemTrunck->nBlockSize = nBlockSize;
                m_BlockSizeHashMap[nBlockSize] = pMemTrunck;
                //printf("allocate init size to %d, ", pMemTrunck->nSize);
            } else {
                // 按原来大小的两倍来动态追加申请内存. PS:有可能申请失败;
                pMemTrunck = m_BlockSizeHashMap[nBlockSize];
                reAllocSize = pMemTrunck->nSize;
                pMemTrunck->pStartPos = (unsigned char *)realloc(pMemTrunck->pStartPos, 2 * pMemTrunck->nSize);
                memset(pMemTrunck->pStartPos + pMemTrunck->nSize, NULL, pMemTrunck->nSize);
                pMemTrunck->nSize = 2 * pMemTrunck->nSize;
                pMemTrunck->nCount = 2 * pMemTrunck->nCount;
                pMemTrunck->nBlockSize = nBlockSize;
                isRealloc = true;
                //printf("reAllocate size to total %d, ", pMemTrunck->nSize);
            }
            //
            pMemTrunck = m_BlockSizeHashMap[nBlockSize];
            // generate block list
            if (!pMemTrunck)
            {
                return nullptr;
            }
            //
            unsigned long nStartPosOffset = 0;
            if (isRealloc) {
                nStartPosOffset = pMemTrunck->nSize / 2;
            }
            unsigned int nLength = m_BlockSizeArray.size();
            std::function<unsigned long(unsigned long, unsigned long, unsigned long)> blockGenFunc = [=](unsigned long nPos, unsigned long nLeftSize, unsigned long nBlockSize) {
                unsigned int _pos = nPos;
                while (_pos < nPoolSize)
                {
                    unsigned long nRemain = nPoolSize - _pos;
                    if (nRemain >= nBlockSize)
                    {
                        LMemoryBlock *pBlock = LMemoryBlock::create((void *)(pMemTrunck->pStartPos),  nBlockSize, _pos);
                        // 最后的预留字节存储链表节点地址
                        LLinkNode<LMemoryBlock *> * pNode = LLinkNode<LMemoryBlock *>::create(pBlock);
                        auto p = (LLinkNode<LMemoryBlock *> **)((unsigned char *)(pBlock->getFreeStart()) + nBlockSize - SIZE_RESERVED_SPACE);
                        *p = pNode;
                        if (isRealloc) {
                            pMemTrunck->blockList.push_head(pNode);
                        } else {
                            pMemTrunck->blockList.push_back(pNode);
                        }
                        //
                        _pos = _pos + nBlockSize;
                    }
                    else
                    {
                        LMemoryBlock *pBlock = LMemoryBlock::create((void *)(pMemTrunck->pStartPos + _pos), nRemain, _pos);
                        pBlock->setIsFree(false); // 大小不足, 禁用
                        if (isRealloc) {
                            pMemTrunck->blockList.push_head(pBlock);
                        } else {
                            pMemTrunck->blockList.push_back(pBlock);
                        }
                        //
                        _pos = _pos + nRemain;
                    }
                }
                return nPoolSize - _pos;
            };
            unsigned long nPos = nStartPosOffset;
            unsigned long nLeftSize = reAllocSize;
            do
            {
                nLeftSize = blockGenFunc(nPos, nLeftSize, nBlockSize);
                nPos = nPos + nBlockSize;
            } while (nLeftSize >= nBlockSize);
            //printf("block size = %d, new count of block = %d, total block = %d\n", nBlockSize, pMemTrunck->blockList.getSize(), pMemTrunck->nCount);
        }
        return pMemTrunck;
    }

    void LMemoryPool::generateBlockSizeList(unsigned long nMaxBlockSize, unsigned long nMinBlockSize, bool bIsOptimize) {
        unsigned long _nMaxBlockSize = nMaxBlockSize;
        unsigned long _nMinBlockSize = nMinBlockSize;
        if (_nMinBlockSize <= SIZE_RESERVED_SPACE) {
            _nMinBlockSize = _nMaxBlockSize;
        }
        if (_nMaxBlockSize > 0) {
            if (bIsOptimize) {
                double logValueMax = log((int) _nMaxBlockSize) / log(2);
                unsigned long nMaxBlockIndex = (unsigned long) logValueMax;
                if (logValueMax > nMaxBlockIndex) {
                    nMaxBlockIndex = nMaxBlockIndex + 1;
                }
                double logValueMin = log(_nMinBlockSize) / log(2);
                unsigned long nMinBlockIndex = (unsigned long) logValueMin;
                for (unsigned long nIndex = nMinBlockIndex; nIndex <= nMaxBlockIndex; ++nIndex) {
                    m_BlockSizeArray.push_back(pow(2, nIndex));
                }
            }
            else {
                m_BlockSizeArray.push_back(_nMinBlockSize);
                if (_nMinBlockSize != _nMaxBlockSize){
                    m_BlockSizeArray.push_back(_nMaxBlockSize);
                }
            }
        }
    }

    void* LMemoryPool::getBuffer(unsigned long nSize) {
        auto pMemTrunk = m_BlockSizeHashMap[nSize + SIZE_RESERVED_SPACE];
        pMemTrunk = (!!pMemTrunk) ? pMemTrunk : reAllocate(nSize + SIZE_RESERVED_SPACE, COUNT_OBJECT_INIT);
        if (!pMemTrunk) {
            return nullptr;
        }
        LLinkNode<LMemoryBlock *> * pNode = (LLinkNode<LMemoryBlock *> *)pMemTrunk->blockList.getFront();
        while (!pNode || !pNode->getValue()->getIsFree()) {
            reAllocate(nSize + SIZE_RESERVED_SPACE, 2 * pMemTrunk->nCount);
            pNode = (LLinkNode<LMemoryBlock *> *)pMemTrunk->blockList.getFront();
        }
        LMemoryBlock * pBlock = pMemTrunk->blockList.getFront()->getValue();
        if (pBlock) {
            if (pBlock->getIsFree() && pBlock->getFreeSize() >= nSize) {
                pBlock->setIsFree(false);
                // pMemTrunk->blockList.removeFront();
                pMemTrunk->blockList.moveToTail(pNode);
                return  (void *)(pBlock->getFreeStart());
            }
        }
        return nullptr;
    }

    void LMemoryPool::recycleBuffer(void * pStart, unsigned long nSize) {
        if (!pStart || nSize <= 0){
            return;
        }
        auto pMemTrunk = m_BlockSizeHashMap[ + SIZE_RESERVED_SPACE];
        if (!pMemTrunk) {
            return;
        }
        memset((void *)pStart, NULL, nSize);
        auto pNode = *((LLinkNode<LMemoryBlock *> **)((unsigned char *)pStart + nSize));
        auto pBlock = pNode->getValue();
        pBlock->setIsFree(true);
        // pMemTrunk->blockList.push_back(pNode);
        pMemTrunk->blockList.moveToHead(pNode);
    }

NS_LONG_END