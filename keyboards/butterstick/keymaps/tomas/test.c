#define mu_assert(message, test)     do {         if (!(test)) {             return message;         }     } while (0)

#define mu_run_test(test)     do {         char *message = test();         tests_run++;         if (message) {             return message;         }     } while (0)

int tests_run = 0;

enum ASSERT_TYPES {
    UINT,
    INT
};

#define BUFF_SIZE 1024
char buffer[BUFF_SIZE];

#define ASSERT_EQ(type, actual, expected)     do {         if (actual != expected) {             switch (type) {                 case UINT:                     snprintf(buffer, BUFF_SIZE, "%s FAILED\nline %d\nactual = %u\nexpected = %u\n", name, __LINE__, actual, expected);                     break;                 case INT:                     snprintf(buffer, BUFF_SIZE, "%s FAILED\nline %d\nactual = %d\nexpected = %d\n", name, __LINE__, actual, expected);                     break;                 default:                     snprintf(buffer, BUFF_SIZE, "%s FAILED\nline %d\nunsupported ASSERT_EQ type\n", name, __LINE__);                     break;             }             return buffer;         }     } while (0)

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define MATRIX_ROWS 2
#define MATRIX_COLS 10
#define LAYOUT_butter( 										    k09, k08, k07, k06, k05, k04, k03, k02, k01, k00,			k19, k18, k17, k16, k15, k14, k13, k12, k11, k10		) { 															{ k00, k01, k02, k03, k04, k05, k06, k07, k08, k09},		{ k10, k11, k12, k13, k14, k15, k16, k17, k18, k19},	}

#define PROGMEM
#define memcpy_P memcpy
const struct Chord *
pgm_read_word (const struct Chord *const *chord) {
    return *chord;
}

typedef struct {
    uint8_t col;
    uint8_t row;
} keypos_t;

typedef struct {
    keypos_t key;
    bool pressed;
    uint16_t time;
} keyevent_t;

typedef struct {
    bool interrupted:1;
    bool reserved2:1;
    bool reserved1:1;
    bool reserved0:1;
    uint8_t count:4;
} tap_t;

typedef struct {
    keyevent_t event;
    tap_t tap;
} keyrecord_t;

keyrecord_t pressed = { {{0, 0}, true, 0}, {0, 0, 0, 0, 0} };
keyrecord_t depressed = { {{0, 0}, false, 0}, {0, 0, 0, 0, 0} };

enum keycodes {
    KC_NO,
    KC_TILDE,
    KC_GRAVE,
    KC_EXCLAIM,
    KC_1,
    KC_AT,
    KC_2,
    KC_HASH,
    KC_3,
    KC_DOLLAR,
    KC_4,
    KC_PERCENT,
    KC_5,
    KC_CIRCUMFLEX,
    KC_6,
    KC_AMPERSAND,
    KC_7,
    KC_ASTERISK,
    KC_8,
    KC_LEFT_PAREN,
    KC_9,
    KC_RIGHT_PAREN,
    KC_0,
    KC_UNDERSCORE,
    KC_MINUS,
    KC_PLUS,
    KC_EQUAL,
    KC_LEFT_CURLY_BRACE,
    KC_LBRACKET,
    KC_RIGHT_CURLY_BRACE,
    KC_RBRACKET,
    KC_PIPE,
    KC_BSLASH,
    KC_COLON,
    KC_SCOLON,
    KC_DOUBLE_QUOTE,
    KC_QUOTE,
    KC_LEFT_ANGLE_BRACKET,
    KC_COMMA,
    KC_RIGHT_ANGLE_BRACKET,
    KC_DOT,
    KC_QUESTION,
    KC_SLASH,
    KC_Q,
    KC_W,
    KC_E,
    KC_R,
    KC_T,
    KC_Y,
    KC_U,
    KC_I,
    KC_O,
    KC_P,
    KC_A,
    KC_S,
    KC_D,
    KC_F,
    KC_G,
    KC_H,
    KC_J,
    KC_K,
    KC_L,
    KC_Z,
    KC_X,
    KC_C,
    KC_V,
    KC_B,
    KC_N,
    KC_M,
    KC_ESC,
    KC_LSFT,
    KC_LCTL,
    KC_LGUI,
    KC_LALT,
    KC_RALT,
    KC_RCTL,
    KC_RGUI,
    KC_RSFT,
    KC_TAB,
    KC_DEL,
    KC_INS,
    KC_BSPC,
    KC_ENTER,
    KC_SPACE,
    KC_F1,
    KC_F2,
    KC_F3,
    KC_F4,
    KC_F5,
    KC_F6,
    KC_F7,
    KC_F8,
    KC_F9,
    KC_F10,
    KC_F11,
    KC_F12,

