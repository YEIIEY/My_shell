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

