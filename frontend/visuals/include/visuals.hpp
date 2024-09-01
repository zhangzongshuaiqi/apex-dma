#pragma once

#include <pch.hpp>

#include <render.hpp>

namespace frontend {
class SINGLETON(Visuals) {
public:
  enum Location : int32_t {
    Location_Top = 0,
    Location_Bottom,
    Location_Left,
    Location_Right,
  };

  using VisualsExtension = void(void);

  Visuals(const std::function<VisualsExtension>& extension = [] {}) : _extension(extension) {};
  ~Visuals() = default;

  void routine();

  void reset_offsets(const glm::vec2& mins, const glm::vec2& maxs);
  void render_box(const ImColor& color);
  void render_bar(const int32_t location,
                  const float fraction,
                  const ImColor& color,
                  const std::string& text = "",
                  const int32_t font = backend::Render::Font_VerdanaBold,
                  const int32_t hue_shift = 120);
  void render_text(const std::string& text,
                   const int32_t font,
                   const int32_t flags,
                   const int32_t location,
                   const ImColor& color);

private:
  std::function<VisualsExtension> _extension{};

  glm::ivec2 _mins{};
  glm::ivec2 _maxs{};
  glm::ivec4 _offsets{};
  glm::ivec4 _text_offsets{};
};
}  // namespace frontend