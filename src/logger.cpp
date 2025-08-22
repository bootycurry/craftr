#include "logger.hpp"
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

void Logger::log(LogDestination destination, const Glib::ustring &message,
                 LogType log_type) {
  switch (destination) {
  case LOG_FILE:
    log_to_file(message, log_type);
    break;
  case LOG_WINDOW:
    log_to_display(message, log_type);
    break;
  case CONSOLE:
    log_to_console(message, log_type);
    break;
  }
}

void Logger::set_file_path(const std::string &dir_path) {
  log_dir = std::filesystem::path(dir_path);
}

void Logger::set_target_display(LogDisplay *log_display) {
  display_window = log_display;
}

void Logger::log_to_file(const Glib::ustring &message, LogType log_type) {
  if (!std::filesystem::exists(log_dir)) {
    std::filesystem::create_directories(log_dir);
  }

  const auto now = std::chrono::floor<std::chrono::seconds>(
      std::chrono::system_clock::now());
  const std::chrono::zoned_time local_time{std::chrono::current_zone(), now};
  std::string log_file_name = std::format("{:%d%m%Y%H%M%S}.log", local_time);
  std::filesystem::path log_file_path = log_dir / log_file_name;

  std::fstream log_file(log_file_path, std::ios::app);

  if (!log_file.is_open()) {
    log_to_console("CRITICAL ERROR: Failed to open log file at: " +
                       log_file_path.string(),
                   ERROR);
    perror("Reason");
  }

  std::string log_type_str;
  switch (log_type) {
  case LogType::INFO:
    log_type_str = "INFO";
    break;
  case LogType::WARNING:
    log_type_str = "WARNING";
    break;
  case LogType::ERROR:
    log_type_str = "ERROR";
    break;
  case LogType::DEBUG:
    log_type_str = "DEBUG";
    break;
  }

  std::string log_time = std::format("{:%d-%m-%Y %H:%M:%S}", local_time);

  log_file << "[" << log_time << "] [" << log_type_str << "] " << message
           << "\n";

  log_file.close();
}
void Logger::log_to_display(const Glib::ustring &message, LogType log_type) {
  display_window->log(message, log_type);
}

void Logger::log_to_console(const Glib::ustring &message, LogType log_type) {

  const auto now = std::chrono::floor<std::chrono::seconds>(
      std::chrono::system_clock::now());
  const std::chrono::zoned_time local_time{std::chrono::current_zone(), now};

  std::string log_type_str;
  switch (log_type) {
  case LogType::INFO:
    log_type_str = "INFO";
    break;
  case LogType::WARNING:
    log_type_str = "WARNING";
    break;
  case LogType::ERROR:
    log_type_str = "ERROR";
    break;
  case LogType::DEBUG:
    log_type_str = "DEBUG";
    break;
  }

  std::string log_time = std::format("{:%d-%m-%Y %H:%M:%S}", local_time);

  std::cout << "[" << log_time << "] [" << log_type_str << "] " << message
            << std::endl;
}
