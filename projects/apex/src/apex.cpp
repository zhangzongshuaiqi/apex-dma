#include <apex.hpp>

#include <display.hpp>
#include <render.hpp>

#include <extensions/menu.hpp>
#include <extensions/visuals.hpp>

void projects::Apex::render_routine() {
  logger->debug("Creating backend::Display");
  backend::Display::create();

  logger->debug("Creating backend::Render");
  backend::Render::create([] {
    frontend::Menu::get()->routine();
    frontend::Visuals::get()->routine();
  });

  logger->debug("Creating frontend::Menu");
  frontend::Menu::create(menu::routine);

  logger->debug("Creating frontend::Visuals");
  frontend::Visuals::create(visuals::routine);

  while (!backend::Stopper::get()->stop_requested()) {
    EntityList::get()->routine();

    backend::Input::get()->routine();

    backend::Render::get()->routine();
  }

  logger->debug("Destroying frontend::Menu");
  frontend::Visuals::destroy();

  logger->debug("Destroying frontend::Visuals");
  frontend::Menu::destroy();

  logger->debug("Destroying backend::Render");
  backend::Render::destroy();

  logger->debug("Destroying backend::Display");
  backend::Display::destroy();

  _threads_finished.count_down();
}

int main() {
  projects::Apex::create()->wait();

  projects::Apex::destroy();

  return EXIT_SUCCESS;
}