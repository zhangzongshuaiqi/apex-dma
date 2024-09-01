#pragma once

#include <pch.hpp>

#include <display.hpp>

namespace backend {
class LOGGER(Render) {
public:
  using RenderExtension = void();

  enum Font : int32_t {
    Font_Roboto = 0,
    Font_Verdana,
    Font_VerdanaBold,
    Font_SmallestPixel,
  };

  enum TextAlign : int32_t {
    TextAlign_None = 0,
    TextAlign_Center_Horizontal = 1 << 0,
    TextAlign_Center_Vertical = 1 << 1,
    TextAlign_Top_Vertical = 1 << 2,
    TextAlign_Bottom_Vertical = 1 << 3,
    TextAlign_Right_Horizontal = 1 << 4,
  };

  enum TextFlags : int32_t {
    TextFlags_None = 0,
    TextFlags_Shadow = 1 << 0,
    TextFlags_Outline = 1 << 1,
  };

  Render(const std::function<RenderExtension>& extension = [] {}) : _extension(extension) {
    const auto& displayRect = Display::get()->get_window_rect();
    const auto& displayWindow = Display::get()->get_window();

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = displayRect.right;
    swapChainDesc.BufferDesc.Height = displayRect.bottom;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = displayWindow;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    const D3D_FEATURE_LEVEL featureLevels[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    D3D_FEATURE_LEVEL featureLevel;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 2,
                                  D3D11_SDK_VERSION, &swapChainDesc, &_swap_chain, &_device,
                                  &featureLevel, &_device_context);

    logger->debug("Created device and swap chain");

    ID3D11Texture2D* backBuffer = nullptr;

    _swap_chain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

    _device->CreateRenderTargetView(backBuffer, nullptr, &_render_target_view);

    backBuffer->Release();

    // Debatable if this should be in here or if it should go in the menu
    // frontend module... I think it's okay going here as it won't really be
    // cared for by the end user but rather the developer.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsLight();

    ImGui::GetStyle().WindowRounding = 3.f;
    ImGui::GetStyle().ChildRounding = 2.f;

    ImGui_ImplWin32_Init(displayWindow);
    ImGui_ImplDX11_Init(_device, _device_context);

    ImFontConfig fontConfig = {};

    fontConfig.GlyphRanges = ImGui::GetIO().Fonts->GetGlyphRangesChineseFull();

    _roboto_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(robotoFontData.data(),
                                                              robotoFontData.size(), 16.f);

    logger->debug("Created Roboto font");

    _verdana_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(verdanaFontData.data(),
                                                               verdanaFontData.size(), 16.f);

    logger->debug("Created Verdana font");

    _verdana_bold_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
        verdanaBoldFontData.data(), verdanaBoldFontData.size(), 16.f);

    logger->debug("Created Verdana Bold font");

    _smallest_pixel_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
        smallestPixelFontData.data(), smallestPixelFontData.size(), 16.f);

