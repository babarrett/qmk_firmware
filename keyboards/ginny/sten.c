/*	This is a stripped down version of the Georgi engine meant for use with
 *	Ginni. As such serial-Steno features are disabled, chords are 16bits and
 *	crap is removed where possible
 *
 *	Do not use this on anything other then Ginny if you want to be sane
 */
#include "sten.h"

// Chord state
uint16_t cChord				= 0;		// Current Chord
int			 chordIndex		= 0;		// Keys in previousachord
int16_t  chordState[32];			// Full Chord history
#define  QWERBUF		24				// Size of chords to buffer for output

bool	 		repeatFlag		= false;	// Should we repeat?
uint16_t 	pChord				= 0;			// Previous Chord
uint16_t 	stickyBits 		= 0;			// Or'd with every incoming press
int		 		pChordIndex		= 0;			// Keys in previousachord
uint16_t 	pChordState[32];				// Previous chord sate 

// Key Dicts
extern struct keyEntry		keyDict[];
extern size_t keyLen;
extern struct comboEntry	cmbDict[];
extern size_t comboLen; 
extern struct funcEntry 	funDict[];
extern size_t funcsLen;
extern struct stringEntry	strDict[];
extern size_t stringLen; 

// Mode state
enum MODE { STENO = 0, QWERTY, COMMAND };
enum MODE pMode;
enum MODE cMode = QWERTY;

// Command State
#define MAX_CMD_BUF   20
uint8_t	 CMDLEN		= 0;
uint8_t	 CMDBUF[MAX_CMD_BUF];

// Key Repeat state
bool     inChord			= false;
bool	 	 repEngaged		= false;
uint16_t repTimer			= 0;
#define  REP_INIT_DELAY 750
#define  REP_DELAY		25

// Mousekeys state
bool		inMouse			= false;
int8_t	mousePress;

// All processing done at chordUp goes through here
bool send_steno_chord_user(steno_mode_t mode, uint8_t chord[6]) { 
	// Check for mousekeys, this is release
#ifdef MOUSEKEY_ENABLE
	if (inMouse) {
		inMouse = false;
		mousekey_off(mousePress);
		mousekey_send();
	}
#endif

	// handle command mode
	if (cChord == (GRM | GRI | GLI | GLM)) {
#ifndef NO_DEBUG
		uprintf("COMMAND Toggle\n");
#endif
		if (cMode != COMMAND) {   // Entering Command Mode
			CMDLEN = 0;
			pMode = cMode;
			cMode = COMMAND;
		} else {                  // Exiting Command Mode
			cMode = pMode;

			// Press all and release all
			for (int i = 0; i < CMDLEN; i++) {
				register_code(CMDBUF[i]);
			}
			clear_keyboard();
		}
	}

	// Process and reset state
	processChord();
	cChord = 0;
	inChord = false;
	chordIndex = 0;
	clear_keyboard();
	repEngaged  = false;
	for (int i = 0; i < 32; i++)
		chordState[i] = 0xFFFF;

	return false;
}

// Update Chord State 
bool process_steno_user(uint16_t keycode, keyrecord_t *record) { 
	// Everything happens in here when steno keys come in.
	// Bail on keyup
	if (!record->event.pressed) return true;

	// Update key repeat timers
	repTimer = timer_read();
	inChord  = true;

	// Switch on the press adding to chord
	bool pr = record->event.pressed;
	switch (keycode) {
			// Mods and stuff
			case STN_ST1:			pr ? (cChord |= (ST1)): (cChord &= ~(ST1)); break;
			case STN_ST2:			pr ? (cChord |= (ST2)): (cChord &= ~(ST2)); break;
			case STN_ST3:			pr ? (cChord |= (ST3)): (cChord &= ~(ST3)); break;
			case STN_ST4:			pr ? (cChord |= (ST4)): (cChord &= ~(ST4)); break;

			// All the letter keys
			case STN_S1:			pr ? (cChord |= (LSU)) : (cChord &= ~(LSU));  break;
			case STN_S2:			pr ? (cChord |= (LSD)) : (cChord &= ~(LSD));  break;
			case STN_TL:			pr ? (cChord |= (LFT)) : (cChord &= ~(LFT)); break;
			case STN_KL:			pr ? (cChord |= (LK)) : (cChord &= ~(LK)); break;
			case STN_PL:			pr ? (cChord |= (LP)) : (cChord &= ~(LP)); break;
			case STN_WL:			pr ? (cChord |= (LW)) : (cChord &= ~(LW)); break;
			case STN_HL:			pr ? (cChord |= (LH)) : (cChord &= ~(LH)); break;
			case STN_RL:			pr ? (cChord |= (LR)) : (cChord &= ~(LR)); break;
	}

	// Store previous state for fastQWER
	if (pr) {
		chordState[chordIndex] = cChord; 
		chordIndex++;
	}

	return true; 
}
void matrix_scan_user(void) {
	// We abuse this for early sending of key
	// Key repeat only on QWER/SYMB layers
	if (cMode != QWERTY || !inChord) return;

	// Check timers
#ifndef NO_REPEAT
	if (repEngaged && timer_elapsed(repTimer) > REP_DELAY) {
		// Process Key for report
		processChord();

		// Send report to host
		send_keyboard_report();
		clear_keyboard();
		repTimer = timer_read();
	}

	if (!repEngaged && timer_elapsed(repTimer) > REP_INIT_DELAY) {
		repEngaged = true;
	}
#endif
};

