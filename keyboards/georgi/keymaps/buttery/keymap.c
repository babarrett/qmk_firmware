#include QMK_KEYBOARD_H

// use pyexpander to generate keymap.c:
// python3 expander3.py -f keymap.c.in | cat -s > keymap.c
// the preprocessor code is written to be readable not to produce nice output

enum pseudolayers {
    ALWAYS_ON, QWERTY
};

// Keyboard states and settings
#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 250

#define LEADER_TIMEOUT 500

#define TAP_TIMEOUT 50

uint8_t current_pseudolayer = 1;
bool lock_next = false;
uint32_t keycodes_buffer = 0;
uint16_t chord_timer = 0;
uint16_t dance_timer = 0;

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

// Definitions of internal keycodes and the actual QMK layer
enum internal_keycodes {
    TOP1 = SAFE_RANGE, TOP2, TOP3, TOP4, TOP5, TOP6,             TOP7, TOP8, TOP9, TOP10, TOP11, TOP12,
    BOT1, BOT2, BOT3, BOT4, BOT5, BOT6,                          BOT7, BOT8, BOT9, BOT10, BOT11, BOT12,
                      THU1, THU2, THU3,                          THU4, THU5, THU6
};

// No need for QMK layers, we can make our own. And we dont' even need GAME layer since we do not use steno!
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_georgi(
    TOP1, TOP2, TOP3, TOP4, TOP5, TOP6,             TOP7, TOP8, TOP9, TOP10, TOP11, TOP12,
    BOT1, BOT2, BOT3, BOT4, BOT5, BOT6,             BOT7, BOT8, BOT9, BOT10, BOT11, BOT12,
                      THU1, THU2, THU3,             THU4, THU5, THU6
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
#define H_TOP10 ((uint32_t) 1 << 9)
#define H_TOP11 ((uint32_t) 1 << 10)
#define H_TOP12 ((uint32_t) 1 << 11)
#define H_BOT1 ((uint32_t) 1 << 12)
#define H_BOT2 ((uint32_t) 1 << 13)
#define H_BOT3 ((uint32_t) 1 << 14)
#define H_BOT4 ((uint32_t) 1 << 15)
#define H_BOT5 ((uint32_t) 1 << 16)
#define H_BOT6 ((uint32_t) 1 << 17)
#define H_BOT7 ((uint32_t) 1 << 18)
#define H_BOT8 ((uint32_t) 1 << 19)
#define H_BOT9 ((uint32_t) 1 << 20)
#define H_BOT10 ((uint32_t) 1 << 21)
#define H_BOT11 ((uint32_t) 1 << 22)
#define H_BOT12 ((uint32_t) 1 << 23)
#define H_THU1 ((uint32_t) 1 << 24)
#define H_THU2 ((uint32_t) 1 << 25)
#define H_THU3 ((uint32_t) 1 << 26)
#define H_THU4 ((uint32_t) 1 << 27)
#define H_THU5 ((uint32_t) 1 << 28)
#define H_THU6 ((uint32_t) 1 << 29)

// The chord structure and chord functions (send key, switch pseudolayer, ...)
enum chord_states {
    IDLE,
    READY,
    ACTIVATED,
    DEACTIVATED,
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
    uint8_t* state;
    uint8_t* counter;
    uint16_t value1;
    uint8_t value2;
    void (*function) (const struct Chord*);
};

void key_in(int16_t keycode) {
    if (command_mode == 1 && command_ind < 5) {
        command_buffer[command_ind] = keycode;
        command_ind++;
    } else if (in_leader_mode && leader_ind < 5) {
        leader_buffer[leader_ind] = keycode;
        leader_ind++;
    } else if (dynamic_macro_mode && dynamic_macro_ind < 20) {
        dynamic_macro_buffer[dynamic_macro_ind] = keycode;
        dynamic_macro_ind++;
    } else {
        register_code(keycode);
        send_keyboard_report();
    }
}

void key_out(int16_t keycode) {
    unregister_code(keycode);
    send_keyboard_report();
}

void single_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            key_in(self->value1);
            break;
        case DEACTIVATED:
            key_out(self->value1);
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            break;
        case RESTART:
            key_out(self->value1);
            break;
        default:
            break;
    }
}

void key_layer_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            break;
        case DEACTIVATED:
            register_code(self->value1);
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            unregister_code(self->value1);
            send_keyboard_report();
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            current_pseudolayer = self->value2;
            break;
        case RESTART:
            current_pseudolayer = self->pseudolayer;
            break;
        default:
            break;
    }
}