    SAFE_RANGE
};

// this does not track history, maybe it should?
// also now I do not have build up and tear down for every test and I really should
bool keyboard[SAFE_RANGE - 1];
int16_t current_time;

void
register_code (int16_t keycode) {
    keyboard[keycode] = 1;
};

void
unregister_code (int16_t keycode) {
    keyboard[keycode] = 0;
};

void
send_keyboard_report (void) {   /*still don't know what this does */
};

void matrix_scan_user (void);
void
wait_ms (uint16_t ms) {
    current_time += ms;
};

uint16_t
timer_read (void) {
    uint16_t result = current_time;

    return result;
};

uint16_t
timer_elapsed (uint16_t timer) {
    uint16_t result = current_time - timer;

    return result;
};

void
layer_move (int16_t layer) {    /*ignoring for now */
};

void
clear_keyboard (void) {
    for (int i = 0; i < SAFE_RANGE - 1; i++) {
        keyboard[i] = 0;
}};

void
reset_keyboard (void) {         /*ignoring for now */
};

void
pause_ms (uint16_t ms) {
    for (int i = 0; i < ms; i++) {
        current_time++;
        matrix_scan_user ();
}};

enum pseudolayers {
    ALWAYS_ON, QWERTY, NUM
};

// Macros to simplify chord definitions

// Keyboard states and settings

#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 200
#define LEADER_TIMEOUT 750
#define TAP_TIMEOUT 50

enum internal_keycodes {
    TOP1 = SAFE_RANGE,
    TOP2,
    TOP3,
    TOP4,
    TOP5,
    TOP6,
    TOP7,
    TOP8,
    TOP9,
    TOP0,
    BOT1,
    BOT2,
    BOT3,
    BOT4,
    BOT5,
    BOT6,
    BOT7,
    BOT8,
    BOT9,
    BOT0,

    FIRST_INTERNAL_KEYCODE = TOP1,
    LAST_INTERNAL_KEYCODE = BOT0
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_butter (TOP1,
                         TOP2,
                         TOP3,
                         TOP4,
                         TOP5,
                         TOP6,
                         TOP7,
                         TOP8,
                         TOP9,
                         TOP0,
                         BOT1,
                         BOT2,
                         BOT3,
                         BOT4,
                         BOT5,
                         BOT6,
                         BOT7,
                         BOT8,
                         BOT9,
                         BOT0),
};

    // "Don't fuck with this, thanks." -- germ
    // Sorry, it has been fucked with.
size_t keymapsCount = 1;

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

// The chord structure and chord functions (send key, switch pseudolayer, ...)
uint8_t current_pseudolayer = 1;
bool lock_next = false;
uint16_t chord_timer = 0;
uint16_t dance_timer = 0;
bool autoshift_mode = true;

uint8_t keycodes_buffer_array[20] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

uint8_t keycode_index = 0;

uint8_t command_mode = 0;

uint16_t command_buffer[] = {
    0,
    0,
    0,
    0,
    0,
};

uint8_t command_ind = 0;

bool in_leader_mode = false;

uint16_t leader_buffer[] = {
    0,
    0,
    0,
    0,
    0,
};

uint8_t leader_ind = 0;
uint16_t leader_timer = 0;

bool dynamic_macro_mode = false;

