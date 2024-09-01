#include <visuals.hpp>

#include <iostream>

void frontend::Visuals::routine() {
  _extension();
}

void frontend::Visuals::reset_offsets(const glm::vec2& mins, const glm::vec2& maxs) {
  _offsets = {};
  _text_offsets = {};

  _mins = mins;
  _maxs = maxs;
}

void frontend::Visuals::render_box(const ImColor& color) {
  backend::Render::rect({_mins.x - 1.f, _mins.y - 1.f}, {_maxs.x + 1.f, _maxs.y + 1.f},
                        {0, 0, 0, static_cast<int32_t>(255.f * color.Value.w) - 255 / 2});
  backend::Render::rect({_mins.x + 1.f, _mins.y + 1.f}, {_maxs.x - 1.f, _maxs.y - 1.f},
                        {0, 0, 0, static_cast<int32_t>(255.f * color.Value.w) - 255 / 2});

  backend::Render::rect(_mins, _maxs, color);
}

void frontend::Visuals::render_bar(const int32_t location,
                                   const float fraction,
                                   const ImColor& color,
                                   const std::string& text,
                                   const int32_t font,
                                   const int32_t hue_shift) {
  ImColor healthHSV = color;

  const auto healthInverse = 1.f - fraction;

  if (hue_shift) {
    ImColor colorHSV{};

    ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, colorHSV.Value.x,
                                colorHSV.Value.y, colorHSV.Value.z);

    const auto healthColor =
        360.f * colorHSV.Value.x - static_cast<float>(hue_shift) * healthInverse;
    healthHSV =
        ImColor::HSV(healthColor / 360.f, colorHSV.Value.y, colorHSV.Value.z, color.Value.w);
  }

  switch (location) {
    case Location_Top: {
      auto topMins = glm::vec2{_mins.x - 1, _mins.y - 6} + glm::vec2{0, _offsets[Location_Top]};
      auto topMaxs = glm::vec2{_maxs.x + 1, _mins.y - 2} + glm::vec2{0, _offsets[Location_Top]};

      backend::Render::rect_filled(
          topMins, topMaxs, {0, 0, 0, static_cast<int32_t>(color.Value.w * 255.f) - 255 / 2});

      topMins += glm::vec2{1, 1};
      topMaxs -= glm::vec2{1, 1};

      const auto& healthWidth = (topMaxs.x - topMins.x) * healthInverse;

      backend::Render::rect_filled(topMins, topMaxs - glm::vec2{healthWidth, 0}, healthHSV);

      if (!text.empty()) {
        backend::Render::text(text, topMaxs - glm::vec2{healthWidth, 0}, healthHSV, font,
                              backend::Render::TextAlign_Center_Horizontal |
                                  backend::Render::TextAlign_Center_Vertical,
                              backend::Render::TextFlags_Outline);
      }

      _offsets[Location_Top] -= 5;

      break;
    }
    case Location_Bottom: {
      auto topMins = glm::vec2{_mins.x - 1, _maxs.y + 2} + glm::vec2{0, _offsets[Location_Bottom]};
      auto topMaxs = glm::vec2{_maxs.x + 1, _maxs.y + 6} + glm::vec2{0, _offsets[Location_Bottom]};

      backend::Render::rect_filled(
          topMins, topMaxs, {0, 0, 0, static_cast<int32_t>(color.Value.w * 255.f) - 255 / 2});

      topMins += glm::vec2{1, 1};
      topMaxs -= glm::vec2{1, 1};

      const auto& healthWidth = (topMaxs.x - topMins.x) * healthInverse;

      backend::Render::rect_filled(topMins, topMaxs - glm::vec2{healthWidth, 0}, healthHSV);

      if (!text.empty()) {
        backend::Render::text(text, topMaxs - glm::vec2{healthWidth, 0}, healthHSV, font,
                              backend::Render::TextAlign_Center_Horizontal |
                                  backend::Render::TextAlign_Center_Vertical,
                              backend::Render::TextFlags_Outline);
      }

      _offsets[Location_Bottom] += 5;

      break;
    }
    case Location_Left: {
      auto topMins = glm::vec2{_mins.x - 6, _mins.y - 1} + glm::vec2{_offsets[Location_Left], 0};
      auto topMaxs = glm::vec2{_mins.x - 2, _maxs.y + 1} + glm::vec2{_offsets[Location_Left], 0};

      backend::Render::rect_filled(
          topMins, topMaxs, {0, 0, 0, static_cast<int32_t>(color.Value.w * 255.f) - 255 / 2});

      topMins += glm::vec2{1, 1};
      topMaxs -= glm::vec2{1, 1};

      const auto& healthWidth = (topMaxs.y - topMins.y) * healthInverse;

      backend::Render::rect_filled(topMins + glm::vec2{0, healthWidth}, topMaxs, healthHSV);

      if (!text.empty()) {
        backend::Render::text(text, topMins + glm::vec2{0, healthWidth}, healthHSV, font,
                              backend::Render::TextAlign_Center_Horizontal |
                                  backend::Render::TextAlign_Center_Vertical,
                              backend::Render::TextFlags_Outline);
      }

      _offsets[Location_Left] -= 5;

      break;
    }
    case Location_Right: {
      auto topMins = glm::vec2{_maxs.x + 2, _mins.y - 1} + glm::vec2{_offsets[Location_Right], 0};
      auto topMaxs = glm::vec2{_maxs.x + 6, _maxs.y + 1} + glm::vec2{_offsets[Location_Right], 0};

      backend::Render::rect_filled(
          topMins, topMaxs, {0, 0, 0, static_cast<int32_t>(color.Value.w * 255.f) - 255 / 2});

      topMins += glm::vec2{1, 1};
      topMaxs -= glm::vec2{1, 1};

      const auto& healthWidth = (topMaxs.y - topMins.y) * healthInverse;

      backend::Render::rect_filled(topMins + glm::vec2{0, healthWidth}, topMaxs, healthHSV);

      if (!text.empty()) {
        backend::Render::text(text, topMins + glm::vec2{0, healthWidth}, healthHSV, font,
                              backend::Render::TextAlign_Center_Horizontal |
                                  backend::Render::TextAlign_Center_Vertical,
                              backend::Render::TextFlags_Outline);
      }

      _offsets[Location_Right] += 5;

      break;
    }
  }
}

