#pragma once

#include <pch.hpp>

#include <input.hpp>

namespace backend {
class LOGGER(Display) {
public:
  Display() {
    WNDCLASSEX windowClass = {};

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = backend::Input::window_procedure;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    windowClass.lpszClassName = "wednesday000";
    windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.lpszMenuName = "Wednesday Overlay";

    RegisterClassEx(&windowClass);

    GetWindowRect(GetDesktopWindow(), &_window_rect);

    _window = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
                             windowClass.lpszClassName, windowClass.lpszMenuName, WS_POPUP, 0, 0,
                             _window_rect.right, _window_rect.bottom, nullptr, nullptr,
                             windowClass.hInstance, nullptr);

    if (!_window) {
      logger->critical("Failed to create window");

      return;
    }

    const MARGINS margins{0, 0, _window_rect.right, _window_rect.bottom};

    DwmExtendFrameIntoClientArea(_window, &margins);

    ShowWindow(_window, SW_SHOW);

    // update(true);
  }

  ~Display() {
    DestroyWindow(_window);

    UnregisterClass("wednesday000", GetModuleHandle(nullptr));

    _window = nullptr;
  }

  constexpr inline HWND get_window() const { return _window; }
  constexpr inline RECT get_window_rect() const { return _window_rect; }

  inline void update(const bool passthrough) const {
    SetWindowLong(
        _window, GWL_EXSTYLE,
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT | (passthrough ? WS_EX_LAYERED : 0));
  }

private:
  HWND _window{};
  RECT _window_rect{};
};
}  // namespace backend
