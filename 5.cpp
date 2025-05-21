#include <iostream>
#include <ctime>
#include <vector>
#include <unistd.h>

const int LOG_NORMAL = 0;
const int LOG_ERROR = 1;
const int LOG_WARNING = 2;


class Log
{
public:
    struct Stamp
    {
        std::string text_;
        time_t rawtime_;
        int event;
    };

    static Log* Instance()
    {
        static Log Log;
        return &Log;
    }

    void message(int event, std::string text)
    {
        history.push_back({text, time(&rawtime), event});
    }

    void print()
    {
        for (int j = history.size()-1, i = 0; j >= 0 && i < 10; i++, j--)
        {
            Stamp stamp = history[j];
            std::string e = (stamp.event == 0) ? "Normal" : 
                ((stamp.event == 1) ? "error" : "warning");
            std::cout<<"time: " << stamp.rawtime_
                << " event: " << e 
                << " text: " << stamp.text_ << "\n";
        }
    }

private:
    Log() {
        rawtime = std::time(NULL);
        std::cout << "Log\n";
    };

    time_t rawtime;

    std::vector<Stamp> history;

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    ~Log() {};
};

int main(void) { 
    Log *log = Log::Instance(); 
    log->message(LOG_NORMAL, "Program loaded");
    sleep(3);
    log->message(LOG_ERROR, "Error"); 
    log->print(); 
}