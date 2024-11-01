# 认识单双链表

**前置知识**：无，会的可以直接跳过。

## 基本概念

内存空间是所有程序的公共资源，在一个复杂的系统运行环境下，空闲的内存空间可能散落在内存各处。我们知道，存储数组的内存空间必须是连续的，而当数组非常大时，内存可能无法提供如此大的连续空间。此时链表的灵活性优势就体现出来了。

链表（linked list）是一种线性数据结构，其中的每个元素都是一个节点对象，各个节点通过“引用”相连接。引用记录了下一个节点的内存地址，通过它可以从当前节点访问到下一个节点。

链表的设计使得各个节点可以分散存储在内存各处，它们的内存地址无须连续。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/41a6f6121f8a48faa36b415dbf7c785b.png)

观察上图，链表的组成单位是节点（node）对象。每个节点都包含两项数据：节点的“值”和指向下一节点的“引用”：

- 链表的首个节点被称为“头节点”，最后一个节点被称为“尾节点”。
- 尾节点指向的是“空”，它在 Java、C++ 和 Python 中分别被记为 `null`、`nullptr` 和 `None` 。
- 在 C、C++、Go 和 Rust 等支持指针的语言中，上述“引用”应被替换为“指针”。

如下面代码所示，链表节点 `ListNode` 除了包含值，还需额外保存一个引用（指针）。因此在相同数据量下，**链表比数组占用更多的内存空间**。

```cpp
/* 链表节点结构体 */
struct ListNode {
  int val;         // 节点值
  ListNode *next;  // 指向下一节点的指针
  ListNode(int x) : val(x), next(nullptr) {}  // 构造函数
};
```

双链表是在上述单链表的基础上，再加一个指向前一个节点的指针，插入和删除操作需要额外再对指向前一个节点的指针进行操作，双链表的数据结构如下所示：

```cpp
/* 双向链表节点结构体 */
struct DoubleListNode {
  int val;         // 节点值
  DoubleListNode *next;  // 指向后继节点的指针
  DoubleListNode *prev;  // 指向前驱节点的指针
  DoubleListNode(int x) : val(x), next(nullptr), prev(nullptr) {}  // 构造函数
};
```

## 链表操作

链表的主要操作就两个：插入节点和删除节点。
### 插入节点

在链表中插入节点非常容易。如下图所示，假设我们想在相邻的两个节点 `n0` 和 `n1` 之间插入一个新节点 `P` ，**则只需改变两个节点引用（指针）即可**，时间复杂度为 $O(1)$ 。相比之下，在数组中插入元素的时间复杂度为 $O(n)$ ，在大数据量下的效率较低。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/e538739ae1ad4245bdac43da1bdb2aea.png)

插入节点的代码示例如下：

```cpp
/* 在链表的节点 n0 之后插入节点 P */
void insert(ListNode *n0, ListNode *P) {
  ListNode *n1 = n0->next;
  P->next = n1;
  n0->next = P;
}
```

### 删除节点

如下图所示，在链表中删除节点也非常方便，**只需改变一个节点的引用（指针）即可**。请注意，尽管在删除操作完成后节点 `P` 仍然指向 `n1` ，但实际上遍历此链表已经无法访问到 `P` ，这意味着 `P` 已经不再属于该链表了。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/276d1ec7f91445e89271b72884594c33.png)

删除节点的代码示例如下：

```cpp
/* 删除链表的节点 n0 之后的首个节点 */
void remove(ListNode *n0) {
  if (n0->next == nullptr)
    return;
  // n0 -> P -> n1
  ListNode *P = n0->next;
  ListNode *n1 = P->next;
  n0->next = n1;
  // 释放内存
  delete P;
}
```

## 链表的具体实现

