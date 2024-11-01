# 二分搜索、时间复杂度和空间复杂度

**前置知识**：无，二分搜索内容会的可以跳过，其余内容不建议跳过，时间复杂度不建议跳过。

## 二分搜索

**基本概念**：二分查找是一种基于分治策略的高效搜索算法，利用数组的有序性，每轮缩小一半搜索范围，直至找到的目标元素或搜索区间为空为止。

!!! qusetion "找到目标值"

    给定一个长度为 $n$ 的数组 `nums` ，元素按从小到大的顺序排列且不重复。请查找并返回元素 `target` 在该数组中的索引。若数组不包含该元素，则返回 $-1$ 。示例如下图所示。

    ![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/79c907e0a49342b888fb5d3523b429ae.png)

如下图所示，我们先初始化指针 $i = 0$ 和 $j = n - 1$ ，分别指向数组首元素和尾元素，代表搜索区间 $[0, n - 1]$ 。请注意，中括号表示闭区间，其包含边界值本身。

接下来，循环执行以下两步：

1. 计算中点索引 $m = \lfloor {(i + j) / 2} \rfloor$ ，其中 $\lfloor \: \rfloor$ 表示向下取整操作。
2. 判断 `nums[m]` 和 `target` 的大小关系，分为以下三种情况：
    - 当 `nums[m] < target` 时，说明 `target` 在中间位置的右侧，即区间 $[m + 1, j]$ 中，因此执行 $i = m + 1$ ；
    - 当 `nums[m] > target` 时，说明 `target` 在中间位置的左侧，即区间 $[i, m - 1]$ 中，因此执行 $j = m - 1$ ；
    - 当 `nums[m] = target` 时，说明找到 `target` ，因此返回索引 $m$ ；

若数组不包含目标元素，搜索区间最终会缩小为空，此时返回 $-1$ 。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/57426cd066c84dafa7c848856ab21a67.gif)

值得注意的是，由于 $i$ 和 $j$ 都是 `int` 类型，**因此 $i + j$ 可能会超出 `int` 类型的取值范围**。为了避免大数越界，我们通常采用公式 $m = \lfloor {i + (j - i) / 2} \rfloor$ 来计算中点。

代码如下所示：

```cpp
// 左闭右闭的区间中寻找
int binary_search(std::vector<int> &nums, int target) {
  if (nums.size() <= 0)
    return -1;
  
  sort(nums.begin(), nums.end());
  int ret = -1;
  int left = 0; 
  int right = nums.size() - 1;
  int mid = -1;
  while (left <= right) {
    mid = left + (right - left) / 2;
    if (nums[mid] > target) {
      right = mid - 1;
    } else if (nums[mid] < target) {
      left = mid + 1;
    } else {
      ret = mid;
      break;
    }
  }

  return ret;
}
```

**时间复杂度为 $O(\log n)$** ：在二分循环中，区间每轮缩小一半，因此循环次数为 $\log_2 n$ 。

**空间复杂度为 $O(1)$** ：指针 $i$ 和 $j$ 使用常数大小空间。

### 区间表示方法

除了上述双闭区间外，常见的区间表示还有“左闭右开”区间，定义为 $[0, n)$ ，即左边界包含自身，右边界不包含自身。在该表示下，区间 $[i, j)$ 在 $i = j$ 时为空。

我们可以基于该表示实现具有相同功能的二分查找算法：

```cpp
// 左闭右闭的区间中寻找
int binary_search(std::vector<int> &nums, int target) {
  if (nums.size() <= 0)
    return -1;
  
  sort(nums.begin(), nums.end());
  int ret = -1;
  int left = 0; 
  int right = nums.size();
  int mid = -1;
  while (left < right) {
    mid = left + (right - left) / 2;
    if (nums[mid] > target) {
      right = mid;
    } else if (nums[mid] < target) {
      left = mid + 1;
    } else {
      ret = mid;
      break;
    }
  }

  return ret;
}
```

如下图所示，在两种区间表示下，二分查找算法的初始化、循环条件和缩小区间操作皆有所不同。

由于“双闭区间”表示中的左右边界都被定义为闭区间，因此通过指针 $i$ 和指针 $j$ 缩小区间的操作也是对称的。这样更不容易出错，**因此一般建议采用“双闭区间”的写法**。

