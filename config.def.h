/* See LICENSE file for copyright and license details. */
/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
//static const char *fonts[]          = { "mononoki Nerd Font:pixelsize=14:antialias=true:autohint=true" };
static const char *fonts[]          = { "Iosevka NFM:style=Regular:pixelsize=14:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=10";
//static const char col_bg1[] = "#2e3440"; //3b4252
//static const char col_bg[] =  "#4c566a";
//static const char border1[] =  "#3b4252";
//static const char col_fg[] =  "#eceff4";
//static const char col_fg1[] = "#81a1c1";
static const char col_bg1[] = "#000000";
static const char col_bg[] =  "#14171b";
static const char border1[] =  "#cdcdcd";
static const char col_fg[] =  "#cdcdcd";
static const char col_fg1[] = "#7d8fa3";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg,    col_bg1,   border1   },
	[SchemeSel]  = { col_fg1,   col_bg1,   col_fg1  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",           NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "St",             NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Steam",             NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Xephyr",         NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,             NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "shiftview.c"
#include <X11/XF86keysym.h>

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,              KEY,      tagnextmon,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      tagprevmon,     {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } },
/*	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
/*	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \		 */
/*	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \       */
/*	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },        */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/*static const char *dmenucmd[] = { "dmenu_run",
								  "-m", dmenumon,
				  "-fn", dmenufont,
				  "-nb", col_bg1,
				  "-nf", col_fg,
				  "-sb", col_bg,
				  "-sf", col_fg1,
				  NULL };*/
static const char *rofi_druncmd[] = { "rofi", "-show", "drun", NULL };
static const char *rofi_runcmd[]  = { "rofi", "-show", "run",  NULL };
static const char *termcmd[]      =  { "kitty", NULL };
static const char *editorcmd[]      =  { "emacsclient", "-c" , NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,        spawn,          {.v = rofi_druncmd } },
		{ MODKEY|ShiftMask,             XK_space,        spawn,          {.v = rofi_runcmd } },
	{ MODKEY,                       XK_Return,       spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_e,            spawn,          {.v = editorcmd } },
	{ MODKEY,                       XK_i,            togglebar,      {0} },
	STACKKEYS(MODKEY,                                focus)
	STACKKEYS(MODKEY|ShiftMask,                      push)
	{ MODKEY,                       XK_o,            incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,        incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,            setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,            setmfact,       {.f = +0.05} },
	  //{ MODKEY,                       XK_Return,       zoom,           {0} },     the hell is this?
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,              XK_a,            incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_a,            incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,            incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,            incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,            incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,            incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,            incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,            incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,            incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,            incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,            incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,            incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,            incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,            incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,            togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,            defaultgaps,    {0} },
		{ MODKEY,                       XK_Tab,          view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,            killclient,     {0} },
	{ MODKEY,                       XK_t,            setlayout,      {.v = &layouts[0]} },
	{ MODKEY,				XK_BackSpace,    cyclelayout,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_BackSpace,    cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_f,            togglefloating, {0} },
	{ MODKEY,                       XK_s,            togglesticky,   {0} },
	{ MODKEY,                       XK_f,            togglefullscr,  {0} },
	{ MODKEY,                       XK_0,            view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,            tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,        focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,       focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,        tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,       tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_g,            shiftview,      {.i = -1}},
	{ MODKEY,                       XK_semicolon,    shiftview,      {.i = 1}},
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,            quit,           {0} },
	{ MODKEY|ShiftMask,             XK_t,        spawn,         SHCMD("touchpad_toggle")},
	{0, XF86XK_TouchpadOn,    spawn, SHCMD("touchpad_toggle")},
	{0, XF86XK_TouchpadOff,    spawn, SHCMD("touchpad_toggle")},
	{0, XF86XK_TouchpadToggle,    spawn, SHCMD("touchpad_toggle")},
	{0, XF86XK_AudioRaiseVolume,    spawn, SHCMD("pamixer -i 5")},
	{0, XF86XK_AudioLowerVolume,    spawn, SHCMD("pamixer -d 5")},
	{0, XF86XK_MonBrightnessUp,     spawn, SHCMD("xbacklight -inc 5")},
	{0, XF86XK_MonBrightnessDown,   spawn, SHCMD("xbacklight -dec 5")},
	{0,		XK_Print,           spawn, SHCMD("SCR=~/pictures/screenshots/$(date +'%d-%m-%Y-%T').png;maim > $SCR && cat $SCR | xclip -selection clipboard -t image/png -i")},
	{ShiftMask, XK_Print,           spawn, SHCMD("SCR=~/pictures/screenshots/$(date +'%d-%m-%Y-%T').png;maim -s > $SCR && cat $SCR | xclip -selection clipboard -t image/png -i")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
