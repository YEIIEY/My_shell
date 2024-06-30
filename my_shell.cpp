#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>
#include <fstream>

// 定义常量
#define ARGS_COUNT 8
#define COMMANDS_COUNT 8
#define BUF_SIZE 128

// 错误处理宏定义，简化错误信息输出和程序退出逻辑
#define ERROR_EMPTY(errorStr) do { std::cerr << errorStr << " Please check and try again :D\n"; return false; } while(0)
#define ERROR_IOFILE do { std::cerr << "\033[1;31mError:\033[0m I/O file format error! \nPlease check and try again :D\n"; return false; } while(0)
#define ERROR_STR do { std::cerr << "\033[1;31mError:\033[0m Your str miss ' or \" ! \nPlease check and try again :D\n"; return false; } while(0)
#define ERROR_FORK do { std::cerr << "\033[1;31mError:\033[0m Failed to fork process!"; return; } while(0)
#define ERROR_OPEN do { std::cerr << "\033[1;31mError:\033[0m I/O file format error! \nPlease check and try again :D\n"; exit(0); return; } while(0)
#define ERROR_EXECUTE(errorCmd) do { std::cerr << "\033[1;31mError:\033[0m Failed to execute cmd " << errorCmd << "!\n"; exit(0); return; } while(0)
#define ERROR_EXIT do { std::cerr << "\033[1;31mError:\033[0m Failed to exit with status " << WEXITSTATUS(status) << "!\n"; return; } while(0)

// 定义命令结构体，包含输入重定向、输出重定向、追加重定向、命令参数和命令本身
struct Command {
    std::string read;
    std::string write;
    std::string overwrite;
    std::string append;  // 追加重定向
    std::vector<std::string> args;
    std::string cmd;

    Command() : read(""), write(""), overwrite(""), append(""), cmd("") {}
};

// fetchFileName函数：从字符串中提取文件名
bool fetchFileName(std::string::iterator& bufIter, std::string::iterator bufEnd, std::string& filename) {
    while (bufIter != bufEnd && isspace(*bufIter)) bufIter++;
    while (bufIter != bufEnd && !isspace(*bufIter) && *bufIter != '|' && *bufIter != '<' && *bufIter != '>') {
        filename.push_back(*bufIter);
        bufIter++;
    }
    return !filename.empty();
}

// splitCommands函数：将用户输入的命令行字符串分割成多个命令对象
bool splitCommands(std::string &buf, std::vector<Command> &commands) {
    bool waitCommand = true;  // 标志等待命令输入
    commands.emplace_back();
    auto args = &commands.back().args;
    auto bufEnd = buf.end();

    std::string::iterator start;
    char quote;

    for (auto bufIter = buf.begin(); bufIter != buf.end(); ) {
        switch(*bufIter) {
            case ' ':
                while (bufIter != buf.end() && isspace(*bufIter)) ++bufIter;
                break;

            case '\n':
                if (waitCommand) ERROR_EMPTY("\033[1;31mError:\033[0m Next command shouldn't be empty!\n");
                return true;

            case '|':
                if (waitCommand) ERROR_EMPTY("\033[1;31mError:\033[0m Pipe should be used after a command!\n");
                waitCommand = true;
                bufIter++;
                if (!args->empty()) {
                    commands.back().args.push_back("");  // 添加空参数来防止生成空命令
                    commands.emplace_back();
                    args = &commands.back().args;
                }
                break;

            case '<':
                if (waitCommand) ERROR_EMPTY("\033[1;31mError:\033[0m I/O redirection should be used after a command!\n");
                bufIter++;
                while (bufIter != buf.end() && isspace(*bufIter)) ++bufIter;
                if (!fetchFileName(bufIter, bufEnd, commands.back().read)) ERROR_IOFILE;
                break;

            case '>':
                if (waitCommand) ERROR_EMPTY("\033[1;31mError:\033[0m I/O redirection should be used after a command!\n");
                bufIter++;
                if (*bufIter == '>') { 
                    bufIter++;
                    while (bufIter != buf.end() && isspace(*bufIter)) ++bufIter;
                    if (!fetchFileName(bufIter, bufEnd, commands.back().append)) ERROR_IOFILE;
                } else {
                    while (bufIter != buf.end() && isspace(*bufIter)) ++bufIter;
                    if (!fetchFileName(bufIter, bufEnd, commands.back().write)) ERROR_IOFILE;
                }
                break;

            case '\'':
            case '\"':
                if (waitCommand) ERROR_EMPTY("\033[1;31mError:\033[0m String parameter should be used after a command!\n");
                quote = *bufIter++;
                start = bufIter;
                while (bufIter != buf.end() && *bufIter != quote) bufIter++;
                if (bufIter == buf.end() || bufIter - start - 1 < 0) ERROR_STR;
                args->emplace_back(start, bufIter);
                bufIter++;
                break;

            default:
                start = bufIter;
                while (bufIter != buf.end() && !isspace(*bufIter) && *bufIter != '|' && *bufIter != '<' && *bufIter != '>') ++bufIter;
                args->emplace_back(start, bufIter);

                if (waitCommand) {
                    waitCommand = false;
                    commands.back().cmd = args->back();
                }
                break;
        }
    }

    // 检查最后一个命令是否为空，如果是则移除
    if (commands.back().cmd.empty() && commands.back().args.empty()) {
        commands.pop_back();
    }

    return true;
}

