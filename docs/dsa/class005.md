# 认识栈和队列

**前置知识**：数组和链表，会的可以直接跳过，对于固定数组实现双端队列值得一看。

## 栈

栈（stack）是一种遵循先入后出逻辑的线性数据结构。我们可以将栈类比为桌面上的一摞盘子，如果想取出底部的盘子，则需要先将上面的盘子依次移走。我们将盘子替换为各种类型的元素（如整数、字符、对象等），就得到了栈这种数据结构。如下图所示，我们把堆叠元素的顶部称为“栈顶”，底部称为“栈底”。将把元素添加到栈顶的操作叫作“入栈”，删除栈顶元素的操作叫作“出栈”。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/3c923c4a70cf48c796572bdb45b1987f.png)
### 栈的常用操作

栈的常用操作如下表所示，具体的方法名需要根据所使用的编程语言来确定。在此，我们以常见的 `push()`、`pop()`、`peek()` 命名为例。

| 方法 | 描述 | 时间复杂度 |
| -------- | ---------------------- | ---------- |
| `push()` | 元素入栈（添加至栈顶） | $O(1)$     |
| `pop()`  | 栈顶元素出栈           | $O(1)$     |
| `peek()` | 访问栈顶元素           | $O(1)$     |

通常情况下，我们可以直接使用编程语言内置的栈类。然而，某些语言可能没有专门提供栈类，这时我们可以将该语言的“数组”或“链表”当作栈来使用，并在程序逻辑上忽略与栈无关的操作。

栈使用示例代码：

```cpp
/* 初始化栈 */
stack<int> stack;

/* 元素入栈 */
stack.push(1);
stack.push(3);
stack.push(2);
stack.push(5);
stack.push(4);

/* 访问栈顶元素 */
int top = stack.top();

/* 元素出栈 */
stack.pop(); // 无返回值

/* 获取栈的长度 */
int size = stack.size();

/* 判断是否为空 */
bool empty = stack.empty();
```

### 栈的实现

为了深入了解栈的运行机制，我们来尝试自己实现一个栈类。栈遵循先入后出的原则，因此我们只能在栈顶添加或删除元素。然而，数组和链表都可以在任意位置添加和删除元素，**因此栈可以视为一种受限制的数组或链表**。换句话说，我们可以“屏蔽”数组或链表的部分操作，使其对外表现的逻辑符合栈的特性。

#### 基于链表的实现

使用链表实现栈时，我们可以将链表的头节点视为栈顶，尾节点视为栈底。如下图所示，对于入栈操作，我们只需将元素插入链表头部，这种节点插入方法被称为“头插法”。而对于出栈操作，只需将头节点从链表中删除即可。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/c52eded490cc4c98bc5af60a64db9f4c.gif)


以下是基于链表实现栈的示例代码：

```cpp
/* 基于链表实现的栈 */
class LinkedListStack {
private:
  ListNode *stackTop; // 将头节点作为栈顶
  int stkSize;        // 栈的长度

public:
  LinkedListStack() {
    stackTop = nullptr;
    stkSize = 0;
  }

  ~LinkedListStack() {
    // 遍历链表删除节点，释放内存
    freeMemoryLinkedList(stackTop);
  }

  /* 获取栈的长度 */
  int size() {
    return stkSize;
  }

  /* 判断栈是否为空 */
  bool isEmpty() {
    return size() == 0;
  }

  /* 入栈 */
  void push(int num) {
    ListNode *node = new ListNode(num);
    node->next = stackTop;
    stackTop = node;
    stkSize++;
  }

  /* 出栈 */
  int pop() {
    int num = top();
    ListNode *tmp = stackTop;
    stackTop = stackTop->next;
    // 释放内存
    delete tmp;
    stkSize--;
    return num;
  }

  /* 访问栈顶元素 */
  int top() {
    if (isEmpty())
      throw out_of_range("栈为空");
    return stackTop->val;
  }

  /* 将 List 转化为 Array 并返回 */
  vector<int> toVector() {
    ListNode *node = stackTop;
    vector<int> res(size());
    for (int i = res.size() - 1; i >= 0; i--) {
      res[i] = node->val;
      node = node->next;
    }
    return res;
  }
};
```
#### 基于数组的实现