uint16_t dynamic_macro_buffer[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

uint8_t dynamic_macro_ind;

bool a_key_went_through = false;

enum chord_states {
    IDLE,
    READY,
    ACTIVATED,
    DEACTIVATED,
    PRESS_FROM_ACTIVE,
    FINISHED_FROM_ACTIVE,
    IDLE_IN_DANCE,
    READY_IN_DANCE,
    FINISHED,
    LOCKED,
    READY_LOCKED,
    RESTART,
    IN_ONE_SHOT
};

struct Chord {
    uint32_t keycodes_hash;
    uint8_t pseudolayer;
    uint8_t *state;
    uint8_t *counter;
    uint16_t value1;
    uint8_t value2;
    void (*function) (const struct Chord *);
};

bool
handle_US_ANSI_shifted_keys (int16_t keycode, bool in) {
    bool is_US_ANSI_shifted = true;
    int16_t regular_keycode = KC_NO;

    switch (keycode) {
    case KC_TILDE:
        regular_keycode = KC_GRAVE;
        break;
    case KC_EXCLAIM:
        regular_keycode = KC_1;
        break;
    case KC_AT:
        regular_keycode = KC_2;
        break;
    case KC_HASH:
        regular_keycode = KC_3;
        break;
    case KC_DOLLAR:
        regular_keycode = KC_4;
        break;
    case KC_PERCENT:
        regular_keycode = KC_5;
        break;
    case KC_CIRCUMFLEX:
        regular_keycode = KC_6;
        break;
    case KC_AMPERSAND:
        regular_keycode = KC_7;
        break;
    case KC_ASTERISK:
        regular_keycode = KC_8;
        break;
    case KC_LEFT_PAREN:
        regular_keycode = KC_9;
        break;
    case KC_RIGHT_PAREN:
        regular_keycode = KC_0;
        break;
    case KC_UNDERSCORE:
        regular_keycode = KC_MINUS;
        break;
    case KC_PLUS:
        regular_keycode = KC_EQUAL;
        break;
    case KC_LEFT_CURLY_BRACE:
        regular_keycode = KC_LBRACKET;
        break;
    case KC_RIGHT_CURLY_BRACE:
        regular_keycode = KC_RBRACKET;
        break;
    case KC_PIPE:
        regular_keycode = KC_BSLASH;
        break;
    case KC_COLON:
        regular_keycode = KC_SCOLON;
        break;
    case KC_DOUBLE_QUOTE:
        regular_keycode = KC_QUOTE;
        break;
    case KC_LEFT_ANGLE_BRACKET:
        regular_keycode = KC_COMMA;
        break;
    case KC_RIGHT_ANGLE_BRACKET:
        regular_keycode = KC_DOT;
        break;
    case KC_QUESTION:
        regular_keycode = KC_SLASH;
        break;
    default:
        is_US_ANSI_shifted = false;
    }
    if (is_US_ANSI_shifted) {
        if (in) {
            register_code (KC_LSFT);
            register_code (regular_keycode);
        } else {
            unregister_code (regular_keycode);
            unregister_code (KC_LSFT);
        }
    }
    return is_US_ANSI_shifted;
}

void
key_in (int16_t keycode) {
    if (command_mode == 1 && command_ind < 5) {
        command_buffer[command_ind] = keycode;
        command_ind++;
        a_key_went_through = true;
    } else if (in_leader_mode && leader_ind < 5) {
        leader_buffer[leader_ind] = keycode;
        leader_ind++;
        a_key_went_through = true;
    } else if (dynamic_macro_mode && dynamic_macro_ind < 20) {
        dynamic_macro_buffer[dynamic_macro_ind] = keycode;
        dynamic_macro_ind++;
        a_key_went_through = true;
    } else {
        if (!handle_US_ANSI_shifted_keys (keycode, true)) {
            register_code (keycode);
        }
        send_keyboard_report ();
        a_key_went_through = true;
    }
}

void
key_out (int16_t keycode) {
    if (command_mode == 0) {
        if (!handle_US_ANSI_shifted_keys (keycode, false)) {
            unregister_code (keycode);
        }
        send_keyboard_report ();
    }
}

void
tap_key (int16_t keycode) {
    key_in (keycode);
    wait_ms (TAP_TIMEOUT);
    key_out (keycode);
}

void
single_dance (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        key_in (self->value1);
        break;
    case DEACTIVATED:
        key_out (self->value1);
        *self->state = IDLE;
        break;
    case RESTART:
        key_out (self->value1);
        break;
    default:
        break;
    }
}

