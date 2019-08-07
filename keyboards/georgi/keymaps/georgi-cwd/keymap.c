/*
 * Good on you for modifying your layout, this is the most nonQMK layout you will come across
 * There are three modes, Steno (the default), Colemak (Toggleable) and a Momentary symbol layer
 *
 * Don't modify the steno layer directly, instead add chords using the keycodes and macros
 * from sten.h to the layout you want to modify.
 *
 * Observe the comment above processQWERTY!
 *
 * http://docs.gboards.ca
 */

#include QMK_KEYBOARD_H
#include "sten.h"
#include "keymap_steno.h"
#define IGNORE_MOD_TAP_INTERRUPT

// Steno Layers
#define FUNCT	( LSD | LK | LP | LH )
#define MEDIA	( LSD | LK | LW | LR )
#define MOVE	( LA | LO )
#define LNUM	( LNO )
#define RNUM	( RNO )
#define SYM		( PWR )

// Keys and chords that, once they appear, are added to every subsequent partial chord
// until the whole thing is sent.
uint32_t stenoLayers[] = {LNUM, RNUM, SYM, MOVE, MEDIA, FUNCT};

// QMK Layers
#define STENO_LAYER   0
#define GAMING		  1
#define GAMING_2	  2

/* Keyboard Layout
 * ,---------------------------------.    ,------------------------------.
 * | FN  | LSU | LFT | LP | LH | ST1 |    | ST3 | RF | RP | RL | RT | RD |
 * |-----+-----+-----+----+----|-----|    |-----|----+----+----+----+----|
 * | PWR | LSD | LK  | LW | LR | ST2 |    | ST4 | RR | RB | RG | RS | RZ |
 * `---------------------------------'    `------------------------------'
 *                   ,---------------,    .---------------.
 *                   | LNO | LA | LO |    | RE | RU | RNO |
 *                   `---------------'    `---------------'
 */

