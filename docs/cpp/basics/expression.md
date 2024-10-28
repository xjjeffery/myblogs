# 表达式

表达式内容和 C 语言基本相同，其内容也较为简单，在此处不作描述。

## 类型转换

与 C 语言一样，如果两个类型具有某种关联，则在进行表达式运算时，会根据类型转换规则设法将运算对象的类型统一后再求值。这种类型转换是自动执行的，被称作为隐式转换，隐式转换大多数是由较小的类型转换为较大的类型。

隐式转换不需要我们关注，我们程序员关注的是显示类型转换，也叫强制类型转换。在 C 语言中使用简单粗暴的转换方式: `(type)(expression);`。这种在 C++ 中也同样适用，但是这种 C 风格的强制转换可能带来一些隐患，在 C++ 中还有更好的方式，有以下一组提供在不同场合的强制转换函数:

- `static_cast<type>(expression)`：主要用于 C++ 中内置的基本数据类型之间的转换，这种强制转换只会在编译时检查。如果编译器检测到您尝试强制转换完全不兼容的类型，则 `static_cast` 会返回错误。您还可以使用它在基类指针和派生类指针之间强制转换，但是，编译器在无法分辨此类转换在运行时是否是安全的。主要用法：
    - 用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换，进行上行转换（把派生类的指针或引用转换成基类表示）是安全的，进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以不是安全的。
    - 用于基本数据类型之间的转换，如把 `int` 转换成 `char`，把 `int` 转换成 `enum`，这种转换的安全性也要开发人员来保证。
    - 把空指针转换成目标类型的空指针。
    - 把任何类型的表达式转换成 `void` 类型。

```cpp
double d = 1.58947;
int i = d;  // warning
int j = static_cast<int>(d);       // No warning
string s = static_cast<string>(d); // Error: cannot convert from double to std:string

// No error but not necessarily safe.
Base* b = new Base();
Derived* d2 = static_cast<Derived*>(b);

int i = 0x7fff;
long l;
float f;
char c;
// 1. 典型的非强制转换（自动转换）
// 传统方式
l = i;
f = i;
// 提倡的新方式
l = static_cast<long>(i);
f = static_cast<float>(i);
// 2. 窄化转换
// 传统方式
// 会显示警告信息
i = l; // 可能丢失数字
i = f; // 可能丢失信息
c = i; // 可能丢失数字
// 不显示警告信息（但仍然难定位）
i = (int)l;
i = (int)f;
c = (char)i;
// 提倡的新方式（不会显示警告信息，且易定位）
i = static_cast<int>(l);
i = static_cast<int>(f);
c = static_cast<char>(i);
```

- `const_cast<type>(expression)`：常量指针被转换成非常量的指针，并且任然指向原来的对象；常量引用被转换成非常量的引用，并且仍然指向原来的对象。此转换函数一般用于修改指针

```cpp
// 常量指针被转化成非常量的指针，并且仍然指向原来的对象
const int i = 0;
int *pi;
pi = &i; // 错误
pi = (int *)&i; // 被反对
pi = const_cast<int *>(&i);

// 常量引用被转换成非常量的引用，并且仍然指向原来的对象
void Func(double& d) { ... }
void ConstCast() {
  const double pi = 3.14;
  Func(const_cast<double&>(pi)); // No error
}
```

- `reinterpret_cast<type>(expression)`：为运算对象的位模式提供较低层上的重新解释。可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针，此函数使用非常危险，不建议使用。
- `dynamic_cast<type>(expression)`：主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。为了安全，`dynamic_cast` 在运行时检查其基类指针和派生类指针之间的强制转换。`dynamic_cast` 是比 `static_cast` 更安全的强制类型转换，但运行时检查会带来一些开销。

```cpp
#include <iostream>

class Base {
public:
  virtual void doSomethingMore() {
    std::cout << "Base function is running\n";
  }
};

class Derived : public Base {
public:
  void doSomethingMore() {
    std::cout << "Derived function is running\n";
  }
};

int main() {
  Base *b = new Base();
  
  // Run-time check to determine whether b is actually a Derived *
  Derived *d = dynamic_cast<Derived *>(b);
  // If b was originally a Derived *, then d is a valid pointer
  if (d) {
    // Safe to call Derived method.
    d->doSomethingMore();
  } else {
    // Run-time check failed.
    std::cout << "d is null" << std::endl;
  }
  return 0;
}

// Output: d is null;
```