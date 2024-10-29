# 函数

C++ 的大部分函数内容与 C 语言基本相同，增加了一些自己的特性从而使函数的使用更加丰富多彩。

## 函数参数

在 C++ 中，函数参数多了一种方式 —— 引用传参，使用引用传参不会发生数据拷贝，如果在函数中不会对参数进行修改，使用 `const` 修饰参数。

如果需要向函数中传递一个数组，在 C 语言中一般都是传递两个参数，一个表示数组的某元素地址，另一个表示元素个数。在 C++ 中可以分别传入数组首元素和尾后元素的指针，这种方法是受到标准库的启发，数组首元素和尾后元素的指针可以使用 `begin()` 和 `end()` 函数获取。

```cpp
#include <iostream>

void printArray(const int *beg, const int *end) {
  while (beg != end)
    std::cout << *beg++ << std::endl;
}

int main() {
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printArray(begin(arr), end(arr));

  return 0;
}
```

C++ 提供了一种新的方式来处理不同数量实参的函数，主要有两个：如果所有的实参类型都相同，可以传递一个名为 `initializer_list` 的标准库类型；如果实参的类型不同，则可以使用可变参数模板。此处只介绍可变参数模板。

`initializer_list` 是一种标准库类型，用于表示某种特定类型的值的数组，但是该数组中的所有元素都是常量值，它是一种模板类型（后续会介绍模板）。

`initializer_list` 提供的操作：

- `initializer_list<T> lst;`：默认初始化，T 类型元素的空列表
- `initializer_list<T> lst{a, b, c, ...};`：lst 的元素数量和初始值一样多，lst 的元素对应初始值副本，列表中的元素是 `const`
- `lst2(lst);/`/`lst2 = lst;`：拷贝或赋值一个 `initializer_list` 对象不会拷贝列表中的元素，拷贝后，原始列表和副本共享元素
- `lst.size();`：列表中的元素数量
- `lst.begin();`：返回指向 lst 中首元素的指针
- `lst.end();`：返回指向 lst 中尾元素下一位置的指针

```cpp
void error_msg(std::initializer_list<string> il) {
  for (auto beg = il.begin(); beg != il.end(); ++beg)
    std::cout << *beg << " ";
  std::cout << std::endl;
}
```

## 函数重载

在 C 语言中是不允许有两个相同函数名的函数出现，但是在 C++ 中可以。C++ 中，允许同一作用域内的几个函数名字相同单但形参列表不同，这种称为函数重载（overloaded）。

!!! warning

    `main` 函数是不能重载的。

形参列表不同指的是参数的类型不同，参数数量不同。如下面所示都是错误的函数重载

```cpp
Record lookup(const Account &);
// bool lookup(const Account &); // 错误，函数返回类型不是函数重载的条件
// Record lookup(const Account &acct); // 错误，只是增加了形参名
// typedef Account acct;
// Record lookup(const acct &);  // 错误，acct 和 Account 是相同的类型
```

如果函数的参数是某种类型的指针或引用，则通过区分其指向的是常量对象还是非常量对象可以实现函数重载，此时的 `const` 是底层的。

```cpp
Record lookup(const Account &); // 函数作用于 Account 的常量引用
Record lookup(Account &);       // 新函数，函数函数作用于 Account 的引用

Record lookup(const Account *); // 函数作用于指向常量 Account 的指针
Record lookup(Account *);       // 新函数，函数函数作用于指向 Account 的指针
```

在实际的项目设计中，参数为非常量对象的函数一般都是由参数为常量对象的参数实现，这得益于 `const_cast`。

```cpp
const std::string &shorterString(const std::string &s1, const std::string &s2) {
  return s1.size() <= s2.size() ? s1 : s2;
}

std::string &shorterString(std::string &s1, std::string &s2) {
  auto &r = shorterString(const_cast<const std::string&>(s1),
                          const_cast<const std::string&>(s2));
  return const_cast<std::string&>(r);
}
```

## 函数参数的默认值

C 语言中是不允许参数拥有默认实参，而 C++ 允许这种做法。调用拥有默认实参的函数时，可以包含该实参，也可以省略该实参，如下所示

```cpp
typedef std::string::size_type sz;
std::string screen(sz ht = 24, sz wid = 80, char backgrnd = ' ');
std::string window;
window = screen();
window = screen(66);
window = screen(66, 256);
window = screen(66, 256, '#');
```

在函数的声明和定义时，函数的参数只能赋予一次默认实参，多次声明也只能为那些没有默认值的参数添加默认实参，而且该形参右侧的所有参数都必须有默认值。也就是说，参数的实参是从参数列表最右侧开始，如果在调用函数传参时，不要跳过某个有默认值的参数，容易引起错误。

## 内联函数

如果需要定义一些规模较小，功能简单的函数，可以将其定义成内联函数(inline)，内联函数可以减少函数调用带来的开销，因为编译器会在编译期间将调用内联函数的地方直接替换成函数内的语句。

```cpp
#include <iostream>

inline const std::string &shorterString(const std::string &s1, const std::string &s2) {
  return s1.size() <= s2.size() ? s1 : s2;
}

int main() {
  std::cout << shorterString(s1, s2) << std::endl;
  // 在编译以后，就编程如下
  std::cout << (s1.size() <= s2.size() ? s1 : s2) << std::endl;

  return 0;
}
```

!!! note

    内联说明只是向编译器发出一个请求，编译器可以选择忽略这个请求。

## `constexpr` 函数

`constepxr` 函数是指能用于常量表达式的函数，该种函数遵循几个约定：函数的返回类型及所有参数的类型都得是字面值类型，而且函数体中必须有且只有一条 `return` 语句。`constexpr` 函数被隐式地指定为内联函数。

```cpp
constexpr int new_sz() { return 42; }
constexpr int foo = new_sz();
```