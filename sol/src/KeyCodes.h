#pragma once

#include <SDL_keycode.h>
#include <SDL_mouse.h>
#undef main
#include <iostream>

// based on the SDL key codes, and by doing so, no conversion is required when
// using SDL2 as the windowing library. a map lookup might be required if using
// a different one, though (e.g.: GLFW, WinAPI, etc.).

// the following defines are copy-paste from SDL_keycode.h

namespace sol
{
enum KeyCode
{
	SOL_UNKNOWN = SDLK_UNKNOWN,

	SOL_RETURN     = SDLK_RETURN,
	SOL_ESCAPE     = SDLK_ESCAPE,
	SOL_BACKSPACE  = SDLK_BACKSPACE,
	SOL_TAB        = SDLK_TAB,
	SOL_SPACE      = SDLK_SPACE,
	SOL_EXCLAIM    = SDLK_EXCLAIM,
	SOL_QUOTEDBL   = SDLK_QUOTEDBL,
	SOL_HASH       = SDLK_HASH,
	SOL_PERCENT    = SDLK_PERCENT,
	SOL_DOLLAR     = SDLK_DOLLAR,
	SOL_AMPERSAND  = SDLK_AMPERSAND,
	SOL_QUOTE      = SDLK_QUOTE,
	SOL_LEFTPAREN  = SDLK_LEFTPAREN,
	SOL_RIGHTPAREN = SDLK_RIGHTPAREN,
	SOL_ASTERISK   = SDLK_ASTERISK,
	SOL_PLUS       = SDLK_PLUS,
	SOL_COMMA      = SDLK_COMMA,
	SOL_MINUS      = SDLK_MINUS,
	SOL_PERIOD     = SDLK_PERIOD,
	SOL_SLASH      = SDLK_SLASH,
	SOL_0          = SDLK_0,
	SOL_1          = SDLK_1,
	SOL_2          = SDLK_2,
	SOL_3          = SDLK_3,
	SOL_4          = SDLK_4,
	SOL_5          = SDLK_5,
	SOL_6          = SDLK_6,
	SOL_7          = SDLK_7,
	SOL_8          = SDLK_8,
	SOL_9          = SDLK_9,
	SOL_COLON      = SDLK_COLON,
	SOL_SEMICOLON  = SDLK_SEMICOLON,
	SOL_LESS       = SDLK_LESS,
	SOL_EQUALS     = SDLK_EQUALS,
	SOL_GREATER    = SDLK_GREATER,
	SOL_QUESTION   = SDLK_QUESTION,
	SOL_AT         = SDLK_AT,

	SOL_LEFTBRACKET  = SDLK_LEFTBRACKET,
	SOL_BACKSLASH    = SDLK_BACKSLASH,
	SOL_RIGHTBRACKET = SDLK_RIGHTBRACKET,
	SOL_CARET        = SDLK_CARET,
	SOL_UNDERSCORE   = SDLK_UNDERSCORE,
	SOL_BACKQUOTE    = SDLK_BACKQUOTE,
	SOL_a            = SDLK_a,
	SOL_b            = SDLK_b,
	SOL_c            = SDLK_c,
	SOL_d            = SDLK_d,
	SOL_e            = SDLK_e,
	SOL_f            = SDLK_f,
	SOL_g            = SDLK_g,
	SOL_h            = SDLK_h,
	SOL_i            = SDLK_i,
	SOL_j            = SDLK_j,
	SOL_k            = SDLK_k,
	SOL_l            = SDLK_l,
	SOL_m            = SDLK_m,
	SOL_n            = SDLK_n,
	SOL_o            = SDLK_o,
	SOL_p            = SDLK_p,
	SOL_q            = SDLK_q,
	SOL_r            = SDLK_r,
	SOL_s            = SDLK_s,
	SOL_t            = SDLK_t,
	SOL_u            = SDLK_u,
	SOL_v            = SDLK_v,
	SOL_w            = SDLK_w,
	SOL_x            = SDLK_x,
	SOL_y            = SDLK_y,
	SOL_z            = SDLK_z,

	SOL_CAPSLOCK = SDLK_CAPSLOCK,

	SOL_F1  = SDLK_F1,
	SOL_F2  = SDLK_F2,
	SOL_F3  = SDLK_F3,
	SOL_F4  = SDLK_F4,
	SOL_F5  = SDLK_F5,
	SOL_F6  = SDLK_F6,
	SOL_F7  = SDLK_F7,
	SOL_F8  = SDLK_F8,
	SOL_F9  = SDLK_F9,
	SOL_F10 = SDLK_F10,
	SOL_F11 = SDLK_F11,
	SOL_F12 = SDLK_F12,