使用数组实现栈时，我们可以将数组的尾部作为栈顶。如下图所示，入栈与出栈操作分别对应在数组尾部添加元素与删除元素，时间复杂度都为 $O(1)$ 。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/5682665052324079a8c6ff717276b26f.gif)

由于入栈的元素可能会源源不断地增加，因此我们可以使用动态数组，这样就无须自行处理数组扩容问题。以下为示例代码：

```cpp
/* 基于数组实现的栈 */
class ArrayStack {
private:
  vector<int> stack;

public:
  /* 获取栈的长度 */
  int size() {
    return stack.size();
  }

  /* 判断栈是否为空 */
  bool isEmpty() {
    return stack.size() == 0;
  }

  /* 入栈 */
  void push(int num) {
    stack.push_back(num);
  }

  /* 出栈 */
  int pop() {
    int num = top();
    stack.pop_back();
    return num;
  }

  /* 访问栈顶元素 */
  int top() {
    if (isEmpty())
      throw out_of_range("栈为空");
    return stack.back();
  }

  /* 返回 Vector */
  vector<int> toVector() {
    return stack;
  }
};
```

> 在算法刷题时，算法输入的数据量都会提供，所以对我们来说输入的最大数据量是已知的，可以直接创建该大小的数组，然后使用该数组实现栈，效率要比库中提供的要高。

#### 两种实现对比

**支持操作**：

- 两种实现都支持栈定义中的各项操作。数组实现额外支持随机访问，但这已超出了栈的定义范畴，因此一般不会用到。

**时间效率**：

- 在基于数组的实现中，入栈和出栈操作都在预先分配好的连续内存中进行，具有很好的缓存本地性，因此效率较高。然而，如果入栈时超出数组容量，会触发扩容机制，导致该次入栈操作的时间复杂度变为 $O(n)$ 。
- 在基于链表的实现中，链表的扩容非常灵活，不存在上述数组扩容时效率降低的问题。但是，入栈操作需要初始化节点对象并修改指针，因此效率相对较低。不过，如果入栈元素本身就是节点对象，那么可以省去初始化步骤，从而提高效率。

综上所述，当入栈与出栈操作的元素是基本数据类型时，例如 `int` 或 `double` ，我们可以得出以下结论：

- 基于数组实现的栈在触发扩容时效率会降低，但由于扩容是低频操作，因此平均效率更高。
- 基于链表实现的栈可以提供更加稳定的效率表现。

**空间效率**：

在初始化列表时，系统会为列表分配“初始容量”，该容量可能超出实际需求；并且，扩容机制通常是按照特定倍率（例如 2 倍）进行扩容的，扩容后的容量也可能超出实际需求。因此，**基于数组实现的栈可能造成一定的空间浪费**。然而，由于链表节点需要额外存储指针，**因此链表节点占用的空间相对较大**。

综上，我们不能简单地确定哪种实现更加节省内存，需要针对具体情况进行分析。

### 栈的典型应用

- **浏览器中的后退与前进、软件中的撤销与反撤销**。每当我们打开新的网页，浏览器就会对上一个网页执行入栈，这样我们就可以通过后退操作回到上一个网页。后退操作实际上是在执行出栈。如果要同时支持后退和前进，那么需要两个栈来配合实现。
- **程序内存管理**。每次调用函数时，系统都会在栈顶添加一个栈帧，用于记录函数的上下文信息。在递归函数中，向下递推阶段会不断执行入栈操作，而向上回溯阶段则会不断执行出栈操作。

## 队列

队列 （queue）是一种遵循先入先出规则的线性数据结构。顾名思义，队列模拟了排队现象，即新来的人不断加入队列尾部，而位于队列头部的人逐个离开。

如下图所示，我们将队列头部称为“队首”，尾部称为“队尾”，将把元素加入队尾的操作称为“入队”，删除队首元素的操作称为“出队”。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/8ceb01aeeb984739a28ed23b1476c97b.png)

### 队列常用操作

队列的常见操作如下表所示。需要注意的是，不同编程语言的方法名称可能会有所不同。我们在此采用与栈相同的方法命名。

| 方法名   | 描述 | 时间复杂度 |
| -------- | ---------------------------- | ---------- |
| `push()` | 元素入队，即将元素添加至队尾 | $O(1)$     |
| `pop()`  | 队首元素出队                 | $O(1)$     |
| `peek()` | 访问队首元素                 | $O(1)$     |

我们可以直接使用编程语言中现成的队列类，队列使用示例代码：

