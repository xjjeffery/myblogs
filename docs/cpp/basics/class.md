# 类

类就是一种类型的抽象，如教师、学生都是人，人就是一个类，而教师和学生则是对象。

C++ 使用类定义自己的数据类型，与 C 语言中的结构体类似，但是 C++ 对类和结构体进行了扩展，在定义数据类型的同时还可以在类或结构体中定义方法等其他新的内容。每个类的实例称为对象，在使用对象时不需要关注此类是如何实现的，可以直接通过对象调用对应的方法，这体现了类的数据抽象思想。

## 类的声明和定义

类使用 `class` 关键字进行声明和定义，其基本方法与结构体的声明和定义相同，如下所示：

```cpp
// 类的声明
class ClassName;

// 类的定义
class ClassName {
  int age;
  std::string name;

  show() {
    std::cout << "age: " << age << " name: " << name << std::endl;
  }
};
```

创建类对象与 C 语言的结构体有点不同，在 C 语言中必须加上 `struct` 关键字才能创建一个结构体对象。而在 C++ 中，可以直接通过 `ClassName` 创建类对象，不需要添加 `class` 关键字。即使在 C++ 中使用 `struct` 创建结构体对象，也不需要添加 `struct` 关键字，这相比 C 语言更加的简单。使用示例如下：

```cpp
ClassName obj1 = {10, "zhangsan"};
obj1.show();

struct StructName {
  int age;
  std::string name;
  show() {
    std::cout << "age: " << age << " name: " << name << std::endl;
  }
};

StructName obj2 = {15, "lisi"};
obj2.show();
```

### `struct` 和 `class` 的区别

通过上面的例子感觉到，在 C++ 中类和结构体非常类似，在 C++ 中都叫做类，那么它们之间有什么不同。在了解它们的不同之处之前，先了解访问控制权限。C++ 中有三种访问说明符：

- `private`：此访问控制权限的数据对象和方法只能允许在类的内部访问，在类的外部无法访问；
- `protected`：此访问控制权限的数据对象和方法允许在类的内部访问，派生类的内部可以访问（后续的继承会有描述），而在类的外部无法访问；
- `public`：此访问控制权限的数据对象和方法在所有地方都能访问。

通过下面的例子来理解：

```cpp
#include <iostream>
#include <string>

class Person {
public:
  std::string name;

protected:
  int salary;

private:
  std::string passwd;

public:
  void func() {
    name = "张三";
    salary = 15000;
    passwd = "123456";
  }
};

int main() {
  Person p;
  p.name = "lisi";
  // p.salary = 10000;  // 错误，protected 数据和成员无法访问
  // p.passwd = "123456";  // 错误，private 数据和成员无法访问
  p.func(); // 类的内部可以访问所有数据和成员

  return 0;
}
```

一个类可以包含 0 个或多个访问说明符，而且对于某个访问说明符能出现多少次也没有严格规定。每个访问说明符指定了接下来的成员的访问级别，其有效范围直到出现下一个访问说明符或者到达类的结尾为止。

使用 `class` 和 `struct` 定义类唯一的区别就是默认的访问权限不同，在不添加任何给访问说明符时，使用 `class` 定义类的默认访问权限是 `private`，而使用 `struct` 定义类的默认访问权限是 `public`。

### 成员函数

在类中声明或定义的函数称为成员函数，成员函数的声明必须在类内部，它的定义可以在类的内部也可以在内的外部。定义在类内部的成员函数是隐式的内联函数，在类外部用 `inline` 关键字修饰函数的定义表示此函数是内联函数。如下示例所示：

```cpp
class SalesData {
public:  
  std::string isbn() const { return book_no_; }
  SalesData &combine(const SalesData &);
  double avg_price() const;

private:
  std::string book_no_;
  unsigned units_sold_ = 0;
  double revenue_ = 0.0;
};
```

关于 `isbn` 函数有一件有意思的事情是：它是如何获得 `book_no_` 成员所依赖的对象的呢？当我们调用一个成员函数时，实际是替某个对象调用它。如果 `isbn` 指向 `SalesData` 的成员，则它隐式地指向调用该函数的对象的成员。

成员函数通过一个名为 `this` 的额外的隐式参数来访问调用它的那个对象，当我们调用一个成员函数时，用请求该函数的对象地址初始化 `this`，所以编译器会把 `total.isbn` 等价认为是 `SalesData::isbn(&total);`，也就是说 `this` 指向 `&total`。

