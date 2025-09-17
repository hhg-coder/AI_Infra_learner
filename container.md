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


四、queue方法
std::queue 是 C++ STL 中的队列容器适配器Container Adapter，它本身不是一个独立的数据结构，而是对底层容器（如 deque 或 list）的封装，提供**先进先出（FIFO）**的队列接口。
这意味着它基于现有的序列容器（如 std::deque或 std::list）提供特定的接口（先进先出 FIFO）。
常用方法
push(const T&) / push(T&&)
向队尾插入元素（底层调用容器的 push_back）。
pop()
移除队首元素（底层调用容器的 pop_front）。
front()
返回队首元素的引用（底层调用容器的 front）。
back()
返回队尾元素的引用（底层调用容器的 back）。
empty()
判断队列是否为空。
size()
返回队列中元素个数。
std::queue 通过适配底层容器（默认 deque），只暴露队列相关的接口，隐藏了底层容器的其他操作。
只允许队首出、队尾进，保证 FIFO。
不支持遍历和随机访问。

五、priority_queue
std::priority_queue也是一个​​容器适配器 (Container Adapter)​​。它的核心特性是元素不是按照插入顺序，而是按照​​优先级​​进行排序和访问的。
核心操作：​​
push(element): 将元素插入队列。
pop(): 移除队列中​​优先级最高​​的元素（默认为最大元素）。
top(): 访问队列中​​优先级最高​​的元素（默认为最大元素）（不移除）。
​​底层数据结构：​​ 通常基于​​堆 (Heap)​​ 数据结构实现（默认是最大堆）（即 top()返回最大值）。堆是一种特殊的完全二叉树，满足堆性质（Heap Property）;
容器适配器：​​ priority_queue封装了一个底层容器（默认是 std::vector），并使用堆算法（std::make_heap, std::push_heap, std::pop_heap）来维护这个容器的堆结构。它只暴露与优先级队列相关的接口。
swap(priority_queue& other) noexcept(...): 交换两个 priority_queue的内容。交换它们的底层容器和比较器对象。时间复杂度通常是 O(1)，具体取决于底层容器和比较器的 swap实现（vector::swap是 O(1)）。

六、stack
std::stack 是 C++ STL 中的栈容器适配器，它本身不是独立的数据结构，而是对底层容器（如 deque 或 vector）的封装，提供**后进先出（LIFO）**的栈接口。
底层原理
容器适配器
stack 默认底层用 std::deque 实现（也可以用 std::vector 或 std::list）。
通过组合方式，将底层容器的部分接口暴露为栈操作。
数据结构
栈只允许在一端（栈顶）插入和删除元素，保证 LIFO 顺序。
不允许遍历和随机访问，只能访问栈顶元素。
成员变量
内部有一个底层容器对象（如 std::deque<T>）。
常用方法
push(const T&) / push(T&&)
向栈顶插入元素（底层调用容器的 push_back）。
pop()
移除栈顶元素（底层调用容器的 pop_back）。
top()
返回栈顶元素的引用（底层调用容器的 back）。
empty()
判断栈是否为空。
size()
返回栈中元素个数。


七、map方法
std::map 是一个有序的关联容器，包含具有唯一键的键值对。键通过比较函数 Compare 进行排序。搜索、删除和插入操作具有对数复杂度。Map 通常实现为红黑树。
数据结构
std::map 通常用**红黑树（Red-Black Tree）**实现，是一种自平衡二叉搜索树。
每个节点存储一个键值对（std::pair<const Key, T>），并维护左右子树和父节点指针，以及颜色信息（红/黑）。
有序性
所有元素按键（key）自动排序，默认用 < 运算符（可自定义比较器）。
查找、插入、删除的时间复杂度均为 O(log n)。
唯一性
每个 key 只能出现一次，插入相同 key 会失败。

常用方法
insert(const value_type&) 插入一个键值对。如果 key 已存在，插入失败。
operator[] 通过 key 访问 value。如果 key 不存在，会自动插入一个默认值。
find(const Key&) 查找 key，返回指向该元素的迭代器，找不到返回 end()。
erase(const Key&) 删除指定 key 的元素。
begin()/end() 返回有序遍历的迭代器。
lower_bound(const Key&) 返回第一个不小于 key 的元素迭代器。
upper_bound(const Key&) 返回第一个大于 key 的元素迭代器。
size()/empty()/clear() 容量相关操作。
extract() extract 方法返回的是一个节点句柄（node handle），类型为 std::map<Key, T>::node_type。这个类型可以安全地转移节点到其他 map，也可以访问和修改节点的 key/value。
merge() merge 会把另一个 map 中所有不重复 key 的节点直接“搬到”当前 map。已存在的 key 不会被覆盖，剩下的节点还留在源 map。底层实现：直接移动节点指针，不做元素拷贝或移动构造，效率极高。