```cpp
/* 初始化队列 */
queue<int> queue;

/* 元素入队 */
queue.push(1);
queue.push(3);
queue.push(2);
queue.push(5);
queue.push(4);

/* 访问队首元素 */
int front = queue.front();

/* 元素出队 */
queue.pop();

/* 获取队列的长度 */
int size = queue.size();

/* 判断队列是否为空 */
bool empty = queue.empty();
```

### 队列实现

为了实现队列，我们需要一种数据结构，可以在一端添加元素，并在另一端删除元素，链表和数组都符合要求。可以通过[leetcode622 设计循环队列](https://leetcode.cn/problems/design-circular-queue/)来测试自己实现的队列。

#### 基于链表的实现

如下图所示，我们可以将链表的“头节点”和“尾节点”分别视为“队首”和“队尾”，规定队尾仅可添加节点，队首仅可删除节点。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/1f04f343f82647cfbd23813382966f5b.gif)

用链表实现队列的代码：

```cpp
/* 基于链表实现的队列 */
class LinkedListQueue {
private:
  ListNode *front, *rear; // 头节点 front ，尾节点 rear
  int queSize;

public:
  LinkedListQueue() {
    front = nullptr;
    rear = nullptr;
    queSize = 0;
  }

  ~LinkedListQueue() {
    // 遍历链表删除节点，释放内存
    freeMemoryLinkedList(front);
  }

  /* 获取队列的长度 */
  int size() {
    return queSize;
  }

  /* 判断队列是否为空 */
  bool isEmpty() {
    return queSize == 0;
  }

  /* 入队 */
  void push(int num) {
    // 在尾节点后添加 num
    ListNode *node = new ListNode(num);
    // 如果队列为空，则令头、尾节点都指向该节点
    if (front == nullptr) {
      front = node;
      rear = node;
    }
    // 如果队列不为空，则将该节点添加到尾节点后
    else {
      rear->next = node;
      rear = node;
    }
    queSize++;
  }

  /* 出队 */
  int pop() {
    int num = peek();
    // 删除头节点
    ListNode *tmp = front;
    front = front->next;
    // 释放内存
    delete tmp;
    queSize--;
    return num;
  }

  /* 访问队首元素 */
  int peek() {
    if (size() == 0)
      throw out_of_range("队列为空");
    return front->val;
  }

  /* 将链表转化为 Vector 并返回 */
  vector<int> toVector() {
    ListNode *node = front;
    vector<int> res(size());
    for (int i = 0; i < res.size(); i++) {
      res[i] = node->val;
      node = node->next;
    }
    return res;
  }
};
```
#### 基于数组的实现

在数组中删除首元素的时间复杂度为 $O(n)$ ，这会导致出队操作效率较低。然而，我们可以采用以下巧妙方法来避免这个问题。

我们可以使用一个变量 `front` 指向队首元素的索引，并维护一个变量 `size` 用于记录队列长度。定义 `rear = front + size` ，这个公式计算出的 `rear` 指向队尾元素之后的下一个位置。

基于此设计，**数组中包含元素的有效区间为 `[front, rear - 1]`**，各种操作的实现方法如下图所示。

- 入队操作：将输入元素赋值给 `rear` 索引处，并将 `size` 增加 1 。
- 出队操作：只需将 `front` 增加 1 ，并将 `size` 减少 1 。

可以看到，入队和出队操作都只需进行一次操作，时间复杂度均为 $O(1)$ 。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/965907eb8bcc4261a0b71b4fadf765a6.gif)

你可能会发现一个问题：在不断进行入队和出队的过程中，`front` 和 `rear` 都在向右移动，**当它们到达数组尾部时就无法继续移动了**。为了解决此问题，我们可以将数组视为首尾相接的“环形数组”。

对于环形数组，我们需要让 `front` 或 `rear` 在越过数组尾部时，直接回到数组头部继续遍历。这种周期性规律可以通过“取余操作”来实现，代码如下所示：