// forkToExecute函数：创建子进程执行命令
void forkToExecute(Command &command, int fd_in, int fd_out) {
    if (command.cmd == "exit") {
        exit(EXIT_SUCCESS);
    } else if (command.cmd == "cd") {
        if (command.args.size() > 1) {
            if (chdir(command.args[1].c_str()) != 0) {
                std::cerr << "\033[1;31mError:\033[0m Cannot cd to :" << command.args[1] << "\n";
            }
        } else {
            std::cerr << "\033[1;31mError:\033[0m Missing directory argument for cd command.\n";
        }
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        ERROR_FORK;
    } else if (pid == 0) {
        // 处理输入重定向
        if (!command.read.empty()) {
            int in = open(command.read.c_str(), O_RDONLY);
            if (in < 0) {
                ERROR_OPEN;
            } else {
                dup2(in, STDIN_FILENO);
                close(in);
            }
        } else if (fd_in > 0) {
            dup2(fd_in, STDIN_FILENO);
        }

        // 处理输出重定向
        int outFd = -1;
        if (!command.write.empty()) {
            outFd = open(command.write.c_str(), O_WRONLY | O_CREAT, 0666);
        } else if (!command.overwrite.empty()) {
            outFd = open(command.overwrite.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
        } else if (!command.append.empty()) {  // 处理追加重定向
            outFd = open(command.append.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
        }

        if (outFd >= 0) {
            dup2(outFd, STDOUT_FILENO);
            if (outFd != STDOUT_FILENO) close(outFd);
        } else if (fd_out > 0) {
            dup2(fd_out, STDOUT_FILENO);
        }

        // 转换参数为C字符串数组
        std::vector<char*> args_cstr;
        for (auto &arg : command.args) {
            if (!arg.empty()) {
                args_cstr.push_back(const_cast<char*>(arg.c_str()));
            }
        }
        args_cstr.push_back(nullptr);

        if (execvp(command.cmd.c_str(), args_cstr.data()) < 0) {
            ERROR_EXECUTE(command.cmd.c_str());
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            ERROR_EXIT;
        }
    }
}

// executeCommands函数：顺序执行命令列表中的所有命令
void executeCommands(std::vector<Command> &commands) {
    int commandsCount = commands.size();
    if (commandsCount == 1) {
        forkToExecute(commands[0], -1, -1);
    } else if (commandsCount == 2) {
        int fd[2];
        pipe(fd);
        forkToExecute(commands[0], -1, fd[1]);
        close(fd[1]);
        forkToExecute(commands[1], fd[0], -1);
        close(fd[0]);
    } else {
        int pipes[2][2];
        pipe(pipes[0]);
        forkToExecute(commands[0], -1, pipes[0][1]);
        close(pipes[0][1]);

        for (int i = 1; i < commandsCount - 1; ++i) {
            pipe(pipes[i % 2]);
            forkToExecute(commands[i], pipes[(i - 1) % 2][0], pipes[i % 2][1]);
            close(pipes[(i - 1) % 2][0]);
            close(pipes[i % 2][1]);
        }
        forkToExecute(commands[commandsCount - 1], pipes[(commandsCount - 2) % 2][0], -1);
        close(pipes[(commandsCount - 2) % 2][0]);
    }
}

// runShellScript函数：运行shell脚本文件
void runShellScript(const std::string &scriptFilename) {
    std::ifstream scriptFile(scriptFilename);
    if (!scriptFile.is_open()) {
        std::cerr << "\033[1;31mError:\033[0m Failed to open script file: " << scriptFilename << "\n";
        return;
    }

    std::string line;
    std::vector<Command> commands;
    while (std::getline(scriptFile, line)) {
        if (!splitCommands(line, commands)) {
            std::cerr << "\033[1;31mError:\033[0m Failed to parse commands in script. Please check the script file.\n";
            continue;
        }
        executeCommands(commands);
        commands.clear();
    }
    scriptFile.close();
}

// 主函数
int main(int argc, char *argv[]) {
    std::cout << "Welcome to The Shell! \n";
    std::string input;
    std::vector<Command> commands;

    if (argc > 1) {
        std::string scriptFilename = argv[1];
        runShellScript(scriptFilename);
        return 0;
    }

    while (true) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << cwd << " $ ";
        } else {
            std::cerr << "Error getting current directory\n";
        }

        std::getline(std::cin, input);

        if (!splitCommands(input, commands)) {
            continue;
        }
        executeCommands(commands);
        commands.clear();
    }

    return 0;
}