swap原理：
swap 方法只需交换这些成员变量（如根节点指针、大小、比较器等）
伪代码：
void swap(map& other) noexcept {
    std::swap(this->root, other.root);         // 交换红黑树根节点指针
    std::swap(this->size, other.size);         // 交换元素数量
    std::swap(this->comp, other.comp);         // 交换比较器
    std::swap(this->alloc, other.alloc);       // 交换分配器
    // 其他必要的成员变量
}

八、set方法
std::set 是一个关联容器，它包含一个已排序的唯一 Key 类型对象的集合。排序使用键比较函数 Compare 完成。搜索、删除和插入操作具有对数复杂度。Set 通常实现为 红黑树。
底层原理
数据结构
std::set 通常用**红黑树（Red-Black Tree）**实现，是一种自平衡二叉搜索树。
每个节点只存储一个元素（key），没有 value。
元素在内存中自动有序排列（默认用 < 比较，可自定义比较器）。
唯一性
每个元素只能出现一次，插入重复元素会失败。
有序性
所有元素始终保持有序，支持高效的有序遍历。
效率
查找、插入、删除的时间复杂度均为 O(log n)。
底层原理和方法基本和map差不多
可以理解为：
set 是 map 的“只有 key 没有 value”的版本。


九、multimap和multiset
底层数据结构：​​ 与 std::map和 std::set一样，在标准库实现中，multimap和 multiset也几乎总是基于​​红黑树 (Red-Black Tree)​​ 实现。
是 std::map和 std::set的变体，核心区别在于​​允许存储重复的键​​。
底层实现要点
插入：允许重复 key，插入时会放在等价 key 的区间内（通常插在已有 key 的后面）。
查找：find 返回第一个等于 key 的元素，equal_range 返回所有等于 key 的区间。
遍历：有序遍历，重复 key 会连续出现。
节点操作：extract/merge 直接操作树节点指针，避免元素拷贝
可以把 multimap/multiset 看作是“允许重复 key 的 map/set”，用法和实现方式几乎一样，只是插入和查找时会处理重复 key 的情况。

十、unordered_map、unordered_set、unordered_multimap、unordered_multiset方法
无序关联容器
底层原理
1. 数据结构
这四个容器都基于**哈希表（Hash Table）**实现。
元素分布在一组“桶”（bucket）中，桶的数量动态调整。桶 (Bucket)：​​ 存储具有相同哈希值（模桶数）的元素的容器（链表或树）。
通过哈希函数将 key 映射到某个桶，桶内通常用链表或链式结构存储元素（C++11/14/17 标准实现多为链表）。
2. 有序性与唯一性
无序：元素在内存中的排列顺序与插入顺序、key 大小无关，只与哈希值有关。
唯一性：
unordered_map/unordered_set：key 唯一。
unordered_multimap/unordered_multiset：key 可重复。
3. 效率
查找、插入、删除的平均时间复杂度为 O(1)，最坏情况下 O(n)（哈希冲突严重时）。

在接口设计上，multi版本没有operator[]，因为无法唯一确定返回值。
常用获取键值对的方法：find(key)：返回第一个 key 匹配的迭代器。相同key的分布不保证连续
equal_range(key)：
返回所有 key 匹配的区间（pair<iterator, iterator>），可遍历所有同 key 的元素。
auto range = mm.equal_range(key);
for (auto it = range.first; it != range.second; ++it) {
    std::cout << it->first << " : " << it->second << std::endl;
}

所有unordered容器都提供桶接口(bucket_count, load_factor等)用于性能调优。
冲突解决 (Collision Resolution)：​​ 不同的键可能被哈希到同一个桶（即产生​​哈希冲突 (Hash Collision)​​）。
标准库通常使用​​链地址法 (Separate Chaining)​​ 来解决冲突：每个桶实际上是一个链表（或小型平衡树，如红黑树，用于优化长链表性能），存储所有哈希到该桶的元素。

自动扩容rehash（开销O(n)）：
装载因子越大，哈希冲突概率越高，查找效率可能下降。
当装载因子超过最大值（max_load_factor），哈希表会自动扩容并重新分布元素。
可以通过 unordered_map 的 load_factor() 和 max_load_factor() 方法获取和设置。
load_factor = 元素个数 / 桶（bucket）个数

在方法接口上，unordered_ 系列多了哈希相关方法和参数，遍历顺序无序。
swap(unordered_X& other): 交换两个容器的内容。通常是 O(1)（交换内部指针）。

桶在内存的排列
桶在内存中通常是一组连续的指针数组（比如 std::vector<Node*>），每个指针指向一个链表的头节点（链式法）。
每个元素通过哈希函数计算 key 的哈希值，然后对桶数量取模，决定放入哪个桶。
桶本身是连续分配的，但桶内的元素（链表节点）在内存中不一定连续。