```cpp
/* 基于环形数组实现的队列 */
class ArrayQueue {
private:
  int *nums;       // 用于存储队列元素的数组
  int front;       // 队首指针，指向队首元素
  int queSize;     // 队列长度
  int queCapacity; // 队列容量

public:
  ArrayQueue(int capacity) {
    // 初始化数组
    nums = new int[capacity];
    queCapacity = capacity;
    front = queSize = 0;
  }

  ~ArrayQueue() {
    delete[] nums;
  }

  /* 获取队列的容量 */
  int capacity() {
    return queCapacity;
  }

  /* 获取队列的长度 */
  int size() {
    return queSize;
  }

  /* 判断队列是否为空 */
  bool isEmpty() {
    return size() == 0;
  }

  /* 入队 */
  void push(int num) {
    if (queSize == queCapacity) {
      cout << "队列已满" << endl;
      return;
    }
    // 计算队尾指针，指向队尾索引 + 1
    // 通过取余操作实现 rear 越过数组尾部后回到头部
    int rear = (front + queSize) % queCapacity;
    // 将 num 添加至队尾
    nums[rear] = num;
    queSize++;
  }

  /* 出队 */
  int pop() {
    int num = peek();
    // 队首指针向后移动一位，若越过尾部，则返回到数组头部
    front = (front + 1) % queCapacity;
    queSize--;
    return num;
  }

  /* 访问队首元素 */
  int peek() {
    if (isEmpty())
      throw out_of_range("队列为空");
    return nums[front];
  }

  /* 将数组转化为 Vector 并返回 */
  vector<int> toVector() {
    // 仅转换有效长度范围内的列表元素
    vector<int> arr(queSize);
    for (int i = 0, j = front; i < queSize; i++, j++) {
      arr[i] = nums[j % queCapacity];
    }
    return arr;
  }
};
```

以上实现的队列仍然具有局限性：其长度不可变。然而，这个问题不难解决，我们可以将数组替换为动态数组，从而引入扩容机制，有兴趣的读者可以尝试自行实现。

两种实现的对比结论与栈一致，在此不再赘述。

**队列典型应用**：

- **淘宝订单**。购物者下单后，订单将加入队列中，系统随后会根据顺序处理队列中的订单。在双十一期间，短时间内会产生海量订单，高并发成为工程师们需要重点攻克的问题。
- **各类待办事项**。任何需要实现“先来后到”功能的场景，例如打印机的任务队列、餐厅的出餐队列等，队列在这些场景中可以有效地维护处理顺序。

## 双端队列

在队列中，我们仅能删除头部元素或在尾部添加元素。如下图所示，双端队列（double-ended queue）提供了更高的灵活性，允许在头部和尾部执行元素的添加或删除操作。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/74851d7976a541c080498ef5154403a1.png)

### 双端队列常用操作

双端队列的常用操作如下表所示，具体的方法名称需要根据所使用的编程语言来确定。

| 方法名         | 描述 | 时间复杂度 |
| -------------- | ---------------- | ---------- |
| `push_first()` | 将元素添加至队首 | $O(1)$     |
| `push_last()`  | 将元素添加至队尾 | $O(1)$     |
| `pop_first()`  | 删除队首元素     | $O(1)$     |
| `pop_last()`   | 删除队尾元素     | $O(1)$     |
| `peek_first()` | 访问队首元素     | $O(1)$     |
| `peek_last()`  | 访问队尾元素     | $O(1)$     |

同样地，我们可以直接使用编程语言中已实现的双端队列类，双端队列使用示例：

```cpp
/* 初始化双端队列 */
deque<int> deque;

/* 元素入队 */
deque.push_back(2);   // 添加至队尾
deque.push_back(5);
deque.push_back(4);
deque.push_front(3);  // 添加至队首
deque.push_front(1);

/* 访问元素 */
int front = deque.front(); // 队首元素
int back = deque.back();   // 队尾元素

/* 元素出队 */
deque.pop_front();  // 队首元素出队
deque.pop_back();   // 队尾元素出队

/* 获取双端队列的长度 */
int size = deque.size();

/* 判断双端队列是否为空 */
bool empty = deque.empty();
```

### 双端队列实现

双端队列的实现与队列类似，可以选择链表或数组作为底层数据结构。使用 [leetcode641 设计循环双端队列](https://leetcode.cn/problems/design-circular-deque/)测试实现的双端队列。

#### 基于双向链表的实现

回顾上一节内容，我们使用普通单向链表来实现队列，因为它可以方便地删除头节点（对应出队操作）和在尾节点后添加新节点（对应入队操作）。对于双端队列而言，头部和尾部都可以执行入队和出队操作。换句话说，双端队列需要实现另一个对称方向的操作。为此，我们采用“双向链表”作为双端队列的底层数据结构。

如下图所示，我们将双向链表的头节点和尾节点视为双端队列的队首和队尾，同时实现在两端添加和删除节点的功能。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/a16b72e2108f49ee9223422fd51a0476.gif)