在成员函数内部可以直接使用调用该函数的对象的成员，而无须通过成员访问运算符来做到这一点，因为 `this` 所指的正式这个对象。对于我们来说，`this` 形参是隐式定义的，实际上，任何自定义名为 `this` 的参数或变量的行为都是非法的。

因为 `this` 总是指向 “这个” 对象，所以是一个 const pointer，我们不允许改变 `this` 中保存的地址。

`isbn` 函数的参数列表在之后有一个 `const` 关键字，这里的 `const` 是用来修改隐式 `this` 指针的类型，使其称为一个指向常量对象的常量指针。在默认情况下，`this` 的类型是指向类类型非常量版本的常量指针，如在 `SalesData` 成员函数中，`this` 的类型是 `SalesData * const`。尽管 `this` 是隐式的，但它仍然需要遵循初始化规则。如果此时创建一个 `SalesData` 类型的常量对象，因为这个对象是常量类型，所以不能修改此对象的成员。但是 `SalesData` 类中的 `isbn` 是普通函数以及 `this` 是一个普通的指针参数，有可能会在 `isbn` 函数中对成员进行修改操作，这两者是冲突的。我们需要把 `this` 声明成 `const SalesData * const`，但是 `this` 是隐式的并且不会出现参数列表中，所以在哪将 `this` 声明成指向常量的指针就成为我们必须面对的问题。C++ 语言的做法是允许把 `const` 关键字放在成员函数的参数列表之后，紧跟在参数列表后面的 `const` 表示 `this` 是一个指向常量的指针。像这样使用 `const` 的成员函数被称作常量成员函数（const member function），常量对象以及常量对象的引用或指针都只能调用常量成员函数。

!!! info "成员函数重载"

    通过区分成员函数是否是常量成员函数，可以对其进行重载操作。常量对象会优先调用常量成员函数，而非常量对象虽然可以调用非常量成员函数和常量成员函数，但显然此时非常量版本是一个更好的匹配。

编译器分两步处理类，首先编译成员的声明，然后才轮到成员函数体（如果有的话）。因此成员函数体可以随意使用类中的其他成员而无须在意这些成员出现的次序。所以 `isbn` 可以访问 `book_no_` 不会报错。

当我们在类的外部定义成员函数时，成员函数的定义必须与它的声明匹配。也就是说，返回类型、参数列表和函数名都得与类内部的声明保持一致。如果成员被声明成常量成员函数，那么它的定义也必须在参数列表后明确指定 `const` 属性。同时，类外部定义的成员的名字必须包含它所属的类名。

```cpp
double SalesData::avg_price() const {
  if (units_sold_)
    return revenue_ / units_sold_;
  else
    return 0;
}
```

定义返回 `this` 对象的函数

```cpp
SalesData &SalesData::combine(const SalesData &sa) {
  this->units_sold += sa.units_sold;
  this->revenue += sa.revenue;
  // 返回调用该函数的对象
  return *this;
}
```

该函数一个值得关注的部分是它的返回类型和返回语句。一般来说，当我们定义的函数类似于某个内置运算符时，应该令该函数的行为尽量模仿这个运算符。内置的赋值运算符把它的左侧运算对象当成左值返回，因此为了与它保持一致，`combine` 函数必须返回引用类型。

如果返回 `*this` 对象的函数是一个常量成员函数，那么其返回的类型是一个常量引用，所以我们不能通过此返回值再做后续的修改操作。

### 定义类相关的非成员函数

我们定义非成员函数的方式与定义其他函数一样，通常把函数的声明和定义分离开来。如果函数在概念上属于类但是不定义在类中，则它一般应与类声明（而非定义）在同一个头文件内。在这种方式下，用户使用接口的任何部分都只需要引入一个文件。示例如下：

```cpp
#ifndef SALESDATA_H_
#define SALESDATA_H_

class SalesData {
public:  
  std::string isbn() const { return book_no_; }
  SalesData &combine(const SalesData &);
  double avg_price() const;

private:
  std::string book_no_;
  unsigned units_sold_ = 0;
  double revenue_ = 0.0;
};

SalesData add(const SalesData &lhs, const SalesData &rhs) {
  SalesData sum = lhs;
  sum.combine(rhs);
  return sum;
}

std::ostream &print(std::ostream &os, const SalesData &item) {
  os << item.isbn() << " " << item.units_sold_ << " "
     << item.revenue_ << " " << item.avg_price();
  return os;
}

std::istream &read(std::istream &is, SalesData &item) {
  double price = 0;
  is >> item.book_no_ >> item.units_sold_ >> price;
  item.revenue_ = price * item.units_sold_;
  return is;
}

#endif
```

