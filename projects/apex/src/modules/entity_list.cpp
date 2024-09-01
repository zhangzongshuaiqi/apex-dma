#include <modules/entity_list.hpp>

#include <memory.hpp>
#include <stopper.hpp>

#include <sdk/offsets.hpp>

void projects::EntityList::routine() {
  std::unique_lock _{_mutex};

  _players = {};

  _view = std::nullopt;
  _local_player = std::nullopt;

  static const auto baseAddress = backend::Memory::get()->base_address("r5apex.exe");

  if (!baseAddress.has_value()) {
    backend::Stopper::get()->stop();

    logger->error("Failed to get base address");

    return;
  }

  const auto& localPlayerAddress =
      backend::Memory::get()->read(baseAddress.value() + LOCAL_PLAYER_OFFSET);

  if (!localPlayerAddress.has_value())
    return;

  _local_player = transmute(backend::Memory::get()->read<Player>(localPlayerAddress.value()));

  if (!_local_player.has_value())
    return;

  const auto& viewRender = backend::Memory::get()->read(baseAddress.value() + VIEW_RENDER_OFFSET);

  if (!viewRender.has_value())
    return;

  const auto& viewMatrixContainer =
      backend::Memory::get()->read(viewRender.value() + VIEW_MATRIX_OFFSET);

  if (!viewMatrixContainer.has_value())
    return;

  const auto& viewMatrix = backend::Memory::get()->read<glm::mat4>(viewMatrixContainer.value());

  if (!viewMatrix.has_value())
    return;

  glm::mat4 viewMatrixTranslated{};

  for (size_t r = 0; r < 4; r++)
    for (size_t c = 0; c < 4; c++)
      viewMatrixTranslated[c][r] = viewMatrix.value()[r][c];

  _view = viewMatrixTranslated;

  const auto& entities = backend::Memory::get()->read<EntityEntry, APEX_PLAYER_COUNT>(
      baseAddress.value() + ENTITY_LIST_OFFSET + sizeof(EntityEntry));

  if (!entities.has_value())
    return;

  for (size_t i = 1; i < APEX_PLAYER_COUNT + 1; i++) {
    const auto& entry = entities.value().at(i - 1);

    if (!entry.entity)
      continue;

    // if (reinterpret_cast<uintptr_t>(entry.entity) == localPlayerAddress.value())
    //   continue;

    auto& player = _players.at(i - 1);

    player =
        transmute(backend::Memory::get()->read<Player>(reinterpret_cast<uintptr_t>(entry.entity)));
  }
}

std::optional<projects::PlayerCache> projects::EntityList::transmute(
    const std::optional<projects::Player>& player) {
  if (!player.has_value())
    return std::nullopt;

  static const auto baseAddress = backend::Memory::get()->base_address("r5apex.exe");

  return PlayerCache{
      .velocity = player->velocity,
      .origin = player->origin,
      .shield = player->shield,
      .shield_max = player->shield_max,
      .health = player->health,
      .team = player->team,
      .mins = player->mins,
      .maxs = player->maxs,
      .health_max = player->health_max,
      .life_state = player->life_state,
      .rgfl = ([&player] -> glm::mat4x3 {
        glm::mat4x3 buf{};

        for (size_t r = 0; r < 3; r++)
          for (size_t c = 0; c < 4; c++)
            buf[c][r] = player->rgfl[r][c];

        return buf;
      })(),
      .primary_weapon_handle = player->primary_weapon_handle,
      .camera_origin = player->camera_origin,
      .camera_angles = player->camera_angles,
      .buttons = player->buttons,
      .name = ([&player] -> std::string {
        const auto& nameArrayLocation = backend::Memory::get()->read(
            baseAddress.value() + NAME_LIST_OFFSET + ((player->name_index - 1) * 24));

        if (!nameArrayLocation.has_value())
          return {};

        const auto& nameArray = backend::Memory::get()->read<char, 64>(nameArrayLocation.value());

        if (!nameArray.has_value())
          return {};

        return std::string(nameArray.value().data());
      })(),
  };
}