    logger->debug("Created Smallest Pixel font");
  }

  ~Render() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    // ImGui::DestroyContext();

    logger->debug("Destroyed ImGui context");

    if (_render_target_view) {
      logger->debug("Releasing render target view");

      _render_target_view->Release();
      _render_target_view = nullptr;
    }

    if (_swap_chain) {
      logger->debug("Releasing swap chain");

      _swap_chain->Release();
      _swap_chain = nullptr;
    }

    if (_device) {
      logger->debug("Releasing device");

      _device->Release();
      _device = nullptr;
    }

    if (_device_context) {
      logger->debug("Releasing device context");

      _device_context->Release();
      _device_context = nullptr;
    }
  }

  void routine();

  static void rect(const glm::ivec2& min,
                   const glm::ivec2& max,
                   const ImColor& color,
                   const float& rounding = 0.f,
                   const float& thickness = 1.f) {
    ImVec2 imMin = {static_cast<float>(min.x), static_cast<float>(min.y)};
    ImVec2 imMax = {static_cast<float>(max.x), static_cast<float>(max.y)};

    ImGui::GetBackgroundDrawList()->AddRect(imMin, imMax, color, rounding, ImDrawFlags_None,
                                            thickness);
  }

  static void rect_filled(const glm::ivec2& min,
                          const glm::ivec2& max,
                          const ImColor& color,
                          const float& rounding = 0.f) {
    ImVec2 imMin = {static_cast<float>(min.x), static_cast<float>(min.y)};
    ImVec2 imMax = {static_cast<float>(max.x), static_cast<float>(max.y)};

    ImGui::GetBackgroundDrawList()->AddRectFilled(imMin, imMax, color, rounding);
  }

  static void text(const std::string& text,
                   const glm::ivec2& pos,
                   const ImColor& color,
                   const int32_t font,
                   const int32_t align = TextAlign_None,
                   const int32_t flags = TextFlags_None) {
    get()->_text_queue.push_back([text, pos, color, font, align, flags] {
      text_queue(text, pos, color, font, align, flags);
    });
  }

  static glm::ivec2 text_size(const std::string& text, const int32_t font) {
    ImFont* imFont = nullptr;

    switch (font) {
      case Font_Roboto:
        imFont = get()->_roboto_font;

        break;
      case Font_Verdana:
        imFont = get()->_verdana_font;

        break;
      case Font_VerdanaBold:
        imFont = get()->_verdana_bold_font;

        break;
      default:
        imFont = get()->_roboto_font;

        break;
    }

    const auto& textSize = ImGui::CalcTextSize(text.c_str());

    return {static_cast<int32_t>(textSize.x), static_cast<int32_t>(textSize.y)};
  }

private:
  static void text_queue(const std::string& text,
                         const glm::ivec2& pos,
                         const ImColor& color,
                         const int32_t font,
                         const int32_t align = TextAlign_None,
                         const int32_t flags = TextFlags_None) {
    ImVec2 imPos = {static_cast<float>(pos.x), static_cast<float>(pos.y)};

    ImFont* imFont = nullptr;

    if (align & TextAlign_Center_Horizontal)
      imPos.x -= ImGui::CalcTextSize(text.c_str()).x / 2.f;

    if (align & TextAlign_Center_Vertical)
      imPos.y -= ImGui::CalcTextSize(text.c_str()).y / 2.f;

    if (align & TextAlign_Top_Vertical)
      imPos.y -= ImGui::CalcTextSize(text.c_str()).y;

    if (align & TextAlign_Bottom_Vertical)
      imPos.y += ImGui::CalcTextSize(text.c_str()).y;

    if (align & TextAlign_Right_Horizontal)
      imPos.x -= ImGui::CalcTextSize(text.c_str()).x;

    switch (font) {
      case Font_Roboto:
        imFont = get()->_roboto_font;

        break;
      case Font_Verdana:
        imFont = get()->_verdana_font;

        break;
      case Font_VerdanaBold:
        imFont = get()->_verdana_bold_font;

        break;
      case Font_SmallestPixel:
        imFont = get()->_smallest_pixel_font;

        break;
      default:
        imFont = get()->_roboto_font;

        break;
    }

    if (flags & TextFlags_Shadow) {
      ImGui::GetBackgroundDrawList()->AddText(imFont, imFont->FontSize, imPos + ImVec2{1, 1},
                                              IM_COL32(0, 0, 0, 255 / 2), text.c_str());
    }

    if (flags & TextFlags_Outline) {
      for (size_t i = 0; i < 0b100; i++) {
        ImGui::GetBackgroundDrawList()->AddText(
            imFont, imFont->FontSize, imPos + ImVec2{i & 0b01 ? 1.f : -1.f, i & 0b10 ? 1.f : -1.f},
            IM_COL32(0, 0, 0, 255 / 2), text.c_str());
      }
    }

    ImGui::GetBackgroundDrawList()->AddText(imFont, imFont->FontSize, imPos, color, text.c_str());
  }

  IDXGISwapChain* _swap_chain{};
  ID3D11Device* _device{};
  ID3D11DeviceContext* _device_context{};
  ID3D11RenderTargetView* _render_target_view{};

  std::function<RenderExtension> _extension{};

  std::vector<std::function<void()>> _text_queue{};

  ImFont* _roboto_font{};
  ImFont* _verdana_font{};
  ImFont* _verdana_bold_font{};
  ImFont* _smallest_pixel_font{};
};
}  // namespace backend