	SOL_PRINTSCREEN = SDLK_PRINTSCREEN,
	SOL_SCROLLLOCK  = SDLK_SCROLLLOCK,
	SOL_PAUSE       = SDLK_PAUSE,
	SOL_INSERT      = SDLK_INSERT,
	SOL_HOME        = SDLK_HOME,
	SOL_PAGEUP      = SDLK_PAGEUP,
	SOL_DELETE      = SDLK_DELETE,
	SOL_END         = SDLK_END,
	SOL_PAGEDOWN    = SDLK_PAGEDOWN,
	SOL_RIGHT       = SDLK_RIGHT,
	SOL_LEFT        = SDLK_LEFT,
	SOL_DOWN        = SDLK_DOWN,
	SOL_UP          = SDLK_UP,

	SOL_NUMLOCKCLEAR = SDLK_NUMLOCKCLEAR,
	SOL_KP_DIVIDE    = SDLK_KP_DIVIDE,
	SOL_KP_MULTIPLY  = SDLK_KP_MULTIPLY,
	SOL_KP_MINUS     = SDLK_KP_MINUS,
	SOL_KP_PLUS      = SDLK_KP_PLUS,
	SOL_KP_ENTER     = SDLK_KP_ENTER,
	SOL_KP_1         = SDLK_KP_1,
	SOL_KP_2         = SDLK_KP_2,
	SOL_KP_3         = SDLK_KP_3,
	SOL_KP_4         = SDLK_KP_4,
	SOL_KP_5         = SDLK_KP_5,
	SOL_KP_6         = SDLK_KP_6,
	SOL_KP_7         = SDLK_KP_7,
	SOL_KP_8         = SDLK_KP_8,
	SOL_KP_9         = SDLK_KP_9,
	SOL_KP_0         = SDLK_KP_0,
	SOL_KP_PERIOD    = SDLK_KP_PERIOD,

	SOL_APPLICATION    = SDLK_APPLICATION,
	SOL_POWER          = SDLK_POWER,
	SOL_KP_EQUALS      = SDLK_KP_EQUALS,
	SOL_F13            = SDLK_F13,
	SOL_F14            = SDLK_F14,
	SOL_F15            = SDLK_F15,
	SOL_F16            = SDLK_F16,
	SOL_F17            = SDLK_F17,
	SOL_F18            = SDLK_F18,
	SOL_F19            = SDLK_F19,
	SOL_F20            = SDLK_F20,
	SOL_F21            = SDLK_F21,
	SOL_F22            = SDLK_F22,
	SOL_F23            = SDLK_F23,
	SOL_F24            = SDLK_F24,
	SOL_EXECUTE        = SDLK_EXECUTE,
	SOL_HELP           = SDLK_HELP,
	SOL_MENU           = SDLK_MENU,
	SOL_SELECT         = SDLK_SELECT,
	SOL_STOP           = SDLK_STOP,
	SOL_AGAIN          = SDLK_AGAIN,
	SOL_UNDO           = SDLK_UNDO,
	SOL_CUT            = SDLK_CUT,
	SOL_COPY           = SDLK_COPY,
	SOL_PASTE          = SDLK_PASTE,
	SOL_FIND           = SDLK_FIND,
	SOL_MUTE           = SDLK_MUTE,
	SOL_VOLUMEUP       = SDLK_VOLUMEUP,
	SOL_VOLUMEDOWN     = SDLK_VOLUMEDOWN,
	SOL_KP_COMMA       = SDLK_KP_COMMA,
	SOL_KP_EQUALSAS400 = SDLK_KP_EQUALSAS400,

	SOL_ALTERASE   = SDLK_ALTERASE,
	SOL_SYSREQ     = SDLK_SYSREQ,
	SOL_CANCEL     = SDLK_CANCEL,
	SOL_CLEAR      = SDLK_CLEAR,
	SOL_PRIOR      = SDLK_PRIOR,
	SOL_RETURN2    = SDLK_RETURN2,
	SOL_SEPARATOR  = SDLK_SEPARATOR,
	SOL_OUT        = SDLK_OUT,
	SOL_OPER       = SDLK_OPER,
	SOL_CLEARAGAIN = SDLK_CLEARAGAIN,
	SOL_CRSEL      = SDLK_CRSEL,
	SOL_EXSEL      = SDLK_EXSEL,

