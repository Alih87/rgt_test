#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
using namespace std::filesystem;

class LogFileManager {
    public:
        LogFileManager(const std::string& _path) : path_(_path) {
            std::cout << "Creating logs directory...\n";
            try {
                if (exists(path_)) {
                    std::cout << "Directory already exists.\n";
                } else {
                    create_directory(path_);
                    std::cout << "Directory created!\n";
                }
            } catch (const std::exception& e) {
                std::cerr << e.what();
            }
        }
        ~LogFileManager() {
            closeLogFile();
        }

        void openLogFile(const std::string& filename) {
            size_t slash = path_.find('/');
            if (slash == std::string::npos) {
                file = std::make_shared<std::fstream>(path_ + '/' + filename, std::ios::in | std::ios::app);
            } else {
                file = std::make_shared<std::fstream>(path_ + filename, std::ios::in | std::ios::app);
            }
            if (file->is_open()) {
                std::cout << "Log file opened successfully.\n";
            } else {
                std::cerr << "Failed to open log file.\n";
            }
        }

        void closeLogFile(const std::string& filename = "") {
            if (file && filename == "") {
                file->close();
                std::cout << "Log file closed.\n";
            } else if (file && filename != "") {
                file->close();
                std::cout << "Log file " << filename << " closed.\n";
            }
        }

        void writeLog(const std::string& filename, const std::string &message) {
            openLogFile(filename);
            if (file && file->is_open()) {
                (*file) << message << std::endl;
                std::cout << "Log written!" << std::endl;
            }
        }

        std::vector<std::string> readLogs(const std::string& filename) {
            std::vector<std::string> logs;
            openLogFile(filename);
            if (file && file->is_open()) {
                std::string line;
                while (std::getline(*file, line)) {
                    logs.push_back(line);
                }
            }
            return logs;
        }
    
    private:
        std::string path_;
        std::shared_ptr<std::fstream> file;
};

int main() {
    std::shared_ptr<LogFileManager> logManager = std::make_shared<LogFileManager>("logs/");
    //logManager->writeLog("app.log", "This is a test log entry.");
    std::vector<std::string> logs = logManager->readLogs("app.log");
    for (const auto& log : logs) {
         std::cout << log << std::endl;
    }

    return 0;
}
