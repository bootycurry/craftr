#include <iostream>
#include <ctime>

std::string _get_datetime() {
    char date[20];
    char time_str[20];

    time_t current_time;
    tm* curr_time;

    time(&current_time);
    curr_time = localtime(&current_time);

    strftime(date, sizeof(date), "%F", curr_time);
    strftime(time_str, sizeof(time_str), "%T", curr_time);
    
    std::string datetime_str = std::string(date) + " " + std::string(time_str);
    return datetime_str;
}

void log_info(std::string message) {
    std::cout << "\033[92;1m" << "[INFO] " << "[" << _get_datetime() << "] " << message << "\033[0m" << std::endl;
}


void log_debug(std::string message) {
    std::cout << "\033[94;1m" << "[DEBUG] " << "[" << _get_datetime() << "] " << message << "\033[0m" << std::endl;
}
void log_warn(std::string message) {
    std::cout << "\033[93;1m" << "[WARN] " << "[" << _get_datetime() << "] " << message << "\033[0m" << std::endl;
}
void log_error(std::string message) {
    std::cout << "\033[91;1m" << "[ERROR] " << "[" << _get_datetime() << "] " << message << "\033[0m" << std::endl;
}