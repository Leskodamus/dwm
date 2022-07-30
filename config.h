/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static int smartgaps          		= 1;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
static const char *fonts[]          = { "IBMPlexMono:size=12", "SymbolsNerdFont:size=14" };
static const char dmenufont[]       = "IBMPlexMono:size=12";
static const char dmenubg[]         = "#000000";
static const char dmenuprompt[]     = "Run:";
static const char col_black[]	    = "#000000";
static const char col_gray1[]       = "#444444";
static const char col_gray2[]       = "#222222";
static const char col_white[]	    = "#ffffff";
static const char col_yellow[]	    = "#ffff00";
static const char col_blue[]	    = "#1303fc";
static const char col_green[]	    = "#1eff00";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_black, col_gray2 },
	[SchemeSel]  = { col_white, col_gray1, col_gray1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,       NULL,       "Event Listener",       0,            0,           -1 }
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	/* { "[\\]",     dwindle }, */
	/* { "H[]",      deck }, */
	/* { "TTT",      bstack }, */
	/* { "===",      bstackhoriz }, */
	/* { "HHH",      grid }, */
	/* { "###",      nrowgrid }, */
	/* { "---",      horizgrid }, */
	/* { ":::",      gaplessgrid }, */
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#include <X11/XF86keysym.h>

/* #define MODKEY Mod1Mask */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", dmenubg, "-p", dmenuprompt, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)

    /* tag keys  */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

    /* xf keys (for laptop)  */
	{ 0,	XF86XK_AudioRaiseVolume,   spawn,  SHCMD("/usr/bin/pamixer -i 5 -u; pkill -RTMIN+1 dwmblocks") },
	{ 0,	XF86XK_AudioLowerVolume,   spawn,  SHCMD("/usr/bin/pamixer -d 5 -u; pkill -RTMIN+1 dwmblocks") },
	{ 0,	XF86XK_AudioMute,          spawn,  SHCMD("/usr/bin/pamixer -t; pkill -RTMIN+1 dwmblocks") },
	{ 0,	XF86XK_MonBrightnessUp,    spawn,  SHCMD("/usr/bin/xbacklight -inc 2; pkill -RTMIN+2 dwmblocks") },
	{ 0,	XF86XK_MonBrightnessDown,  spawn,  SHCMD("/usr/bin/xbacklight -dec 2; pkill -RTMIN+2 dwmblocks") },

    /* shortcuts for applications/scripts  */
	{ MODKEY|ShiftMask, 		XK_e, 		spawn, 		SHCMD("~/.local/bin/pdmenu") },
	{ MODKEY|ControlMask,		XK_l,	   	spawn,	   	SHCMD("slock") },
	{ MODKEY,					XK_e,	   	spawn,	   	SHCMD("st -e nnn -e") },
	{ 0,	                    XK_Print,  	spawn,	   	SHCMD("flameshot gui") },
	{ MODKEY,					XK_t,	   	spawn,	   	SHCMD("~/.local/bin/ntodo") },

    /* dunst shortcuts */
    { MODKEY|ControlMask,           XK_period,   spawn,      SHCMD("dunstctl history-pop") },
    { MODKEY|ControlMask,           XK_space,    spawn,      SHCMD("dunstctl close") },
    { MODKEY|Mod1Mask|ControlMask,  XK_space,    spawn,      SHCMD("dunstctl close-all") },

    /* more dwm shortcuts */
	{ MODKEY,						XK_numbersign,  togglescratch,	{.v = scratchpadcmd } },
	{ MODKEY,						XK_d,      	spawn,         {.v = dmenucmd } },
	{ MODKEY,						XK_Return, 	spawn,         {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,						XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,						XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,						XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,						XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +10 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -10 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	/* { MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } }, */
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_i,      shiftview,      { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      shiftview,      { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,				XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,				XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_w,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,				XK_z,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY, 						XK_r, 	   resetlayout,    {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,             			XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ Mod1Mask|ControlMask,         XK_BackSpace, quit,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	/* { ClkTagBar,            MODKEY,         Button1,        tag,            {0} }, */
	/* { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} }, */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,	Button1,    resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