通过前面的概念理解了链表的基本模型以及链表的操作方式，现在可以实现一个链表。我们可以按照 [leetcode707 设计链表](https://leetcode.cn/problems/design-linked-list/)进行实现并测试。

### 单链表的实现

链表节点是默认给出的，与上面描述的是一样的。

```cpp
class MyLinkedList {
public:
  MyLinkedList() : size(0) {
    dummy = new ListNode(-1);
    tail = dummy;
  }
    
  int get(int index) {
    if (index < 0 || index >= size)
      return -1;

     ListNode *cur_node = dummy->next;
     int i = 0;
     for (int i = 0; i < index; ++i) {
       cur_node = cur_node->next;
     }

     return cur_node->val;
  }
    
  void addAtHead(int val) {
    ListNode *new_node = new ListNode(val);
    new_node->next = dummy->next;
    dummy->next = new_node;
    ++size;

    if (tail == dummy)
      tail = new_node;
  }
  
  void addAtTail(int val) {
    ListNode *new_node = new ListNode(val);
    new_node->next = tail->next;
    tail->next = new_node;
    tail = new_node;
    ++size;
  }
  
  void addAtIndex(int index, int val) {
    if (index < 0 || index > size)
      return;

    if (index == size) {
      addAtTail(val);
      return;
    }

    ListNode *new_node = new ListNode(val);
    ListNode *prev_node = dummy;
    ListNode *cur_node = dummy->next;
    for (int i = 0; i < index; ++i) {
      prev_node =prev_node->next;
      cur_node = cur_node->next;
    }
    new_node->next = cur_node;
    prev_node->next = new_node;
    ++size;
  }
  
  void deleteAtIndex(int index) {
    if (index < 0 || index >= size) 
      return;

    ListNode *prev_node = dummy;
    ListNode *cur_node = dummy->next;
    for (int i = 0; i < index; ++i) {
      prev_node = prev_node->next;
      cur_node = cur_node->next;
    }

    if (cur_node == tail) 
      tail = prev_node;

    prev_node->next = cur_node->next;
    delete cur_node;
    cur_node = nullptr;
    --size;
  }

private:
  ListNode *dummy;
  ListNode *tail;
  int size;
};
```
###  双链表的实现

双链表的实现无非在单链表的基础上增加一个指针的处理

```cpp
class MyLinkedList {
public:
  MyLinkedList() : size(0) {
    dummy = new ListNode(-1);
    tail = dummy;
  }
  
  int get(int index) {
    if (index< 0 || index >= size)
      return -1;

    ListNode *cur_node = dummy->next;
    int i = 0;
    for (int i = 0; i < index; ++i) {
      cur_node = cur_node->next;
    }

    return cur_node->val;
  }
  
  void addAtHead(int val) {
    ListNode *new_node = new ListNode(val);
    new_node->next = dummy->next;
    new_node->prev = dummy;
    dummy->next = new_node;
    ++size;

    if (tail == dummy)
      tail = new_node;
  }
  
  void addAtTail(int val) {
    ListNode *new_node = new ListNode(val);
    new_node->next = tail->next;
    new_node->prev = tail;
    tail->next = new_node;
    tail = new_node;
    ++size;
  }
  
  void addAtIndex(int index, int val) {
    if (index < 0 || index > size)
      return;

    if (index == size) {
      addAtTail(val);
      return;
    }

    ListNode *new_node = new ListNode(val);
    ListNode *prev_node = dummy;
    ListNode *cur_node = dummy->next;
    for (int i = 0; i < index; ++i) {
      prev_node = prev_node->next;
      cur_node = cur_node->next;
    }
    new_node->next = cur_node;
    new_node->prev = prev_node;
    prev_node->next = new_node;
    ++size;
  }
  
  void deleteAtIndex(int index) {
    if (index < 0 || index >= size) 
      return;

    ListNode *prev_node = dummy;
    ListNode *cur_node = dummy->next;
    for (int i = 0; i < index; ++i) {
      prev_node = prev_node->next;
      cur_node = cur_node->next;
    }

    if (cur_node == tail) 
      tail = prev_node;

    prev_node->next = cur_node->next;
    cur_node->next->prev = prev_node;
    delete cur_node;
    cur_node = nullptr;
    --size;
  }

private:
  ListNode *dummy;
  ListNode *tail;
  int size;
};
```

## 链表入门题

### 反转链表

题目描述如下图所示:

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/260a4d19fd14447c833d960e85e62d2e.png)

**思路**：反转链表需要修改当前节点的 `next` 指针的指向为前一个节点，因此需要记录前一个节点和当前节点，当修改完以后还要往后遍历修改下一个节点，而前面修改当前节点的 `next` 指针的指向已经让当前节点与整个链表断开联系，所以需要提前记录当前节点的下一个节点。故分别使用 `prev`、 `cur`、 `next` 表示，其中 `prev` 和 `next` 一开始为空指针，`cur` 为参数节点。循环判断当前节点是否为空，若不为空表示需要反转，用 `next` 保存下一个节点的地址，然后将当前节点的指针指向 `prev`，再更新 `prev` 为当前指针表示此节点已完成反转，最后更新 `cur` 为下一个节点，执行下一个循环，直达循环结束，返回 `prev` 节点即为整个反转后链表的头结点。

代码示例如下：

```cpp
class Solution {
public:
  ListNode* reverseList(ListNode* head) {
    ListNode *prev = nullptr;
    ListNode *cur = nullptr;
    ListNode *next = nullptr;
    cur = head;
    while (cur != nullptr) {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    return prev;
  }
};
```

**测试地址**：[leetcode206 反转链表](https://leetcode.cn/problems/reverse-linked-list/)

### 合并两个有序链表

题目描述如下图所示:

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/af4d211362b849a0bc3b39b9720907cb.png)