![请添加图片描述](https://i-blog.csdnimg.cn/direct/2e9b41667fa440b48a63b1bd27a547d7.png)

### 查找大于等于目标值的左边界

如果有这么一个问题：给定一个长度为 $n$ 的数组 `nums` ，元素按从小到大的顺序排列且不重复。请查找并返回大于等于 `target` 在该数组中的左边界索引。若数组不包含该元素，则返回 $-1$ 。

这类问题也可以使用二分法来查找，其效率也很高，假设有这个一组数 $[2, 4, 4, 5, 7, 8, 10]$，那么大于等于 $6$ 的左边界当然是索引 $4$，这是我们从头遍历逐个对比得到的结果。使用二分法与上述的查找一样：

- 首先获取中间位置的索引 $m = \lfloor {i + (j - i) / 2} \rfloor$；
- 判断 `nums[m]` 和 `target` 的大小关系，分为以下三种情况。
    - 当 `nums[m] < target` 时，说明 `target` 的左边界在中间位置的右侧，即区间 $[m + 1, j]$ 中，只需执行 $i = m + 1$ 。
    - 当 `nums[m] >= target` 时，说明此时的中间位置可以作为左边界，我们可以记录一下，中间位置的左边区域，即区间 $[i, m - 1]$ 中还有没有更左的边界我们是不知道的，因此执行 $j = m - 1$ 继续在左边区域寻找。

代码示例如下：

```cpp
int findLeft(const vector<int>& nums, int target) {
  if (nums.size() <= 0)
    return -1;
  
  sort(nums.begin(), nums.end());
  int ret = -1;
  int left = 0; 
  int right = nums.size() - 1; // 获取中点的位置
  int mid = -1;
  while (left <= right) {
    mid = left + (right - left) / 2;
    if (nums[mid] >= target) {  // 当前位置是左边界的一种可能，左边区域可能还存在更左的边界，向左缩小范围
      ret = mid;
      right = mid - 1;
    } else {  // 当前位置的左边区域不可能存在左边界，到右边区域进一步查找，向右缩小范围
      left = mid + 1;
    }
  }

  return ret;
}
```

### 查找小于等于目标值的右边界

给定一个长度为 $n$ 的数组 `nums` ，元素按从小到大的顺序排列且不重复。请查找并返回小于等于 `target` 在该数组中的右边界索引。若数组不包含该元素，则返回 $-1$ 。

此问题与上述刚好是一个相反的问题，基本的处理框架都是相同的，不同点是在小于等于和大于的条件下，处理逻辑与上述的处理逻辑刚好相反。

代码示例如下：

```cpp
int findRight(const vector<int>& nums, int target) {
  if (nums.size() <= 0)
    return -1;
  
  sort(nums.begin(), nums.end());
  int ret = -1;
  int left = 0; 
  int right = nums.size() - 1;
  int mid = -1;
  while (left <= right) {
    mid = left + (right - left) / 2;  // 获取中点的位置
    if (nums[mid] <= target) {  // 当前位置是右边界的一种可能，右边区域可能还存在更右的边界，向右缩小范围
      ret = mid;
      left = mid + 1;
    } else {  // 当前位置的右边区域不可能存在右边界，到左边区域进一步查找，向左缩小范围
      right = mid - 1;
    }
  }

  return ret;
}
```

如果想验证上面三个算法是否正确，可以使用前一篇中提到的对数器，以查找小于等于目标值的右边界为例，代码示例如下：

```cpp
#include <time.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <algorithm>

const int kTestCounts = 1000;
const int kVectorCapacity = 1000;
const int kMaxValue = 100;

int findRight(std::vector<int> &nums, int target);
int isRight(std::vector<int> &nums, int target);
void generateRandomVector(std::vector<int> &nums, const int vector_capacity, const int max_value);
void printVector(const std::vector<int> &nums);

int main() {
  srand((unsigned)time(nullptr));
  bool flag = true;
  for (int i = 0; i < kTestCounts; ++i) {
    // 1. 创建一个样本
    std::vector<int> nums1;
    generateRandomVector(nums1, kVectorCapacity, kMaxValue);
    // 2. 复制这个样本
    std::vector<int> nums2(nums1);
    // 3. 执行算法并比较
    int target = rand() % kMaxValue;
    if (isRight(nums2, target) != findRight(nums1, target)) {
      printVector(nums1);
      std::cout << "-------------------\n";
      printVector(nums2);
      std::cout << "-------------------\n";
      std::cout << "target = " << target << " find_left = " << findRight(nums1, target) << std::endl;
      flag = false;
      break;
    }
  }

  if (flag) {
    std::cout << "algorithm is successed !\n";
  } else {
    std::cout << "algorithm is failed !\n";
  }

  return 0;
}

void printVector(const std::vector<int> &nums) {
  for (auto num : nums)
    std::cout << num << " ";
  std::cout << '\n';
}

void generateRandomVector(std::vector<int> &nums, const int vector_capacity, const int max_value) {
  int capacity = 0;
  do {
    capacity = rand() % vector_capacity;
  } while (capacity == 0);

  nums.resize(capacity, 0);
  for (int i = 0; i < capacity; ++i) {
    int num = rand() % max_value;
    nums[i] = num;
  }
}

int isRight(std::vector<int> &nums, int target) {
  sort(nums.begin(), nums.end());
  int ret = -1;
  for (int i = nums.size() - 1; i >= 0; --i) {
    if (nums[i] <= target) {
      ret = i;
      break;
    }
  }

  return ret;
}

int findRight(std::vector<int> &nums, int target) {
  if (nums.size() <= 0)
    return -1;
  
  sort(nums.begin(), nums.end());
  int ret = -1;
  int left = 0; 
  int right = nums.size() - 1;
  int mid = -1;
  while (left <= right) {
    mid = left + (right - left) / 2;
    if (nums[mid] <= target) {
      ret = mid;
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return ret;
}
```

### 查找数组中的峰值

二分搜索法只能在有序数组中使用吗，当然不是。比如寻找数组的峰值，可以在无序数组中进行。峰值元素是指其值严格大于左右相邻值的元素。给你一个大小为 $n$ 的整数数组 `nums`，找到峰值元素并返回其索引。数组可能包含多个峰值，在这种情况下，返回任何一个峰值所在位置即可。你可以假设 `nums[-1] = nums[n] = -∞`。

峰值有以下几种可能：

- 只有一个元素，那么峰值就在 $0$ 位置；
- 如果数组的元素大于 $1$
    - 比较 $0$ 位置的元素与 $1$ 位置的元素大小，如果 $0$ 位置大则此位置是峰值；
    - 比较 $n - 1$ 位置的元素与 $n - 2$ 位置的元素大小，如果 $n - 1$ 位置大则此位置是峰值；
- 比较中间位置与其相邻两元素的大小
    - 比较相邻两个元素都大，则此位置是峰值；
    - 大于左边相邻元素，小于右边相邻元素，则右边区域可能存在峰值，故将范围缩小至右半区域；
    - 小于左边相邻元素，大于右边相邻元素，则左边区域可能存在峰值，故将范围缩小至左半区域；


```cpp
int findPeekElement(const vector<int>& nums) {
  if (nums.size() < 1)
    return -1;
  
  int ret = -1;
  int right = nums.size() - 1;
  int left = 1;
  if (nums.size() == 1)
    return 0;

  if (nums[0] > nums[1])
    return 0;
  
  if (nums[right] > nums[right - 1])
    return right;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (nums[mid] > nums[mid - 1] && nums[mid] > nums[mid + 1]) {
      ret = mid;
      break;
    } else if (nums[mid] < nums[mid + 1]) { // 右边区域可能存在峰值
      left = mid + 1;
    } else {  // 左边可能存在峰值
      right = mid - 1;
    }
  }

  return ret;
}
```

**测试地址**：[leetcode162 寻找峰值](https://leetcode.cn/problems/find-peak-element/)

### 二分搜索的优点与局限性

二分查找在时间和空间方面都有较好的性能：

- 二分查找的时间效率高。在大数据量下，对数阶的时间复杂度具有显著优势。例如，当数据大小 $n = 2^{20}$ 时，线性查找需要 $2^{20} = 1048576$ 轮循环，而二分查找仅需 $\log_2 2^{20} = 20$ 轮循环。
- 二分查找无须额外空间。相较于需要借助额外空间的搜索算法(例如哈希查找)，二分查找更加节省空间。

然而，二分查找并非适用于所有情况，主要有以下原因：

- 二分查找仅适用于有序数据。若输入数据无序，为了使用二分查找而专门进行排序，得不偿失。因为排序算法的时间复杂度通常为 $O(n \log n)$ ，比线性查找和二分查找都更高。对于频繁插入元素的场景，为保持数组有序性，需要将元素插入到特定位置，时间复杂度为 $O(n)$ ，也是非常昂贵的。
- 二分查找仅适用于数组。二分查找需要跳跃式（非连续地）访问元素，而在链表中执行跳跃式访问的效率较低，因此不适合应用在链表或基于链表实现的数据结构。
- 小数据量下，线性查找性能更佳。在线性查找中，每轮只需 1 次判断操作；而在二分查找中，需要 1 次加法、1 次除法、1 ~ 3 次判断操作、1 次加法（减法），共 4 ~ 6 个单元操作；因此，当数据量 $n$ 较小时，线性查找反而比二分查找更快。

!!! info "二分答案法"

    二分法还有一个非常重要的算法是 “二分答案法”，这个算法很优秀，在后续的课程里面会有讲解。

## 时间复杂度

时间复杂度统计的是常数操作，一般指固定时间的操作，与执行时间和数据量无关，如加减乘除，寻址等操作。统计以后会有一个渐近表达式，我们排除掉低阶项、常数项以及高阶项系数，最后的表达式就是时间复杂度。时间复杂度分析统计的不是算法运行时间，**而是算法运行时间随着数据量变大时的增长趋势**，当数据量很大很大时，这种趋势的本质可以排除了低阶项、常数时间的干扰。

对于严格固定流程的算法一定要强调最差的时间复杂度，如插入排序，在数组原本就有序的情况下，其时间复杂度就是 O($n$)，但是我们仍要使用其最差的情况作为时间复杂度，即 O($n^2$)。

而那些利用随机行为作为重要部分的算法，要看平均或者期望的时间复杂度，因为最差的时间复杂度无意义。比如我们现在要生成一个相邻不相等的数组，并且只能有 $0$，$1$，$2$，$3$ 这四个数，如果第一次随机生成的数是 $3$，后面每次随机生成的都是 $3$，其时间复杂度就是 O($∞$)，这是毫无意义的。算法流程上利用随机行为作为重要部分的，还有随机快速排序等算法，也只在乎平均或者期望的时间复杂度，因为最差的时间复杂度无意义。

!!! question "假设我有一个动态数组，现在向数组中添加元素，那么数组就会产生扩容(每次以 2 倍的方式扩容)，那么数组扩容的代价是多少？"

    假设数组初始容量是 $0$，当添加一个元素的时候，扩容代价是 O($1$)；再添加一个元素，此时的扩容代价是 O($2$)；再连续添加两个元素的扩容代价均是 O($4$)；在连续添加四个元素的扩容代价是 O($8$)，如此反复，我们会发现扩容的总代价是要小于 $2n$(添加 $n$ 个元素)，那么其扩容的总代价就是 O($n$)，平均到每单个元素添加时，其扩容总代价就是 O($1$)，这就是时间复杂度的均摊。

时间复杂度均摊的意义：在其它算法中有类似动态数组的数据结构时，算法时间复杂度好估计。并查集、单调队列、单调栈、哈希表等结构，均有这个概念。

### 不能以代码结构判断时间复杂度

以代码结构来判断时间复杂度是一个错误的行为，如一个循环的时间复杂度是 O($n$)，两个循环的时间复杂度是 O($n^2$)。时间复杂度只能是对算法流程充分理解才能分析出来，而不是简单的看代码结构，这是一个常见的错误。甚至有些算法的实现用了多层循环嵌套，但时间复杂度是O(N)的。

**一个循环实现冒泡排序**，其复杂度仍然是 O($n^2$)，代码示例如下：

```cpp
void bubbleSort(vector<int>& nums) {
  if (nums.empty() || nums.size() < 2)
    return;
  
  int start = 0;
  int end = nums.size() - 1;
  while (end < 0) {
    if (nums[start] > nums[start + 1]) {
      swap(nums[start], nums[start + 1]);
    }

    if (start < end - 1)
      start++
    else
      start = 0;
  }
}
```

又比如：N/1 + N/2 + N/3 + … + N/N，这个流程的时间复杂度是O(N * logN)，著名的调和级数。

### 常见类型

设输入数据大小为 $n$ ，常见的时间复杂度类型如下图所示（按照从低到高的顺序排列）。

$$
\begin{aligned}
O(1) < O(\log n) < O(n) < O(n \log n) < O(n^2) < O(2^n) < O(n!) \newline
\text{常数阶} < \text{对数阶} < \text{线性阶} < \text{线性对数阶} < \text{平方阶} < \text{指数阶} < \text{阶乘阶}
\end{aligned}
$$

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/34229670c27742dc924933c0106d3a71.png)

时间复杂度非常重要，可以直接判断某个方法能不能通过一个题目，根据数据量猜解法，后续的内容都会讲。

## 空间复杂度

「空间复杂度 space complexity」用于衡量算法占用内存空间随着数据量变大时的增长趋势。这个概念与时间复杂度非常类似，只需将“运行时间”替换为“占用内存空间”。

算法在运行过程中使用的内存空间主要包括以下几种：

- 入参：用于存储算法的输入数据；
- 辅助空间：用于存储算法在运行过程汇总的变量、对象、函数上下文等数据；
- 出参：用于存储算法的输出数据。

一般情况下，空间复杂度是不计入参和出参，只统计为了支持算法而额外开辟的辅助空间。

理想情况下，我们希望算法的时间复杂度和空间复杂度都能达到最优。然而在实际情况中，同时优化时间复杂度和空间复杂度通常非常困难。

**降低时间复杂度通常需要以提升空间复杂度为代价，反之亦然**。我们将牺牲内存空间来提升算法运行速度的思路称为“以空间换时间”；反之，则称为“以时间换空间”。选择哪种思路取决于我们更看重哪个方面。在大多数情况下，时间比空间更宝贵，因此“以空间换时间”通常是更常用的策略。当然，在数据量很大的情况下，控制空间复杂度也非常重要。