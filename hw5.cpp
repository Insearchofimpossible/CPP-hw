#include <iostream>
#include <string>
#include <deque>
#include <chrono>
#include <iomanip>

enum LogLevel {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

class Log {
private:
    struct LogEvent {
        std::chrono::system_clock::time_point time;
        LogLevel level;
        std::string text;
    };

    std::deque<LogEvent> events;

    Log() = default;
    ~Log() = default;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    std::string getLevelString(LogLevel level) {
        switch (level) {
            case LOG_NORMAL:  return "NORMAL";
            case LOG_WARNING: return "WARNING";
            case LOG_ERROR:   return "ERROR";
            default:          return "UNKNOWN";
        }
    }

public:
    static Log* Instance() {
        static Log instance;
        return &instance;
    }

    void message(LogLevel level, const std::string& text) {
        if (events.size() >= 10) {
            events.pop_front();
        }
        events.push_back({std::chrono::system_clock::now(), level, text});
    }

    void print() {
        for (const auto& ev : events) {
            std::time_t t = std::chrono::system_clock::to_time_t(ev.time);
            std::cout << "[" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "] "
                      << "[" << getLevelString(ev.level) << "] "
                      << ev.text << "\n";
        }
    }
};

int main() {
    Log *log = Log::Instance();
    
    log->message(LOG_NORMAL, "program loaded");
    
    for (int i = 1; i <= 15; ++i) {
        log->message(LOG_WARNING, "test event " + std::to_string(i));
    }
    
    log->message(LOG_ERROR, "error happens! help me!");
    
    log->print();
    
    return 0;
}