// Note: You can only use basic keycodes here!
//
// P() is just a wrapper to make your life easier.
// PC() applies the mapping to all of the StenoLayers. For overloading, define these last.
//
// FN is unavailable. That is reserved for system use.
// Chords containing PWR are always available, even in steno mode.
//
// http://docs.gboards.ca
uint32_t processQwerty(bool lookup) {
	// Special keys
	P( RT  | RS  | RD  | RZ | LNO,		SEND_STRING(VERSION); SEND_STRING(__DATE__));
	P( LFT | LK  | LP  | LW,			REPEAT());

/* Function layer
 * ,-----------------------------------,    ,-----------------------------------,
 * |     |     |     | NCTFUNCTF |     |    |     | F1  | F2  | F3  | F4  |     |
 * |     +     +     +     +     +     |    |     + F5  + F6  + F7  + F8  +     |
 * |     | FUNCTFUNC |     |     |     |    |     | F9  | F10 | F11 | F12 |     |
 * `-----+-----+-----+-----+-----+-----'    `-----+-----+-----+-----+-----+-----'
*/
	P( FUNCT | RF,			SEND(KC_F1));
	P( FUNCT | RP,			SEND(KC_F2));
	P( FUNCT | RL,			SEND(KC_F3));
	P( FUNCT | RT,			SEND(KC_F4));

	P( FUNCT | RF | RR,		SEND(KC_F5));
	P( FUNCT | RP | RB,		SEND(KC_F6));
	P( FUNCT | RL | RG,		SEND(KC_F7));
	P( FUNCT | RT | RS,		SEND(KC_F8));

	P( FUNCT | RR,			SEND(KC_F9));
	P( FUNCT | RG,			SEND(KC_F10));
	P( FUNCT | RB,			SEND(KC_F11));
	P( FUNCT | RS,			SEND(KC_F12));


/* Movement layer
 * ,-----------------------------------,    ,-----------------------------------,
 * |     | PgU |     |  ↑  |     | Hm  |    |     |     |     |     |     |     |
 * |     +     +     +     +     +     |    |     +     +     +     +     +     |
 * | SFT | PgD |  <- |  ↓  |  -> | End |    |     |     |     |     |     | SFT |
 * `-----+-----+-----+-----+-----+-----'    `-----+-----+-----+-----+-----+-----'
 *
 *                               MOVE
 *                     ,-----+----+----,    .----+----+-----.
 *                     |     |    |    |    |    |    |     |
 *                     `-----+----+----'    `----+----+-----'
*/
	P( MOVE | LK,			SEND(KC_LEFT));
	P( MOVE | LW,			SEND(KC_DOWN));
	P( MOVE | LP,			SEND(KC_UP));
	P( MOVE | LR,			SEND(KC_RIGHT));

	P( MOVE | ST1,			SEND(KC_HOME));
	P( MOVE | LSD,			SEND(KC_PGDN))
	P( MOVE | LSU,			SEND(KC_PGUP));
	P( MOVE | ST2,			SEND(KC_END));

    PC( MOVE | PWR,			SEND(KC_RSFT));
	PC( MOVE | RZ,			SEND(KC_RSFT));

/* Media Layer
 * ,-----------------------------------,    ,-----------------------------------,
 * |     |     |     |     |     |     |    |     |Prev |Play | PLY |Next | VolU|
 * |     +     +     +     +     +     |    |     +     +     +     +     +     |
 * |     | MEDIAMEDIAMEDIAMEDIAM |     |    |     |     |     |     |Mute | VolD|
 * `-----+-----+-----+-----+-----+-----'    `-----+-----+-----+-----+-----+-----'
*/
	P( MEDIA | RF,			SEND(KC_MPRV));
	P( MEDIA | RP,			SEND(KC_MPLY));
	P( MEDIA | RL,			SEND(KC_MPLY));
	P( MEDIA | RT,			SEND(KC_MNXT));
	P( MEDIA | RD,			SEND(KC_VOLU));

	P( MEDIA | RS,			SEND(KC_MUTE));
	P( MEDIA | RZ,			SEND(KC_VOLD));


/* Numbers
 * ,-----------------------------------,    ,-----------------------------------,
 * |     |  1  |  2  |  3  |  4  |  5  |    |  6  |  7  |  8  |  9  |  0  |     |
 * |     +     +     +     +     +     |    |     +  4  +  5  +  6  +     +     |
 * |     |     |     |     |     |     |    |     |  1  |  2  |  3  |     |     |
 * `-----+-----+-----+-----+-----+-----'    `-----+-----+-----+-----+-----+-----'
 *                     ,---------------,    .---------------.
 *                     | num |    |    |    |    |    | num |
 *                     `---------------'    `---------------'
*/
	// Left hand
    P( LNUM | LSU,		SEND(KC_1));
    P( LNUM | LFT,		SEND(KC_2));
    P( LNUM | LP,			SEND(KC_3));
    P( LNUM | LH,			SEND(KC_4));
    P( LNUM | ST1,		SEND(KC_5));

    P( LNUM | ST3,          SEND(KC_6));
    P( LNUM | RF,			SEND(KC_7));
    P( LNUM | RP,			SEND(KC_8));
    P( LNUM | RL,			SEND(KC_9));
    P( LNUM | RT,			SEND(KC_0));

    P( LNUM | RF  | RR,		SEND(KC_4));
    P( LNUM | RP  | RB,		SEND(KC_5));
    P( LNUM | RG  | RL,		SEND(KC_6));

    P( LNUM | RR,			SEND(KC_1));
    P( LNUM | RB,			SEND(KC_2));
    P( LNUM | RG,			SEND(KC_3));

	// Right hand

    P( RNUM | LSU,		SEND(KC_1));
    P( RNUM | LFT,		SEND(KC_2));
    P( RNUM | LP,			SEND(KC_3));
    P( RNUM | LH,			SEND(KC_4));
    P( RNUM | ST1,		SEND(KC_5));

    P( RNUM | ST3,	  SEND(KC_6));
    P( RNUM | RF,			SEND(KC_7));
    P( RNUM | RP,			SEND(KC_8));
    P( RNUM | RL,			SEND(KC_9));
    P( RNUM | RT,			SEND(KC_0));

    P( RNUM | RF  | RR,		SEND(KC_4));
    P( RNUM | RP  | RB,		SEND(KC_5));
    P( RNUM | RG  | RL,		SEND(KC_6));

    P( RNUM | RR,			SEND(KC_1));
    P( RNUM | RB,			SEND(KC_2));
    P( RNUM | RG,			SEND(KC_3));


/* Symbols
 * ,-----------------------------------,    ,-----------------------------------,
 * |     |  `  |  [  |  {  |  (  |  <  |    |  >  |  )  |  }  |  ]  |  ?  |     |
 * |     +  ~  +  -  +  '  +  :  +  _  |    |  \  +  =  +  "  +  +  +  ?  +     |
 * | SYM |  !  |  @  |  #  |  $  |  %  |    |  |  |  ^  |  &  |  *  |  ?  | SFT |
 * `-----+-----+-----+-----+-----+-----'    `-----+-----+-----+-----+-----+-----'
 *                     ,---------------,    .---------------.
 *                     |     |    |    |    |    |    |     |
 *                     `---------------'    `---------------'
*/
	// Left hand
    P( SYM | LSU,			SEND(KC_GRV));
    P( SYM | LFT,			SEND(KC_LBRC));
    P( SYM | LP,			SEND_STRING("{"));
    P( SYM | LH,			SEND_STRING("("));
    P( SYM | ST1,			SEND_STRING("<"));

    P( SYM | LSU | LSD,		SEND_STRING("~"));
    P( SYM | LFT | LK,		SEND(KC_MINS));
    P( SYM | LP  | LW,		SEND(KC_QUOTE));
    P( SYM | LH  | LR,		SEND_STRING(":"));
    P( SYM | ST1 | ST2,		SEND_STRING("_"));

    P( SYM | LSD,			SEND_STRING("!"));
    P( SYM | LK,			SEND_STRING("@"));
    P( SYM | LW,			SEND_STRING("#"));
    P( SYM | LR,			SEND_STRING("$"));
    P( SYM | ST2,			SEND_STRING("%"));

	// Right hand
    P( SYM | ST3,			SEND_STRING(">"));
    P( SYM | RF,			SEND_STRING(")"));
    P( SYM | RP,			SEND_STRING("}"));
    P( SYM | RL,			SEND_STRING("]"));
    P( SYM | RT,			SEND_STRING("?"));

    P( SYM | ST3 | ST4,		SEND(KC_BSLASH));
    P( SYM | RF  | RR,		SEND(KC_EQUAL));
    P( SYM | RP  | RB,		SEND_STRING("\""));
    P( SYM | RG  | RL,		SEND_STRING("+"));
    P( SYM | RT  | RS,		SEND_STRING("?"));

    P( SYM | ST4,			SEND_STRING("|"));
    P( SYM | RR,			SEND_STRING("^"));
    P( SYM | RB,			SEND_STRING("&"));
    P( SYM | RG,			SEND_STRING("*"));
    P( SYM | RS,			SEND_STRING("?"));

    P( SYM | RZ,			SEND(KC_RSFT));


/* Letters
 *            TAB
 * ,-----------------------------------,    ,-----------------------------------,
 * |     |  Q  |  W  |  F  |  P  |  G  |    |  J  |  L  |  U  |  Y  |  ;  | ctl |
 * +-----+- A -+- R -+- S -+- T -+- D -|    |- H -+- N -+- E -+- I -+- O -+-----|
 * | esc |  Z  |  X  |  C  |  V  |  B  |    |  K  |  M  |  ,  |  .  |  /  | del |
 * `-----+-----+-----+-----+-----+-----'    `-----+-----+-----+-----+-----+-----'
 *                  ALT   GUI   CTL              CTL   GUI   ALT
 *                         CTL+GUI                CTL+GUI
 *                     ,-----+----+----,    .----+----+-----.
 *                     | BSP |SPC |SFT |    |    | SPC| BSP |
 *                     `-----+----+----'    `----+----+-----'
 *                               MVE
 */

	// Left hand
	P( LSU,					SEND(KC_Q));
	P( LFT,					SEND(KC_W));
	P( LP,					SEND(KC_F));
	P( LH,					SEND(KC_P));
	P( ST1,					SEND(KC_G));

	P( LSU | LSD,			SEND(KC_A));
	P( LFT | LK,			SEND(KC_R));
	P( LP  | LW,			SEND(KC_S));
	P( LH  | LR,			SEND(KC_T));
	P( ST1 | ST2,			SEND(KC_D));

	P( LSD,					SEND(KC_Z));
	P( LK,					SEND(KC_X));
	P( LW,					SEND(KC_C));
	P( LR,					SEND(KC_V));
	P( ST2,					SEND(KC_B));

  PC( LSU | LFT,		        SEND(KC_TAB));

  PC( ST2 | LR,		        SEND(KC_LCTL));
  PC( LR | LW, 		        SEND(KC_LGUI));
  PC( LW | LK, 		        SEND(KC_LALT));
  PC( ST2 | LR | LW,        SEND(KC_LCTL); SEND(KC_LGUI));

    //Left Thumbs

  //[][X][] [][][] Space
  PC( LA,                    SEND(KC_SPC));

  //[X][][] [][][] BackSpace
  PC( LNO,                    SEND(KC_BSPC));

  //[][][X] [][][] Shift
  PC( LO,                    SEND(KC_LSFT));

	// Right hand
	P( ST3,					SEND(KC_J));
	P( RF,					SEND(KC_L));
	P( RP,					SEND(KC_U));
	P( RL,					SEND(KC_Y));
	P( RT,					SEND(KC_SCLN));

	P( ST3 | ST4,			SEND(KC_H));
	P( RF  | RR,			SEND(KC_N));
	P( RP  | RB,			SEND(KC_E));
	P( RG  | RL,			SEND(KC_I));
	P( RT  | RS,			SEND(KC_O));

	P( ST4,					SEND(KC_K));
	P( RR,					SEND(KC_M));
	P( RB,					SEND(KC_COMM));
	P( RG,					SEND(KC_DOT));
	P( RS,					SEND(KC_SLSH));

	PC( RD,					SEND(KC_RCTL));
	P( RZ,					SEND(KC_DEL));

  PC( ST4 | RR,		        SEND(KC_RCTL));
  PC( RR | RB, 		        SEND(KC_RGUI));
  PC( RB | RG, 		        SEND(KC_RALT));
  PC( ST4 | RR | RB,		SEND(KC_RCTL); SEND(KC_RGUI));

    // Right Thumbs

  //[][][] [][X][] Space
  PC( RU,                    SEND(KC_SPC));

  //[][][] [][][X] BackSpace
  PC( RNO,                    SEND(KC_BSPC));

  //[][][] [][][X] Enter
  P( RE,                    SEND(KC_ENT));

// Thumb Chords and modifiers
//

	// overrides

	P( PWR,			  	SEND(KC_ESC));

  //[][X][] [][X][] Escape
  P( LA | RU,			SEND(KC_ESC));

  //[][][] [X][X][] Tab
  P( RE | RU,			SEND(KC_TAB));

  //[][][X] [X][][] Control
  PC( LO | RE,			SEND(KC_LCTL));

  //[][X][X] [X][X][] Control+Shift
  PC( LO | RE | LA | RU,     SEND(KC_LCTL); SEND(KC_LSFT));

  //[X][][] [][][X] Gui
  PC( LNO | RNO,     SEND(KC_LGUI));

  //[X][][X] [X][][X] Control+Gui
  PC( LNO | RNO | LO | RE,     SEND(KC_LCTL); SEND(KC_LGUI));

	return 0;
}

