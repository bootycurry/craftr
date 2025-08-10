#include <craftrEditor/log_display.hpp>
#include <iostream>
#include <gtkmm/enums.h>
#include <gtkmm/cssprovider.h>


LogDisplay::LogDisplay()
: reset_button("Reset Log"),
main_box(Gtk::Orientation::VERTICAL)
{
  reset_button.signal_clicked().connect([this]() {
        auto buffer = log_text.get_buffer();
        buffer->set_text("");
    });

  scroll_container.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  scroll_container.set_child(log_text);
  main_box.append(reset_button);
  main_box.append(scroll_container);
  scroll_container.set_hexpand(true);
  scroll_container.set_vexpand(true);
  

  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(
      "textview text { background-color: #000000; font-size: 22px}" 
  );

  auto context = log_text.get_style_context();
  context->add_provider(
      css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_USER
  );

  
  Glib::RefPtr<Gtk::TextBuffer> buffer = log_text.get_buffer();
  Glib::RefPtr<Gtk::TextTagTable> tag_table = buffer->get_tag_table();
  auto tag = Gtk::TextTag::create("INFO");
  tag -> property_foreground() = "#0000FF";
  tag_table -> add(tag);
  tag = Gtk::TextTag::create("ERROR");
  tag -> property_foreground() = "#FF0000";
  tag_table -> add(tag);
  tag = Gtk::TextTag::create("DEBUG");
  tag -> property_foreground() = "#00FF00";
  tag_table -> add(tag);
  tag = Gtk::TextTag::create("WARNING");
  tag -> property_foreground() = "#FFFF00";
  tag_table -> add(tag);
  


}


LogDisplay::~LogDisplay()
{
}

void LogDisplay::log(const Glib::ustring& message, bool replace)
{
  
  
  Glib::RefPtr<Gtk::TextBuffer> buffer = log_text.get_buffer();
  Glib::RefPtr<Gtk::TextTagTable> tag_table = buffer->get_tag_table();
  Glib::RefPtr<Gtk::TextTag> tag;
  if (replace)
  {
    tag = tag_table -> lookup("INFO");
  }

  
  buffer -> insert_with_tag(buffer -> end(), "\n" + message + "\n", tag);
  
  
  
  
}