void
key_layer_dance (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        current_pseudolayer = self->value2;
        a_key_went_through = false;
        break;
    case DEACTIVATED:
    case RESTART:
        if (!a_key_went_through) {
            tap_key (self->value1);
        }
        current_pseudolayer = self->pseudolayer;
        *self->state = IDLE;    // does not have effect if the state was RESTART
        break;
    default:
        break;
    }
}

void
key_mod_dance (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        key_in (self->value2);
        a_key_went_through = false;
        break;
    case DEACTIVATED:
    case RESTART:
        key_out (self->value2);
        if (!a_key_went_through) {
            tap_key (self->value1);
        }
        *self->state = IDLE;    // does not have effect if the state was RESTART
        break;
    default:
        break;
    }
}

void
key_key_dance (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        break;
    case DEACTIVATED:
        tap_key (self->value1);
        *self->state = IDLE;
        break;
    case FINISHED:
    case PRESS_FROM_ACTIVE:
        key_in (self->value2);
        break;
    case RESTART:
        key_out (self->value2);
        break;
    default:
        break;
    }
}

void
autoshift_dance_impl (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        *self->counter = 0;
        break;
    case DEACTIVATED:
    case RESTART:
        tap_key (self->value1);
        *self->state = IDLE;
        break;
    case FINISHED_FROM_ACTIVE:
        if (*self->counter == 2) {
            key_in (KC_LSFT);
            tap_key (self->value1);
            key_out (KC_LSFT);
            *self->state = IDLE;
            // the skip to IDLE is usually just a lag optimization,
            // in this case it has a logic function, on a short
            // press (still longer than a tap) the key does not get shifted
        } else {
            *self->counter += 1;
            *self->state = PRESS_FROM_ACTIVE;
            dance_timer = timer_read ();
        }
        break;
    default:
        break;
    }
}

void
autoshift_dance (const struct Chord *self) {
    if (autoshift_mode) {
        autoshift_dance_impl (self);
    } else {
        single_dance (self);
    }
}

void
autoshift_toggle (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        autoshift_mode = !autoshift_mode;
        *self->state = IDLE;
    }
}

void
temp_pseudolayer (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        current_pseudolayer = self->value1;
        break;
    case DEACTIVATED:
        current_pseudolayer = self->pseudolayer;
        break;
    case RESTART:
        current_pseudolayer = self->pseudolayer;
        break;
    default:
        break;
    }
}

void
perm_pseudolayer (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        current_pseudolayer = self->value1;
    }
}

void
switch_layer (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        layer_move (self->value1);
    }
}

void
lock (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        lock_next = true;
    }
}

void
one_shot_key (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        break;
    case DEACTIVATED:
        key_in (self->value1);
        *self->state = IN_ONE_SHOT;
        break;
    case FINISHED:
    case PRESS_FROM_ACTIVE:
        key_in (self->value1);
        a_key_went_through = false;
        break;
    case RESTART:
        if (a_key_went_through) {
            key_out (self->value1);
        } else {
            *self->state = IN_ONE_SHOT;
        }
    default:
        break;
    }
}

void
one_shot_layer (const struct Chord *self) {
    switch (*self->state) {
    case ACTIVATED:
        break;
    case DEACTIVATED:
        current_pseudolayer = self->value1;
        *self->state = IN_ONE_SHOT;
        break;
    case FINISHED:
    case PRESS_FROM_ACTIVE:
        current_pseudolayer = self->value1;
        a_key_went_through = false;
        break;
    case RESTART:
        if (a_key_went_through) {
            current_pseudolayer = self->pseudolayer;
        } else {
            *self->state = IN_ONE_SHOT;
        }
    default:
        break;
    }
}

void
command (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        command_mode++;
    }
}

bool
identical (uint16_t * buffer1, uint16_t * buffer2) {
    bool same = true;

    for (int i = 0; i < 5; i++) {
        same = same && (buffer1[i] == buffer2[i]);
    }
    return same;
}

