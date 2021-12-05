#ifndef __STUDY_PROCESSINFO_H__
#define __STUDY_PROCESSINFO_H__

#include"StringPiece.h"
#include"Timestamp.h"
#include"Types.h"


#include<string>
#include<vector>

namespace study {
namespace ProcessInfo {

pid_t pid();    // 返回进程pid
std::string pidString();    // 将进程pid字符串化
uid_t uid();    // 返回userId
std::string username(); // 返回用户姓名
uid_t euid();
Timestamp startTime();  // 时间戳
int clockTicksPerSecond();
int pageSize(); 
bool isDebugBuild();    // 是否开启调试

std::string hostname();
std::string procname();
StringPiece procname(const std::string& stat);

std::string procStatus();    // 读取进程状态

std::string procStat();

std::string threadStat();

std::string exePath();

int openedFiles();  // 统计进程打开的文件数目
int maxOpenFiles();

struct CpuTime {
    double userSeconds;
    double systemSeconds;

    CpuTime() : userSeconds(0.0), systemSeconds(0.0) {}

    double total() const {return userSeconds + systemSeconds;}
};

CpuTime cpuTime();

int numThreads();
std::vector<pid_t> threads();

}   // namespace ProcessInfo
}   // namespace study

#endif