void key_key_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            break;
        case DEACTIVATED:
            register_code(self->value1);
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            unregister_code(self->value1);
            send_keyboard_report();
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            register_code(self->value2);
            send_keyboard_report();
            break;
        case RESTART:
            unregister_code(self->value2);
            send_keyboard_report();
            break;
        default:
            break;
    }
}

void temp_pseudolayer(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            current_pseudolayer = self->value1;
            break;
        case DEACTIVATED:
            current_pseudolayer = self->pseudolayer;
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            break;
        case RESTART:
            current_pseudolayer = self->pseudolayer;
            break;
        default:
            break;
    }
}

void perm_pseudolayer(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        current_pseudolayer = self->value1;
    }
}

void lock(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        lock_next = true;
    }
}

void one_shot_key(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            key_in(self->value1);
            break;
        case FINISHED_FROM_ACTIVE:
            key_in(self->value1);
            break;
        case RESTART:
            key_out(self->value1);
        default:
            break;
    }
}

void one_shot_layer(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            current_pseudolayer = self->value1;
            break;
        case FINISHED_FROM_ACTIVE:
            current_pseudolayer = self->value1;
            break;
        case RESTART:
            current_pseudolayer = self->pseudolayer;
        default:
            break;
    }
}

void command(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        command_mode++;
    }
}

bool identical(uint16_t* buffer1, uint16_t* buffer2) {
    bool same = true;
    for (int i = 0; i < 5; i++) {
        same = same && (buffer1[i] == buffer2[i]);
    }
    return same;
}

void leader(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        in_leader_mode = true;
    }
}

void dynamic_macro_record(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        for (int i = 0; i < 20; i++) {
            dynamic_macro_buffer[i] = 0;
        }
        dynamic_macro_mode = true;
    }
}

void dynamic_macro_next(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode && dynamic_macro_ind < 20) {
            dynamic_macro_buffer[dynamic_macro_ind] = 0;
            dynamic_macro_ind++;
        }
    }
}

void dynamic_macro_end(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode) {
            dynamic_macro_mode = false;
        }
    }
}

void dynamic_macro_play(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        int ind_start = 0;
        while (ind_start < 20) {
            for (int i = ind_start; i < 20; i++) {
                register_code(dynamic_macro_buffer[i]);
                if (dynamic_macro_buffer[i] == 0) {
                    break;
                }
            }
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            for (int i = ind_start; i < 20; i++) {
                    unregister_code(dynamic_macro_buffer[i]);
                if (dynamic_macro_buffer[i] == 0) {
                    ind_start = i + 1;
                    break;
                }
            }
            send_keyboard_report();
        }
    }
}

void clear(const struct Chord* self);

void reset_keyboard_kb(void){
#ifdef WATCHDOG_ENABLE
    MCUSR = 0;
    wdt_disable();
    wdt_reset();
#endif
    reset_keyboard();
}

void reset(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        reset_keyboard_kb();
    }
}

// Macros to simplify chord definitions

