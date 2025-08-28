#include "craftrEditor/scene.hpp"

Scene::Scene()
{
    scene_view.signal_realize().connect(sigc::mem_fun(*this, &Scene::on_realize));
    scene_view.signal_unrealize().connect(sigc::mem_fun(*this, &Scene::on_unrealize));
    scene_view.signal_render().connect(sigc::mem_fun(*this, &Scene::on_render), false);
    
    
}

void Scene::on_realize()
{
    return;
}

void Scene::on_unrealize()
{
    return;
}

bool Scene::on_render(const Glib::RefPtr<Gdk::GLContext> &)
{
    return true;
}