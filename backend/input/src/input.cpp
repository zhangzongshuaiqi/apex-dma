#include <input.hpp>

#include <stopper.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

void backend::Input::routine() {
  MSG message;

  while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);

    if (message.message == WM_QUIT) {
      backend::Stopper::get()->stop();

      logger->info("WM_QUIT received");

      break;
    }
  }
}

LRESULT CALLBACK backend::Input::window_procedure(HWND window,
                                                  UINT message,
                                                  WPARAM short_param,
                                                  LPARAM long_param) {
  if (ImGui_ImplWin32_WndProcHandler(window, message, short_param, long_param))
    return true;

  switch (message) {
    case WM_DESTROY:
      PostQuitMessage(EXIT_SUCCESS);

      return false;
  }

  return DefWindowProc(window, message, short_param, long_param);
}