#!/bin/bash

# Command format: ./visualize.bash [Parser Rule Name] -gui
# e.g. If you want to see AST for the whole python program, use the following command:
#     ./visualize.bash file_input -gui
# Please ensure that you've already installed ANTLR4 to directory /usr/local/lib/antlr-4.13.1-complete.jar

export CLASSPATH=".:/usr/local/lib/antlr-4.13.1-complete.jar:$CLASSPATH"

# 检查 Python3Demo.g4 文件是否存在
if [ ! -f "Python3Demo.g4" ]; then
    echo "Error: Python3Demo.g4 not found."
    exit 1
fi

# 设置工作目录
output_dir="/tmp/Python3Demo"

# 如果输出目录不存在，则创建它
if [ ! -d "$output_dir" ]; then
    mkdir -p "$output_dir"
fi

# 检查目录权限，若权限不足则修改权限
if [ ! -w "$output_dir" ]; then
    echo "Directory $output_dir is not writable. Changing permissions..."
    sudo chmod 777 -R "$output_dir"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to change permissions for $output_dir."
        exit 1
    fi
    echo "Permissions for $output_dir have been changed."
fi

# 如果目录中没有 Java 文件或文件不存在，才执行 antlr4
if [ ! -f "$output_dir/Python3Demo*.java" ]; then
    echo "Running antlr4 to generate Java files..."
    java -Xmx500M -cp "/usr/local/lib/antlr-4.13.1-complete.jar:$CLASSPATH" org.antlr.v4.Tool -o "$output_dir" Python3Demo.g4
fi

# 如果 Java 文件不存在，或者 .class 文件不存在，则运行 javac
if [ ! -f "$output_dir/Python3Demo.class" ]; then
    echo "Compiling Java files..."
    javac "$output_dir"/Python3Demo*.java
fi

# 切换到输出目录，然后运行 grun（即 java）命令，并将所有参数传递给它
echo "Running grun with arguments: $@"
echo "All finished! Please type your code below. Type Ctrl+D to end. Enjoy coding~"
cd "$output_dir" && java -Xmx500M -cp "/usr/local/lib/antlr-4.13.1-complete.jar:$CLASSPATH" org.antlr.v4.gui.TestRig Python3Demo "$@"
