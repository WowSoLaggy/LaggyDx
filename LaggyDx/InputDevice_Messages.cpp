#include "stdafx.h"
#include "InputDevice.h"

#include <LaggySdk/Message.h>


namespace Dx
{
  void InputDevice::processMessage(const Sdk::Message& i_inputMessage)
  {
    switch (i_inputMessage.message)
    {
    case WM_ACTIVATEAPP:
      Keyboard::ProcessMessage(
        static_cast<UINT>(i_inputMessage.message),
        static_cast<WPARAM>(i_inputMessage.wParam),
        static_cast<LPARAM>(i_inputMessage.lParam));
      Mouse::ProcessMessage(
        static_cast<UINT>(i_inputMessage.message),
        static_cast<WPARAM>(i_inputMessage.wParam),
        static_cast<LPARAM>(i_inputMessage.lParam));
      break;

    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
      Mouse::ProcessMessage(
        static_cast<UINT>(i_inputMessage.message),
        static_cast<WPARAM>(i_inputMessage.wParam),
        static_cast<LPARAM>(i_inputMessage.lParam));
      break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
      Keyboard::ProcessMessage(
        static_cast<UINT>(i_inputMessage.message),
        static_cast<WPARAM>(i_inputMessage.wParam),
        static_cast<LPARAM>(i_inputMessage.lParam));
      break;
    }
  }

} // ns Dx
