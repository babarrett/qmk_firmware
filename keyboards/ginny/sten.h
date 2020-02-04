/* 	2019, g Heavy Industries
	 	Blessed mother of Christ, please keep this readable
 		and protect us from segfaults. For thine is the clock,
 		the slave and the master. Until we return from main.

 		Amen.

		This is a stripped down version of the Georgi engine meant for use with
 		Ginni. As such serial-Steno features are disabled, chords are 16bits and
 		crap is removed where possible
 
 		Do not use this on anything other then Ginny if you want to be sane. Modify
		the Georgi engine instead!
*/

#include QMK_KEYBOARD_H
#include "keymap_steno.h"
#include "keymap.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "wait.h"

// Maximum values for combos
#define COMBO_MAX 4
#define COMBO_END 0x00

// In memory chord datatypes
struct funcEntry {
	uint16_t	chord;
	void 			(*act)(void);
} funcEntry_t;

struct stringEntry {
	uint16_t	chord;
	char*			str;
} stringEntry_t;

struct comboEntry {
	uint16_t	chord;
	uint8_t		keys[COMBO_MAX];
} comboEntry_t;

struct keyEntry {
	uint16_t	chord;
	uint8_t		key;
} keyEntry_t;


// Chord Temps
extern uint16_t cChord;			
extern uint16_t test;			

// Function defs
void 			processChord(void);
uint16_t	processQwerty(bool lookup);
uint16_t 	processFakeSteno(bool lookup);
void 			saveState(uint16_t cChord);
void 			restoreState(void);

// Macros for use in keymap.c
void 			SEND(uint8_t kc);
void 			REPEAT(void);
void 			SET_STICKY(uint16_t);
void 			SWITCH_LAYER(int);
void 			CLICK_MOUSE(uint8_t);

#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define NAME(arg) EVALUATOR(fn, arg)
#define REBOOT() wdt_enable(WDTO_250MS); for (;;) ;

// Keymap helpers
// New Approach, multiple structures
#define P_KEYMAP(chord, keycode)	 			{chord, keycode},
#define K_KEYMAP(chord, ...)						{chord, {__VA_ARGS__},
#define S_KEYMAP(chord, string) 				{chord, string},
#define X_KEYMAP(chord, name, func)			{chord, &name},
#define X_ACTION(chord, name, func)			void name(void) {func}
#define TEST_COLLISION(chord,...)				case chord: break;
#define BLANK(...)

// Shift to internal representation
// i.e) S(teno)R(ight)F
#define STN(n) (1u<<n)
enum ORDER { 
		SLSU= 0, SLSD, SLT, SLK, SLP, SLW, SLH, SLR, SST1, SST2,
		SRES1, SRES2, SST3, SST4
};

// Ginny Define
#define GLP STN(SLSU)
#define GLR STN(SLSD)
#define GLM STN(SLT)
#define GLI STN(SLK)
#define GLT STN(SLP)

#define GRT STN(SLW)
#define GRI STN(SLH)
#define GRM STN(SLR)
#define GRR STN(SST1)
#define GRP STN(SST2)
#define RES1 STN(SRES1) // Use reserved for sticky state
#define RES2 STN(SRES2)
#define RES3 STN(SRES3)

// Original 32bit version
#define LSU STN(SLSU)
#define LSD STN(SLSD)
#define LFT STN(SLT)  	
#define LK  STN(SLK)
#define LP  STN(SLP)
#define LW  STN(SLW)
#define LH  STN(SLH)
#define LR  STN(SLR)
#define ST1 STN(SST1)
#define ST2 STN(SST2)
#define ST3 STN(SST3)
#define ST4 STN(SST4)

// Asetniop aliases, to Ginny Fingers
#define AA		GLP
#define AS		GLR
#define AE		GLM
#define AT		GLI
#define AN		GRI
#define AI		GRM
#define AO		GRR
#define AP		GRP

#define AL		GLT							// Left/Right thumbs
#define AR		GRT

#define NUM		RES1						// Sticky Layer 1
#define USR   RES2						// Sticky Layer 2
#define CMD		RES2 | RES1			// Sticky Layer 3