**思路**：可以直接在原链表上操作，也可以创建新链表操作，下面使用第二种方式。首先创建一个新链表，然后分别遍历两个旧链表，直到其中一个链表遍历结束则停止遍历，每次遍历都要比较两个链表当前节点的值，将较小的节点添加到新链表中，最后将还有剩余的链表全部添加到新链表中。

代码示例如下：

```cpp
class Solution {
public:
  ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (list1 == nullptr || list2 == nullptr)
      return list1 == nullptr ? list2 : list1;

    ListNode *dummy = new ListNode(-1);
    ListNode *cur = dummy;
    ListNode *l1 = list1;
    ListNode *l2 = list2;
    while (l1 != nullptr && l2 != nullptr) {
      if (l1->val <= l2->val) {
        cur->next = l1;
        l1 = l1->next;
      } else {
        cur->next = l2;
        l2 = l2->next;
      }

      cur = cur->next;
    }

    if (l1 != nullptr)
      cur->next = l1;

    if (l2 != nullptr)
      cur->next = l2;
    
    return dummy->next;
  }
};
```

**测试地址**：[leetcode21 合并两个有序链表](https://leetcode.cn/problems/merge-two-sorted-lists/)

### 两数相加

题目描述如下图所示:

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/751c65bd5e0a40cea997758287efd1f3.png)

**思路**：可以复用老链表，不过下面的实现没有这么做，都是生成的新节点(为了好理解)。创建一个新链表，因为是两数相加，所以需要用一个变量保存进位信息(默认为0)，用一个变量保存求和信息。同时从头结点开始遍历两个链表，只要结点不为空，就将结点的值加到和上，在加上进位信息的值，通过最后的和获取进位信息和值信息，将值信息放到新链表节点中。循环执行，每次都需要将和结果置为 0，最后判断进位信息是否为 1，如果是 1 在新链表中再添加一个值为 1 的节点。

代码示例如下：

```cpp
class Solution {
public:
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode *cur1 = l1;
    ListNode *cur2 = l2;
    int sum = 0;
    int carry = 0;
    ListNode *dummy = new ListNode(-1);
    ListNode *cur = dummy;
    while (cur1 != nullptr || cur2 != nullptr) {
      sum = 0;
      sum += carry;
      if (cur1 != nullptr) {
        sum += cur1->val;
        cur1 = cur1->next;
      }

      if (cur2 != nullptr) {
        sum += cur2->val;
        cur2 = cur2->next;
      }

      carry = sum / 10;
      int val = sum % 10;
      ListNode *new_node = new ListNode(val);
      cur->next = new_node;
      cur = cur->next;
    }

    if (carry != 0) {
      ListNode *new_node = new ListNode(carry);
      cur->next = new_node;
      cur = cur->next;
    }

    return dummy->next;
  }
};
```

**测试地址**：[leetcode2 两数相加](https://leetcode.cn/problems/add-two-numbers/)

### 划分链表

题目描述如下图所示:

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/6601c0a102ff4f96be092499926581cc.png)

**思路**：创建两新链表，分别保存小于目标值的节点和大于等于目标值的节点，不打乱顺序，最后将大于等于的那些节点追加到小于部分的链表后面。由于是直接将整个链表添加到新链表中，因此最后的结果可能会是一个环，为了解决这个问题需要在添加到新链表时，将当前节点与原链表解除关系。

代码描述如下：

```cpp
class Solution {
public:
  ListNode* partition(ListNode* head, int x) {
    if (head == nullptr)
      return nullptr;

    // 使用两个链表分别保存所有小于和大于等于的的节点
    ListNode *small = new ListNode(-1);
    ListNode *large = new ListNode(-1);
    ListNode *p1 = small;
    ListNode *p2 = large;
    while (head != nullptr) {
      if (head->val < x) {
        p1->next = head;
        p1 = p1->next;
      } else {
        p2->next = head;
        p2 = p2->next;
      }

      // 因为在符合条件时，是直接将整个链表添加上去，所以在最后两个链表是尾尾相连的
      // 为了防止这种相连的过程可以在每次添加时断开该节点与原节点的联系
      // ListNode *temp = head->next;
      // head->next = nullptr;
      // head = temp;
      head = head->next;
    }

    // 除了上面的方法外，还有一种方法
    // 因为最后肯定会修改小于部分链表的最后一个结点指向，所以即使是小于部分的尾指向大于等于部分的尾也没有关系
    // 但是如果是大于部分的尾，直接修改小于部分尾的指向是无效的，此时只需要将大于等于部分的尾指向空即可
    p2->next = nullptr;
    // 最后将大于等于的链表直接加到小于链表后
    p1->next = large->next;
    return small->next;
  }
};
```

**测试地址**：[leetcode86 划分链表](https://leetcode.cn/problems/partition-list/)