void
leader (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        in_leader_mode = true;
    }
}

void
dynamic_macro_record (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        for (int i = 0; i < 20; i++) {
            dynamic_macro_buffer[i] = 0;
        }
        dynamic_macro_mode = true;
    }
}

void
dynamic_macro_next (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode && dynamic_macro_ind < 20) {
            dynamic_macro_buffer[dynamic_macro_ind] = 0;
            dynamic_macro_ind++;
        }
    }
}

void
dynamic_macro_end (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode) {
            dynamic_macro_mode = false;
        }
    }
}

void
dynamic_macro_play (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        int ind_start = 0;

        while (ind_start < 20) {
            for (int i = ind_start; i < 20; i++) {
                register_code (dynamic_macro_buffer[i]);
                if (dynamic_macro_buffer[i] == 0) {
                    break;
                }
            }
            send_keyboard_report ();
            wait_ms (TAP_TIMEOUT);
            for (int i = ind_start; i < 20; i++) {
                unregister_code (dynamic_macro_buffer[i]);
                if (dynamic_macro_buffer[i] == 0) {
                    ind_start = i + 1;
                    break;
                }
            }
            send_keyboard_report ();
        }
    }
}

void clear (const struct Chord *self);

void
reset_keyboard_kb (void) {
#ifdef WATCHDOG_ENABLE
    MCUSR = 0;
    wdt_disable ();
    wdt_reset ();
#endif
    reset_keyboard ();
}

void
reset (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        reset_keyboard_kb ();
    }
}

uint8_t state_0 = IDLE;
const struct Chord chord_0 PROGMEM = { H_TOP1, QWERTY, &state_0, NULL, KC_Q, 0, single_dance };

uint8_t state_1 = IDLE;
const struct Chord chord_1 PROGMEM = { H_TOP2, QWERTY, &state_1, NULL, KC_W, 0, single_dance };

// Register all chords, load chording logic
const struct Chord *const list_of_chords[] PROGMEM = {
    &chord_0,
    &chord_1,

};

const uint16_t **leader_triggers PROGMEM = NULL;

void (*leader_functions[]) (void) = {

};

struct Chord *last_chord = NULL;

bool
are_hashed_keycodes_in_sound (uint32_t keycodes_hash, uint32_t sound) {
    return (keycodes_hash & sound) == keycodes_hash;
}

uint8_t
keycode_to_index (uint16_t keycode) {
    return keycode - FIRST_INTERNAL_KEYCODE;
}

void
sound_keycode_array (uint16_t keycode) {
    uint8_t index = keycode_to_index (keycode);

    keycode_index++;
    keycodes_buffer_array[index] = keycode_index;
}

void
silence_keycode_hash_array (uint32_t keycode_hash) {
    for (int i = 0; i < 20; i++) {
        bool index_in_hash = ((uint32_t) 1 << i) & keycode_hash;

        if (index_in_hash) {
            uint8_t current_val = keycodes_buffer_array[i];

            keycodes_buffer_array[i] = 0;
            for (int j = 0; j < 20; j++) {
                if (keycodes_buffer_array[j] > current_val) {
                    keycodes_buffer_array[j]--;
                }
            }
            keycode_index--;
        }
    }
}

bool
are_hashed_keycodes_in_array (uint32_t keycode_hash) {
    for (int i = 0; i < 20; i++) {
        bool index_in_hash = ((uint32_t) 1 << i) & keycode_hash;
        bool index_in_array = (bool) keycodes_buffer_array[i];

        if (index_in_hash && !index_in_array) {
            return false;
        }
    }
    return true;
}

void
kill_one_shots (void) {
    for (int i = 0; i < 2; i++) {
        // const struct Chord* chord = list_of_chords[i];
        struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;

        memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
        struct Chord *chord = &chord_storage;

        if (*chord->state == IN_ONE_SHOT) {
            *chord->state = RESTART;
            chord->function (chord);
            if (*chord->state == RESTART) {
                *chord->state = IDLE;
            }
        }
    }
}

