//==--- ripple/glow/vk/window/window.hpp ------------------- -*- C++ -*- ---==//
//
//                            Ripple - Glow
//
//                      Copyright (c) 2020 Ripple
//
//  This file is distributed under the MIT License. See LICENSE for details.
//
//==------------------------------------------------------------------------==//
//
/// \file  window.hpp
/// \brief This file defines a window.
//
//==------------------------------------------------------------------------==//

#ifndef RIPPLE_GLOW_WINDOW_WINDOW_HPP
#define RIPPLE_GLOW_WINDOW_WINDOW_HPP

#include "../engine/engine.hpp"
#include "../backend/platform/platform.hpp"
#include <ripple/core/util/portability.hpp>

namespace ripple::glow {

/// The Window type defines the generic functionality for a window. This can
/// be a native window for the platform, which is the _common_ case, in which
/// case rendering will per performed on the native window, but it could also
/// be rendering to a swapchain which is not displayed, such as in _headless_
/// mode.
class Window {
  // clang-format off
  /// Defines the type of the platform.
  using platform_t   = backend::platform_type_t;
  /// Defines the type of the driver pointer.
  using engine_ptr_t = Engine*;
  // clang-format on

 public:
  //==--- [construction] ---------------------------------------------------==//

  /// Constructor for the window, which sets the \p title and the \p width and
  /// \p height as the dimensions of the window.
  /// \param title  The title of the window.
  /// \param width  The (width) of the window  - pixels in the x dimension.
  /// \param height The (height) of the window - pixels in the y dimension.
  Window(const std::string& title, uint32_t width, uint32_t height);

  // clang-format off

  /// Defaulted destructor.
  ~Window()             = default;
  /// Deleted copy constructor.
  Window(const Window&) = delete;
  /// Deleted move constructor.
  Window(Window&&)      = delete;

  //===--- [operator overloads] --------------------------------------------==//

  /// Deleted copy assignment operator.
  auto operator=(const Window&) -> Window& = delete;
  /// Deleted move assignment operator.
  auto operator=(Window&&) -> Window&      = delete;

  // clang-format on

  //==--- [interface] ------------------------------------------------------==//

  /// Returns true of the window is alive.
  ripple_no_discard auto is_alive() const -> bool;

  /// Polls the event loop of the window, returning an event.
  auto poll_input() -> void;

 private:
  platform_t   _platform;        //!< The platform for the window.
  engine_ptr_t _engine;          //!< Pointer to the engine.
  bool         _is_alive = true; //!< If the window is alive.

  /// Initializes the window and the rendering context for the window.
  auto init() -> bool;
};

} // namespace ripple::glow

#endif // RIPPLE_GLOW_WINDOW_WINDOW_HPP