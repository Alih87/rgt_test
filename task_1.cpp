#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <sstream>

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

        LogFileManager& operator=(LogFileManager& another) {
            if (this != &another) {
                this->path_ = another.path_;
                this->file = another.file;
            }
            return *this;
        }

        ~LogFileManager() {
            closeLogFile();
        }

        void openLogFile(const std::string& filename) {
            file[filename] = std::make_shared<std::fstream>();
            size_t slash = path_.find('/');
            if (slash == std::string::npos) {
                file[filename]->open(path_ + '/' + filename, std::ios::in | std::ios::app);
            } else {
                file[filename]->open(path_ + filename, std::ios::in | std::ios::app);
            }
            if (file[filename]->is_open()) {
                std::cout << "Log file opened successfully.\n";
            } else {
                std::cerr << "Failed to open log file.\n";
            }
        }

        void closeLogFile(const std::string& filename = "") {
            if (filename == "") {
                for (auto& [key, val] : file) {
                    if (val && val->is_open()) {
                        val->close();
                        std::cout << "Log file " << key << " closed successfully.\n";
                    }
                }
                file.clear();
            } else {
                auto it = file.find(filename);
                if (it != file.end() && it->second && it->second->is_open()) {
                    it->second->close();
                    std::cout << "Log file " << filename << " closed.\n";
                    file.erase(it);
                } else {
                    std::cerr << "Log file " << filename << " not found or already closed.\n";
                }
            }
        }

        void writeLog(const std::string& filename, const std::string &message) {
            openLogFile(filename);
            if (file[filename] && file[filename]->is_open()) {
                (*file[filename]) << getCurrentDateTime() << " " << message << std::endl;
                std::cout << "Log written to " << filename << "\n";
            } else {
                std::cerr << "Failed to write log to " << filename << "\n";
            }
        }

        std::vector<std::string> readLogs(const std::string& filename) {
            std::vector<std::string> logs;
            openLogFile(filename);
            if (file[filename] && file[filename]->is_open()) {
                std::string line;
                while (std::getline(*file[filename], line)) {
                    logs.push_back(line);
                }
            }
            return logs;
        }
    
    private:
        std::string path_;
        std::unordered_map<std::string, std::shared_ptr<std::fstream>> file;

        std::string getCurrentDateTime() {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "]";
            return ss.str();
        }
};

int main() {
    std::shared_ptr<LogFileManager> logManager = std::make_shared<LogFileManager>("logs/");

    logManager->writeLog("app.log", "This is a test log entry with time!");
    logManager->writeLog("app1.log", "This is ANOTHER test log entry with time!");
    logManager->closeLogFile("app.log");
    // std::vector<std::string> logs = logManager->readLogs("app1.log");
    // for (const auto& log : logs) {
    //      std::cout << log << std::endl;
    // }

    return 0;
}