// "Layers"
// Steno layer should be first in your map.
// When PWR | FN | ST3 | ST4 is pressed, the layer is increased to the next map. You must return to STENO_LAYER at the end.
// If you need more space for chords, remove the two gaming layers.
// Note: If using NO_ACTION_TAPPING, LT will not work!

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// Main layer, everything goes through here
	[STENO_LAYER] = LAYOUT_georgi(
			STN_FN,  STN_S1,  STN_TL,  STN_PL,	STN_HL,  STN_ST1,		STN_ST3, STN_FR,  STN_PR,  STN_LR,	STN_TR,  STN_DR,
			STN_PWR, STN_S2,  STN_KL,  STN_WL,	STN_RL,  STN_ST2,		STN_ST4, STN_RR,  STN_BR,  STN_GR,	STN_SR,  STN_ZR,
								   STN_N1,		STN_A,	 STN_O,			STN_E,	 STN_U,   STN_N7
	),
	// Gaming layer with Numpad, Very limited
	[GAMING] = LAYOUT_georgi(
		KC_LCTL, KC_Q,	  KC_W,    KC_F,	KC_P,	 KC_G,						 KC_J,	  KC_L,    KC_U,	KC_Y,	 KC_SCLN, KC_ENT,
		KC_LSFT, KC_A,	  KC_R,    KC_S,	KC_T,	 KC_D,						 KC_H,	  KC_N,    KC_E,	KC_I,	 KC_O,   KC_DQUO,
								   KC_LALT, KC_SPC,  LT(GAMING_2, KC_ENT),		 KC_DEL,  KC_ASTR, TO(STENO_LAYER)
	),

	[GAMING_2] = LAYOUT_georgi(
		KC_LCTL, KC_1,	  KC_2,    KC_3,	KC_4,	 KC_5,			KC_6,	 KC_7,	  KC_8,    KC_9,	KC_0,	 KC_MINS,
		KC_LSFT, KC_Z,	  KC_X,    KC_C,	KC_V,	 KC_B,			KC_N,	 KC_M,	  KC_LT,   KC_GT,	KC_QUES, KC_RSFT,
								   KC_LALT, KC_SPC,  KC_ENT,		KC_DEL,  RESET, TO(STENO_LAYER)
	)
};

// Don't fuck with this, thanks.
size_t keymapsCount  = sizeof(keymaps)/sizeof(keymaps[0]);
size_t stenoLayerCount = sizeof(stenoLayers)/sizeof(stenoLayers[0]);