void
process_finished_dances (void) {
    for (int i = 0; i < 2; i++) {
        struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;

        memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
        struct Chord *chord = &chord_storage;

        if (*chord->state == ACTIVATED) {
            *chord->state = PRESS_FROM_ACTIVE;
            chord->function (chord);
            if (a_key_went_through) {
                kill_one_shots ();
            }
            dance_timer = timer_read ();
        } else if (*chord->state == IDLE_IN_DANCE) {
            *chord->state = FINISHED;
            chord->function (chord);
            if (*chord->state == FINISHED) {
                *chord->state = RESTART;
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
            }
        } else if (*chord->state == PRESS_FROM_ACTIVE) {
            *chord->state = FINISHED_FROM_ACTIVE;
            chord->function (chord);
            if (a_key_went_through) {
                kill_one_shots ();
            }
            dance_timer = timer_read ();
        }
    }
}

void
deactivate_active_taphold_chords (struct Chord *caller) {
    if (caller->function == key_layer_dance || caller->function == key_key_dance) {
        return;
    }

    for (int i = 0; i < 2; i++) {
        struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;

        memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
        struct Chord *chord = &chord_storage;

        if (*chord->state == ACTIVATED
            && (chord->function == key_layer_dance || chord->function == key_key_dance)) {
            *chord->state = DEACTIVATED;
            chord->function (chord);
            if (*chord->state == DEACTIVATED) {
                *chord->state = IDLE;   // not necessary but let's keep it here for clarity
            }
        }
    }
}

uint8_t
keycodes_buffer_array_min (uint8_t * first_keycode_index) {
    for (int i = 0; i < 20; i++) {
        if (keycodes_buffer_array[i] == 1) {
            if (first_keycode_index != NULL) {
                *first_keycode_index = (uint8_t) i;
            }
            return 1;
        }
    }
    return 0;
}

void
remove_subchords (void) {
    for (int i = 0; i < 2; i++) {
        struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;

        memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
        struct Chord *chord = &chord_storage;

        if (!
            (*chord->state == READY || *chord->state == READY_IN_DANCE
             || *chord->state == READY_LOCKED)) {
            continue;
        }

        for (int j = 0; j < 2; j++) {
            if (i == j) {
                continue;
            }

            struct Chord *chord_ptr_2 = (struct Chord *) pgm_read_word (&list_of_chords[j]);
            struct Chord chord_storage_2;

            memcpy_P (&chord_storage_2, chord_ptr_2, sizeof (struct Chord));
            struct Chord *chord_2 = &chord_storage_2;

            if (are_hashed_keycodes_in_sound (chord_2->keycodes_hash, chord->keycodes_hash)) {
                if (*chord_2->state == READY) {
                    *chord_2->state = IDLE;
                }
                if (*chord_2->state == READY_IN_DANCE) {
                    *chord_2->state = IDLE_IN_DANCE;
                }
                if (*chord_2->state == READY_LOCKED) {
                    *chord_2->state = LOCKED;
                }
            }
        }
    }
}

