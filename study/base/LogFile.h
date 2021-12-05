#ifndef __STUDY_LOGFILE_H__
#define __STUDY_LOGFILE_H__

#include"noncopyable.h"
#include"my_mutex.h"

#include<string>
#include<memory>

namespace study {

namespace FileUtil {
class AppendFile;
}   // namespace FileUtil

class LogFile : noncopyable {
public:
    LogFile(const std::string& basename,
            off_t rollSize,
            bool threadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);

    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();

private:
    void append_unlocked(const char* logline, int len);

    static std::string getLogFileName(const std::string& basename, time_t* now);

    const std::string basename_;
    const off_t rollSize_;
    const int flushInterval_;
    const int checkEveryN_;

    int count_;
    std::unique_ptr<MutexLock> mutex_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<FileUtil::AppendFile> file_;

    const static int kRollPerSeconds_ = 60 * 60 * 24;
};

}   // namespace study

#endif // __STUDY_LOGFILE_H__