// Try and match cChord
uint16_t	processQwerty(bool lookup) {
	// search through all four dicts
	// run if lookup is false
	// Single key chords
	uprintf("Trying to match keys\n");
	for (int i = 0; i < keyLen; i++) {
		if (keyDict[i].chord == cChord) {
			if (!lookup) 
				register_code(keyDict[i].key);
			return cChord;
		}
	}
	
	uprintf("Trying to match combos\n");
	// combos
	for (int i = 0; i < comboLen; i++) {
		if (cmbDict[i].chord == cChord) {
			if (!lookup) {
				for (int j = 0; (j < COMBO_MAX) && (cmbDict[i].keys[j] != COMBO_END); i++) {
					register_code(cmbDict[i].keys[j]);
				}
			}
			return cChord;
		}
	}

	// strings
	uprintf("%d\n", stringLen);
	for (uint16_t i = 0; i < stringLen; i++) {
		//uprintf("testing: %d", &strDict);
		//uprintf("testing: %i", i);
		/*if (strDict[i].chord == cChord) {
			//if (!lookup) SEND_STRING("asdf");
			//if (!lookup) send_string(strDict[i].str);
			return cChord;
		}*/
	}

	// functions
	uprintf("Trying to match funcs\n");
	for (int i = 0; i < funcsLen; i++) {
		if (funDict[i].chord == cChord) {
			if (!lookup) funDict[i].act();
			return cChord;
		}
	}

	return 0;
}
// Traverse the chord history to a given point
// Returns the mask to use
void processChord(void) {
	// Save the clean chord state
	uint16_t savedChord = cChord;

	// Apply Stick Bits if needed
	if (stickyBits != 0) {
		cChord |= stickyBits;
		for (int i = 0; i <= chordIndex; i++)
			chordState[i] |= stickyBits;
	}

	// First we test if a whole chord was passsed
	// If so we just run it handling repeat logic
	if (processQwerty(true) == cChord) {
		processQwerty(false);
		// Repeat logic
		if (repeatFlag) {
			restoreState();
			repeatFlag = false;
			processChord();
		} else {
			saveState(cChord);
		}
		return;
	}

	// Iterate through chord picking out the individual 
	// and longest chords
	uint16_t bufChords[QWERBUF];
	int		 bufLen		= 0;
	uint16_t mask		= 0;

	// We iterate over it multiple times to catch the longest
	// chord. Then that gets addded to the mask and re run.
	while (savedChord != mask) {
		uint16_t test				= 0;
		uint16_t longestChord	= 0;

		for (int i = 0; i <= chordIndex; i++) {
			cChord = chordState[i] & ~mask;
			if (cChord == 0)
				continue;

			test = processQwerty(true);
			if (test != 0) {
				longestChord = test;
			}
		}
		
		mask |= longestChord;
		bufChords[bufLen] = longestChord;
		bufLen++;

		// That's a loop of sorts, halt processing
		if (bufLen >= QWERBUF) {
			return;
		}
	}
	
	// Now that the buffer is populated, we run it
	for (int i = 0; i < bufLen ; i++) {
		cChord = bufChords[i];
		processQwerty(false);
	}

	// Save state in case of repeat
	if (!repeatFlag) {			
		saveState(savedChord);
	}

	// Restore cChord for held repeat
	cChord = savedChord;

	return;
}
void saveState(uint16_t cleanChord) {
	pChord = cleanChord;
	pChordIndex = chordIndex;
	for (int i = 0; i < 32; i++) 
		pChordState[i] = chordState[i];
}
void restoreState() {
	cChord = pChord;
	chordIndex = pChordIndex;
	for (int i = 0; i < 32; i++) 
		chordState[i] = pChordState[i];
}

// Macros for calling from keymap.c
void SEND(uint8_t kc) {
	// Send Keycode, Does not work for Quantum Codes
	if (cMode == COMMAND && CMDLEN < MAX_CMD_BUF) {
#ifndef NO_DEBUG
		uprintf("CMD LEN: %d BUF: %d\n", CMDLEN, MAX_CMD_BUF);
#endif
		CMDBUF[CMDLEN] = kc;
		CMDLEN++;
	} 

	if (cMode != COMMAND) register_code(kc);
	return;
}
void REPEAT(void) {
	if (cMode != QWERTY)
		return;

	repeatFlag = true;
	return;
}
void SET_STICKY(uint16_t stick) {
	stickyBits = stick;
	return;
}
void CLICK_MOUSE(uint8_t kc) {
#ifdef MOUSEKEY_ENABLE
	mousekey_on(kc);
	mousekey_send();

	// Store state for later use
	inMouse = true;
	mousePress = kc;
#endif
}