void
process_ready_chords (void) {
    uint8_t first_keycode_index = 0;

    while (keycodes_buffer_array_min (&first_keycode_index)) {
        // find ready chords
        for (int i = 0; i < 2; i++) {
            struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
            struct Chord chord_storage;

            memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
            struct Chord *chord = &chord_storage;

            // if the chord does not contain the first keycode
            bool contains_first_keycode =
                ((uint32_t) 1 << first_keycode_index) & chord->keycodes_hash;
            if (!contains_first_keycode) {
                continue;
            }

            if (!are_hashed_keycodes_in_array (chord->keycodes_hash)) {
                continue;
            }

            if (*chord->state == LOCKED) {
                *chord->state = READY_LOCKED;
                continue;
            }

            if (!(chord->pseudolayer == current_pseudolayer || chord->pseudolayer == ALWAYS_ON)) {
                continue;
            }

            if (*chord->state == IDLE) {
                *chord->state = READY;
                continue;
            }

            if (*chord->state == IDLE_IN_DANCE) {
                *chord->state = READY_IN_DANCE;
            }
        }

        // remove subchords
        remove_subchords ();

        // execute logic
        // this should be only one chord
        struct Chord *chord = NULL;

        for (int i = 0; i < 2; i++) {
            struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
            struct Chord chord_storage;

            memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
            chord = &chord_storage;

            if (*chord->state == READY_LOCKED) {
                *chord->state = RESTART;
                chord->function (chord);
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
                break;
            }

            if (*chord->state == READY || *chord->state == READY_IN_DANCE) {
                if (last_chord && last_chord != chord) {
                    process_finished_dances ();
                }

                bool lock_next_prev_state = lock_next;

                *chord->state = ACTIVATED;
                chord->function (chord);
                dance_timer = timer_read ();
                deactivate_active_taphold_chords (chord);

                if (lock_next && lock_next == lock_next_prev_state) {
                    lock_next = false;
                    *chord->state = PRESS_FROM_ACTIVE;
                    chord->function (chord);
                    if (*chord->state == PRESS_FROM_ACTIVE) {
                        *chord->state = LOCKED;
                    }
                    if (a_key_went_through) {
                        kill_one_shots ();
                    }
                }
                break;
            }
        }

        // silence notes
        silence_keycode_hash_array (chord->keycodes_hash);
    }
}

void
deactivate_active_chords (uint16_t keycode) {
    uint32_t hash = (uint32_t) 1 << (keycode - SAFE_RANGE);
    bool broken;

    for (int i = 0; i < 2; i++) {
        struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;

        memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
        struct Chord *chord = &chord_storage;

        broken = are_hashed_keycodes_in_sound (hash, chord->keycodes_hash);
        if (!broken) {
            continue;
        }

        switch (*chord->state) {
        case ACTIVATED:
            *chord->state = DEACTIVATED;
            chord->function (chord);

            if (*chord->state == DEACTIVATED) {
                dance_timer = timer_read ();
                *chord->state = IDLE_IN_DANCE;
            }
            if (*chord->state != IN_ONE_SHOT) {
                kill_one_shots ();
            }
            break;
        case PRESS_FROM_ACTIVE:
        case FINISHED_FROM_ACTIVE:
            *chord->state = RESTART;
            chord->function (chord);
            if (*chord->state == RESTART) {
                *chord->state = IDLE;
            }
            kill_one_shots ();
            break;
        default:
            break;
        }
    }

}

void
process_command (void) {
    command_mode = 0;
    for (int i = 0; i < 5; i++) {
        if (command_buffer[i]) {
            register_code (command_buffer[i]);
        }
        send_keyboard_report ();
    }
    wait_ms (TAP_TIMEOUT);
    for (int i = 0; i < 5; i++) {
        if (command_buffer[i]) {
            unregister_code (command_buffer[i]);
        }
        send_keyboard_report ();
    }
    for (int i = 0; i < 5; i++) {
        command_buffer[i] = 0;
    }
    command_ind = 0;
}

