## lexer 内嵌代码解释

本文讲述了 Python3Lexer.g4 文件中的内嵌 C++ 代码的作用，
有兴趣的同学可以尝试理解。

### 内嵌代码的语法

在 antlr 生成的代码中插入 C++ 代码的语法有四类：

1. 文件头部。
```
@header {something}
```
其中 `something` 会被直接拷贝到生成的头文件和源文件中，如 Python3Lexer.h 和 Python3Lexer.cpp。
在这里往往放置 `#include` 指令（C++），或者 `import` 指令（Java）。

2. 成员。
```
@lexer::members {something}
```
这里写入的内容会被拷贝到生成的 Lexer 类中。可以在这里写一些成员变量和成员函数，值得注意的是，可以写函数覆盖基类函数。

3. 匹配条件(Predicates)
```
NEWLINE: {1 + 3 > 4}? '\n';
```
这里括号中是一个表达式，当尝试匹配后面的规则时，会先检测是否满足这个表达式。

4. 动作(Action)
```
NEWLINE: '\n' { std::cout << "Hello, New Line\n"; }
```
匹配成功后会执行的动作。


### 我们用到的内嵌代码

1. 引入了 `list` 和 `regex` 两个头文件

2. 添加了三个成员：`std::list<antlr4::Token *> tokens`，`std::stack<int> indents` 和 `int opened`。

3. 添加了函数 `createDedent`、`make_CommonToken`、`getIndentationCount` 和 `atStartOfInput`。

4. 重写了 Lexer 基类的 `emit` 函数和 `nextToken` 函数。

5. 添加了匹配到换行和三类括号时的特殊行为。

### lexer 工作流程

我们可以在 main.cpp 中看到这样几行代码：
```cpp
ANTLRInputStream input(std::cin);
Python3Lexer lexer(&input);
CommonTokenStream tokens(&lexer);
tokens.fill();
```

在调用了 `fill` 函数后，才真正开始进行词法解析。

我们不去管懒加载之类的操作，只关注于重点的话，流程大致分为如下几步：

1. `fill` 函数调用 `nextToken` 函数
2. `nextToken` 函数调用 `match` 函数
3. `match` 函数在匹配成功后调用 `emit` 函数
4. `emit` 函数接受一个 `Token` 对象，并将该对象存入类成员 `token` 中
5. 经过一些标志位的检查（如是否需要跳过该 Token），`nextToken` 函数返回 `token`。
6. `fill` 函数将 `token` 存入某个地方。

值得注意的是，`antlr` 在上述过程中采用 `unqiue_ptr` 进行内存管理，
`emit` 将传入参数的所有权移交给成员 `token`，而 `nextToken` 函数将 `token` 的所有权移交给返回值。


### 内嵌代码的作用

总得来收，这些内嵌代码的作用就是正确处理缩进问题。

- 在当前有未闭合括号时，忽略所有缩进。
代码中的 `opened` 变量记录了当前有多少个未闭合的括号。

- 每次换行后，计算缩进量。如果缩进量大于当前缩进量，则创建一个 `INDENT` Token，小于则创建一个 `DEDENT` Token。尤其要注意的特殊行为是，如果缩进量等于当前缩进量，则不创建任何 Token，只创建换行。

其中，阅读 `NEWLINE` 的 action 代码可以看到这样一行

```cpp
emit(make_CommonToken(NEWLINE, newLine));
```

这里手动emit了一个 TOKEN。
随后，如果满足条件，可能会 emit 一个 `INDENT` 或 `DEDENT` TOKEN。

因为在祖传的这份g4中，`NEWLINE` 会匹配换行和新行开头的所有缩进，
这导致有可能一个 `NEWLINE` 产生了多个 TOKEN。

而默认的emit函数的行为是将旧 `TOKEN` 析构，
所以我们添加了成员 `tokens`，重写了 `emit` 函数。
同样的原因，我们也要重写 `nextToken` 函数，将 `tokens` 中的 `TOKEN` 依次返回。

这里还有一个小问题是，你可以注意到括号的 action 里面并没有写 `emit`，
但仍然能正常添加 `TOEKN`。我们可以看一下 antlr 的源码：
```cpp
do {
	match(...);
} while (type == MORE);
if (token == nullptr)
	emit();
return std::move(token);
```
这里 `emit()` 是默认的行为，也就是将 `token` 设置为当前匹配出来的TOKEN。
而执行action的代码在 `match` 函数中，
如果action中进行了手动emit，默认的行为就不会发生。
这也就要求我们必须在重写的 `emit` 函数中将成员 `token` 置为非空。


`nextToken`，主要有两个作用。
首先就是要将 `tokens` 中的内容顺序返回。
其次，当遇到文件结束符时，由于我们希望 `suite` 以 `DEDENT` 结尾，
因此要手动再 `emit` 一些 `DEDENT`.
另外一个注意的点是，在这里为了方便直接调用了基类的 `nextToken` 函数。
函数的返回值是 `unique_ptr`，而它存储的内容已经在 `tokens` 中，
因此如果我们不做任何处理，有可能造成提前析构。
（这里假设了 `token` 中存储的一定是真实有效的地址，
我考虑过改成直接在 `emit` 的时候传入一个没用的地址，比如 `0x0001`，
这样可能特征更清晰一些，即便因为一些访问RE了，也很容易知道是这里的问题）

### 一些待解决的问题

`tokens` 没有写内存管理。
