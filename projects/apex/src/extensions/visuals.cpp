#include <extensions/visuals.hpp>

#include <config.hpp>
#include <display.hpp>
#include <render.hpp>

#include <visuals.hpp>

#include <modules/entity_list.hpp>

std::optional<glm::ivec2> projects::visuals::world_to_screen(const glm::vec3& world,
                                                             const glm::mat4& view) {
  const auto& clip = view * glm::vec4(world, 1.f);

  if (clip.w < .001f)
    return std::nullopt;

  const auto& normalized = glm::vec2(clip) / clip.w;
  const auto& screenSize = backend::Display::get()->get_window_rect();

  glm::vec2 screen = {};

  screen.x = (1.f + normalized.x) * screenSize.right / 2.f;
  screen.y = (1.f - normalized.y) * screenSize.bottom / 2.f;

  return screen;
}

std::optional<std::pair<glm::ivec2, glm::ivec2>> projects::visuals::bounding_box(
    const glm::mat4x3& matrix,
    const glm::vec3& mins,
    const glm::vec3& maxs,
    const glm::mat4& view) {
  std::array<glm::vec2, 8> points{};

  bool onScreen = false;

  for (size_t i = 0; i < 8; i++) {
    const auto& point = glm::vec3(i & 0b001 ? maxs.x : mins.x, i & 0b010 ? maxs.y : mins.y,
                                  i & 0b100 ? maxs.z : mins.z);

    const auto& world = matrix * glm::vec4(point, 1.f);
    const auto& screen = world_to_screen(glm::vec3(world), view);

    onScreen = onScreen || screen.has_value();

    if (!screen.has_value())
      continue;

    points.at(i) = screen.value();
  }

  if (!onScreen)
    return std::nullopt;

  glm::vec2 minsScreen = points.at(0);
  glm::vec2 maxsScreen = points.at(0);

  for (const auto& point : points | std::views::drop(1)) {
    minsScreen = {std::min(minsScreen.x, point.x), std::min(minsScreen.y, point.y)};
    maxsScreen = {std::max(maxsScreen.x, point.x), std::max(maxsScreen.y, point.y)};
  }

  return std::make_pair(minsScreen, maxsScreen);
}

void projects::visuals::routine() {
  std::shared_lock<std::shared_mutex> _{EntityList::get()->mutex()};

  const auto& localPlayer = EntityList::get()->local_player();

  if (!localPlayer.has_value())
    return;

  const auto& view = EntityList::get()->view();

  if (!view.has_value())
    return;

  for (const auto& player : EntityList::get()->players()) {
    if (!player.has_value())
      continue;

    if (player->life_state)
      continue;

    // if (player->team == localPlayer->team)
    //   continue;

    const auto& boundingBox = bounding_box(player->rgfl, player->mins, player->maxs, view.value());

    if (!boundingBox.has_value())
      continue;

    const auto& [mins, maxs] = boundingBox.value();

    frontend::Visuals::get()->reset_offsets(mins, maxs);

    STATIC_CONFIG_VALUE(configPlayersBox, bool, "visuals_players_box");
    STATIC_CONFIG_VALUE(configPlayersBoxColor, ImColor, "visuals_players_box_color");

    if (configPlayersBox)
      frontend::Visuals::get()->render_box(configPlayersBoxColor);

    STATIC_CONFIG_VALUE(configPlayersHealthbar, bool, "visuals_players_healthbar");
    STATIC_CONFIG_VALUE(configPlayersHealthbarText, bool, "visuals_players_healthbar_text");
    STATIC_CONFIG_VALUE(configPlayersHealthbarTextLimit, float,
                        "visuals_players_healthbar_text_limit");
    STATIC_CONFIG_VALUE(configPlayersHealthbarColor, ImColor, "visuals_players_healthbar_color");
    STATIC_CONFIG_VALUE(configPlayersHealthbarLocation, int32_t,
                        "visuals_players_healthbar_location");
    STATIC_CONFIG_VALUE(configPlayersHealthbarTextFont, int32_t,
                        "visuals_players_healthbar_text_font");

    if (configPlayersHealthbar) {
      const auto& healthFraction =
          static_cast<float>(player->health) / static_cast<float>(player->health_max);

      frontend::Visuals::get()->render_bar(
          configPlayersHealthbarLocation, healthFraction, configPlayersHealthbarColor,
          (configPlayersHealthbarText && healthFraction <= configPlayersHealthbarTextLimit)
              ? std::to_string(player->health)
              : "",
          configPlayersHealthbarTextFont);
    }

    STATIC_CONFIG_VALUE(configPlayersShieldbar, bool, "visuals_players_shieldbar");
    STATIC_CONFIG_VALUE(configPlayersShieldbarText, bool, "visuals_players_shieldbar_text");
    STATIC_CONFIG_VALUE(configPlayersShieldbarTextLimit, float,
                        "visuals_players_shieldbar_text_limit");
    STATIC_CONFIG_VALUE(configPlayersShieldbarColor, ImColor, "visuals_players_shieldbar_color");
    STATIC_CONFIG_VALUE(configPlayersShieldbarLocation, int32_t,
                        "visuals_players_shieldbar_location");
    STATIC_CONFIG_VALUE(configPlayersShieldbarTextFont, int32_t,
                        "visuals_players_shieldbar_text_font");

    if (configPlayersShieldbar) {
      const auto& shieldFraction =
          static_cast<float>(player->shield) / static_cast<float>(player->shield_max);

      frontend::Visuals::get()->render_bar(
          configPlayersShieldbarLocation, shieldFraction, configPlayersShieldbarColor,
          (configPlayersShieldbarText && shieldFraction <= configPlayersShieldbarTextLimit)
              ? std::to_string(player->shield)
              : "",
          configPlayersShieldbarTextFont, 0);
    }

    STATIC_CONFIG_VALUE(configPlayersName, bool, "visuals_players_name");
    STATIC_CONFIG_VALUE(configPlayersNameFont, int32_t, "visuals_players_name_font");
    STATIC_CONFIG_VALUE(configPlayersNameColor, ImColor, "visuals_players_name_color");
    STATIC_CONFIG_VALUE(configPlayersNameLocation, int32_t, "visuals_players_name_location");

    if (configPlayersName) {
      frontend::Visuals::get()->render_text(player->name, configPlayersNameFont,
                                            backend::Render::TextFlags_Shadow,
                                            configPlayersNameLocation, configPlayersNameColor);
    }
  }
}