void
process_leader (void) {
    in_leader_mode = false;
    for (int i = 0; i < 0; i++) {
        uint16_t trigger[5];

        memcpy_P (trigger, leader_triggers[i], 5 * sizeof (uint16_t));

        if (identical (leader_buffer, trigger)) {
            (*leader_functions[i]) ();
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        leader_buffer[i] = 0;
    }
}

bool
process_record_user (uint16_t keycode, keyrecord_t * record) {
    if (keycode < FIRST_INTERNAL_KEYCODE || keycode > LAST_INTERNAL_KEYCODE) {
        return true;
    }

    if (record->event.pressed) {
        sound_keycode_array (keycode);
    } else {
        process_ready_chords ();
        deactivate_active_chords (keycode);
    }
    chord_timer = timer_read ();
    leader_timer = timer_read ();

    return false;
}

void
matrix_scan_user (void) {
    bool chord_timer_expired = timer_elapsed (chord_timer) > CHORD_TIMEOUT;

    if (chord_timer_expired && keycodes_buffer_array_min (NULL)) {
        process_ready_chords ();
    }

    bool dance_timer_expired = timer_elapsed (dance_timer) > DANCE_TIMEOUT;

    if (dance_timer_expired) {  // would love to have && in_dance but not sure how
        process_finished_dances ();
    }

    bool in_command_mode = command_mode == 2;

    if (in_command_mode) {
        process_command ();
    }

    bool leader_timer_expired = timer_elapsed (leader_timer) > LEADER_TIMEOUT;

    if (leader_timer_expired && in_leader_mode) {
        process_leader ();
    }

}

// for now here
void
clear (const struct Chord *self) {
    if (*self->state == ACTIVATED) {
        // kill all chords
        for (int i = 0; i < 2; i++) {
            struct Chord *chord_ptr = (struct Chord *) pgm_read_word (&list_of_chords[i]);
            struct Chord chord_storage;

            memcpy_P (&chord_storage, chord_ptr, sizeof (struct Chord));
            struct Chord *chord = &chord_storage;

            *chord->state = IDLE;

            if (chord->counter) {
                *chord->counter = 0;
            }
        }

        // clear keyboard
        clear_keyboard ();
        send_keyboard_report ();

        // switch to default pseudolayer
        current_pseudolayer = 1;

        // clear all keyboard states
        lock_next = false;
        autoshift_mode = true;
        command_mode = 0;
        in_leader_mode = false;
        dynamic_macro_mode = false;
        a_key_went_through = false;

        for (int i; i < 20; i++) {
            dynamic_macro_buffer[i] = 0;
        }

    }
}

static char *
test_pause_ms () {
    char name[] = "pause_ms";

    current_time = 0;
    clear_keyboard ();

    pause_ms (500);
    ASSERT_EQ (UINT, current_time, 500);

    printf ("%s PASSED\n", name);
    return 0;
}

static char *
test_single_dance () {
    char name[] = "single_dance";

    current_time = 0;
    clear_keyboard ();

    pause_ms (500);
    ASSERT_EQ (UINT, state_0, IDLE);
    process_record_user (TOP1, &pressed);
    pause_ms (CHORD_TIMEOUT);
    ASSERT_EQ (UINT, state_0, IDLE);
    pause_ms (1);
    ASSERT_EQ (UINT, state_0, ACTIVATED);
    pause_ms (DANCE_TIMEOUT);
    ASSERT_EQ (UINT, state_0, ACTIVATED);
    pause_ms (1);
    ASSERT_EQ (UINT, state_0, PRESS_FROM_ACTIVE);
    pause_ms (DANCE_TIMEOUT);
    ASSERT_EQ (UINT, state_0, PRESS_FROM_ACTIVE);
    pause_ms (1);
    ASSERT_EQ (UINT, state_0, FINISHED_FROM_ACTIVE);
    process_record_user (TOP1, &depressed);
    ASSERT_EQ (UINT, state_0, IDLE);

    printf ("%s PASSED\n", name);
    return 0;
}

static char *
test_single_dance_fast () {
    char name[] = "single_dance_fast";

    current_time = 0;
    clear_keyboard ();

    pause_ms (500);
    ASSERT_EQ (UINT, state_0, IDLE);
    process_record_user (TOP1, &pressed);
    pause_ms (CHORD_TIMEOUT);
    ASSERT_EQ (UINT, state_0, IDLE);
    pause_ms (1);
    ASSERT_EQ (UINT, state_0, ACTIVATED);
    process_record_user (TOP1, &depressed);
    ASSERT_EQ (UINT, state_0, IDLE);

    printf ("%s PASSED\n", name);
    return 0;
}

// I can not test this without keyboard tracking history

// KL
// KM
// KK
// AS
// AT
// MO
// DF
// TO
// LOCK
// OSK
// OSL
// CMD
// LEADER
// DYNAMIC MACRO
// CLEAR_KB
// RESET

static char *
all_tests () {

    mu_run_test (test_pause_ms);

    mu_run_test (test_single_dance);

    mu_run_test (test_single_dance_fast);

    return 0;
}

int
main (int argc, char **argv) {
    char *result = all_tests ();

    if (result != 0) {
        printf ("%s\n", result);
    } else {
        printf ("\nALL TESTS PASSED\n");
    }
    printf ("Tests run: %d / %d\n", tests_run, 3);

    return result != 0;
}
