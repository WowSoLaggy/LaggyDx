#pragma once

#include <cstdint>


namespace Dx
{
  enum class KeyboardKey : std::int32_t
  {
    Back = 0,          // VK_BACK, 0x8
    Tab,               // VK_TAB, 0x9
    Enter,             // VK_RETURN, 0xD
    Pause,             // VK_PAUSE, 0x13
    CapsLock,          // VK_CAPITAL, 0x14
    Kana,              // VK_KANA, 0x15
    Escape,            // VK_ESCAPE, 0x1B
    ImeConvert,        // VK_CONVERT, 0x1C
    ImeNoConvert,      // VK_NONCONVERT, 0x1D
    Space,             // VK_SPACE, 0x20
    PageUp,            // VK_PRIOR, 0x21
    PageDown,          // VK_NEXT, 0x22
    End,               // VK_END, 0x23
    Home,              // VK_HOME, 0x24
    Left,              // VK_LEFT, 0x25
    Up,                // VK_UP, 0x26
    Right,             // VK_RIGHT, 0x27
    Down,              // VK_DOWN, 0x28
    Select,            // VK_SELECT, 0x29
    Print,             // VK_PRINT, 0x2A
    Execute,           // VK_EXECUTE, 0x2B
    PrintScreen,       // VK_SNAPSHOT, 0x2C
    Insert,            // VK_INSERT, 0x2D
    Delete,            // VK_DELETE, 0x2E
    Help,              // VK_HELP, 0x2F
    D0,                // 0x30
    D1,                // 0x31
    D2,                // 0x32
    D3,                // 0x33
    D4,                // 0x34
    D5,                // 0x35
    D6,                // 0x36
    D7,                // 0x37
    D8,                // 0x38
    D9,                // 0x39
    A,                 // 0x41
    B,                 // 0x42
    C,                 // 0x43
    D,                 // 0x44
    E,                 // 0x45
    F,                 // 0x46
    G,                 // 0x47
    H,                 // 0x48
    I,                 // 0x49
    J,                 // 0x4A
    K,                 // 0x4B
    L,                 // 0x4C
    M,                 // 0x4D
    N,                 // 0x4E
    O,                 // 0x4F
    P,                 // 0x50
    Q,                 // 0x51
    R,                 // 0x52
    S,                 // 0x53
    T,                 // 0x54
    U,                 // 0x55
    V,                 // 0x56
    W,                 // 0x57
    X,                 // 0x58
    Y,                 // 0x59
    Z,                 // 0x5A
    LeftWindows,       // VK_LWIN, 0x5B
    RightWindows,      // VK_RWIN, 0x5C
    Apps,              // VK_APPS, 0x5D
    Sleep,             // VK_SLEEP, 0x5F
    NumPad0,           // VK_NUMPAD0, 0x60
    NumPad1,           // VK_NUMPAD1, 0x61
    NumPad2,           // VK_NUMPAD2, 0x62
    NumPad3,           // VK_NUMPAD3, 0x63
    NumPad4,           // VK_NUMPAD4, 0x64
    NumPad5,           // VK_NUMPAD5, 0x65
    NumPad6,           // VK_NUMPAD6, 0x66
    NumPad7,           // VK_NUMPAD7, 0x67
    NumPad8,           // VK_NUMPAD8, 0x68
    NumPad9,           // VK_NUMPAD9, 0x69
    Multiply,          // VK_MULTIPLY, 0x6A
    Add,               // VK_ADD, 0x6B
    Separator,         // VK_SEPARATOR, 0x6C
    Subtract,          // VK_SUBTRACT, 0x6D
    Decimal,           // VK_DECIMANL, 0x6E
    Divide,            // VK_DIVIDE, 0x6F
    F1,                // VK_F1, 0x70
    F2,                // VK_F2, 0x71
    F3,                // VK_F3, 0x72
    F4,                // VK_F4, 0x73
    F5,                // VK_F5, 0x74
    F6,                // VK_F6, 0x75
    F7,                // VK_F7, 0x76
    F8,                // VK_F8, 0x77
    F9,                // VK_F9, 0x78
    F10,               // VK_F10, 0x79
    F11,               // VK_F11, 0x7A
    F12,               // VK_F12, 0x7B
    F13,               // VK_F13, 0x7C
    F14,               // VK_F14, 0x7D
    F15,               // VK_F15, 0x7E
    F16,               // VK_F16, 0x7F
    F17,               // VK_F17, 0x80
    F18,               // VK_F18, 0x81
    F19,               // VK_F19, 0x82
    F20,               // VK_F20, 0x83
    F21,               // VK_F21, 0x84
    F22,               // VK_F22, 0x85
    F23,               // VK_F23, 0x86
    F24,               // VK_F24, 0x87
    NumLock,           // VK_NUMLOCK, 0x90
    Scroll,            // VK_SCROLL, 0x91
    LeftShift,         // VK_LSHIFT, 0xA0
    RightShift,        // VK_RSHIFT, 0xA1
    LeftControl,       // VK_LCONTROL, 0xA2
    RightControl,      // VK_RCONTROL, 0xA3
    LeftAlt,           // VK_LMENU, 0xA4
    RightAlt,          // VK_RMENU, 0xA5
    BrowserBack,       // VK_BROWSER_BACK, 0xA6
    BrowserForward,    // VK_BROWSER_FORWARD, 0xA7
    BrowserRefresh,    // VK_BROWSER_REFRESH, 0xA8
    BrowserStop,       // VK_BROWSER_STOP, 0xA9
    BrowserSearch,     // VK_BROWSER_SEARCH, 0xAA
    BrowserFavorites,  // VK_BROWSER_FAVORITES, 0xAB
    BrowserHome,       // VK_BROWSER_HOME, 0xAC
    VolumeMute,        // VK_VOLUME_MUTE, 0xAD
    VolumeDown,        // VK_VOLUME_DOWN, 0xAE
    VolumeUp,          // VK_VOLUME_UP, 0xAF
    MediaNextTrack,    // VK_MEDIA_NEXT_TRACK, 0xB0
    MediaPreviousTrack,// VK_MEDIA_PREV_TRACK, 0xB1
    MediaStop,         // VK_MEDIA_STOP, 0xB2
    MediaPlayPause,    // VK_MEDIA_PLAY_PAUSE, 0xB3
    LaunchMail,        // VK_LAUNCH_MAIL, 0xB4
    SelectMedia,       // VK_LAUNCH_MEDIA_SELECT, 0xB5
    LaunchApplication1,// VK_LAUNCH_APP1, 0xB6
    LaunchApplication2,// VK_LAUNCH_APP2, 0xB7
    OemSemicolon,      // VK_OEM_1, 0xBA
    OemPlus,           // VK_OEM_PLUS, 0xBB
    OemComma,          // VK_OEM_COMMA, 0xBC
    OemMinus,          // VK_OEM_MINUS, 0xBD
    OemPeriod,         // VK_OEM_PERIOD, 0xBE
    OemQuestion,       // VK_OEM_2, 0xBF
    OemTilde,          // VK_OEM_3, 0xC0
    OemOpenBrackets,   // VK_OEM_4, 0xDB
    OemPipe,           // VK_OEM_5, 0xDC
    OemCloseBrackets,  // VK_OEM_6, 0xDD
    OemQuotes,         // VK_OEM_7, 0xDE
    Oem8,              // VK_OEM_8, 0xDF
    OemBackslash,      // VK_OEM_102, 0xE2
    ProcessKey,        // VK_PROCESSKEY, 0xE5
    OemCopy,           // 0XF2
    OemAuto,           // 0xF3
    OemEnlW,           // 0xF4
    Attn,              // VK_ATTN, 0xF6
    Crsel,             // VK_CRSEL, 0xF7
    Exsel,             // VK_EXSEL, 0xF8
    EraseEof,          // VK_EREOF, 0xF9
    Play,              // VK_PLAY, 0xFA
    Zoom,              // VK_ZOOM, 0xFB
    Pa1,               // VK_PA1, 0xFD
    OemClear,          // VK_OEM_CLEAR, 0xFE
  };

} // ns Dx
