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
底层原理：块状连续存储结构
与 std::vector 不同，deque 的元素不是连续存储的：典型的实现使用一系列单独分配的固定大小数组，并带有额外的簿记（map，实现也是用vector），这意味着对 deque 的索引访问必须执行两次指针解引用，而 vector 的索引访问只需执行一次。
deque 的存储空间会根据需要自动扩展和收缩。deque 的扩展比 std::vector 的扩展更便宜，因为它不涉及将现有元素复制到新的内存位置。另一方面，deque 通常具有较大的最小内存成本；只包含一个元素的 deque 必须分配其完整的内部数组（例如，在 64 位 libstdc++ 上是对象大小的 8 倍；在 64 位 libc++ 上是对象大小的 16 倍或 4096 字节，以较大者为准）。
双端操作：​​ 你可以在队列的​​头部​​和​​尾部​​高效地进行插入 (push_front, emplace_front) 和删除 (pop_front) 操作。（vector头部操作是 O(n)）
​​随机访问：​​ 像 vector一样，它支持通过下标 (operator[], at()) 直接访问任意位置的元素（随机访问）。
​​动态大小：​​ 它可以根据需要动态增长或缩小。
​​非单一连续存储：​​ 与 vector的单一连续内存块不同，deque通常由多个固定大小的内存块（称为 "块"、"页" 或 "缓冲区"）组成。这些块分散在内存中，但通过一个中央索引结构（通常是一个指针数组或向量，称为 "映射器" 或 "控制器"）进行管理。
中间插入/删除：​​ 时间复杂度是 ​​O(n)​​（线性时间），与 vector类似（需要移动元素）。
共同点：
都支持随机访问（如 operator[], at()）。
都支持尾部插入和删除（push_back, pop_back）。
都支持插入、删除、遍历（如 insert, erase, begin, end）。
都有容量相关方法（如 size(), empty(), clear()）。
都支持 emplace_back, emplace 等原地构造元素的方法。
deque与vector的比较
不同点：
头部操作
vector 不支持高效的头部插入和删除（push_front, pop_front），如果用 insert(begin(), ...)，效率很低，因为需要移动所有元素。
deque 支持高效的头部插入和删除（push_front, pop_front），效率为 O(1)。
中间插入和删除
两者都支持 insert 和 erase，但效率都是 O(n)，因为都需要移动一部分元素。
vector 是移动插入点之后的所有元素。
deque 是移动插入点到最近端（头或尾）之间的所有元素。
内存布局
vector 的所有元素都在一块连续的内存中，适合需要与C数组兼容的场景。
deque 的元素分布在多个小块内存中，不保证物理连续，但逻辑上可以随机访问。
容量管理
vector 有 capacity() 和 reserve() 方法，可以预分配内存，减少扩容次数。
deque 没有 capacity() 和 reserve()，因为它的分段结构不需要一次性分配大块内存。
收缩内存
vector 支持 shrink_to_fit()，可以释放多余的内存。
deque 一般不支持 shrink_to_fit()，即使有实现也不一定有效果。
vector 的 shrink_to_fit() 可以通过重新分配一块刚好够用的连续内存来释放多余空间。
deque 每次扩容或缩容时，只需增加或减少分段的数量，不存在“多余的连续空间”需要释放，所以 shrink_to_fit() 没有实际意义，或者实现起来没有明显效果。
效率差异
vector 适合频繁在尾部插入/删除，且需要高效随机访问和内存连续的场景。
deque 适合需要在头尾都高效插入/删除的场景，但不适合与C数组直接兼容的场景。
总结
如果你需要高效的头尾插入/删除，用 deque。
如果你需要高效的随机访问和内存连续性，用 vector。
两者都不适合频繁在中间插入/删除元素的场景。


三、list方法：
底层原理：双向链表
与vector不同的方法：
​​splice系列：
void splice(iterator pos, list& other);：将另一个链表 other的所有元素移动到当前链表的 pos位置之前。other变为空。
void splice(iterator pos, list& other, iterator it);：将 other链表中由 it指向的单个元素移动到当前链表的 pos位置之前。
void splice(iterator pos, list& other, iterator first, iterator last);：将 other链表中 [first, last)范围内的元素移动到当前链表的 pos位置之前。

​​merge：​​
void merge(list& other);：假设当前链表和 other链表都已经是​​升序排序​​(前提)的，将 other合并到当前链表中，合并后的链表仍然保持升序。other变为空。通常使用归并排序的合并步骤，效率很高 (O(n))。
void merge(list& other, Compare comp);：使用自定义比较函数 comp进行合并。
remove和 remove_if：​​
void remove(const T& value);：移除所有值等于 value的元素。
void remove_if(Predicate pred);：移除所有满足谓词 pred的元素。

unique：
void unique();：移除​​连续的​​重复元素（通常需要先调用 sort来保证所有重复元素连续）。
void unique(BinaryPredicate binary_pred);：使用自定义二元谓词判断连续元素是否“重复”。

​​sort：​​
void sort();：对链表元素进行升序排序（使用 <运算符）。
void sort(Compare comp);：使用自定义比较函数 comp进行排序。

reverse：​​
void reverse();：反转链表中元素的顺序。

无​​ (reserve, capacity, shrink_to_fit)

关于swap:
​​std::vector::swap:​​ ​​常数时间 O(1)​​。它通常只交换三个内部指针（指向数据起始位置的指针、指向最后一个元素后一位的指针、指向分配内存末尾的指针）以及可能的分配器（如果分配器状态也需要交换）。不涉及元素本身的拷贝、移动或逐个交换。
​​std::list::swap:​​ ​​常数时间 O(1)​​。它通常只交换内部指向头节点（或哨兵节点）的指针以及可能的分配器。链表中的节点本身及其连接关系保持不变，只是“归属”发生了变化。不涉及节点或元素的拷贝、移动或逐个交换。
​​结论：​​ 两者的 swap成员函数在时间复杂度上​​都是 O(1)​​，非常高效。

迭代器、引用、指针的失效 (Iterator, Reference, Pointer Invalidation):​​
​​std::vector::swap:​​ ​​使两个 vector 的所有迭代器、引用和指针失效​​。交换后，原来指向容器 A 中元素的迭代器/引用/指针，现在指向的是容器 B 的内存（如果它还存在的话），反之亦然。使用它们会导致未定义行为。
​​std::list::swap:​​ ​​通常不会使迭代器、引用和指针失效​​。这是链表结构的一个巨大优势。交换后：
原来指向容器 A 中某个元素的迭代器/引用/指针，​​仍然指向同一个元素节点​​，只是这个节点现在属于容器 B 了。
原来指向容器 B 中某个元素的迭代器/引用/指针，​​仍然指向同一个元素节点​​，只是这个节点现在属于容器 A 了。
你可以继续安全地使用这些迭代器/引用/指针来访问或操作元素。​​但是，end()迭代器会交换​​。原来指向 A.end() 的迭代器，在 swap 后会指向 B.end()，反之亦然。
​​结论：​​ vector::swap​​使所有迭代器失效​​；list::swap​​保持迭代器有效​​（指向的元素节点不变，但所属容器变了，end()迭代器交换）。

​​std::vector::swap:​​ 交换的是底层动态数组的​​所有权​​。
​​std::list::swap:​​ 交换的是​​链表头节点（哨兵节点）的所有权​​。
结论：​​ 两者都只交换​​管理权​​（vector 交换数组管理权，list 交换链表头管理权），不移动实际数据或节点。