void frontend::Visuals::render_text(const std::string& text,
                                    const int32_t font,
                                    const int32_t flags,
                                    const int32_t location,
                                    const ImColor& color) {
  switch (location) {
    case Location_Top: {
      backend::Render::text(
          text, {_mins.x + (_maxs.x - _mins.x) / 2, _mins.y - 3 + _offsets[Location_Top]}, color,
          font,
          backend::Render::TextAlign_Center_Horizontal | backend::Render::TextAlign_Top_Vertical,
          flags);

      _offsets[Location_Top] -= backend::Render::text_size(text, font).y + 2;

      break;
    }
    case Location_Bottom: {
      backend::Render::text(
          text, {_mins.x + (_maxs.x - _mins.x) / 2, _maxs.y + 3 + _offsets[Location_Bottom]}, color,
          font, backend::Render::TextAlign_Center_Horizontal, flags);

      _offsets[Location_Bottom] += backend::Render::text_size(text, font).y + 2;

      break;
    }
    case Location_Left: {
      backend::Render::text(
          text, {_mins.x - 3 + _offsets[Location_Left], _mins.y + _text_offsets[Location_Left]},
          color, font, backend::Render::TextAlign_Right_Horizontal, flags);

      _text_offsets[Location_Left] += backend::Render::text_size(text, font).y + 2;

      break;
    }
    case Location_Right: {
      backend::Render::text(
          text, {_maxs.x + 3 + _offsets[Location_Right], _mins.y + _text_offsets[Location_Right]},
          color, font, backend::Render::TextAlign_None, flags);

      _text_offsets[Location_Right] += backend::Render::text_size(text, font).y + 2;

      break;
    }
  }
}