在编写 `read` 和 `print` 函数时，有两个重要点：

1. `read` 和 `print` 分别接受一个各自 IO 类型的引用作为其参数，这是因为 IO 类属于不能被拷贝的类型，因此只能通过引用来传递它们。而且，因为读取和写入的操作会改变流的内容，所以这两个函数接收的普通引用，而非对常量的引用。
2. `print` 函数不负责换行，一般来说，执行输出任务的函数应该尽量减少对格式的控制，这样可以确保由用户代码来决定是否换行。

## 构造函数

类中有一些特殊的成员函数，用来控制其对象的初始化过程，这类函数叫做构造函数。构造函数的任务时初始化类对象，无论何时只要类的对象被创建，就会执行构造函数。

构造函数的名字和类名相同，和其他函数不一样的是，构造函数没有返回类型；除此之外，与其他普通函数的声明和定义相同。

不同于其他成员函数，构造函数不能被声明成 `const` 的，因为在构造函数中会对成员进行初始化，也就是说会改变成员的值，如果构造函数被声明成 `const`，对象成员就无法初始化，那么这个对象就没有任何数据，也就没有任何意义。因此，创建类的一个 `const` 对象时，直到构造函数完成初始化过程，对象才能真正取得其 “常量” 属性。

但是在前面的 `SalesData` 类中并没有定义构造函数，但是使用了 `SalesData` 对象的程序还是可以正确编译和运行，并且对程序执行了初始化。这是因为类会通过一个特殊的构造函数来控制默认初始化的过程，这个函数叫做默认构造函数，默认构造函数无须任何实参。只要类没有显示地定义构造函数，编译器就会为我们隐式地定义一个默认构造函数并对成员执行默认初始化。

某些类不能依赖编译器隐式定义的默认构造函数，原因有三：

1. 编译器只有在发现类不包含任何构造函数的情况下才会替我们定义一个默认的构造函数，一旦我们定义了一些其他的构造函数，除非我们在定义一个默认的构造函数，否则类将没有默认构造函数。
2. 对于某些类来说，编译器默认定义的构造函数可能执行错误的操作，如内置的数据类型和复合类型，在默认初始化时是未定义的值。
3. 编译器不能为某些类合成默认的构造函数，例如，如果类中包含其他类类型的成员且这个成员的类型没有默认构造类型，那么编译器无法初始化该成员。对于这样的类，必须要自定义构造函数。

```cpp
class SalesData {
public:
  SalesData() = default;
  SalesData(const std::string &s) : book_no_(s) { }
  SalesData(const std::string &s, unsigned n, double p) : 
            book_no_(s), units_sold_(n), revenue(p*n) { }
  SalesData(std::istream &);
  std::string isbn() const { return book_no_; }
  SalesData &combine(const SalesData &);
  double avg_price() const;

private:
  std::string book_no_;
  unsigned units_sold_ = 0;
  double revenue_ = 0.0;
};
```

`SalesData() = default;` 因为该构造函数不接受任何实参，因此它是一个默认构造函数。C++11 新标准中，可以在参数列表后加上 `= default` 来要求编译器生成默认构造函数，表示此构造函数需要默认的初始化行为。

接下来的两个构造函数都出现了冒号和以及冒号和花括号之间的代码，这之间的部分称为构造函数初始值列表，它负责为新创建对象的一个或几个数据成员赋初值。没有出现在函数初始值列表的成员要么通过相应的类内初始化（如果存在的话），或者执行默认初始化。

除了定义类的对象如何初始化之外，类还需要控制拷贝、赋值和销毁对象时发生的行为，这些分别是拷贝构造函数、拷贝赋值函数以及析构函数，在后续会有介绍。

!!! note "成员初始化"

    除了在构造函数中初始化成员，C++11 新标准中，还允许在类内进行初始化，并且必须以符号 `=` 或花括号表示，如示例中的 `units_sold_` 和 `revenue_`。

## 类的控制和封装

### 友元

