#pragma once

#include <pch.hpp>

#include <config.hpp>
#include <memory.hpp>
#include <stopper.hpp>

#include <menu.hpp>
#include <visuals.hpp>

#include <extensions/config.hpp>

#include <modules/entity_list.hpp>

namespace projects {
class LOGGER(Apex) {
public:
  Apex() {
    logger->info("Apex started");

    logger->debug("Creating backend::Stopper");
    backend::Stopper::create();

    logger->debug("Creating backend::Config");
    backend::Config::create(config::routine);

    logger->debug("Creating backend::Memory");
    backend::Memory::create();

    logger->debug("Creating projects::EntityList");
    EntityList::create();

    _render_thread = std::thread(&Apex::render_routine, this);

    _render_thread.detach();
  }

  ~Apex() {
    logger->info("Apex stopped");

    logger->debug("Destroying projects::EntityList");
    EntityList::destroy();

    logger->debug("Destroying backend::Memory");
    backend::Memory::destroy();

    logger->debug("Destroying backend::Config");
    backend::Config::destroy();

    logger->debug("Destroying backend::Stopper");
    backend::Stopper::destroy();
  }

  inline void wait() const { _threads_finished.wait(); }

  void render_routine();

private:
  std::latch _threads_finished{1};

  std::thread _render_thread{};
};
}  // namespace projects