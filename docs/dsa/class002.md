# 三傻排序、对数器

**前置知识**：无，三傻排序会的可以直接跳过，对数器一定要理解

三傻排序是所有排序中时间复杂度最差的（时间复杂度请看后面的内容），在实际的工作中，插入排序在数据量小的时候还会用到，其余两个基本上不会用到。

## 三傻排序

### 选择排序

选择排序的逻辑很简单，首先找到数组中最小的那个元素，其次，将它和数组的第一个元素交换（如果第一个元素就是最小的，那么就和自己交换）。再次，在剩下的元素中找到最小的元素，将它与数组的第二个元素交换位置。如此反复，直到整个数组排序。

设数组的长度为 $n$ ，选择排序的算法流程如下图所示：

1. 初始状态下，所有元素未排序，即未排序（索引）区间为 $[0, n-1]$ 。
2. 选取区间 $[0, n-1]$ 中的最小元素，将其与索引 $0$ 处的元素交换。完成后，数组前 1 个元素已排序。
3. 选取区间 $[1, n-1]$ 中的最小元素，将其与索引 $1$ 处的元素交换。完成后，数组前 2 个元素已排序。
4. 以此类推，经过 $n - 1$ 轮选择与交换后，数组前 $n - 1$ 个元素已排序。
5. 仅剩的一个元素必定是最大元素，无须排序，因此数组排序完成。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/695c3bb40d0f43bbb2fd07db80d5b79c.gif)

选择排序的算法实现：

```cpp
void swap(int *num1, int *num2) {
  int tmp = *num1;
  *num1 = *num2;
  *num2 = tmp;
}

void selectSort(std::vector<int> &nums) {
  int min_index = 0;
  for (int i = 0; i < nums.size(); ++i) {
    min_index = i;
    for (int j = i+1; j < nums.size(); ++j) {
      if (nums[min_index] > nums[j])
        min_index = j;
    }

    swap(&nums[i], &nums[min_index]);
  }
}
```

**算法特性**：

- **时间复杂度为 $O(n^2)$、非自适应排序**：外循环共 $n - 1$ 轮，第一轮的未排序区间长度为 $n$ ，最后一轮的未排序区间长度为 $2$ ，即各轮外循环分别包含 $n$、$n - 1$、$\dots$、$3$、$2$ 轮内循环，求和为 $\frac{(n - 1)(n + 2)}{2}$ 。
- **空间复杂度为 $O(1)$、原地排序**：指针 $i$ 和 $j$ 使用常数大小的额外空间。
- **非稳定排序**：如下图所示，元素 `nums[i]` 有可能被交换至与其相等的元素的右边，导致两者的相对顺序发生改变。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/62778b2578ff4629a9681832bf0acde6.png)

### 冒泡排序

冒泡排序的排序原理是：通过连续地比较与交换相邻元素实现排序。这个过程就像气泡从底部升到顶部一样，因此得名冒泡排序。

通过下面的动图可知，冒泡过程可以利用元素交换操作来模拟：从数组最左端开始向右遍历，依次比较相邻元素大小，如果“左元素 > 右元素”就交换二者。遍历完成后，最大的元素会被移动到数组的最右端。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/8332467088444749b48b4a4c05938d7d.gif)

冒泡排序的代码实现：

```cpp
void swap(int *num1, int *num2) {
  int tmp = *num1;
  *num1 = *num2;
  *num2 = tmp;
}

void bubbleSort(std::vector<int> &nums) {
  for (int i = nums.size() - 1; i >= 0; --i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] > nums[j + 1])
        swap(&nums[j], &nums[j+1]);
    }
  }
}
```

上面的代码还可以进行优化，如果数组中的元素是有序的，在一轮比较操作完成以后不会执行任何交换操作，我们可以直接返回结果。因此，可以增加一个标志位 `flag` 来监测这种情况，一旦出现就立即返回。经过优化，冒泡排序的最差时间复杂度和平均时间复杂度仍为 $O(n^2)$ ；但当输入数组完全有序时，可达到最佳时间复杂度 $O(n)$ 。

```cpp
void swap(int *num1, int *num2) {
  int tmp = *num1;
  *num1 = *num2;
  *num2 = tmp;
}

void bubbleSort(std::vector<int> &nums) {
  bool flag = true;
  for (int i = nums.size() - 1; i >= 0; --i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] > nums[j + 1]) {
        swap(&nums[j], &nums[j+1]);
        flag = false;
      }
    }

    if (flag)
      break;
  }
}
```

**算法特性**：

- **时间复杂度为 $O(n^2)$、自适应排序**：各轮“冒泡”遍历的数组长度依次为 $n - 1$、$n - 2$、$\dots$、$2$、$1$ ，总和为 $(n - 1) n / 2$ 。在引入 `flag` 优化后，最佳时间复杂度可达到 $O(n)$ 。
- **空间复杂度为 $O(1)$、原地排序**：指针 $i$ 和 $j$ 使用常数大小的额外空间。
- **稳定排序**：由于在“冒泡”中遇到相等元素不交换。

