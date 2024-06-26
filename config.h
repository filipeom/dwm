/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int gappih = 0;   /* horiz inner gap between windows */
static const unsigned int gappiv = 0;   /* vert inner gap between windows */
static const unsigned int gappoh =
    0; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    0; /* vert outer gap between windows and screen edge */
static const int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int showsystray = 1; /* 0 means no systray */
static const int showbar = 1;     /* 0 means no bar */
static const int topbar = 1;      /* 0 means bottom bar */
static const int user_bh = 0; /* 0 means that dwm will calculate bar height, >=
                                  1 means dwm will user_bh as bar height */
static const char *fonts[] = {"CaskaydiaCove NF:size=13"};
static const char dmenufont[] = "CaskaydiaCove NF:size=13";
static const char bg[] = "#f6f2ee";
static const char fg[] = "#3d2b5a";
static const char inactive[] = "#888888";
static const char bg2[] = "#e7d2be";
static const char fg2[] = "#3d2b5a";
static const char active[] = "#ff0000";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {fg, bg, inactive},
    [SchemeSel] = {fg2, bg2, active},
    [SchemeStatus] = {fg, bg, NULL}, // Statusbar right {text,background,not
                                     // used but cannot be empty}
    [SchemeTagsSel] = {fg, bg,
                       NULL}, // Tagbar left selected {text,background,not
                              // used but cannot be empty}
    [SchemeTagsNorm] = {inactive, bg,
                        NULL}, // Tagbar left unselected {text,background,not
                               // used but cannot be empty}
    [SchemeInfoSel] = {fg, bg,
                       NULL}, // infobar middle  selected {text,background,not
                              // used but cannot be empty}
    [SchemeInfoNorm] = {inactive, bg,
                        NULL}, // infobar middle  unselected
                               // {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = {"\ue6b1", "2", "3", "4", "5", "6", "7", "www", "mail"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"thunderbird", NULL, NULL, 1 << 8, 0, -1},
    {"discord", NULL, NULL, 1 << 3, 0, -1},
    {"Slack", NULL, NULL, 1 << 3, 0, -1},
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon, "-fn", dmenufont,
                                 "-nb",       bg,    "-nf",    fg,    "-sb",
                                 bg2,         "-sf", fg2,      NULL};
static const char *termcmd[] = {"kitty", NULL};
// static const char *musiccmd[] = { "kitty", "-e", "ncmpcpp", NULL };
static const char *wancmd[] = {"toggle-wifi", NULL};
static const char *micmute[] = {"amixer", "set", "Capture", "toggle", NULL};
static const char *volmute[] = {"change-vol", "--toggle-mute", NULL};
static const char *volraise[] = {"change-vol", "--change-volume", "+5", NULL};
static const char *vollower[] = {"change-vol", "--change-volume", "-5", NULL};
static const char *backinc[] = {"xbacklight", "-inc", "20", NULL};
static const char *backdec[] = {"xbacklight", "-dec", "20", NULL};

static Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = +1}},
    {MODKEY, XK_period, focusmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {0, XF86XK_WLAN, spawn, {.v = wancmd}},
    {0, XF86XK_AudioMicMute, spawn, {.v = micmute}},
    {0, XF86XK_AudioMute, spawn, {.v = volmute}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = volraise}},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = vollower}},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = backinc}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = backdec}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
