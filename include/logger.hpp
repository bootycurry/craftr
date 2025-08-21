#ifndef CRAFTR_LOGGER
#define CRAFTR_LOGGER

#include "craftrEditor/log_display.hpp"
#include <filesystem>
#include <gtkmm.h>

enum LogDestination { CONSOLE, LOG_FILE, LOG_WINDOW };

class Logger {
public:
  static Logger &instance() {
    static Logger logger;
    return logger;
  }

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void set_file_path(const std::string &dir_path);
  void set_target_display(LogDisplay *display);
  void log(LogDestination destination, const Glib::ustring &message,
           LogType log_type);

private:
  LogDisplay *display_window;
  std::filesystem::path log_dir;
  void log_to_display(const Glib::ustring &message, LogType log_type);
  void log_to_file(const Glib::ustring &message, LogType log_type);
  void log_to_console(const Glib::ustring &message, LogType log_type);

  Logger() = default;
  ~Logger() = default;
};

#endif
