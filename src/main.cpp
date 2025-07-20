#include <iostream>
#include <glad/glad.h>
#include <logger.hpp>



#include <gtkmm/window.h>
#include <gtkmm/application.h>



class MyWindow : public Gtk::Window
{
  public:
    MyWindow();
};

MyWindow::MyWindow()
{
  set_title("Basic application");
  set_default_size(200, 200);
}

int main(int argc, char* argv[])
{
  
}