实现代码如下所示：

```cpp
class MyCircularDeque {
public:
  MyCircularDeque(int k) : capacity_(k), size_(0) {}

  bool insertFront(int value) {
    if (isFull())
      return false;

    list_.push_front(value);
    size_++;
    return true;
  }

  bool insertLast(int value) {
    if (isFull())
      return false;

    list_.push_back(value);
    size_++;
    return true;
  }

  bool deleteFront() {
    if (isEmpty())
      return false;

    list_.pop_front();
    size_--;
    return true;
  }

  bool deleteLast() {
    if (isEmpty())
      return false;

    list_.pop_back();
    size_--;
    return true;
  }

  int getFront() {
    if (isEmpty()) {
      return -1;
    }

    return list_.front();
  }

  int getRear() {
    if (isEmpty()) {
      return -1;
    }

    return list_.back();
  }

  bool isEmpty() { return size_ == 0; }

  bool isFull() { return size_ == capacity_; }

private:
  int capacity_;
  int size_;
  list<int> list_;
};
```

#### 基于数组的实现

如下图所示，与基于数组实现队列类似，我们也可以使用环形数组来实现双端队列。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/5c6fcf115b584608a4b8c538c323e4c7.gif)

在队列的实现基础上，仅需增加“队首入队”和“队尾出队”的方法：

```cpp
class MyCircularDeque {
public:
  MyCircularDeque(int k) : capacity_(k), size_(0), arr_(new int[capacity_]), front_(0), end_(0) {}

  bool insertFront(int value) {
    if (isFull())
      return false;
    if (isEmpty()) {
      front_ = 0;
      end_ = 1;
    } else {
      front_ = (front_ == 0 ? (capacity_ - 1) : (front_ - 1));
    }
    arr_[front_] = value;
    size_++;
    return true;
  }

  bool insertLast(int value) {
    if (isFull())
      return false;

    if (isEmpty()) {
      front_ = capacity_ - 1;
      end_ = 0;
    }
    arr_[end_] = value;
    end_ = (end_ + 1) == capacity_ ? 0 : (end_ + 1);
    size_++;
    return true;
  }

  bool deleteFront() {
    if (isEmpty())
      return false;

    front_ = (front_ + 1) == capacity_ ? 0 : (front_ + 1);
    size_--;
    return true;
  }

  bool deleteLast() {
    if (isEmpty())
      return false;

    end_ = end_ == 0 ? (capacity_ - 1) : (end_ - 1);
    size_--;
    return true;
  }

  int getFront() {
    if (isEmpty()) {
      return -1;
    }

    return arr_[front_];
  }

  int getRear() {
    if (isEmpty()) {
      return -1;
    }

    int index = end_ == 0 ? (capacity_ - 1) : (end_ - 1);
    return arr_[index];
  }

  bool isEmpty() { return size_ == 0; }

  bool isFull() { return size_ == capacity_; }

private:
  int capacity_;
  int size_;
  int *arr_;
  int front_;
  int end_;
};
```

### 双端队列应用

双端队列兼具栈与队列的逻辑，**因此它可以实现这两者的所有应用场景，同时提供更高的自由度**。

我们知道，软件的“撤销”功能通常使用栈来实现：系统将每次更改操作 `push` 到栈中，然后通过 `pop` 实现撤销。然而，考虑到系统资源的限制，软件通常会限制撤销的步数（例如仅允许保存 $50$ 步）。当栈的长度超过 $50$ 时，软件需要在栈底（队首）执行删除操作。**但栈无法实现该功能，此时就需要使用双端队列来替代栈**。请注意，“撤销”的核心逻辑仍然遵循栈的先入后出原则，只是双端队列能够更加灵活地实现一些额外逻辑。

## 栈与队列例题

### 用栈实现队列

问题描述如下图所示

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/fc181a0666e1475ab334018a839000d4.png)

