# ANTLR in Python Interpreter

## 配置教程

### 配置 Antlr C++ 运行环境

Python 解释器采用 Antlr 作为前端语法分析器，其中核心代码编译时间较长，因此我们提前编译好了 Antlr 的运行环境，这样你的程序在编译时就不需要再编译 Antlr 的运行环境了。

为了在你自己的电脑上也使用 Antlr 运行环境，你需要将 Antlr 编译好的运行环境安装到你的电脑上。

将 `antlr4-runtime_4.13.1_amd64.deb` 文件下载到 WSL 中，打开文件所在目录，执行以下命令安装：

```shell
sudo apt install ./antlr4-runtime_4.13.1_amd64.deb
```

在这个包中，含有 Antlr 4.13.1 的动态链接库、静态链接库以及头文件，如果不装这个包，
将导致你的程序在编译时找不到 Antlr 的头文件和动态链接库，从而编译失败。

使用 Archlinux 的同学可以直接使用以下命令安装运行环境:
```shell
pacman -S antlr4-runtime
```

如有在其他环境下编程的同学（比如 Windows、Mac 和除 Debian,Arch 之外的 Linux 系统），请联系助教。

### 生成语法树

#### ~~使用 VScode 插件~~（由于增加语法的原因已弃用）

~~首先在 Windows 环境下安装插件（注意，不要在 WSL 环境下安装）：~~

![vscode-plugin](https://github.com/ACMClassCourse-2023/Python-Interpreter-2023/blob/main/docs/vscode-plugin.png)

~~安装后，点击卸载旁的箭头，安装 2.3.1 版本。~~

![vscode-install](https://github.com/ACMClassCourse-2023/Python-Interpreter-2023/blob/main/docs/vscode-install.png)

~~安装完后重新加载。打开 `Python3.g4` 文件，右边会出现对应插件的图标，点击，等待其中的 PARSER RULES 等部分加载完毕。~~

![vscode-antlr](https://github.com/ACMClassCourse-2023/Python-Interpreter-2023/blob/main/docs/vscode-antlr.png)

~~接下来配置运行文件。点击左侧的运行和调试，创建 `launch.json` 文件，并写入~~

```javascript
{ 
     "version": "2.3.1", 
     "configurations": [ 
     
          { 
               "name": "Debug ANTLR4 grammar", 
               "type": "antlr-debug", 
               "request": "launch", 
               "input": "./a.txt", // 输入文件（即你要运行的 Python 代码文件） 
               "grammar": "./resources/Python3Parser.g4", // 语法文件
               "startRule": "file_input", // 语法入口规则，我们的公式语法入口规则是 file_input
               "printParseTree": true, // 是否 打印/可视化 parse tree 
               "visualParseTree": true 
          } 
     ] 
} 
```

~~最后打开要运行的文件，在左侧的运行和调试中，点击运行即可生成，如下图所示。~~

![vscode-antlr-result](https://github.com/ACMClassCourse-2023/Python-Interpreter-2023/blob/main/docs/vscode-antlr-result.png)

#### ~~使用 Clion 插件~~（由于增加语法的原因已弃用）

~~由于本次 `.g4` 文件的特性，目前 ANTLR 插件只能支持不带 `INDENT` 和 `DEDENT` 规则的解释。~~

~~首先在插件市场中找到插件：~~

![plugin-market](https://github.com/ACMClassCourse-2023/Python-Interpreter-2023/blob/main/docs/plugin-market.png)

~~安装后，右键 `.g4` 中的 `return_stmt` 或任何不包含 `INDENT` 和 `DEDENT` 的规则，点击 `test rule`：~~

![right-click](https://github.com/ACMClassCourse-2023/Python-Interpreter-2023/blob/main/docs/right-click.png)

~~之后在屏幕下方的 `antlr-preview` 中，左侧是待测试的代码，右侧是依据代码生成的语法树结构图。~~

#### 使用ANTLR内置的可视化工具
首先，请确保ANTLR4的jar包已经被正常安装至`/usr/local/lib`。并进入`./resources`目录：

```bash
cd resources
```

然后，运行`visualize.bash`。格式为：

```bash
bash visualize.bash [Parser Rule] -gui
```

其中`[Parser Rule]`表示你希望生成的语法树的根节点的类型，例如：如果你的输入为一整个程序，则参数为`file_input`；如果你希望输入函数定义，则参数为`funcdef`等，具体可用的Parser Rule类型即为Python3Parser.g4中包含的语法规则。

为了确保java生成的GUI界面可以被正常展示，请确保你的操作系统是Win11/最新版Win10/Linux系统，否则你的windows有概率无法正常显示语法树（只有高版本Windows才配了转接WSL图像的库和支持）。

生成可视化语法树基于的文件是`Python3Demo.g4`，除此之外，这个文件没有特殊的作用（即与Coding没有直接关联），因此你不必理解其中的内容。如果要对其进行修改，请确保你理解其中的内容。

## ANTLR 是什么

ANTLR（全名：ANother Tool for Language Recognition）是基于 LL(\*)算法实现的语法解析器生成器（parser generator），用 Java 语言编写，使用自上而下（top-down）的递归下降 LL 剖析器方法。

ANTLR 可以将输入的代码转化成与之对应的**树形结构**，即语法树，以便后续程序操作。按照上面的配置操作，即可得到一份 `Python` 代码对应的语法树。
