#include <menu.hpp>

#include <config.hpp>
#include <memory.hpp>
#include <stopper.hpp>

bool frontend::Menu::begin_group(const char* id, const bool reserve) {
  const auto& paddedSpace =
      ImGui::GetContentRegionAvail().x - (reserve ? ImGui::GetStyle().ItemSpacing.x : 0.f);

  bool allGood = false;

  if (ImGui::BeginChild(
          id, ImVec2(paddedSpace / (reserve ? 2.f : 1.f), 0.f),
          ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY,
          ImGuiWindowFlags_MenuBar)) {
    allGood = true;

    if (ImGui::BeginMenuBar()) {
      ImGui::MenuItem(id, "", nullptr, false);

      ImGui::EndMenuBar();
    }
  }

  return allGood;
}

void frontend::Menu::color_edit(const char* id, ImColor& color, const bool alpha) {
  ImGui::ColorEdit4(id, &color.Value.x,
                    ImGuiColorEditFlags_NoLabel |
                        (alpha ? ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar
                               : ImGuiColorEditFlags_None));
}

bool frontend::Menu::foldable_checkbox(const char* id, bool& value) {
  static std::unordered_map<std::string, bool> foldableStates{};

  if (!foldableStates.contains(id))
    foldableStates[id] = false;

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));

  if (ImGui::ArrowButton((std::string(id) + " ##arrow").c_str(),
                         foldableStates[id] ? ImGuiDir_Down : ImGuiDir_Right))
    foldableStates[id] = !foldableStates[id];

  ImGui::PopStyleColor(3);

  ImGui::SameLine();

  ImGui::Checkbox(id, &value);

  return foldableStates[id];
}

void frontend::Menu::percent_slider(const char* id, float& value) {
  int32_t percent = static_cast<int32_t>(value * 100.f);

  if (ImGui::SliderInt(id, &percent, 0, 100, "%d%%"))
    value = static_cast<float>(percent) / 100.f;
}
void frontend::Menu::location_combo(const char* id, int32_t& location) {
  ImGui::Combo(id, &location, "Top\0Bottom\0Left\0Right\0");
}
void frontend::Menu::font_combo(const char* id, int32_t& font) {
  ImGui::Combo(id, &font, "Roboto\0Verdana\0Verdana Bold\0Smallest Pixel\0");
}

void frontend::Menu::end_group() {
  ImGui::EndChild();
}

void frontend::Menu::routine() {
  static bool windowOpen = true;

  static bool newModal = false;
  static bool deleteModal = false;
  static bool saveModal = false;
  static bool loadModal = false;

  static std::string currentConfig = "";

  static const auto& fetchConfigList = [] -> std::vector<std::string> {
    std::vector<std::string> list;

    for (const auto& entry : std::filesystem::directory_iterator("C:\\wednesday.wtf\\configs\\")) {
      const auto& path = entry.path();
      const auto& filename = path.filename().string();
      const auto& filenameShort = filename.substr(0, filename.size() - std::string(".json").size());

      if (entry.is_regular_file() && path.extension() == ".json")
        list.push_back(filenameShort);
    }

    return list;
  };

  static std::vector<std::string> configList = fetchConfigList();

  if (ImGui::Begin("wednesday.wtf", &windowOpen, ImGuiWindowFlags_NoCollapse)) {
    if (ImGui::BeginTabBar("##tabs")) {
      _extension();

      if (ImGui::BeginTabItem("Connection")) {
        if (ImGui::BeginChild("##connection")) {
          if (begin_group("Client")) {
            ImGui::Text("Windows Version: %s", backend::Memory::get()->windows_version().c_str());

            ImGui::Spacing();

            ImGui::Text("PCILeech Version: %s", backend::Memory::get()->fpga_version().c_str());
            ImGui::Text("PCILeech Identifier: %s", backend::Memory::get()->fpga_id().c_str());
          }

          end_group();

          ImGui::SameLine();

          if (begin_group("Server", false)) {
          }

          end_group();
        }

        ImGui::EndChild();
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Settings")) {
        if (ImGui::BeginChild("##settings")) {
          if (begin_group("Menu")) {
            STATIC_CONFIG_VALUE(configMenuDarkMode, bool, "menu_dark_mode");

            static bool lastDarkMode = configMenuDarkMode;

            ImGui::Checkbox("Dark Mode", &configMenuDarkMode);

            if (configMenuDarkMode != lastDarkMode) {
              configMenuDarkMode ? ImGui::StyleColorsDark() : ImGui::StyleColorsLight();

              lastDarkMode = configMenuDarkMode;
            }
          }

          end_group();

          ImGui::SameLine();

          if (begin_group("Configs", false)) {
            if (ImGui::BeginListBox("##configs", ImVec2(-1, 0))) {
              for (const auto& entry : configList) {
                if (ImGui::Selectable(entry.c_str(), currentConfig == entry))
                  currentConfig = currentConfig == entry ? "" : entry;
              }

              ImGui::EndListBox();
            }

            if (ImGui::Button("Load") && !currentConfig.empty())
              loadModal = true;

            ImGui::SameLine();

            if (ImGui::Button("Save") && !currentConfig.empty())
              saveModal = true;

            ImGui::SameLine();

            if (ImGui::Button("Delete") && !currentConfig.empty())
              deleteModal = true;

            ImGui::SameLine();

            if (ImGui::Button("New"))
              newModal = true;

            ImGui::SameLine();

            if (ImGui::Button("Refresh")) {
              configList.clear();

              configList = fetchConfigList();
            }
          }

          end_group();
        }

        ImGui::EndChild();
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    if (newModal) {
      ImGui::OpenPopup("New Config");

      if (ImGui::BeginPopupModal("New Config", &newModal, ImGuiWindowFlags_AlwaysAutoResize)) {
        static std::string buffer = "";

        ImGui::InputText("##name", &buffer);

        if (ImGui::Button("Create")) {
          std::ofstream file("C:\\wednesday.wtf\\configs\\" + buffer + ".json");

          file << "{}";
          file.close();

          newModal = false;

          buffer.clear();
          configList.clear();

          configList = fetchConfigList();
        }

        ImGui::EndPopup();
      }
    }

    if (deleteModal) {
      ImGui::OpenPopup("Delete Config");

      if (ImGui::BeginPopupModal("Delete Config", &deleteModal,
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete %s?", currentConfig.c_str());

        if (ImGui::Button("Confirm")) {
          std::filesystem::remove("C:\\wednesday.wtf\\configs\\" + currentConfig + ".json");

          currentConfig.clear();
          configList.clear();

          configList = fetchConfigList();

          deleteModal = false;
        }

        ImGui::EndPopup();
      }
    }

    if (saveModal) {
      ImGui::OpenPopup("Save Config");

      if (ImGui::BeginPopupModal("Save Config", &saveModal, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to save %s?", currentConfig.c_str());

        if (ImGui::Button("Save")) {
          backend::Config::get()->save("C:\\wednesday.wtf\\configs\\" + currentConfig + ".json");

          saveModal = false;
        }

        ImGui::EndPopup();
      }
    }

    if (loadModal) {
      ImGui::OpenPopup("Load Config");

      if (ImGui::BeginPopupModal("Load Config", &loadModal, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to load %s?", currentConfig.c_str());

        if (ImGui::Button("Load")) {
          backend::Config::get()->load("C:\\wednesday.wtf\\configs\\" + currentConfig + ".json");

          loadModal = false;
        }

        ImGui::EndPopup();
      }
    }

    ImGui::End();

    if (!windowOpen) {
      backend::Stopper::get()->stop();

      logger->info("Window closed");
    }
  }
}