```cpp
#ifndef SALESDATA_H_
#define SALESDATA_H_

class SalesData {
public:  
  std::string isbn() const { return book_no_; }
  SalesData &combine(const SalesData &);
  double avg_price() const;

private:
  std::string book_no_;
  unsigned units_sold_ = 0;
  double revenue_ = 0.0;
};

SalesData add(const SalesData &lhs, const SalesData &rhs) {
  SalesData sum = lhs;
  sum.combine(rhs);
  return sum;
}

std::ostream &print(std::ostream &os, const SalesData &item) {
  os << item.isbn() << " " << item.units_sold_ << " "
     << item.revenue_ << " " << item.avg_price();
  return os;
}

std::istream &read(std::istream &is, SalesData &item) {
  double price = 0;
  is >> item.book_no_ >> item.units_sold_ >> price;
  item.revenue_ = price * item.units_sold_;
  return is;
}

#endif
```

上述代码其实是错误的，无法正常编译，因为数据成员是 `private` 的，在类的外部是无法访问的，那么 `add`、`print` 以及 `read` 岂不是无法实现。

类可以允许其他类或函数访问它的非公有成员，方法是令其他类或函数称为它的友元（friend），只需要类或者函数声明前加上 `friend` 关键字，并将其放在类定义内部即可。优化后的示例（类之间的友元基本使用是一样的）：

```cpp
#ifndef SALESDATA_H_
#define SALESDATA_H_

class SalesData {
  friend SalesData add(const SalesData &lhs, const SalesData &rhs);
  friend std::ostream &print(std::ostream &os, const SalesData &item);
  friend std::istream &read(std::istream &is, SalesData &item);

public:  
  std::string isbn() const { return book_no_; }
  SalesData &combine(const SalesData &);
  double avg_price() const;

private:
  std::string book_no_;
  unsigned units_sold_ = 0;
  double revenue_ = 0.0;
};

SalesData add(const SalesData &lhs, const SalesData &rhs) {
  SalesData sum = lhs;
  sum.combine(rhs);
  return sum;
}

std::ostream &print(std::ostream &os, const SalesData &item) {
  os << item.isbn() << " " << item.units_sold_ << " "
     << item.revenue_ << " " << item.avg_price();
  return os;
}

std::istream &read(std::istream &is, SalesData &item) {
  double price = 0;
  is >> item.book_no_ >> item.units_sold_ >> price;
  item.revenue_ = price * item.units_sold_;
  return is;
}

#endif
```

!!! note

    - 友元声明只能出现在类定义的内部，但是在类内部出现的具体位置不限，友元不是类的成员也不受它所在区域访问控制级别的约束。
    - 友元的声明仅仅指定了访问的权限，而非真正意义上的函数声明，如果希望在类的某个成员函数中能够调用某个友元函数，那么就必须类定义之前专门对函数进行一次声明。就算在类的内部定义该函数，我们也要在类的外部提供相应的声明从而使得函数可见。

    ```cpp
    struct X {
      friend void f () { /* 友元函数可以定义在类的内部 */ }
      X() { f(); }  // 错误，f 还没被声明
      void g();
      void h();
    };

    void X::g() { return f(); } // 错误，f 还没被声明
    void f();
    void X::h() { return f(); } // 正确，现在 f 的声明在作用域中了
    ```

    - 友元不具备传递性，每个类负责控制自己的友元类或友元函数。
    - 对于一组重载函数，如果想把这组重载函数声明为友元，则必须对每一个函数进行友元声明。

### 定义类型成员

在类中不仅可以定义函数成员和数据之外，类还可以自定义某种类型在类中的别名，这些类型别名与其他成员一样存在访问限制。

```cpp
class Screen {
public:
  typedef std::string::size_type pos;
private:
  pos cursor = 0;
  pos height = 0, width = 0;
  std::string contents;
};
```

用来定义类型的成员必须先定义后使用。

### 可变数据成员

如果我希望能够修改类的某个数据成员，即使是在一个 `const` 成员函数内，可以通过在变量的声明中加入 `mutable` 关键字，表示这个变量用于不会是 `const`，即使它是一个 `const` 对象的成员。

```cpp
class Screen {
public:
  void someMember() const;
private:
  mutable size_t access_ctr_;
};

void Screen::someMember() const {
  ++access_ctr_;
}
```

任何时候只要调用 `someMember` 函数，都可以改变 `access_ctr_` 的值。

## 类的作用域

每个类都会定义它自己的作用域。在类的作用域之外，普通的数据和函数成员只能由对象、引用或者指针使用成员访问运算符来访问。对于类类型成员则使用作用域运算符访问

