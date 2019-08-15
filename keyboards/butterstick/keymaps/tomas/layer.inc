enum internal_keycodes {
    TOP1 = SAFE_RANGE, TOP2, TOP3, TOP4, TOP5, TOP6, TOP7, TOP8, TOP9, TOP0,
    BOT1, BOT2, BOT3, BOT4, BOT5, BOT6, BOT7, BOT8, BOT9, BOT0,
    
    FIRST_INTERNAL_KEYCODE = TOP1,
    LAST_INTERNAL_KEYCODE = BOT0
};

// No need for QMK layers, we can make our own. And we dont' even need GAME layer since we do not use steno!
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_butter(
    TOP1, TOP2, TOP3, TOP4, TOP5, TOP6, TOP7, TOP8, TOP9, TOP0,
    BOT1, BOT2, BOT3, BOT4, BOT5, BOT6, BOT7, BOT8, BOT9, BOT0
  )
};

// "Don't fuck with this, thanks." -- germ
// Sorry, it has been fucked with.
size_t keymapsCount = 1;

// Bit masks for hashing
#define H_TOP1 ((uint32_t) 1 << 0)
#define H_TOP2 ((uint32_t) 1 << 1)
#define H_TOP3 ((uint32_t) 1 << 2)
#define H_TOP4 ((uint32_t) 1 << 3)
#define H_TOP5 ((uint32_t) 1 << 4)
#define H_TOP6 ((uint32_t) 1 << 5)
#define H_TOP7 ((uint32_t) 1 << 6)
#define H_TOP8 ((uint32_t) 1 << 7)
#define H_TOP9 ((uint32_t) 1 << 8)
#define H_TOP0 ((uint32_t) 1 << 9)
#define H_BOT1 ((uint32_t) 1 << 10)
#define H_BOT2 ((uint32_t) 1 << 11)
#define H_BOT3 ((uint32_t) 1 << 12)
#define H_BOT4 ((uint32_t) 1 << 13)
#define H_BOT5 ((uint32_t) 1 << 14)
#define H_BOT6 ((uint32_t) 1 << 15)
#define H_BOT7 ((uint32_t) 1 << 16)
#define H_BOT8 ((uint32_t) 1 << 17)
#define H_BOT9 ((uint32_t) 1 << 18)
#define H_BOT0 ((uint32_t) 1 << 19)