#include QMK_KEYBOARD_H
#include "sten.h"
#include "keymap_steno.h"

/* Note: Don't edit this file! 
 *
 * You can tweak what dictionaries/languages are loaded in dicts.def
 * Your personal keymap is over in user.def
 *
 * This is the most non-QMK powered device ever :)
 * Happy Hacking,
 *		-- Germ
 */

/* If for some reason you're still here, maybe due to horror, shock or
 * some other godforsaked reason. Meet X Macros.
 *
 * The we abuse the include system to generate data structures that are
 * used by the internal chording engine. The alternative to this is 
 * using a external generator (Like is done for the ASETNIOP base keymaps)
 * With this disgusting bodge, you can just edit your .defs and compile!
 */

// Clear all X Macros
#define PRES BLANK
#define KEYS BLANK
#define SUBS BLANK
#define EXEC BLANK

// Process single key pushes
#undef 	PRES
#define PRES P_KEYMAP			
struct keyEntry keyDict[] = {
	#include "dicts.def"
};
#undef 	PRES
#define PRES BLANK

// Process Combos
#undef 	KEYS
#define KEYS BLANK
struct comboEntry cmbDict[] = {
	#include "dicts.def"
};
#undef 	KEYS
#define KEYS BLANK

// Process String subs
#undef  SUBS
#define SUBS S_KEYMAP
struct stringEntry strDict[] = {
	#include "dicts.def"
};
#undef 	SUBS
#define SUBS BLANK

// Generate function stubs 
#undef 	EXEC
#define EXEC X_ACTION
	#include "dicts.def"
#undef 	EXEC
#define EXEC BLANK

// Process the function structure
#undef 	EXEC
#define EXEC X_KEYMAP
struct funcEntry funDict[] = {
	#include "dicts.def"
};
#undef EXEC

// Test for collisions! 
// Switch statement will explode on duplicate 
// chords. This will be optimized out
#undef 	PRES
#undef 	KEYS
#undef 	SUBS
#undef 	EXEC
#define PRES TEST_COLLISION
#define KEYS TEST_COLLISION
#define SUBS TEST_COLLISION
#define EXEC TEST_COLLISION
void testCollisions(void) {
	switch (5) {
		#include "dicts.def"
	}
}

// Test for unexpected input
// Should return blank lines for all valid input
#undef 	PRES
#undef 	KEYS
#undef 	SUBS
#undef 	EXEC
#define PRES BLANK
#define KEYS BLANK
#define SUBS BLANK
#define EXEC BLANK
#include "dicts.def"

// Get size data back into the engine
size_t funcsLen = sizeof(funDict)/sizeof(funDict[0]);
size_t stringLen = sizeof(strDict)/sizeof(strDict[0]);
size_t keyLen = sizeof(keyDict)/sizeof(keyDict[0]);
size_t comboLen = sizeof(cmbDict)/sizeof(cmbDict[0]);

// Hey a QMK keymap!
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_ginny(
		STN_S1, STN_S2, STN_TL, STN_KL, STN_PL, STN_WL, STN_HL, STN_RL, STN_ST1, STN_ST2
)};