**思路**：使用两个栈实现队列，由于栈是后进先出，所以先将数据逐个添加一个栈以后，在将这个栈中的数据出栈到另一个栈，此时该栈中数据元素的顺序与队列相同。但是将数据添加到第一个栈的时候，必须确保另一个栈是空的；将此栈中的数据放到另一个栈中，必须一次性倒完，也就是说必须确保至少有一个栈时刻为空。

代码示例如下：

```cpp
class MyQueue {
public:
  MyQueue() {}

  void push(int x) {
    while (!out.empty()) {
      in.push(out.top());
      out.pop();
    }
    in.push(x);
    while (!in.empty()) {
      out.push(in.top());
      in.pop();
    }
  }

  int pop() {
    int val = out.top();
    out.pop();
    return val;
  }

  int peek() {
    return out.top();
  }

  bool empty() {
    return in.empty() && out.empty();
  }

private:
  stack<int> in;
  stack<int> out;
};
```

**测试地址**：[leetcode232 用栈实现队列](https://leetcode.cn/problems/implement-queue-using-stacks/)

### 用队列实现栈

问题描述如下图所示

![<div align="center"> <img src="./assets/sector013_016/impl_stack_by_queue.png" /> </div> <br/>](https://i-blog.csdnimg.cn/direct/49b5cf231613473d95b37720cfa8e46e.png)

**思路**：由于队列是先近先出的，所以需要确保队头的元素永远都是新添加的元素，只要在新加元素时，将当前除刚添加的元素一次出队再重新入队。

代码示例如下：

```cpp
class MyStack {
public:
  MyStack() {

  }
  
  void push(int x) {
    qu.push(x);
    for (int i = 0; i < qu.size() - 1; ++i) {
      qu.push(qu.front());
      qu.pop();
    }
  }
  
  int pop() {
    int val = qu.front();
    qu.pop();
    return val;
  }
  
  int top() {
    return qu.front();
  }
  
  bool empty() {
    return qu.empty();
  }

private:
  queue<int> qu;
};
```

**测试地址**：[leetcode225 用队列实现栈](https://leetcode.cn/problems/implement-stack-using-queues/)

### 最小栈

问题描述如下图所示

![<div align="center"> <img src="./assets/sector013_016/min_stack.png" /> </div> <br/>](https://i-blog.csdnimg.cn/direct/5c114cb468c14d80a51927c858e5dbb9.png)

**思路**：此题使用一个栈是很难实现，需要一个辅助栈用来保存最小值，因此有两个栈。如果有数据需要入栈，则保存数据的栈直接入栈，然后判断辅助栈是否为空，如果为空则直接入栈；如果不为空，则比较辅助栈的栈顶元素与新添元素，如果栈顶元素小于新添元素，则再次将栈顶元素入栈一次，否则将新添元素入栈。

> 直接使用库中的栈比自己用数组实现栈效率还是要差一点，主要是因为相差在常数操作的时间，并不是时间复杂度。因为题目中输入的最大数据量是已知的，所以数组大小可以是固定的。

代码示例如下：

```cpp
// 提交时把类名、构造方法改成MinStack
class MinStack1 {
public:
  MinStack() {

  }
  
  void push(int val) {
    st.push(val);
    if (min.empty() || min.top() > val)
      min.push(val);
    else
      min.push(min.top());
  }
  
  void pop() {
    if (st.empty())
      return;
    
    st.pop();
    min.pop();
  }
  
  int top() {
    return st.top();
  }
  
  int getMin() {
    return min.top();
  }

private:
  stack<int> st;
  stack<int> min;
};

// 提交时把类名、构造方法改成MinStack
class MinStack2 {
public:
  MinStack()
    : arr_(new int[80001]), min_(new int[80001]), size_(0) {

  }
  
  void push(int val) {
    if (size_ == 80001)
      return;

    arr_[size_] = val;
    if (size_ == 0 || min_[size_-1] >= val) {
      min_[size_] = val;
    } else {
      min_[size_] = min_[size_-1];
    }
    size_++;
  }
  
  void pop() {
    if (size_ == 0)
      return;

    size_--;
  }
  
  int top() {
    return arr_[size_ - 1];
  }
  
  int getMin() {
    return min_[size_ - 1];
  }

private:
  int *arr_;
  int *min_;
  int size_;
};
```

**测试地址**：[leetcode155 最小栈](https://leetcode.cn/problems/min-stack/)

!!! note

    栈和队列虽然很简单，但是很重要，尤其是双端队列，后续的很多算法都是建立在栈和队列之上。