# 语句

基本控制语句在 C/C++ 中大同小异，没有什么特别难以理解的东西。

## 范围 `for` 语句

在 C++ 中多了一个范围 `for` 语句，这个语句可以遍历容器或其他序列的所有元素。

```cpp
for (declaration : expression)
  // statement
```

## `try` 语句块和异常处理

异常是指存在于运行时的反常行为，这行为超出了函数正常功能的范围。在 C 语言中，我们一般都是通过条件判断防止程序出现异常，并没有一个明确的异常处理函数。C++ 提供了一套异常处理机制和异常处理函数。异常处理包括：

- `throw` 表达式(throw expression)：通过 `throw` 表达式将遇到无法处理的问题抛出
- `try` 语句块(try block)：使用 `try` 语句块处理异常。`try` 语句块以关键字 `try` 开始，并以一个或多个 `catch` 子句结束。`try` 语句块中使用 `throw` 抛出异常，某个 `catch` 子句捕获异常，然后执行对应的异常处理代码。

```cpp
try {
  // program-statements
} catch (exception-declaration) {
  // handler-statements
} catch (exception-declaration) {
  // handler-statements
} // ...
```

!!! example

    ```cpp
    while (cin >> item1 >> item2) {
      try {
        // 执行添加两个 Sales_item 对象的代码
        // 如果添加失败，代码抛出一个 runtime_error 异常
      } catch (runtimer_error err) {
        cout << err.what() << "\nTry Again? Entry y or n\n";
        char c;
        cin >> c;
        if (!cin || c ==  'n')
          break;
      }
    }
    ```

异常类型只定义了一个名为 `what` 的成员函数，该函数没有任何参数，返回一个指向 C 风格字符串的 `const char *`，该字符串提供了关于异常的一些文本。如果异常类型有一个字符串初始值，则 `what` 返回该字符串。对于无出迟滞的异常类型来说，`what` 返回的内容由编译器决定。

如果代码抛出异常，而在 `try` 语句块中没有找到任何匹配的 `catch` 子句，则程序会转到名为 `terminate` 的标准库函数。

C++ 标准库定义了一组类，用于报告标准库函数遇到的问题，这些异常类也可以在用户编写的程序中使用，它们分别定义在如下的四个头文件中：

- `exception` 头文件：最通用的异常类，只报告异常的发生，不提供任何额外信息
- `stdexcept` 头文件：定义几种常见的类型
- `new` 头文件：定义了 `bad_alloc` 异常类型
- `type_info` 头文件：定义了 `bad_cast` 异常类型

![](../assets/basic/stdexcept定义的异常类.png)