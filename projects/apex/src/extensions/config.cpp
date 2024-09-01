#include <extensions/config.hpp>

#include <visuals.hpp>

void projects::config::routine(backend::Config::ConfigStorage& config) {
  config["visuals_players_box"] = false;
  config["visuals_players_box_color"] = ImColor(255, 255, 255, 255);

  config["visuals_players_healthbar"] = false;
  config["visuals_players_healthbar_color"] = ImColor(0, 255, 0, 255);
  config["visuals_players_healthbar_location"] = frontend::Visuals::Location_Left;

  config["visuals_players_healthbar_text"] = false;
  config["visuals_players_healthbar_text_limit"] = 0.95f;
  config["visuals_players_healthbar_text_font"] = backend::Render::Font_VerdanaBold;

  config["visuals_players_shieldbar"] = false;
  config["visuals_players_shieldbar_color"] = ImColor(0, 102, 255, 255);
  config["visuals_players_shieldbar_location"] = frontend::Visuals::Location_Left;

  config["visuals_players_shieldbar_text"] = false;
  config["visuals_players_shieldbar_text_limit"] = 1.f;
  config["visuals_players_shieldbar_text_font"] = backend::Render::Font_VerdanaBold;

  config["visuals_players_name"] = false;
  config["visuals_players_name_color"] = ImColor(255, 255, 255, 255);
  config["visuals_players_name_font"] = backend::Render::Font_Verdana;
  config["visuals_players_name_location"] = frontend::Visuals::Location_Top;
}