# 解释器作业的完成流程

## Step 1. 配置环境

见文档 [antlr_guide.md](antlr_guide.md)。

## Step 2. 阅读 "./resources/Python3Parser.g4"

**阅读 `.g4` 文件需要一定的正则表达式基础。**
如果你不会正则表达式，可以参考 [正则表达式 - 维基百科](https://zh.wikipedia.org/wiki/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F)。

我们假设有这样的语法规则（并不是我们这次作业的一部分）：

```
    plus: NUMBER '+' NUMBER;
    NUMBER:[0-9]+;
    ADD:'+';
```

## Step 3. 阅读 "./generated/Python3Parser.h"

你可以看到以下代码（对应着上面的语法规则）：

```c++
//...............
    class PlusContext : public antlr4::ParserRuleContext {
    public:
        PlusContext(antlr4::ParserRuleContext *parent, size_t invokingState);
        virtual size_t getRuleIndex() const override;
        std::vector<antlr4::tree::TerminalNode *> NUMBER();
        antlr4::tree::TerminalNode* NUMBER(size_t i);
        antlr4::tree::TerminalNode* ADD();

        virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
        virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

        virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    }
//...............
```

因为在上述的 `plus` 语法中，`NUMBER` 一定要出现至少一次，所以 `PlusContext` 有以下两个函数：

```c++
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
```

第一个函数返回一个 `vector`，包含了指向所有 `NUMBER` 的指针，第二个函数返回指向第 i 个 `NUMBER` 的指针，从 0 开始。

因为 `ADD` 只在 `plus` 中出现了一次，所以它只有以下函数，返回指向唯一的 `ADD` 的指针：

```c++
    antlr4::tree::TerminalNode* ADD()
```

对于一个 `temrinal node`，有以下方法：

```c++
//...............
std::string toString()
Token* TerminalNodeImpl::getSymbol()
/*
 * for example, consider:
 * antlr4::tree::TerminalNode *it;
 * it->toString() returns the string, for example, "123456" or "a"
 * (so you need to converse (std::string)"123456" to (int)123456)
 * it->getSymbol()->getTokenIndex() returns where this word is in the whole input.
 */
//...............
```

## Step 4. 完成 "./src/Evalvisitor.h"

在这一步中，所要做的就是补全相关代码：

```c++
//...............
    std::any visitPlus(Python3Parser::PlusContext *ctx)
    {
        /*
         * TODO
         * the pseudo-code is:
         * return visit(ctx->NUMBER(0))+visit(ctx->NUMBER(1));
         */
    }
//...............
```

当写：

```c++
    visit(ctx->NUMBER(0))
```

等价于写：

```c++
    visitAtom(ctx->NUMBER(0))
```

所以我们只需要用 `visit` 函数来访问各种结点，而不是用 `visitBalabala`，想想为什么？

## Step 5. 编译程序

输入以下代码即可：

```sh
cmake -B build
cmake --build build
```

如果你不会使用 `cmake`，你可以借助于 `Clion` 来实现，如果你不知道这步如何操作，请询问助教。

## 神奇的 std::any

关于这个类，你只需要会一个方法：`std::any_cast`。

譬如说，如果有以下的语法规则：

```
plus: atom '+' atom;
atom: NUMBER | STRING+;
NUMBER:[0-9]+;
STRING:[A-Z]+;
ADD:'+';
```

在 Parser.h 中的 `Context` 长这样：

```c++
//...............
    class PlusContext : public antlr4::ParserRuleContext {
    public:
        PlusContext(antlr4::ParserRuleContext *parent, size_t invokingState);
        virtual size_t getRuleIndex() const override;
        std::vector<AtomContext*> atom();
        AtomContext* atom(size_t i);
        antlr4::tree::TerminalNode* ADD()

        virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
        virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

        virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    }
    //notice that the atom is AtomContext* type rather than a TerminalNode* type.
    //an easy way to tell the difference is: capital letter-TerminalNode; xxxContext otherwise
//...............
```

那么相应的代码就长这样：

```c++
//...............
	std::any visitPlus(Python3Parser::PlusContext *ctx)
    {
        auto ret1 = visit(ctx->NUMBER());
        auto ret2 = visit(ctx->NUMBER());
        if (auto v1int = std::any_cast<int>(&ret1), v2int = std::any_cast<int>(&ret2);
            v1int && v2int)
            return *v1int + *v2int;
        else if (auto v1str = std::any_cast<string>(&ret1), v2str = std::any_cast<string>(&ret2);
            v1str && v2str)
            return *v1str + *v2str;
        else
            throw(std::string("unsupported operand type(s) for +: ") + ret1.type().name() + " + " + ret2.type().name());//no need
    }
//...............
```

我们保证测试文件的语法都是正确的，所以后两行实则是不需要的。

`any_cast` 模板函数可以直接将 `any` 类型转换为你想要的类型，但是如果转换失败，它会直接抛出异常。
而在上文的代码中，我们向模板填入的类型是目标类型的引用，所以如果类型不匹配、转换失败，它会返回 `nullptr`。 

在 OOP 课程中，你们将会学习构造函数和析构函数。所以你们最好是在理解 `std::any` 是如何构造与析构的基础上，进行编程。通过阅读[cppreference](https://zh.cppreference.com/w/cpp/utility/any)来理解。如果这对于你们来说太过困难，请求助助教。

https://www.cnblogs.com/mangoyuan/p/6446046.html

https://www.cnblogs.com/xiaoshiwang/p/9590029.html

搜索 "C++11 traits" 来获得更多信息。