### 插入排序

插入排序十分常见，如我们在打牌的时候，通常会将扑克牌划分为“有序”和“无序”两部分，并假设初始状态下最左 1 张扑克牌已经有序。在无序部分抽出一张扑克牌，插入至有序部分的正确位置；完成后最左 2 张扑克已经有序。不断循环步骤插牌，每一轮将一张扑克牌从无序部分插入至有序部分，直至所有扑克牌都有序。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/64ab2a1e09ee4e28a9203c64abd199ad.png)

通过上面的整理扑克牌可以直到插入排序的排序原理：在未排序区间选择一个基准元素，将该元素与其左侧已排序区间的元素逐一比较大小，如果小于已排序区间的元素则继续向前直到最左侧或大于已排序区间的元素，插入在当前位置。如此反复，直到所有元素完成排序。

插入排序的算法实现：

```cpp
void swap(int *num1, int *num2) {
  int tmp = *num1;
  *num1 = *num2;
  *num2 = tmp;
}

void insertSort(std::vector<int> &nums) {
  for (int i = 1; i < nums.size(); ++i) {
    for (int j = i - 1; j >= 0 && nums[j] > nums[j+1]; --j) {
      swap(&nums[j], &nums[j+1]);
    }
  }
}
```

**算法特性**：

- **时间复杂度为 $O(n^2)$、自适应排序**：在最差情况下，每次插入操作分别需要循环 $n - 1$、$n-2$、$\dots$、$2$、$1$ 次，求和得到 $(n - 1) n / 2$ ，因此时间复杂度为 $O(n^2)$ 。在遇到有序数据时，插入操作会提前终止。当输入数组完全有序时，插入排序达到最佳时间复杂度 $O(n)$ 。
- **空间复杂度为 $O(1)$、原地排序**：指针 $i$ 和 $j$ 使用常数大小的额外空间。
- **稳定排序**：在插入操作过程中，我们会将元素插入到相等元素的右侧，不会改变它们的顺序。

**插入排序的优势**：

插入排序的时间复杂度为 $O(n^2)$ ，而我们即将学习的快速排序的时间复杂度为 $O(n \log n)$ 。尽管插入排序的时间复杂度更高，**但在数据量较小的情况下，插入排序通常更快**。

这个结论与线性查找和二分查找的适用情况的结论类似。快速排序这类 $O(n \log n)$ 的算法属于基于分治策略的排序算法，往往包含更多单元计算操作。而在数据量较小时，$n^2$ 和 $n \log n$ 的数值比较接近，复杂度不占主导地位，每轮中的单元操作数量起到决定性作用。

实际上，许多编程语言（例如 Java）的内置排序函数采用了插入排序，大致思路为：对于长数组，采用基于分治策略的排序算法，例如快速排序；对于短数组，直接使用插入排序。

虽然冒泡排序、选择排序和插入排序的时间复杂度都为 $O(n^2)$ ，但在实际情况中，**插入排序的使用频率显著高于冒泡排序和选择排序**，主要有以下原因：

- 冒泡排序基于元素交换实现，需要借助一个临时变量，共涉及 3 个单元操作；插入排序基于元素赋值实现，仅需 1 个单元操作。因此，**冒泡排序的计算开销通常比插入排序更高**。
- 选择排序在任何情况下的时间复杂度都为 $O(n^2)$ 。**如果给定一组部分有序的数据，插入排序通常比选择排序效率更高**。
- 选择排序不稳定，无法应用于多级排序。

## 对数器

当我们在网上找到某个公司的面试题，想了好久，感觉自己会做，但是找不到在线测试；又或者在和朋友交流面试题时，有一个新的题，感觉自己会做，但也没有在线测试；或者在网上进行刷题练习时，前几个测试用例都过了，突然出现一个巨大无比的数据量，结果我们的代码报错，但是我们根本看不出哪里出错，甚至有的根本不提示哪个例子出错，无法 Debug 等情况下，如何验证自己实现的算法。

对数器就是一个针对没有测试环境或测试环境有限制的时候，自己对算法实现进行验证的一个工具，主要有以下几步：

1. 实现一个随机样本产生器，生成大小随机以及元素随机的数组；
2. 复制多个相同的样本，用于算法的验证
3. 针对问题实现算法 A，这个算法的实现一般来说绝对正确并且容易实现，但是复杂度不好（一般最多是暴力法）；
4. 实现你想要验证的算法 B；
5. 如果有一个随机样本使得结果不一致，可以进行打印输出等方式查看出错的原因，并修改算法；如果在一个很大的样本下，两个算法的结果都是一致的，则认为算法 a 是正确的。

对数器中样本大小可以手动控制，当算法出错时，我们可以将样本变小，使用这个较小的作物样本进行 Debug。最常用的排查方式有 print 大法、断点技术等。