```cpp
Screen::pos row = 3;
Screen::pos col = 4;
Screen screen(row, col, 'c');
screen.get();
Screen *psc = &screen;
psc->get();
```

类的外部定义成员函数时必须同时提供类名和函数名。在类的内部定义的成员函数不需要提供类名，成员函数内部调用类定义的变量和类型，无需提供类名。举个例子

```cpp
void WindowMgr::clear(ScreenIndex i) {
  // s 是一个 Screen 的引用，指向我们想清空的屏幕
  Screen &s = screens[i];
  // 清空屏幕
  s.contents = string(s.height * s.width, ' ');
}
```

编译器在处理参数列表之前已经明确了我们当前正位于 `WindowMgr` 的作用域中，所以不必再专门说明 `ScreenIndex` 是 `WindowMgr` 类定义的。编译器也能知道函数体中用到的 `screens` 也是在 `WindowMgr` 类中定义的。返回类型是类型成员的话，也需要通过作用域来指明。

变量的查找一般是从内向外，如果内层作用域与外层作用域有同名的变量名，则内部会覆盖掉外部的变量。此时如果想要访问外部变量，则必须通过作用域来指明外部变量的区域，简单的 `::` 表示全局变量。对于类型名则是不同的处理方式，如果类中使用了外层作用域的某个类型名字，则类不能在之后重新定义该名字。

```cpp
typedef double Money;
class Account {
public:
  Money balance() { return bal; }

private:
  typedef double Money; // 错误：不能重新定义 Money
  Money bal;
};
```

## 再探构造函数

### 构造函数初始值列表

类成员的初始化可以通过构造函数的参数列表初始化，也可以在构造函数中赋值完成初始化，虽然效果是相同的，但是两者是有差别的。

如果成员是 `const` 或者引用的话，必须将其初始化。与之类似，当成员属于某种类类型且该类没有定义构造函数时，也必须将这个成员初始化。而这些初始化操作的唯一机会就是通过构造函数初始值，如下所示：

```cpp
class ConstRef {
public:
  ConstRef(int ii);

private:
  int i;
  const int ci;
  int &ri;
};

// 这是错误的方式
ConstRef::ConstRef(int ii) {
  i = ii;   // 正确
  ci = ii;  // 错误，不能给 const 赋值
  ri = i;   // 错误：ri 没被初始化
}

// 正确的方式
ConstRef::ConstRef(int ii)
  : i(ii), ci(ii), ri(i) {
  
}
```

如果对构造函数的所有参数都提供了默认实参，则它实际上也定义了默认的构造函数。

### 委托构造函数

C++11 新标准扩展了构造函数初始值的功能，使得我们可以定义所谓的委托构造函数（delegating constructor）。一个委托构造函数使用它所属类的其他构造函数执行它自己的初始化过程，后者收哦它把它自己的一些（或全部）职责委托给了其他构造函数。

委托构造函数的初始值列表只有一个，就是其他构造函数，如下所示：

```cpp
class SalesData {
public:
  SalesData(std::string s, unsigned cnt, double price)
    : bookNo(s), units_sold(cnt), revenue(price * cnt) { }

  // 其余构造函数全都委托给另一个构造函数
  SalesData() : SalesData("", 0, 0.0) { }
  SalesData(std::string s) : SalesData(s, 0, 0.0) { }
  SalesData(std::istream &is) : SalesData() { read(is, *this); }
  friend std::istream &read(std::istream &, Sales_new &);

private:
  //图书编号
  std::string bookNo;
  //销量
  unsigned units_sold = 0;
  //收入
  double revenue = 0.0;
};
```

### 隐式的类类型转换

如果构造函数只接受一个实参，则它实际上定义了转换为此类类型的隐式转换机制，有时我们把这种构造函数称作转换构造函数。如下所示：

```cpp
string null_bbok = "9-99-999";
item.combine(null_book);
```

这种隐式转换只能发生一步，如下所示的转换就是错误的：

```cpp
item.combine("9-99-999"); // 错误的，这里有两步转换

// 实际步骤
// 1. 将 "9-99-999" 转换成 string
// 2. 将 string 转换成 SalesData
// 下面两个都是正确的
item.combine(string("9-99-999")); // 显示地转换成 string，隐式地转换成 SalesData
item.combine(SalesData("9-99-999")); // 隐式地转换成 string，显示地转换成 SalesData
```

