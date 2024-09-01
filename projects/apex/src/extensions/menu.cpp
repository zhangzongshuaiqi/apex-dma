#include <extensions/menu.hpp>

#include <config.hpp>
#include <menu.hpp>

void projects::menu::routine() {
  if (ImGui::BeginTabItem("Visuals")) {
    if (ImGui::BeginChild("##visuals")) {
      if (frontend::Menu::begin_group("Players")) {
        STATIC_CONFIG_VALUE(configPlayersBox, bool, "visuals_players_box");

        if (frontend::Menu::foldable_checkbox("Box", configPlayersBox)) {
          STATIC_CONFIG_VALUE(configPlayersBoxColor, ImColor, "visuals_players_box_color");

          ImGui::Indent();

          ImGui::SeparatorText("Properties");

          frontend::Menu::color_edit("##box_color", configPlayersBoxColor);

          ImGui::Unindent();
        }

        STATIC_CONFIG_VALUE(configPlayersHealthbar, bool, "visuals_players_healthbar");

        if (frontend::Menu::foldable_checkbox("Healthbar", configPlayersHealthbar)) {
          STATIC_CONFIG_VALUE(configPlayersHealthbarText, bool, "visuals_players_healthbar_text");
          STATIC_CONFIG_VALUE(configPlayersHealthbarColor, ImColor,
                              "visuals_players_healthbar_color");
          STATIC_CONFIG_VALUE(configPlayersHealthbarLocations, int32_t,
                              "visuals_players_healthbar_location");
          STATIC_CONFIG_VALUE(configPlayersHealthbarTextFont, int32_t,
                              "visuals_players_healthbar_text_font");
          STATIC_CONFIG_VALUE(configPlayersHealthbarTextLimit, float,
                              "visuals_players_healthbar_text_limit");

          ImGui::Indent();

          ImGui::SeparatorText("Properties");

          frontend::Menu::color_edit("##healthbar_color", configPlayersHealthbarColor);
          frontend::Menu::location_combo("##healthbar_location", configPlayersHealthbarLocations);

          ImGui::SeparatorText("Children");

          if (frontend::Menu::foldable_checkbox("Text ##healthbar_text",
                                                configPlayersHealthbarText)) {
            ImGui::Indent();

            ImGui::SeparatorText("Properties");

            frontend::Menu::percent_slider("##healthbar_text_limit",
                                           configPlayersHealthbarTextLimit);
            frontend::Menu::font_combo("##healthbar_text_font", configPlayersHealthbarTextFont);

            ImGui::Unindent();
          }

          ImGui::Unindent();
        }

        STATIC_CONFIG_VALUE(configPlayersShieldbar, bool, "visuals_players_shieldbar");
        STATIC_CONFIG_VALUE(configPlayersShieldbarText, bool, "visuals_players_shieldbar_text");
        STATIC_CONFIG_VALUE(configPlayersShieldbarColor, ImColor,
                            "visuals_players_shieldbar_color");
        STATICsssss_CONFIG_VALUE(configPlayersShieldbarLocations, int32_t,
                                 "visuals_players_shieldbar_location");
        STATIC_CONFIG_VALUE(configPlayersShieldbarTextFont, int32_t,
                            "visuals_players_shieldbar_text_font");
        STATIC_CONFIG_VALUE(configPlayersShieldbarTextLimit, float,
                            "visuals_players_shieldbar_text_limit");

        if (frontend::Menu::foldable_checkbox("Shieldbar", configPlayersShieldbar)) {
          ImGui::Indent();

          ImGui::SeparatorText("Properties");

          frontend::Menu::color_edit("##shieldbar_color", configPlayersShieldbarColor);
          frontend::Menu::location_combo("##shieldbar_location", configPlayersShieldbarLocations);

          ImGui::SeparatorText("Children");

          if (frontend::Menu::foldable_checkbox("Text ##shieldbar_text",
                                                configPlayersShieldbarText)) {
            ImGui::Indent();

            ImGui::SeparatorText("Properties");

            frontend::Menu::percent_slider("##shieldbar_text_limit",
                                           configPlayersShieldbarTextLimit);
            frontend::Menu::font_combo("##shieldbar_text_font", configPlayersShieldbarTextFont);

            ImGui::Unindent();
          }

          ImGui::Unindent();
        }

        STATIC_CONFIG_VALUE(configPlayersName, bool, "visuals_players_name");
        STATIC_CONFIG_VALUE(configPlayersNameFonts, int32_t, "visuals_players_name_font");
        STATIC_CONFIG_VALUE(configPlayersNameColor, ImColor, "visuals_players_name_color");
        STATIC_CONFIG_VALUE(configPlayersNameLocations, int32_t, "visuals_players_name_location");

        if (frontend::Menu::foldable_checkbox("Name", configPlayersName)) {
          ImGui::Indent();

          ImGui::SeparatorText("Properties");

          frontend::Menu::color_edit("##name_color", configPlayersNameColor);
          frontend::Menu::location_combo("##name_location", configPlayersNameLocations);
          frontend::Menu::font_combo("##name_font", configPlayersNameFonts);

          ImGui::Unindent();
        }

        frontend::Menu::end_group();
      }

      ImGui::EndChild();
      ImGui::EndTabItem();
    }
  }
}
