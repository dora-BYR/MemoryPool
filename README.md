# MemoryPool
A MemoryPool implemented in C++11. Variable size and pool-auto-size supported. 85% performence promoted.
C++11 内存池
1、支持任意变量大小的内存分配
2、支持池自动扩展。
3、性能提升约85%

Welcome to the MemoryPool wiki!
# # Test result
//////////////////////////////// 内存池功能测试 ////////////////////////////////
sizeof(LTestObject) = 376
sizeof(* pObject) = 376
1: object.m_nId = 201604, object.m_nName = (null)
-------------------------- memory pool status -------------------------
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
-------------------------- memory pool status -------------------------
| block(object) size = 384; blocks used/unused/total = 0/200/200
-----------------------------------------------------------------------
2: object1.m_nId = 201605, object1.m_nName = test object1
-------------------------- memory pool status -------------------------
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
-------------------------- memory pool status -------------------------
| block(object) size = 2064; blocks used/unused/total = 0/30/30
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
2: object2.m_nId = 0, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 1, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 2, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 3, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 4, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 5, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 6, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 7, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 8, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 9, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 10, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 11, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 12, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 13, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 14, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 15, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 16, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 17, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 18, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 19, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 20, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 21, object2.m_nName = test object2, object2.m_szSex = famale
-------------------------- memory pool status -------------------------
| block(object) size = 2064; blocks used/unused/total = 22/8/30
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
Test for auto-reSize to pool (size: 30 -> 60)
2: object2.m_nId = 0, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 1, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 2, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 3, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 4, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 5, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 6, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 7, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 8, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 9, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 10, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 11, object2.m_nName = test object2, object2.m_szSex = famale
2: object2.m_nId = 12, object2.m_nName = test object2, object2.m_szSex = famale
-------------------------- memory pool status -------------------------
| block(object) size = 2064; blocks used/unused/total = 35/25/60
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
-------------------------- memory pool status -------------------------
| block(object) size = 520; blocks used/unused/total = 0/1/1
| block(object) size = 2064; blocks used/unused/total = 35/25/60
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
3: string = use memory as buffer
-------------------------- memory pool status -------------------------
| block(object) size = 520; blocks used/unused/total = 1/0/1
| block(object) size = 2064; blocks used/unused/total = 35/25/60
| block(object) size = 384; blocks used/unused/total = 1/199/200
-----------------------------------------------------------------------
3: string = 

//////////////////////////////// 性能测试 ////////////////////////////////
create: pool unused = 2446.4790  ms
create: pool used = 386.7380 ms
-------------------------- memory pool status -------------------------
| block(object) size = 2056; blocks used/unused/total = 1867284/132716/2000000
-----------------------------------------------------------------------