但有时我们并不需要这种转换，因为这种转换可能会带来一种误解。在要求隐式转换（拷贝初始化）的上下文中，我们可以通过将构造函数声明为 `explicit` 关键字抑制构造函数定义的隐式转换。关键字 `explicit` 只对一个实参的构造函数有效，需要多个实参的构造函数不能用于执行隐式转换，所以无须将这些构造函数指定为 `explicit` 的。并且只能在类的内部声明构造时使用 `explicit` 关键字，在外部定义时不需要重复。

当使用 `explicit` 关键字声明构造函数时，它将只能以直接初始化的形式使用。而且，编译器将不会在自动转换过程中使用该构造函数。

### 聚合类

聚合类使得用户可以直接访问其成员，并且具有特殊的初始化语法形式。 当一个类满足如下条件时，我们说它是聚合的： 

- 所有成员都是 `public`的
- 没有定义任何构造函数
- 没有类内初始值
- 没有基类，也没有 `virtual` 函数 

以下是聚合类

```cpp
struct Data {
  int ival;
  string s;
};
```

### 字面值常量类

数据成员都是字面值类型的聚合类是字面值常量类。如果一个类不是聚合类，但它符合下述要求，则它也是一个字面值常量类：

- 数据成员都必须是字面值类型
- 类必须至少含有一个 `constexpr` 构造函数
- 如果一个数据成员含有类内初始值，则内置类型成员的初始值必须是一条常量表达式；或者如果成员属于某种类类型，则初始值必须使用成员自己的 `constexpr` 构造函数。
- 类必须使用析构函数的默认定义，该成员负责销毁类的对象

尽管构造函数不能是 `cons` t的，但是字面值常量类的构造函数可以是 `constexpr` 函数。事实上，一个字面值常量类必须至少提供一个 `constexpr` 构造函数。`constexpr` 构造函数就必须既符合构造函数的要求不能有返回语句，所以要用参数列表的方式初始化类。`constexpr` 构造函数体一般来说应该是空的。

字面值常量类如下所示：

```cpp
class Debug {
public:
  constexpr Debug(bool b = true) : hw(b), io(b), other(b) { }
  constexpr Debug(bool h, bool i, bool o) : hw(h), io(i), other(o) { }
  constexpr bool any() { return hw || io || other; }
  void set_io(bool b) { io = b; }
  void set_hw(bool b) { hw = h; }
  void set_other(bool b) { other = b; }

private:
  bool hw;    // 硬件错误，而非 IO 错误
  bool io;    // IO 错误
  bool other; // 其他错误
};
```

## 类的静态成员

先看下面的例子：

```cpp
class Account {
public:
  void calculate() { amount += amount * interest_rate; }
  static double rate() { return interest_rate; }
  static void rate(double);

private:
  std::string owner;
  double amount;
  static double interest_rate;
  static double initRate();
};

double Account::interest_rate = 3.5;
```

如果在类的成员函数或数据前加上 `static` 关键字，表示此声明为静态成员。类的静态成员和成员函数都存在于任何对象之外，对象中不包含任何与静态数据成员有关的数据，静态成员函数中不包含 `this` 指针，因此不能在静态成员函数中使用 `this` 指针，也不能将静态成员函数声明为 `const`。因此上面的每个 `Account` 对象只包含两个数据成员：`owner` 和 `amount`，没有任何一个成员函数。

静态成员或成员函数可以直接通过作用域运算符直接访问，也可以在创建对象以后通过对象来访问。在定义静态成员函数或成员变量时，`static` 关键字只出现在类内部的声明语句中，并且必须在类的外部进行定义和初始化每个静态成员。但也存在类内部对静态成员初始化，如下所示:

```cpp
class Account {
public:
  void calculate() { amount += amount * interest_rate; }
  static double rate() { return interest_rate; }
  static void rate(double);

private:
  std::string owner;
  double amount;
  static double interest_rate;
  static double initRate();
  static constexpr int period = 30;
  double daily_tbl[period];
};

double Account::interest_rate = 3.5;
```

像这种只是限于编译器替换它的值的情况，则初始化一个 `const` 或 `constexpt static` 不需要分别定义。如果有某个函数的参数接受 `const int &` 时，则必须在内外部定义 `period`（不需要再赋初始值）。

因此，静态成员只能在静态函数中使用，并且不能在静态函数中使用非静态成员，因为静态成员的创建时间早于非静态成员。