// Add all chords

    
    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
    
    uint8_t state_0 = IDLE;
    const struct Chord chord_0 PROGMEM = {H_TOP6 + H_BOT6 + H_TOP7 + H_BOT7, ALWAYS_ON, &state_0, NULL, 0, 0, command};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
    
        
    
    
        
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
        
    

    
        
        
        
            
                
    
    uint8_t state_1 = IDLE;
    const struct Chord chord_1 PROGMEM = {H_TOP1 + H_TOP2 + H_TOP11 + H_TOP12 + H_BOT1 + H_BOT2 + H_BOT11 + H_BOT12, ALWAYS_ON, &state_1, NULL, 0, 0, clear};
    

            
        
    

    

    
    
    

    
        
        
        
            
                
                
    
    uint8_t state_2 = IDLE;
    const struct Chord chord_2 PROGMEM = {H_TOP1, QWERTY, &state_2, NULL, KC_TAB, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_3 = IDLE;
    const struct Chord chord_3 PROGMEM = {H_TOP2, QWERTY, &state_3, NULL, KC_Q, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_4 = IDLE;
    const struct Chord chord_4 PROGMEM = {H_TOP3, QWERTY, &state_4, NULL, KC_W, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_5 = IDLE;
    const struct Chord chord_5 PROGMEM = {H_TOP4, QWERTY, &state_5, NULL, KC_F, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_6 = IDLE;
    const struct Chord chord_6 PROGMEM = {H_TOP5, QWERTY, &state_6, NULL, KC_P, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_7 = IDLE;
    const struct Chord chord_7 PROGMEM = {H_TOP6, QWERTY, &state_7, NULL, KC_G, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_8 = IDLE;
    const struct Chord chord_8 PROGMEM = {H_TOP7, QWERTY, &state_8, NULL, KC_J, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_9 = IDLE;
    const struct Chord chord_9 PROGMEM = {H_TOP8, QWERTY, &state_9, NULL, KC_L, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_10 = IDLE;
    const struct Chord chord_10 PROGMEM = {H_TOP9, QWERTY, &state_10, NULL, KC_U, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_11 = IDLE;
    const struct Chord chord_11 PROGMEM = {H_TOP10, QWERTY, &state_11, NULL, KC_Y, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_12 = IDLE;
    const struct Chord chord_12 PROGMEM = {H_TOP11, QWERTY, &state_12, NULL, KC_SCOLON, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_13 = IDLE;
    const struct Chord chord_13 PROGMEM = {H_TOP12, QWERTY, &state_13, NULL, KC_RCTL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_14 = IDLE;
    const struct Chord chord_14 PROGMEM = {H_TOP1 + H_BOT1, QWERTY, &state_14, NULL, KC_ESC, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_15 = IDLE;
    const struct Chord chord_15 PROGMEM = {H_TOP2 + H_BOT2, QWERTY, &state_15, NULL, KC_A, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_16 = IDLE;
    const struct Chord chord_16 PROGMEM = {H_TOP3 + H_BOT3, QWERTY, &state_16, NULL, KC_R, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_17 = IDLE;
    const struct Chord chord_17 PROGMEM = {H_TOP4 + H_BOT4, QWERTY, &state_17, NULL, KC_S, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_18 = IDLE;
    const struct Chord chord_18 PROGMEM = {H_TOP5 + H_BOT5, QWERTY, &state_18, NULL, KC_T, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_19 = IDLE;
    const struct Chord chord_19 PROGMEM = {H_TOP6 + H_BOT6, QWERTY, &state_19, NULL, KC_D, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_20 = IDLE;
    const struct Chord chord_20 PROGMEM = {H_TOP7 + H_BOT7, QWERTY, &state_20, NULL, KC_H, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_21 = IDLE;
    const struct Chord chord_21 PROGMEM = {H_TOP8 + H_BOT8, QWERTY, &state_21, NULL, KC_N, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_22 = IDLE;
    const struct Chord chord_22 PROGMEM = {H_TOP9 + H_BOT9, QWERTY, &state_22, NULL, KC_E, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_23 = IDLE;
    const struct Chord chord_23 PROGMEM = {H_TOP10 + H_BOT10, QWERTY, &state_23, NULL, KC_I, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_24 = IDLE;
    const struct Chord chord_24 PROGMEM = {H_TOP11 + H_BOT11, QWERTY, &state_24, NULL, KC_O, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_25 = IDLE;
    const struct Chord chord_25 PROGMEM = {H_TOP12 + H_BOT12, QWERTY, &state_25, NULL, KC_RGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_26 = IDLE;
    const struct Chord chord_26 PROGMEM = {H_BOT1, QWERTY, &state_26, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_27 = IDLE;
    const struct Chord chord_27 PROGMEM = {H_BOT2, QWERTY, &state_27, NULL, KC_Z, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_28 = IDLE;
    const struct Chord chord_28 PROGMEM = {H_BOT3, QWERTY, &state_28, NULL, KC_X, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_29 = IDLE;
    const struct Chord chord_29 PROGMEM = {H_BOT4, QWERTY, &state_29, NULL, KC_C, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_30 = IDLE;
    const struct Chord chord_30 PROGMEM = {H_BOT5, QWERTY, &state_30, NULL, KC_V, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_31 = IDLE;
    const struct Chord chord_31 PROGMEM = {H_BOT6, QWERTY, &state_31, NULL, KC_B, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_32 = IDLE;
    const struct Chord chord_32 PROGMEM = {H_BOT7, QWERTY, &state_32, NULL, KC_K, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_33 = IDLE;
    const struct Chord chord_33 PROGMEM = {H_BOT8, QWERTY, &state_33, NULL, KC_M, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_34 = IDLE;
    const struct Chord chord_34 PROGMEM = {H_BOT9, QWERTY, &state_34, NULL, KC_COMMA, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_35 = IDLE;
    const struct Chord chord_35 PROGMEM = {H_BOT10, QWERTY, &state_35, NULL, KC_DOT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_36 = IDLE;
    const struct Chord chord_36 PROGMEM = {H_BOT11, QWERTY, &state_36, NULL, KC_SLASH, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_37 = IDLE;
    const struct Chord chord_37 PROGMEM = {H_BOT12, QWERTY, &state_37, NULL, KC_DEL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_38 = IDLE;
    const struct Chord chord_38 PROGMEM = {H_THU1, QWERTY, &state_38, NULL, KC_BSPC, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_39 = IDLE;
    const struct Chord chord_39 PROGMEM = {H_THU2, QWERTY, &state_39, NULL, KC_SPACE, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_40 = IDLE;
    const struct Chord chord_40 PROGMEM = {H_THU3, QWERTY, &state_40, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_41 = IDLE;
    const struct Chord chord_41 PROGMEM = {H_THU4, QWERTY, &state_41, NULL, KC_ENTER, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_42 = IDLE;
    const struct Chord chord_42 PROGMEM = {H_THU5, QWERTY, &state_42, NULL, KC_SPACE, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_43 = IDLE;
    const struct Chord chord_43 PROGMEM = {H_THU6, QWERTY, &state_43, NULL, KC_BSPC, 0, single_dance};
    

            
        
    

// Register all chords, load chording logic
const struct Chord* const list_of_chords[] PROGMEM = {
    
        &chord_0,
    
        &chord_1,
    
        &chord_2,
    
        &chord_3,
    
        &chord_4,
    
        &chord_5,
    
        &chord_6,
    
        &chord_7,
    
        &chord_8,
    
        &chord_9,
    
        &chord_10,
    
        &chord_11,
    
        &chord_12,
    
        &chord_13,
    
        &chord_14,
    
        &chord_15,
    
        &chord_16,
    
        &chord_17,
    
        &chord_18,
    
        &chord_19,
    
        &chord_20,
    
        &chord_21,
    
        &chord_22,
    
        &chord_23,
    
        &chord_24,
    
        &chord_25,
    
        &chord_26,
    
        &chord_27,
    
        &chord_28,
    
        &chord_29,
    
        &chord_30,
    
        &chord_31,
    
        &chord_32,
    
        &chord_33,
    
        &chord_34,
    
        &chord_35,
    
        &chord_36,
    
        &chord_37,
    
        &chord_38,
    
        &chord_39,
    
        &chord_40,
    
        &chord_41,
    
        &chord_42,
    
        &chord_43,
    
};

const uint16_t leader_triggers[0][5] PROGMEM = {
    
};

void (*leader_functions[]) (void) = {
    
};

struct Chord* last_chord = NULL;

void sound_keycode(uint16_t keycode, uint32_t* sound) {
    uint32_t hash = (uint32_t)1 << (keycode - SAFE_RANGE);
    *sound |= hash;
}
void silence_keycode(uint16_t keycode, uint32_t* sound) {
    uint32_t hash = (uint32_t)1 << (keycode - SAFE_RANGE);
    *sound ^= hash;
}
void silence_hash(uint32_t hash, uint32_t* sound) {
    *sound ^= hash;
}
bool are_hashed_keycodes_in_sound(uint32_t keycodes_hash, uint32_t sound) {
    return (keycodes_hash & sound) == keycodes_hash;
}

void kill_one_shots(struct Chord* caller) {
    if (caller->function == one_shot_key) {
        return;
    }
    
    for (int i = 0; i < 44; i++) {
        // const struct Chord* chord = list_of_chords[i];
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (chord == caller) {
            continue;
        }
        
        if (*chord->state == IN_ONE_SHOT) {
            *chord->state = RESTART;
            chord->function(chord);
            *chord->state = IDLE;
        }
    }
}

void process_finished_dances(void) {
    for (int i = 0; i < 44; i++) {
        // const struct Chord* chord = list_of_chords[i];
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (*chord->state == ACTIVATED) {
            *chord->state = FINISHED_FROM_ACTIVE;
            chord->function(chord);
        }
        
        if (*chord->state == IDLE_IN_DANCE) {
            *chord->state = FINISHED;
            chord->function(chord);
            *chord->state = RESTART;
            chord->function(chord);
            *chord->state = IDLE;
            kill_one_shots(chord);
        }
    }
}

void process_ready_chords(void) {
    // find ready chords
    for (int i = 0; i < 44; i++) {
        // const struct Chord* chord = list_of_chords[i];
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (!are_hashed_keycodes_in_sound(chord->keycodes_hash, keycodes_buffer)) {
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
    for (int i = 0; i < 44; i++) {
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {
            continue;
        }
        
        for (int j = 0; j < 44; j++) {
            if (i == j) {continue;}
            
            struct Chord* chord_ptr_2 = (struct Chord*) pgm_read_word (&list_of_chords[j]);
            struct Chord chord_storage_2;
            memcpy_P(&chord_storage_2, chord_ptr_2, sizeof(struct Chord));
            struct Chord* chord_2 = &chord_storage_2;
            
            if (are_hashed_keycodes_in_sound(chord_2->keycodes_hash, chord->keycodes_hash)) {
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
    
    // execute logic
    for (int i = 0; i < 44; i++) {
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (*chord->state == READY_LOCKED) {
            *chord->state = RESTART;
            silence_hash(chord->keycodes_hash, &keycodes_buffer);
            chord->function(chord);
            *chord->state = IDLE;
            kill_one_shots(chord);
        }
        
        if (*chord->state == READY || *chord->state == READY_IN_DANCE) {
            if (last_chord && last_chord != chord) {
                process_finished_dances();
            }
            
            bool lock_next_prev_state = lock_next;
            
            *chord->state = ACTIVATED;
            silence_hash(chord->keycodes_hash, &keycodes_buffer);
            chord->function(chord);
            dance_timer = timer_read();
            
            if (lock_next && lock_next == lock_next_prev_state) {
                lock_next = false;
                *chord->state = FINISHED_FROM_ACTIVE;
                chord->function(chord);
                *chord->state = LOCKED;
            }
        }
    }
}

void deactivate_active_chords(uint16_t keycode) {
    uint32_t hash = (uint32_t)1 << (keycode - SAFE_RANGE);
    bool broken;
    for (int i = 0; i < 44; i++) {
        // const struct Chord* chord = list_of_chords[i];
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        broken = are_hashed_keycodes_in_sound(hash, chord->keycodes_hash);
        if (!broken) {
            continue;
        }
        
        switch (*chord->state) {
            case ACTIVATED:
                *chord->state = DEACTIVATED;
                chord->function(chord);
                
                if (chord->function == one_shot_key || chord->function == one_shot_layer) {
                    *chord->state = IN_ONE_SHOT;
                } else {
                    dance_timer = timer_read();
                    *chord->state = IDLE_IN_DANCE;
                    kill_one_shots(chord);
                }
                break;
            case FINISHED_FROM_ACTIVE:
                *chord->state = RESTART;
                chord->function(chord);
                *chord->state = IDLE;
                kill_one_shots(chord);
                break;
            default:
                break;
        }
    }
    
}

void process_command(void) {
    command_mode = 0;
    for (int i = 0; i < 5; i++) {
        if (command_buffer[i]) {
            register_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    wait_ms(TAP_TIMEOUT);
    for (int i = 0; i < 5; i++) {
        if (command_buffer[i]) {
            unregister_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    for (int i = 0; i < 5; i++) {
        command_buffer[i] = 0;
    }
    command_ind = 0;
}

void process_leader(void) {
    // struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
    // struct Chord chord_storage;
    // memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
    // struct Chord* chord = &chord_storage;
    
    in_leader_mode = false;
    for (int i = 0; i < 0; i++) {
        uint16_t trigger[5];
        memcpy_P(trigger, leader_triggers[i], 5 * sizeof(uint16_t));
        
        if (identical(leader_buffer, trigger)) {
            (*leader_functions[i])();
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        leader_buffer[i] = 0;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        sound_keycode(keycode, &keycodes_buffer);
    } else {
        process_ready_chords();
        deactivate_active_chords(keycode);
    }
    chord_timer = timer_read();
    leader_timer = timer_read();
    
    return false;
}

void matrix_scan_user(void) {
    bool chord_timer_expired = timer_elapsed(chord_timer) > CHORD_TIMEOUT;
    bool dance_timer_expired = timer_elapsed(dance_timer) > DANCE_TIMEOUT;
    bool leader_timer_expired = timer_elapsed(leader_timer) > LEADER_TIMEOUT;
    if (chord_timer_expired && keycodes_buffer) {
        process_ready_chords();
    }
    if (dance_timer_expired) { // would love to have && in_dance but not sure how
        process_finished_dances();
    }
    if (command_mode == 2) {
        process_command();
    }
    if (leader_timer_expired && in_leader_mode) {
        process_leader();
    }
    
}

// for now here
void clear(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        // kill all chords
        for (int i = 0; i < 44; i++) {
            // const struct Chord* chord = list_of_chords[i];
            struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            struct Chord chord_storage;
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            struct Chord* chord = &chord_storage;
            
            *chord->state = IDLE;
        }
        
        // clear keyboard
        clear_keyboard();
        send_keyboard_report();
        
        // switch to default pseudolayer
        current_pseudolayer = 1;
        
        // clear all keyboard states
        lock_next = false;
    }
}
