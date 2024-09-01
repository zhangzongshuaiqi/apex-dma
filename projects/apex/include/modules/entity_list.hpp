#pragma once

#include <pch.hpp>

#include <sdk/game.hpp>

namespace projects {
constexpr size_t APEX_PLAYER_COUNT = 60;

class LOGGER(EntityList) {
public:
  EntityList() = default;
  ~EntityList() = default;

  void routine();

  constexpr const std::optional<PlayerCache>& local_player() const { return _local_player; }

  constexpr const std::array<std::optional<PlayerCache>, APEX_PLAYER_COUNT>& players() const {
    return _players;
  }

  constexpr const std::optional<glm::mat4>& view() const { return _view; }

  constexpr std::shared_mutex& mutex() { return _mutex; }

private:
  inline std::optional<PlayerCache> transmute(const std::optional<Player>& player);

  std::array<std::optional<PlayerCache>, APEX_PLAYER_COUNT> _players{};

  std::optional<PlayerCache> _local_player{};
  std::optional<glm::mat4> _view{};

  std::shared_mutex _mutex{};
};
}  // namespace projects