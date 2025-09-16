https://cppreference.cn/w/cpp/container
## 容器(container)

容器：包括序列式容器和关联式容器；即各种数据结构，如 vector，list，deque，set，map 等用来存储数据；从实现的角度来看，STL 容器是一种 class template。

任何特定的数据结构都是为了实现某种特定的算法。

![](https://github.com/steveLauwh/SGI-STL/raw/master/The%20Annotated%20STL%20Sources%20V3.3/Other/container.PNG)

## [序列式容器(sequence container)](https://github.com/steveLauwh/SGI-STL/tree/master/The%20Annotated%20STL%20Sources%20V3.3/container/sequence%20container)

* array (C++ 提供，build-in)
* vector
* heap (内含一个 vector)
* priority-queue (内含一个 heap)
* list
* slist (非标准)
* deque
* stack (内含一个 deque)  (adapter 配接器)
* queue (内含一个 deque)  (adapter 配接器)

怎么理解序列式容器，其中的元素都可序(ordered), 但未必有序(sorted)？

ordered 是容器集合被排序，可以使用指定的顺序去遍历集合。 sorted 是一个容器集合根据某些规则确定排序的。

![](https://github.com/steveLauwh/SGI-STL/raw/master/The%20Annotated%20STL%20Sources%20V3.3/Other/sequencecontainer.PNG)

## [关联式容器(associative container)](https://github.com/steveLauwh/SGI-STL/tree/master/The%20Annotated%20STL%20Sources%20V3.3/container/associative%20container)

* RB-tree (非公开)
* set (内含一个 RB-tree)
* map (内含一个 RB-tree)
* multiset (内含一个 RB-tree)
* multimap (内含一个 RB-tree)
* hashtable (非标准)
* hash_set (内含一个 hashtable)  (非标准)
* hash_map (内含一个 hashtable)  (非标准)
* hash_multiset (内含一个 hashtable)  (非标准)
* hash_multimap (内含一个 hashtable)  (非标准)

![](https://github.com/steveLauwh/SGI-STL/raw/master/The%20Annotated%20STL%20Sources%20V3.3/Other/associativecontainer.PNG)

熟悉关联式容器，需要有 [RB-tree](https://github.com/steveLauwh/Data-Structures-And-Algorithms/tree/master/Tree/RB-tree)(红黑树原理) 和 [hash table](https://github.com/steveLauwh/Data-Structures-And-Algorithms/tree/master/Hash%20Table)(哈希表原理) 基础。


一、vector方法：
vector底层原理：一段连续的内存空间
3个迭代器：
_Myfirst 指向的是 vector 容器对象的起始字节位置；_Mylast 指向当前最后一个元素的末尾字节；_myend 指向整个 vector 容器所占用内存空间的末尾字节。
_Myfirst 和 _Mylast 可以用来表示 vector 容器中目前已被使用的内存空间；
_Mylast 和 _Myend 可以用来表示 vector 容器目前空闲的内存空间；
_Myfirst 和 _Myend 可以用表示 vector 容器的容量。
迭代器：begin() , end(), rbegin(), rend(), cbegin(), cend(), crbegin(), crend()  
容量：
  size(), empty()
  capacity() 返回当前已分配的存储空间能容纳的元素数。
  size()     返回当前元素的数量。
  reserve(n)  增加容量至至少 n。如果 n大于当前 capacity()，会重新分配存储空间。​​不会改变 size()​​。
  shrink_to_fit()(C++11)  请求移除未使用的容量，使 capacity()与 size()匹配。这是一个​​非强制性请求​​，实现可以忽略。
元素访问：
  operator[i], at(i), front(), back()
  data()(C++11)  返回指向底层元素数组的指针。对于空 vector，返回值不确定。
修改器（Modifier）
  assign(n, value)  用 n个 value的副本替换所有内容。
  assign(begin_it, end_it) 用迭代器范围 [begin_it, end_it)中的元素替换所有内容。
  pop_back()
  insert(pos_it, value) 在迭代器 pos_it指向的位置​​之前​​插入一个元素。返回指向新插入元素的迭代器。
  emplace(pos_it, args...)(C++11)  在指定位置​​就地构造​​一个元素（传入构造函数的参数）。
  erase(pos_it)  移除位于 pos_it的元素。返回指向被移除元素之后元素的迭代器。
  erase(first_it, last_it) 移除范围 [first_it, last_it)中的元素。
  clear()  清除所有元素（size()变为 0，但 capacity()通常不变）
  resize(n)  将元素数量改为 n。如果 n < size()，多余元素被移除；如果 n > size()，新增元素被​​值初始化​​。
  resize(n, value)  将元素数量改为 n。新增的元素是 value的副本。
  swap(other_vector)  交换两个 vector 的内容。底层原理，交换指向内存的多个指针，不交换内存
  emplace_back  vs push_back​​：
  v.push_back(MyClass(1, 2, 3));// 先构造一个临时对象，再将其移动或拷贝到容器中。
  v.emplace_back(1, 2, 3);// 直接在容器末尾的内存中调用 MyClass(1, 2, 3)进行构造。​​更高效​​。
构造函数
  vector() 默认构造函数，创建空容器。
  vector(n, value) 构造一个包含 n个 value的容器。
  vector(begin_it, end_it) 用迭代器范围构造容器。
  vector(other_vector) 拷贝构造函数。
  vector(initializer_list)(C++11) 使用初始化列表构造，例如 vector<int> v = {1, 2, 3};
  ~vector() 析构函数，销毁所有元素并释放内存。

二、deque方法：


三、list方法：
底层原理：双向链表
