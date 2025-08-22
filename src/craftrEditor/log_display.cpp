#include <chrono>
#include "craftrEditor/log_display.hpp"
#include <format>
#include <gtkmm/cssprovider.h>
#include <gtkmm/enums.h>

LogDisplay::LogDisplay()
    : reset_button("Reset Log"), main_box(Gtk::Orientation::VERTICAL) {
  reset_button.signal_clicked().connect([this]() {
    auto buffer = log_text.get_buffer();
    buffer->set_text("");
  });

  scroll_container.set_policy(Gtk::PolicyType::AUTOMATIC,
                              Gtk::PolicyType::AUTOMATIC);
  scroll_container.set_child(log_text);
  main_box.append(reset_button);
  main_box.append(scroll_container);
  scroll_container.set_hexpand(true);
  scroll_container.set_vexpand(true);

  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data("textview text {  font-size: 22px; }");

  auto context = log_text.get_style_context();
  context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

  Glib::RefPtr<Gtk::TextBuffer> buffer = log_text.get_buffer();
  Glib::RefPtr<Gtk::TextTagTable> tag_table = buffer->get_tag_table();

  auto tag = Gtk::TextTag::create("INFO");
  tag->property_foreground() = "#569CD6";
  tag_table->add(tag);

  tag = Gtk::TextTag::create("ERROR");
  tag->property_foreground() = "#E57373";
  tag_table->add(tag);

  tag = Gtk::TextTag::create("DEBUG");
  tag->property_foreground() = "#81C784";
  tag_table->add(tag);

  tag = Gtk::TextTag::create("WARNING");
  tag->property_foreground() = "#FFB74D";
  tag_table->add(tag);
}

LogDisplay::~LogDisplay() {}

void LogDisplay::log(const Glib::ustring &message, const LogType log_type) {
  Glib::ustring tag_name;
  Glib::ustring prefix;

  switch (log_type) {
  case DEBUG:
    tag_name = "DEBUG";
    prefix = "[DEBUG] ";
    break;
  case WARNING:
    tag_name = "WARNING";
    prefix = "[WARNING] ";
    break;
  case ERROR:
    tag_name = "ERROR";
    prefix = "[ERROR] ";
    break;
  case INFO:
  default:
    tag_name = "INFO";
    prefix = "[INFO] ";
    break;
  }
  const auto now = std::chrono::floor<std::chrono::seconds>(
      std::chrono::system_clock::now());
  const std::chrono::zoned_time local_time{std::chrono::current_zone(), now};
  std::string timestamp = std::format("{:%d-%m-%Y %H:%M:%S}", local_time);
  prefix = "[" + Glib::ustring(timestamp) + "] " + prefix;

  Glib::RefPtr<Gtk::TextBuffer> buffer = log_text.get_buffer();
  Glib::RefPtr<Gtk::TextTag> tag = buffer->get_tag_table()->lookup(tag_name);
  Glib::ustring final_message = "\n" + prefix + message + "\n";

  buffer->insert_with_tag(buffer->end(), final_message, tag);
}
