#pragma once

#include <pch.hpp>

namespace frontend {
class LOGGER(Menu) {
public:
  using MenuExtension = void(void);

  Menu(const std::function<MenuExtension>& extension = [] {}) : _extension(extension) {};
  ~Menu() = default;

  void routine();

  static bool begin_group(const char* id, const bool reserve = true);
  static void end_group();

  static void color_edit(const char* id, ImColor& color, const bool alpha = true);
  static bool foldable_checkbox(const char* id, bool& value);

  static void percent_slider(const char* id, float& value);
  static void location_combo(const char* id, int32_t& location);
  static void font_combo(const char* id, int32_t& font);

private:
  std::function<MenuExtension> _extension{};
};

}  // namespace frontend