	SOL_KP_00              = SDLK_KP_00,
	SOL_KP_000             = SDLK_KP_000,
	SOL_THOUSANDSSEPARATOR = SDLK_THOUSANDSSEPARATOR,
	SOL_DECIMALSEPARATOR   = SDLK_DECIMALSEPARATOR,
	SOL_CURRENCYUNIT       = SDLK_CURRENCYUNIT,
	SOL_CURRENCYSUBUNIT    = SDLK_CURRENCYSUBUNIT,
	SOL_KP_LEFTPAREN       = SDLK_KP_LEFTPAREN,
	SOL_KP_RIGHTPAREN      = SDLK_KP_RIGHTPAREN,
	SOL_KP_LEFTBRACE       = SDLK_KP_LEFTBRACE,
	SOL_KP_RIGHTBRACE      = SDLK_KP_RIGHTBRACE,
	SOL_KP_TAB             = SDLK_KP_TAB,
	SOL_KP_BACKSPACE       = SDLK_KP_BACKSPACE,
	SOL_KP_A               = SDLK_KP_A,
	SOL_KP_B               = SDLK_KP_B,
	SOL_KP_C               = SDLK_KP_C,
	SOL_KP_D               = SDLK_KP_D,
	SOL_KP_E               = SDLK_KP_E,
	SOL_KP_F               = SDLK_KP_F,
	SOL_KP_XOR             = SDLK_KP_XOR,
	SOL_KP_POWER           = SDLK_KP_POWER,
	SOL_KP_PERCENT         = SDLK_KP_PERCENT,
	SOL_KP_LESS            = SDLK_KP_LESS,
	SOL_KP_GREATER         = SDLK_KP_GREATER,
	SOL_KP_AMPERSAND       = SDLK_KP_AMPERSAND,
	SOL_KP_DBLAMPERSAND    = SDLK_KP_DBLAMPERSAND,
	SOL_KP_VERTICALBAR     = SDLK_KP_VERTICALBAR,
	SOL_KP_DBLVERTICALBAR  = SDLK_KP_DBLVERTICALBAR,
	SOL_KP_COLON           = SDLK_KP_COLON,
	SOL_KP_HASH            = SDLK_KP_HASH,
	SOL_KP_SPACE           = SDLK_KP_SPACE,
	SOL_KP_AT              = SDLK_KP_AT,
	SOL_KP_EXCLAM          = SDLK_KP_EXCLAM,
	SOL_KP_MEMSTORE        = SDLK_KP_MEMSTORE,
	SOL_KP_MEMRECALL       = SDLK_KP_MEMRECALL,
	SOL_KP_MEMCLEAR        = SDLK_KP_MEMCLEAR,
	SOL_KP_MEMADD          = SDLK_KP_MEMADD,
	SOL_KP_MEMSUBTRACT     = SDLK_KP_MEMSUBTRACT,
	SOL_KP_MEMMULTIPLY     = SDLK_KP_MEMMULTIPLY,
	SOL_KP_MEMDIVIDE       = SDLK_KP_MEMDIVIDE,
	SOL_KP_PLUSMINUS       = SDLK_KP_PLUSMINUS,
	SOL_KP_CLEAR           = SDLK_KP_CLEAR,
	SOL_KP_CLEARENTRY      = SDLK_KP_CLEARENTRY,
	SOL_KP_BINARY          = SDLK_KP_BINARY,
	SOL_KP_OCTAL           = SDLK_KP_OCTAL,
	SOL_KP_DECIMAL         = SDLK_KP_DECIMAL,
	SOL_KP_HEXADECIMAL     = SDLK_KP_HEXADECIMAL,

	SOL_LCTRL  = SDLK_LCTRL,
	SOL_LSHIFT = SDLK_LSHIFT,
	SOL_LALT   = SDLK_LALT,
	SOL_LGUI   = SDLK_LGUI,
	SOL_RCTRL  = SDLK_RCTRL,
	SOL_RSHIFT = SDLK_RSHIFT,
	SOL_RALT   = SDLK_RALT,
	SOL_RGUI   = SDLK_RGUI,

	SOL_MODE = SDLK_MODE,