下面我们使用对数器对之前的三个排序进行验证，代码示例如下：

```cpp
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
void mySwap(T& val1, T& val2) {
  T temp = val1;
  val1 = val2;
  val2 = temp;
}

// 插入排序
template<typename T>
void insertionSort(vector<T>& vec) {
  if (vec.empty() || vec.size() < 2)
    return;

  for (int i = 1; i < vec.size(); ++i) {
    for (int j = i - 1; j >= 0 && vec[j] > vec[j+1]; --j) {
      mySwap(vec[j], vec[j+1]);
    }
  }
}

// 冒泡排序
template<typename T>
void bubbleSort(vector<T>& vec) {
  if (vec.empty() || vec.size() < 2)
    return;

  bool flag = true;
  for (int i = vec.size() - 1; i > 0; --i) {
    for (int j = 0; j < i; ++j) {
      if (vec[j] > vec[j+1]) {
        mySwap(vec[j], vec[j+1]);
        flag = false;
      }
    }

    if (flag)
      break; 
  }
}

// 选择排序
template<typename T>
void selectionSort(vector<T>& vec) {
  if (vec.empty() || vec.size() < 2)
    return;

  int min_index = 0;
  for (int i = 0; i < vec.size() - 1; ++i) { 
    min_index = i;
    for (int j = i + 1; j < vec.size(); ++j) {
      if (vec[j] < vec[min_index])
        min_index = j;
    }
    mySwap(vec[min_index], vec[i]);
  }
}

// 生成一个随机数组
void generateRandomVector(vector<int>& nums, const int max_capacity, const int max_value) {
  int capacity = rand() % max_capacity;
  if (capacity == 0)
    return;
  
  nums.resize(capacity);
  for (int i = 0; i < capacity; ++i) {
    int val = rand() % max_value;
    nums[i] = val;
  }
}

vector<int> copyVector(const vector<int>& nums) {
  return vector<int>(nums);
}

bool isEqual(const vector<int>& nums1, const vector<int>& nums2) {
  if (nums1.size() != nums2.size())
    return false;
  
  for (int i = 0; i < nums1.size(); ++i) {
    if (nums1[i] != nums2[i])
      return false;
  }
  return true;
}

void printArray(const vector<int> &nums) {
  for (auto i : nums) cout << i << " ";
  cout << '\n';
}

int main() {
  srand((unsigned)time(0));
  const int kTestCounts = 1000000; // 测试次数
  const int kMaxCapcity = 100;  // 数组的最大容量
  const int kMaxValue = 100;    // 数组元素的最大值
  cout << "测试开始\n";
  vector<int> nums1;
  for (int i = 0; i < kTestCounts; ++i) {
    // 1. 生成一个随机样本
    generateRandomVector(nums1, kMaxCapcity, kMaxValue);
    // 2. 复制样本
    vector<int> nums2 = copyVector(nums1);
    vector<int> nums3 = copyVector(nums1);
    // 3. 实现算法
    bubbleSort(nums1);
    selectionSort(nums2);
    insertionSort(nums3);
    // 4. 比较每个算法的结果，用于 Debug
    if (!isEqual(nums1, nums2) || !isEqual(nums2, nums3)) {
      cout << "出错了!\n";
      cout << "-------nums1-------\n";
      printArray(nums1);
      cout << "-------nums2-------\n";
      printArray(nums2);
      cout << "-------nums3-------\n";
      printArray(nums3);
    }
  }

  cout << "测试结束，算法正确\n";
  vector<int> nums;
  generateRandomVector(nums, kMaxCapcity, kMaxValue);
  printArray(nums);
  bubbleSort(nums);
  printArray(nums);

  return 0;
}
```

输出结果（一种可能）：

```bash
测试开始
测试结束，算法正确
44 87 65 29 40 8 14 35 59 64 97 40 82 84 83 21 11 0 66 86 22 65 86 57 28 88 56 2 91 85 82 88 72 99 17 65 60 31 0 19 95 49 11 29 86 95 3 49 47 21 35 21 39 74 30 67 62 86 69 6 23 3 94 96 55 11 13 15 94 13 34 89 14 97 71 0 44 74 49 92 
0 0 0 2 3 3 6 8 11 11 11 13 13 14 14 15 17 19 21 21 21 22 23 28 29 29 30 31 34 35 35 39 40 40 44 44 47 49 49 49 55 56 57 59 60 62 64 65 65 65 66 67 69 71 72 74 74 82 82 83 84 85 86 86 86 86 87 88 88 89 91 92 94 94 95 95 96 97 97 99
```

对数器的门槛其实是比较高的，因为往往需要在两种不同思路下实现功能相同的两个算法，暴力一个、想象中的最优解是另一个。后续的很多题目都会用到对数器，几乎可以验证任何方法，尤其在验证贪心、观察规律方面很有用。