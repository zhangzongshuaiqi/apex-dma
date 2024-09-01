#pragma once

#include <pch.hpp>

namespace backend {
class LOGGER(Input) {
public:
  Input() = default;
  ~Input() = default;

  void routine();

  static LRESULT CALLBACK window_procedure(HWND window,
                                           UINT message,
                                           WPARAM short_param,
                                           LPARAM long_param);
};
}  // namespace backend