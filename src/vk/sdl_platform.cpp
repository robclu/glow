//==--- glow/src/vk/platform/sdl_platform.cpp -------------- -*- C++ -*- ---==//
//
//                            Ripple - Glow
//
//                      Copyright (c) 2020 Ripple
//
//  This file is distributed under the MIT License. See LICENSE for details.
//
//==------------------------------------------------------------------------==//
//
/// \file  sdl_paltform.cpp
/// \brief This file defines the implemenation of a platform which uses SDL.
//
//==------------------------------------------------------------------------==//

#include <ripple/core/log/logger.hpp>
#include <ripple/glow/vk/platform/sdl_platform.hpp>
#include <ripple/glow/vk/context.hpp>
#include <SDL_syswm.h>
#include <SDL_vulkan.h>
#include <cassert>

namespace ripple::glow::vk {

SdlPlatform::SdlPlatform(
  const std::string& title, uint32_t width, uint32_t height)
: base_platform_t(width, height) {
  initialize(title);
}

SdlPlatform::~SdlPlatform() {
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

auto SdlPlatform::create_vulkan_surface(
  VkInstance instance, VkPhysicalDevice device) -> VkSurfaceKHR {
  VkSurfaceKHR surface;
  if (SDL_Vulkan_CreateSurface(_window, instance, &surface)) {
    return surface;
  } else {
    return VK_NULL_HANDLE;
  }
}

auto SdlPlatform::initialize(const std::string& title) -> void {
  assert(SDL_Init(SDL_INIT_EVENTS) == 0);

  const int center_x = SDL_WINDOWPOS_CENTERED;
  const int center_y = SDL_WINDOWPOS_CENTERED;
  uint32_t  flags =
    SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN;

  // If using a paltform with a resizable window:
  if constexpr (GLOW_RESIZABLE_WINDOW) {
    flags |= SDL_WINDOW_RESIZABLE;
  }

  _window = SDL_CreateWindow(
    title.c_str(),
    center_x,
    center_y,
    static_cast<int>(this->_width),
    static_cast<int>(this->_height),
    flags);

  if (_window == nullptr) {
    log_error("Failed to create SDL window.");
  }
}

/// Initializes the vulkan loader, returning true if the loading was
/// successul.
auto SdlPlatform::initialize_vulkan_loader() const -> bool {
  if (!vk::Context::init_loader(
        (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr())) {
    log_error("Sdl platform failed to create Vulkan loader.");
    return false;
  }
  return true;
}

auto SdlPlatform::instance_extensions() -> ext_vector_t {
  unsigned num_ins_exts = 0;
  SDL_Vulkan_GetInstanceExtensions(_window, &num_ins_exts, nullptr);
  auto instance_names = ext_vector_t(num_ins_exts);
  SDL_Vulkan_GetInstanceExtensions(
    _window, &num_ins_exts, instance_names.data());
  return instance_names;
}

} // namespace ripple::glow::vk