# 一、功能概述
Simple Shell 是一个用 C++ 编写的简单命令行解释器，支持基本的命令执行、I/O 重定向和管道操作。它还支持执行简单的 shell 脚本。<br>

## 题目1. shell 程序能够提供命令的输入，执行并显示执行结果的功能
### ① 命令提示
· 当程序启动时，显示欢迎信息：Welcome to The Shell! <br>
· 提示符包含当前路径信息:eg：/home/yin/My_Shell $ <br>
![image](https://github.com/YEIIEY/My_shell/assets/127741388/1450688b-6a33-40d4-ad0f-3006dbe10476)

### ② 目录操作
· ls：列出当前目录的内容 <br>
. pwd: 显示当前工作目录<br>
· mkdir:创建新目录<br>
· cd: 切换当前工作目录<br>
· mv：移动或重命名文件或目录<br>
![image](https://github.com/YEIIEY/My_shell/assets/127741388/fbc86268-f73b-4efb-b81f-1fe7ef8fc13c)

### ③ 文件查看命令：
· cat：显示文件内容<br>
· head：显示文件的前几行<br>
· head file.txt head -n 1 hello.txt： 显示前 1 行<br>
![1-文件查看命令](https://github.com/YEIIEY/My_shell/assets/127741388/bc13a53a-2c94-4319-8f12-a3476900a194)

### ④ 系统信息命令：
· df -h：显示磁盘使用情况<br>
· free -h：显示内存使用情况<br>
· uptime：显示系统运行时间和负载<br>
![1-系统信息命令](https://github.com/YEIIEY/My_shell/assets/127741388/35a9f38f-6c67-4162-8fbd-0e446537702a)

### ⑤ 进程管理命令：
· top：实时显示系统资源使用情况和进程信息<br>
![1-进程管理命令](https://github.com/YEIIEY/My_shell/assets/127741388/6bf56d29-bcdf-4e47-8b73-f981b13377f4)

## 题目2. shell 程序能够提供可 shell 编程的功能，能够执行简单的 shell 脚本。
### · cat test_script.sh：查看脚本内容<br>
该脚本文件包含以下内容<br>
echo "Hello from script!"<br>
echo " Current Directory: $(pwd)"<br>
ls -l<br>
### · ./my_shell test_script.sh：执行自定义的 shell 程序并运行脚本
![2-shell 编程的功能](https://github.com/YEIIEY/My_shell/assets/127741388/c44b9f4b-d03b-4305-a521-e72bb5938ecb)

## 题目3. shell 程序能够提供 I/O 重定向和管道的功能。
### 重定向
· ls > output.txt：将 ls 输出重定向到 output.txt<br>
· cat output.txt：查看 output.txt 的内容<br>
· echo 'hello' > hello.c：将字符串 "hello" 重定向到 hello.c 文件<br>
· wc -l < fruit.txt：计算 fruit.txt 文件的行数<br>
· sort < fruit.txt > sort_fruit.txt：将 fruit.txt 内容排序并重定向到 sort_fruit.txt 文件<br>
其中原本fruit.txt文件为：<br>
banana<br>
orange<br>
apple<br>
· echo "lemon" >> fruit.txt：将字符串 "lemon" 追加到 fruit.txt 文件<br>
![3-重定向](https://github.com/YEIIEY/My_shell/assets/127741388/59bd1a74-bb8f-4c82-af50-fa4847838ba6)
![3-重定向2](https://github.com/YEIIEY/My_shell/assets/127741388/d756057b-1238-4335-b4d4-c55d3a20e6a7)

### 管道管理
· grep -o 'hello' hello.txt | wc -l：查找并计数特定单词<br>
· cat hello.txt | grep 'world'：过滤并打印包含特定单词的行<br>
· cat hello.txt | tr ' ' '\n' | sort | uniq -c：单词频率统计<br>
![3-管道管理](https://github.com/YEIIEY/My_shell/assets/127741388/dd25396c-f13a-4565-b9e7-f777e035e252)

## 错误处理
· 详细的错误信息提示<br>
未输入命令时提示：Error: Next command shouldn't be empty! Please check and try again :D<br>
I/O 文件格式错误：Error: I/O file format error! Please check and try again :D<br>
字符串缺失引号：Error: Your str miss ' or "! Please check and try again :D<br>
进程创建失败：Error: Failed to fork process!<br>
命令执行失败：Error: Failed to execute cmd <command>! Please check and try again :D<br>
进程退出失败：Error: Failed to exit with status <status>! Please check and try again :D<br>
![错误处理](https://github.com/YEIIEY/My_shell/assets/127741388/d70e320c-6a7f-4468-8db7-c4cd9377b796)

# 二、开发文档
## 1.系统结构
Simple Shell 由以下主要模块组成：<br>
· 命令解析模块：解析用户输入的命令行，支持管道和 I/O 重定向。<br>
· 命令执行模块：通过 fork 和 execvp 函数创建子进程并执行命令。<br>
· 错误处理模块：处理各种错误情况并提供详细的错误信息。<br>

## 2.核心流程
### 2.1. 命令解析<br>
用户输入的命令行字符串首先通过 splitCommands 函数进行解析，分割成多个 Command 对象。每个 Command 对象包含命令、参数及 I/O 重定向信息。<br>

### 2.2 命令执行<br>
解析后的命令列表通过 executeCommands 函数进行执行。根据命令数量不同，采用不同的执行策略：<br>
· 单个命令：直接执行。<br>
· 两个命令：使用管道连接。<br>
· 多个命令：使用多个管道连接。<br>

## 3. UML 图
### 3.1. 用例图
#### 描述：
用例图展示了用户与 Simple Shell 系统之间的交互，主要包括用户输入命令、Simple Shell 解析命令、执行命令并返回结果。<br>
#### 文字描述：
1. 用户：通过命令行界面与 Simple Shell 交互，输入命令并期望得到结果。<br>
2. Simple Shell：接收用户输入的命令，解析并执行命令，并将结果返回给用户。<br>
   · 输入命令：用户输入命令字符串。<br>
   · 解析命令：Simple Shell 解析命令字符串，将其分割为命令、参数和重定向信息。<br>
   · 执行命令：Simple Shell 创建子进程并执行解析后的命令。<br>
   · 返回结果：将命令的执行结果返回给用户，输出到命令行界面。<br>
![用例图](https://github.com/YEIIEY/My_shell/assets/127741388/d7677ad1-2259-47fd-a675-a740bb78c0f5)

### 3.2.类图
#### 描述：
类图展示了 Simple Shell 系统中的主要类及其属性和方法。<br>
#### 文字描述：
1. Shell 类：负责整体流程控制，包括命令的解析和执行。<br>
  · 属性：<br>
    · prompt：命令提示符字符串。<br>
  · 方法：<br>
    · run()：主运行函数，循环接收和处理用户命令。<br>
    · parseCommand(input)：解析用户输入的命令。<br>
    · executeCommand(cmd)：执行解析后的命令。<br>
2. Command 类：表示一个命令及其参数和重定向信息。<br>
  · 属性：<br>
    · command：命令名称字符串。<br>
    · args：命令参数列表。<br>
    · inputRedirect：输入重定向文件。<br> 
    · outputRedirect：输出重定向文件。<br>
  · 方法：<br>
    · __init__(command, args, inputRedirect, outputRedirect)：构造函数，初始化命令对象。<br>
![类图](https://github.com/YEIIEY/My_shell/assets/127741388/9390117e-bffc-4e32-9c0a-32096e604462)

### 3.3. 时序图
#### 描述：
时序图展示了用户输入命令、Simple Shell 解析并执行命令、然后返回结果的过程<br>
#### 文字描述：
1. 用户：输入命令到命令行界面。<br>
2. Shell 类的 run() 方法接收用户输入，并调用 parseCommand(input) 方法解析命令。<br>
3. Shell 类调用 executeCommand(cmd) 方法执行解析后的命令。<br>
  · 如果命令涉及到 I/O 重定向或管道，executeCommand(cmd) 方法会处理这些特殊情况<br>
4. Shell 创建子进程并使用 execvp 执行命令。<br>
5. 命令执行完成后，Shell 返回结果到命令行界面。<br>
6. 用户看到命令的执行结果。<br>
![时序图](https://github.com/YEIIEY/My_shell/assets/127741388/5738b013-4a0c-4420-b6d0-bb35afd7a35f)

### 3.4. 流程图
#### 描述：
该流程图展示了从用户输入命令到命令执行结果返回的整个过程。<br>
#### 文字描述：
1. 用户输入命令：用户在命令行界面输入命令。<br>
2. Shell 接收输入：Shell 类的 run() 方法接收用户输入，并调用 parseCommand(input) 方法解析命令。<br>
3. 解析命令：Shell 类的 parseCommand(input) 方法将输入的命令字符串解析成命令和参数。<br>
4. 检查命令类型：Shell 检查命令是否包含 I/O 重定向或管道操作。<br>
  · 是：处理 I/O 重定向或管道。<br>
  · 否：继续执行命令。<br>
5. 创建子进程：Shell 创建一个子进程。<br>
6. 执行命令：使用 execvp 在子进程中执行命令。<br>
7. 等待子进程完成：Shell 等待子进程执行完成。<br>
8. 返回结果：Shell 将命令执行结果返回到命令行界面。<br>
9. 用户查看结果：用户在命令行界面查看命令的执行结果。<br>
![流程图](https://github.com/YEIIEY/My_shell/assets/127741388/96ffc7e5-8f58-4146-af90-924f0dc572d9)

## 4. 错误处理
Simple Shell 提供详细的错误信息，帮助用户快速定位和解决问题。主要错误包括：<br>
· 空命令错误。<br>
· I/O 文件格式错误。<br>
· 字符串引号错误。<br>
· 进程创建失败。<br>
· 命令执行失败。<br>
· 进程退出失败<br>
