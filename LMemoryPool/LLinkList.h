//
// Created by 龙辉 on 16/4/9.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LMEMORYPOOL_LLINKLIST_H
#define LMEMORYPOOL_LLINKLIST_H

#include "define_l.h"

#include <functional>

NS_LONG_BEGIN

    // 双向链表节点 linklist node
    template <typename T>
    class LLinkNode
    {
    public:
        LLinkNode():
                m_pPre(nullptr),
                m_pNext(nullptr),
                m_pValue(nullptr)
        {

        }
        LLinkNode(T &pValue):
                m_pValue(pValue),
                m_pPre(nullptr),
                m_pNext(nullptr)
        {

        };
        virtual ~ LLinkNode(){};

        static LLinkNode * create(T &value)
        {
            auto node = new LLinkNode(value);
            return node;
        }

        bool equal(LLinkNode * pNode)
        {
            return this == pNode;
        };

        LLinkNode<T> * operator++(int n)
        {
            return this->getNext();
        }

    public:
        void setNext(LLinkNode * pNode){ m_pNext = pNode; };
        void setPre(LLinkNode * pNode){ m_pPre = pNode; };
        LLinkNode * getNext(){ return m_pNext; };
        LLinkNode * getPre(){ return m_pPre; };
        T getValue(){ return m_pValue; };

    private:
        T m_pValue;
        LLinkNode * m_pPre;
        LLinkNode * m_pNext;
    };

    // 双向链表 double-linklist
    template <typename T>
    class LLinkList
    {
    public:
        LLinkList()
        {
            m_pHead = new LLinkNode<T>();
            m_pTail = new LLinkNode<T>();
            m_pHead->setPre(nullptr);
            m_pHead->setNext(m_pTail);
            m_pTail->setPre(m_pHead);
            m_pTail->setNext(nullptr);
        }
        virtual ~LLinkList(){};

        static LLinkList * create()
        {
            return new LLinkList();
        }

        // 插入尾部节点 inert linknode from tail
        // @param pValue: 插入的节点对应的值 the value of the linknode to be insert
        void push_back(T &pValue)
        {
            LLinkNode<T> * pNode = new LLinkNode<T>(pValue);
            push_back(pNode);
        }

        // 插入尾部节点 inert linknode from tail
        // @param pNode: 需要插入的节点 the linknode to be insert
        void push_back(LLinkNode<T> * pNode)
        {
            m_pTail->getPre()->setNext(pNode);
            pNode->setPre(m_pTail->getPre());
            pNode->setNext(m_pTail);
            m_pTail->setPre(pNode);
        }

        // 插入头部节点 inert linknode from head
        // @param pValue: 插入的节点对应的值 the value of the linknode to be insert
        void push_head(T &pValue)
        {
            LLinkNode<T> * pNode = new LLinkNode<T>(pValue);
            push_head(pNode);
        }

        // 插入头部节点 inert linknode from head
        // @param pNode: 需要插入的节点 the linknode to be insert
        void push_head(LLinkNode<T> * pNode)
        {
            m_pHead->getNext()->setPre(pNode);
            pNode->setNext(m_pHead->getNext());
            m_pHead->setNext(pNode);
            pNode->setPre(m_pHead);
        }

        // 条件插入 insert by conditions. pNodeInsert will be insert behind pNode
        // @ret if insert successd
        bool insert(LLinkNode<T> * pNode, std::function<bool(LLinkNode<T> * pNode, LLinkNode<T> * pNodeInsert)> conditionFunc)
        {
            for (LLinkNode<T> * pTmpNode = m_pHead; pTmpNode != m_pTail; pTmpNode = pTmpNode->getNext())
            {
                if (conditionFunc(pTmpNode, pNode))
                {
                    pTmpNode->getNext()->setPre(pNode);
                    pNode->setNext(pTmpNode->getNext());
                    pTmpNode->setNext(pNode);
                    pNode->setPre(pTmpNode);
                    return true;
                }
            }
            return false;
        }

        // 条件插入 insert by conditions
        // @ret return the node ptr if insert successd
        LLinkNode<T> * insert(T &pValue, std::function<bool(LLinkNode<T> * pNode, LLinkNode<T> *)> conditionFunc)
        {
            LLinkNode<T> * pNode = new LLinkNode<T>(pValue);
            if (insert(pNode, conditionFunc))
            {
                return pNode;
            }
            else
            {
                delete pNode;
            }
            return nullptr;
        }

        // 删除一个节点 remove a linknode
        // @param pNode: 需要删除的节点 the linknode to be removed
        // @param bNeedDeleteValue: if need delete the value memory
        bool remove(LLinkNode<T> * pNode, bool bNeedDeleteValue = false)
        {
            if(!pNode){
                return false;
            }
            pNode->getPre()->setNext(pNode->getNext());
            pNode->getNext()->setPre(pNode->getPre());
            pNode->setPre(nullptr);
            pNode->setNext(nullptr);
            if (bNeedDeleteValue && pNode->getValue())
            {
                T pValue = pNode->getValue();
                delete (pValue);
            }
            return false;
        }

        // 根据条件删除一个节点 remove a node by conditions
        // @param pNode: 条件函数 the condition function
        // @param bNeedDeleteValue: if need delete the value memory
        bool remove(std::function<bool(LLinkNode<T> &node)> conditionFunc, bool bNeedDeleteValue = false)
        {
            for (LLinkNode<T> * pTmpNode = m_pHead; pTmpNode != m_pTail; pTmpNode = pTmpNode->getNext())
            {
                if (conditionFunc(pTmpNode))
                {
                    remove(pTmpNode, bNeedDeleteValue);
                    return true;
                }
            }
            return true;
        }

        // 遍历函数 iterator the list
        void iteratorList(std::function<void(LLinkNode<T> * pNode)> func)
        {
            for (LLinkNode<T> * pTmpNode = m_pHead->getNext(); pTmpNode != m_pTail; pTmpNode = pTmpNode->getNext())
            {
                if (pTmpNode) {
                    func(pTmpNode);
                }
            }
        }

        // iterator
        LLinkNode<T> * begin()
        {
            return m_pHead;
        }

        LLinkNode<T> * end()
        {
            return m_pTail;
        }


        LLinkNode<T> * getFront()
        {
            if (m_pHead->getNext() && m_pHead->getNext() != m_pTail) {
                return m_pHead->getNext();
            }
            return nullptr;
        }

        bool removeFront()
        {
            return remove(m_pHead->getNext());
        }

        unsigned int getSize() {
            unsigned int nSize = 0;
            LLinkNode<T> * p = m_pHead->getNext();
            while (p) {
                if (p == m_pTail) {
                    break;
                }
                ++nSize;
                p = p->getNext();
            }
            return nSize;
        }

        void moveToHead(LLinkNode<T> * pNode) {
            if (pNode && pNode != m_pHead->getNext()) {
                remove(pNode);
                push_head(pNode);
            }
        }

        void moveToTail(LLinkNode<T> * pNode) {
            if (pNode && pNode != m_pTail->getPre()) {
                remove(pNode);
                push_back(pNode);
            }
        }

    private:
        LLinkNode<T> * m_pHead;
        LLinkNode<T> * m_pTail;

    };

NS_LONG_END

#endif //LMEMORYPOOL_LLINKLIST_H