	SOL_AUDIONEXT    = SDLK_AUDIONEXT,
	SOL_AUDIOPREV    = SDLK_AUDIOPREV,
	SOL_AUDIOSTOP    = SDLK_AUDIOSTOP,
	SOL_AUDIOPLAY    = SDLK_AUDIOPLAY,
	SOL_AUDIOMUTE    = SDLK_AUDIOMUTE,
	SOL_MEDIASELECT  = SDLK_MEDIASELECT,
	SOL_WWW          = SDLK_WWW,
	SOL_MAIL         = SDLK_MAIL,
	SOL_CALCULATOR   = SDLK_CALCULATOR,
	SOL_COMPUTER     = SDLK_COMPUTER,
	SOL_AC_SEARCH    = SDLK_AC_SEARCH,
	SOL_AC_HOME      = SDLK_AC_HOME,
	SOL_AC_BACK      = SDLK_AC_BACK,
	SOL_AC_FORWARD   = SDLK_AC_FORWARD,
	SOL_AC_STOP      = SDLK_AC_STOP,
	SOL_AC_REFRESH   = SDLK_AC_REFRESH,
	SOL_AC_BOOKMARKS = SDLK_AC_BOOKMARKS,

	SOL_BRIGHTNESSDOWN = SDLK_BRIGHTNESSDOWN,
	SOL_BRIGHTNESSUP   = SDLK_BRIGHTNESSUP,
	SOL_DISPLAYSWITCH  = SDLK_DISPLAYSWITCH,
	SOL_KBDILLUMTOGGLE = SDLK_KBDILLUMTOGGLE,
	SOL_KBDILLUMDOWN   = SDLK_KBDILLUMDOWN,
	SOL_KBDILLUMUP     = SDLK_KBDILLUMUP,
	SOL_EJECT          = SDLK_EJECT,
	SOL_SLEEP          = SDLK_SLEEP,
	SOL_APP1           = SDLK_APP1,
	SOL_APP2           = SDLK_APP2,

	SOL_AUDIOREWIND      = SDLK_AUDIOREWIND,
	SOL_AUDIOFASTFORWARD = SDLK_AUDIOFASTFORWARD,

	SOL_SOFTLEFT  = SDLK_SOFTLEFT,
	SOL_SOFTRIGHT = SDLK_SOFTRIGHT,
	SOL_CALL      = SDLK_CALL,
	SOL_ENDCALL   = SDLK_ENDCALL
};

enum KeyModifier
{
	SOL_MOD_NONE   = KMOD_NONE,
	SOL_MOD_LSHIFT = KMOD_LSHIFT,
	SOL_MOD_RSHIFT = KMOD_RSHIFT,
	SOL_MOD_LCTRL  = KMOD_LCTRL,
	SOL_MOD_RCTRL  = KMOD_RCTRL,
	SOL_MOD_LALT   = KMOD_LALT,
	SOL_MOD_RALT   = KMOD_RALT,
	SOL_MOD_LGUI   = KMOD_LGUI,
	SOL_MOD_RGUI   = KMOD_RGUI,
	SOL_MOD_NUM    = KMOD_NUM,
	SOL_MOD_CAPS   = KMOD_CAPS,
	SOL_MOD_MODE   = KMOD_MODE,
	SOL_MOD_SCROLL = KMOD_SCROLL,

	SOL_MOD_CTRL  = KMOD_CTRL,
	SOL_MOD_SHIFT = KMOD_SHIFT,
	SOL_MOD_ALT   = KMOD_ALT,
	SOL_MOD_GUI   = KMOD_GUI,

	SOL_MOD_RESERVED = KMOD_RESERVED
};

#define SOL_BUTTON(X) (SDL_BUTTON(X))

enum MouseButtonCode
{
	SOL_MB_LEFT   = SDL_BUTTON_LEFT,
	SOL_MB_MIDDLE = SDL_BUTTON_MIDDLE,
	SOL_MB_RIGHT  = SDL_BUTTON_RIGHT,
	SOL_MB_X1     = SDL_BUTTON_X1,
	SOL_MB_X2     = SDL_BUTTON_X2,
	SOL_MB_LMASK  = SDL_BUTTON_LMASK,
	SOL_MB_MMASK  = SDL_BUTTON_MMASK,
	SOL_MB_RMASK  = SDL_BUTTON_RMASK,
	SOL_MB_X1MASK = SDL_BUTTON_X1MASK,
	SOL_MB_X2MASK = SDL_BUTTON_X2MASK
};

inline std::ostream &operator<<(std::ostream &os, const MouseButtonCode &mbc)
{
	switch (mbc)
	{
	case MouseButtonCode::SOL_MB_LEFT:
		return os << "left button";
	case MouseButtonCode::SOL_MB_MIDDLE:
		return os << "middle button";
	case MouseButtonCode::SOL_MB_RIGHT:
		return os << "right button";
	case MouseButtonCode::SOL_MB_X1:
		return os << "x1 button";
	case MouseButtonCode::SOL_MB_X2:
		return os << "x2 button";
	default:
		return os << "unkown button";
	};
}

} // namespace sol