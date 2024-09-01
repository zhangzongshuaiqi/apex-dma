#include <render.hpp>

void backend::Render::routine() {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();

  ImGui::NewFrame();

  _extension();

  for (const auto& text : _text_queue)
    text();

  _text_queue.clear();

  ImGui::Render();

  const float clearColor[] = {0.f, 0.f, 0.f, 0.f};

  _device_context->OMSetRenderTargets(1, &_render_target_view, nullptr);
  _device_context->ClearRenderTargetView(_render_target_view, clearColor);

  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  _swap_chain->Present(1, 0);
}