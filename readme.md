# 🐍Python Interpreter

## 🧾 目录

- [✨ 简介](#✨简介)
- [📚 作业说明](#📚作业说明)
  - [⚠️ 实现要求](#⚠️实现要求)
  - [🛎️ 评测方式](#🛎️评测方式)
    - [提交方式](#提交方式)
    - [测试点分布](#测试点分布)
  - [💎Bonus](#💎Bonus)
- [📝Guide](#📝Guide)
  - [📄 语法](#📄语法)
  - [⚙️ANTLR](#⚙️Antlr)
  - [🧪 实现](#🧪实现)
  - [📇 索引](#📇索引)

## ✨简介

本次大作业要求你们实现一个简单的 Python 解释器，接受简化过的 Python 代码，按照控制流执行代码。

> 解释器（Interpreter），是一种计算机程序，能够把解释型语言解释执行。
> 解释器就像一位“中间人”。解释器边解释边执行，因此依赖于解释器的程序运行速度比较缓慢。
> 解释器的好处是它不需要重新编译整个程序，从而减轻了每次程序更新后编译的负担。
> 相对的，编译器一次性将所有源代码编译成二进制文件，执行时无需依赖编译器或其他额外的程序。

## 📚作业说明

### ⚠️实现要求

1. 使用 OOP 实现 Python Interpreter，锻炼 OOP 能力。若不按照要求会在 code review 时扣除一定分数。
2. 作业不低于按数据总的通过比例线性给分。助教会下发部分数据在作业仓库中，还有另一部分数据不会提供。
   这意味着你需要自己手写测试数据给自己测试，如果你对自己造的数据是否满足要求有疑问，请及时向助教询问。
3. 首先完成 Prework 里面的前置任务。前置任务不计分，但要求**必做**。如果不完成前置任务则 Python 解释器将没有分数。

### 🛎️评测方式

使用 `git` 在 OJ 上进行提交。OJ 将根据根目录下的 `CMakeLists.txt` 来构建你的程序。

#### 提交方式

在代码提交页面输入你 `git` 仓库的地址

#### 测试点分布

测试点分布如下：

```text
BigIntegerTest: 1 - 20 
Sample: 21 - 34 
AdvancedTest: 35 - 52 
ComplexTest: 53 - 56 
CornerTest: 57 - 66 
```

### 💎Bonus

1. 修改 `.g4` 与 `Evalvisitor` 来支持更高级的语法规则
2. 增加语法检查
3. 不使用 Antlr4，自己实现 Lexer 和 Parser

实现任何的 bonus 之前，都请与助教联系。

## 📝Guide

关于本次大作业的所有问题，都可以在 [Q&A](https://notes.sjtu.edu.cn/s/wD8G6yIgw#) 中或者本仓库的 issue 中提问。助教会更新在 Q&A 和文档中。

### 📄语法

本次作业使用的 Python 的语法在 [Grammar](docs/grammar.md) 查看。

### ⚙️ANTLR

关于 ANTLR 的安装与使用，详见 [ANTLR](docs/antlr_guide.md)。

### 🧪实现

如果你想知道从何下手，可以参考 [完成流程](docs/workflow_details.md) 与 [实现细节](docs/implementation_details.md)。

### 📇索引

如果你想查看一段 Python 代码，通过 ANTLR 生成的语法树的结构，参考：[1](docs/antlr_guide.md#antlr-配置)。

如果你想知道 `std::any` 是什么，参考：[完成流程](docs/workflow_details.md) 和 [前置任务](Prework/README.md)。

如果你想知道 `ctx` 大致是什么，参考：[实现细节](docs/implementation_details.md)。

如果你想知道怎么遍历树，参考：[4](docs/workflow_details.md#step-4-完成-srcevalvisitorh)。

如果你想看一个 demo 来理解这个作业，参考：[5](https://github.com/ACMClassCourse-2021/Apple-Pie-Interpreter)。

还有一些 [小建议](docs/suggestions.md)。
