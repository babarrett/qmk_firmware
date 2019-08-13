#include QMK_KEYBOARD_H

// use pyexpander to generate keymap.c:
// python3 expander3.py -f keymap.c.in | cat -s > keymap.c
// the preprocessor code is written to be readable not to produce nice output

enum pseudolayers {
    ALWAYS_ON, QWERTY, NUM, SYM, MOVE
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
    const struct Chord chord_0 PROGMEM = {H_TOP6 + H_BOT6 + H_TOP7 + H_BOT7, ALWAYS_ON, &state_0, NULL, 0, 0, leader};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
    
        
    
    
        
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
        
    

    
        
        
        
            
                
    
    uint8_t state_1 = IDLE;
    const struct Chord chord_1 PROGMEM = {H_TOP1 + H_TOP2 + H_TOP11 + H_TOP12 + H_BOT1 + H_BOT2 + H_BOT11 + H_BOT12, ALWAYS_ON, &state_1, NULL, 0, 0, clear};
    

            
        
    

    

    
    
    
        
    
    
    
    
    
    
    
    
    
    
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
    
    
    
        
    
    
    
    
    
    
    
    
    
        
    

    
        
        
        
            
                
    
    uint8_t state_2 = IDLE;
    const struct Chord chord_2 PROGMEM = {H_TOP1 + H_TOP12 + H_BOT1 + H_BOT12, ALWAYS_ON, &state_2, NULL, 0, 0, command};
    

            
        
    

    

    
    
    
        
    
    
        
    
    
        
    
    
    
    
    
    
    
    
        
    
    
        
    
    
        
    
    
        
    
    
        
    
    
        
    
    
    
    
    
    
    
    
        
    
    
        
    
    
        
    
    
    
    
    
    
    
    
    
        
    

    
        
        
        
            
                
    
    uint8_t state_3 = IDLE;
    const struct Chord chord_3 PROGMEM = {H_TOP1 + H_TOP2 + H_TOP3 + H_TOP10 + H_TOP11 + H_TOP12 + H_BOT1 + H_BOT2 + H_BOT3 + H_BOT10 + H_BOT11 + H_BOT12, ALWAYS_ON, &state_3, NULL, 0, 0, reset};
    

            
        
    

    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        
    
    
    
    
        
    
    
    
    
        
    

    
        
        
        
            
                
                
    
    uint8_t state_4 = IDLE;
    const struct Chord chord_4 PROGMEM = {H_THU2 + H_THU5, ALWAYS_ON, &state_4, NULL, KC_ESC, 0, single_dance};
    

            
        
    

    

    
    
    

    
        
        
        
            
                
                
    
    uint8_t state_5 = IDLE;
    const struct Chord chord_5 PROGMEM = {H_TOP1, QWERTY, &state_5, NULL, KC_TAB, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_6 = IDLE;
    const struct Chord chord_6 PROGMEM = {H_TOP2, QWERTY, &state_6, NULL, KC_Q, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_7 = IDLE;
    const struct Chord chord_7 PROGMEM = {H_TOP3, QWERTY, &state_7, NULL, KC_W, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_8 = IDLE;
    const struct Chord chord_8 PROGMEM = {H_TOP4, QWERTY, &state_8, NULL, KC_F, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_9 = IDLE;
    const struct Chord chord_9 PROGMEM = {H_TOP5, QWERTY, &state_9, NULL, KC_P, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_10 = IDLE;
    const struct Chord chord_10 PROGMEM = {H_TOP6, QWERTY, &state_10, NULL, KC_G, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_11 = IDLE;
    const struct Chord chord_11 PROGMEM = {H_TOP7, QWERTY, &state_11, NULL, KC_J, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_12 = IDLE;
    const struct Chord chord_12 PROGMEM = {H_TOP8, QWERTY, &state_12, NULL, KC_L, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_13 = IDLE;
    const struct Chord chord_13 PROGMEM = {H_TOP9, QWERTY, &state_13, NULL, KC_U, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_14 = IDLE;
    const struct Chord chord_14 PROGMEM = {H_TOP10, QWERTY, &state_14, NULL, KC_Y, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_15 = IDLE;
    const struct Chord chord_15 PROGMEM = {H_TOP11, QWERTY, &state_15, NULL, KC_SCOLON, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_16 = IDLE;
    const struct Chord chord_16 PROGMEM = {H_TOP12, QWERTY, &state_16, NULL, KC_RCTL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_17 = IDLE;
    const struct Chord chord_17 PROGMEM = {H_TOP1 + H_BOT1, QWERTY, &state_17, NULL, KC_ESC, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_18 = IDLE;
    const struct Chord chord_18 PROGMEM = {H_TOP2 + H_BOT2, QWERTY, &state_18, NULL, KC_A, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_19 = IDLE;
    const struct Chord chord_19 PROGMEM = {H_TOP3 + H_BOT3, QWERTY, &state_19, NULL, KC_R, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_20 = IDLE;
    const struct Chord chord_20 PROGMEM = {H_TOP4 + H_BOT4, QWERTY, &state_20, NULL, KC_S, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_21 = IDLE;
    const struct Chord chord_21 PROGMEM = {H_TOP5 + H_BOT5, QWERTY, &state_21, NULL, KC_T, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_22 = IDLE;
    const struct Chord chord_22 PROGMEM = {H_TOP6 + H_BOT6, QWERTY, &state_22, NULL, KC_D, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_23 = IDLE;
    const struct Chord chord_23 PROGMEM = {H_TOP7 + H_BOT7, QWERTY, &state_23, NULL, KC_H, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_24 = IDLE;
    const struct Chord chord_24 PROGMEM = {H_TOP8 + H_BOT8, QWERTY, &state_24, NULL, KC_N, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_25 = IDLE;
    const struct Chord chord_25 PROGMEM = {H_TOP9 + H_BOT9, QWERTY, &state_25, NULL, KC_E, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_26 = IDLE;
    const struct Chord chord_26 PROGMEM = {H_TOP10 + H_BOT10, QWERTY, &state_26, NULL, KC_I, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_27 = IDLE;
    const struct Chord chord_27 PROGMEM = {H_TOP11 + H_BOT11, QWERTY, &state_27, NULL, KC_O, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_28 = IDLE;
    const struct Chord chord_28 PROGMEM = {H_TOP12 + H_BOT12, QWERTY, &state_28, NULL, KC_RGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_29 = IDLE;
    const struct Chord chord_29 PROGMEM = {H_BOT1, QWERTY, &state_29, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_30 = IDLE;
    const struct Chord chord_30 PROGMEM = {H_BOT2, QWERTY, &state_30, NULL, KC_Z, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_31 = IDLE;
    const struct Chord chord_31 PROGMEM = {H_BOT3, QWERTY, &state_31, NULL, KC_X, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_32 = IDLE;
    const struct Chord chord_32 PROGMEM = {H_BOT4, QWERTY, &state_32, NULL, KC_C, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_33 = IDLE;
    const struct Chord chord_33 PROGMEM = {H_BOT5, QWERTY, &state_33, NULL, KC_V, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_34 = IDLE;
    const struct Chord chord_34 PROGMEM = {H_BOT6, QWERTY, &state_34, NULL, KC_B, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_35 = IDLE;
    const struct Chord chord_35 PROGMEM = {H_BOT7, QWERTY, &state_35, NULL, KC_K, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_36 = IDLE;
    const struct Chord chord_36 PROGMEM = {H_BOT8, QWERTY, &state_36, NULL, KC_M, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_37 = IDLE;
    const struct Chord chord_37 PROGMEM = {H_BOT9, QWERTY, &state_37, NULL, KC_COMMA, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_38 = IDLE;
    const struct Chord chord_38 PROGMEM = {H_BOT10, QWERTY, &state_38, NULL, KC_DOT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_39 = IDLE;
    const struct Chord chord_39 PROGMEM = {H_BOT11, QWERTY, &state_39, NULL, KC_SLASH, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_40 = IDLE;
    const struct Chord chord_40 PROGMEM = {H_BOT12, QWERTY, &state_40, NULL, KC_DEL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
            
                
                
                
                
    
    uint8_t state_41 = IDLE;
    const struct Chord chord_41 PROGMEM = {H_THU1, QWERTY, &state_41, NULL, KC_BSPC, SYM, key_layer_dance};
    

            
        
    

    
    

    
        
        
        
            
            
                
                
                
                
    
    uint8_t state_42 = IDLE;
    const struct Chord chord_42 PROGMEM = {H_THU2, QWERTY, &state_42, NULL, KC_SPC, NUM, key_layer_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_43 = IDLE;
    const struct Chord chord_43 PROGMEM = {H_THU3, QWERTY, &state_43, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
            
                
                
                
                
                
    
    uint8_t state_44 = IDLE;
    const struct Chord chord_44 PROGMEM = {H_THU4, QWERTY, &state_44, NULL, KC_ENTER, KC_RSFT, key_key_dance};
    

            
        
    

    
    

    
        
        
        
            
            
                
                
                
                
    
    uint8_t state_45 = IDLE;
    const struct Chord chord_45 PROGMEM = {H_THU5, QWERTY, &state_45, NULL, KC_SPC, NUM, key_layer_dance};
    

            
        
    

    
    

    
        
        
        
            
            
                
                
                
                
    
    uint8_t state_46 = IDLE;
    const struct Chord chord_46 PROGMEM = {H_THU6, QWERTY, &state_46, NULL, KC_BSPC, SYM, key_layer_dance};
    

            
        
    

    
    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_47 = IDLE;
    const struct Chord chord_47 PROGMEM = {H_BOT1 + H_BOT2, QWERTY, &state_47, NULL, KC_INS, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_48 = IDLE;
    const struct Chord chord_48 PROGMEM = {H_BOT3 + H_BOT4, QWERTY, &state_48, NULL, KC_LALT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_49 = IDLE;
    const struct Chord chord_49 PROGMEM = {H_BOT4 + H_BOT5, QWERTY, &state_49, NULL, KC_LGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_50 = IDLE;
    const struct Chord chord_50 PROGMEM = {H_BOT5 + H_BOT6, QWERTY, &state_50, NULL, KC_LCTL, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_51 = IDLE;
    const struct Chord chord_51 PROGMEM = {H_BOT7 + H_BOT8, QWERTY, &state_51, NULL, KC_RCTL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_52 = IDLE;
    const struct Chord chord_52 PROGMEM = {H_BOT8 + H_BOT9, QWERTY, &state_52, NULL, KC_RGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_53 = IDLE;
    const struct Chord chord_53 PROGMEM = {H_BOT9 + H_BOT10, QWERTY, &state_53, NULL, KC_RALT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
            
                
    
    uint8_t state_54 = IDLE;
    const struct Chord chord_54 PROGMEM = {H_THU2 + H_THU3, QWERTY, &state_54, NULL, MOVE, 0, temp_pseudolayer};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_55 = IDLE;
    const struct Chord chord_55 PROGMEM = {H_THU4 + H_THU5, QWERTY, &state_55, NULL, KC_TAB, 0, single_dance};
    

            
        
    

    
    

    
    

    
    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_56 = IDLE;
    const struct Chord chord_56 PROGMEM = {H_TOP2, NUM, &state_56, NULL, KC_1, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_57 = IDLE;
    const struct Chord chord_57 PROGMEM = {H_TOP3, NUM, &state_57, NULL, KC_2, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_58 = IDLE;
    const struct Chord chord_58 PROGMEM = {H_TOP4, NUM, &state_58, NULL, KC_3, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_59 = IDLE;
    const struct Chord chord_59 PROGMEM = {H_TOP5, NUM, &state_59, NULL, KC_4, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_60 = IDLE;
    const struct Chord chord_60 PROGMEM = {H_TOP6, NUM, &state_60, NULL, KC_5, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_61 = IDLE;
    const struct Chord chord_61 PROGMEM = {H_TOP7, NUM, &state_61, NULL, KC_6, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_62 = IDLE;
    const struct Chord chord_62 PROGMEM = {H_TOP8, NUM, &state_62, NULL, KC_7, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_63 = IDLE;
    const struct Chord chord_63 PROGMEM = {H_TOP9, NUM, &state_63, NULL, KC_8, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_64 = IDLE;
    const struct Chord chord_64 PROGMEM = {H_TOP10, NUM, &state_64, NULL, KC_9, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_65 = IDLE;
    const struct Chord chord_65 PROGMEM = {H_TOP11, NUM, &state_65, NULL, KC_0, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_66 = IDLE;
    const struct Chord chord_66 PROGMEM = {H_BOT1, NUM, &state_66, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_67 = IDLE;
    const struct Chord chord_67 PROGMEM = {H_BOT2, NUM, &state_67, NULL, KC_F1, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_68 = IDLE;
    const struct Chord chord_68 PROGMEM = {H_BOT3, NUM, &state_68, NULL, KC_F2, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_69 = IDLE;
    const struct Chord chord_69 PROGMEM = {H_BOT4, NUM, &state_69, NULL, KC_F3, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_70 = IDLE;
    const struct Chord chord_70 PROGMEM = {H_BOT5, NUM, &state_70, NULL, KC_F4, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_71 = IDLE;
    const struct Chord chord_71 PROGMEM = {H_BOT6, NUM, &state_71, NULL, KC_F5, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_72 = IDLE;
    const struct Chord chord_72 PROGMEM = {H_BOT7, NUM, &state_72, NULL, KC_F6, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_73 = IDLE;
    const struct Chord chord_73 PROGMEM = {H_BOT8, NUM, &state_73, NULL, KC_F7, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_74 = IDLE;
    const struct Chord chord_74 PROGMEM = {H_BOT9, NUM, &state_74, NULL, KC_F8, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_75 = IDLE;
    const struct Chord chord_75 PROGMEM = {H_BOT10, NUM, &state_75, NULL, KC_F9, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_76 = IDLE;
    const struct Chord chord_76 PROGMEM = {H_BOT11, NUM, &state_76, NULL, KC_F10, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_77 = IDLE;
    const struct Chord chord_77 PROGMEM = {H_BOT12, NUM, &state_77, NULL, KC_RSFT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_78 = IDLE;
    const struct Chord chord_78 PROGMEM = {H_BOT2 + H_BOT3, NUM, &state_78, NULL, KC_F11, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_79 = IDLE;
    const struct Chord chord_79 PROGMEM = {H_BOT3 + H_BOT4, NUM, &state_79, NULL, KC_LALT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_80 = IDLE;
    const struct Chord chord_80 PROGMEM = {H_BOT4 + H_BOT5, NUM, &state_80, NULL, KC_LGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_81 = IDLE;
    const struct Chord chord_81 PROGMEM = {H_BOT5 + H_BOT6, NUM, &state_81, NULL, KC_LCTL, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_82 = IDLE;
    const struct Chord chord_82 PROGMEM = {H_BOT7 + H_BOT8, NUM, &state_82, NULL, KC_RCTL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_83 = IDLE;
    const struct Chord chord_83 PROGMEM = {H_BOT8 + H_BOT9, NUM, &state_83, NULL, KC_RGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_84 = IDLE;
    const struct Chord chord_84 PROGMEM = {H_BOT9 + H_BOT10, NUM, &state_84, NULL, KC_RALT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_85 = IDLE;
    const struct Chord chord_85 PROGMEM = {H_BOT10 + H_BOT11, NUM, &state_85, NULL, KC_F12, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_86 = IDLE;
    const struct Chord chord_86 PROGMEM = {H_TOP2, SYM, &state_86, NULL, KC_GRV, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_87 = IDLE;
    const struct Chord chord_87 PROGMEM = {H_TOP3, SYM, &state_87, NULL, KC_LBRC, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_88 = IDLE;
    const struct Chord chord_88 PROGMEM = {H_TOP10, SYM, &state_88, NULL, KC_RBRC, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_89 = IDLE;
    const struct Chord chord_89 PROGMEM = {H_TOP2 + H_BOT2, SYM, &state_89, NULL, KC_TILD, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_90 = IDLE;
    const struct Chord chord_90 PROGMEM = {H_TOP3 + H_BOT3, SYM, &state_90, NULL, KC_MINS, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_91 = IDLE;
    const struct Chord chord_91 PROGMEM = {H_TOP4 + H_BOT4, SYM, &state_91, NULL, KC_QUOT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_92 = IDLE;
    const struct Chord chord_92 PROGMEM = {H_TOP7 + H_BOT7, SYM, &state_92, NULL, KC_BSLS, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_93 = IDLE;
    const struct Chord chord_93 PROGMEM = {H_TOP8 + H_BOT8, SYM, &state_93, NULL, KC_EQL, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_94 = IDLE;
    const struct Chord chord_94 PROGMEM = {H_BOT2, SYM, &state_94, NULL, KC_EXLM, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_95 = IDLE;
    const struct Chord chord_95 PROGMEM = {H_BOT3 + H_BOT4, SYM, &state_95, NULL, KC_LALT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_96 = IDLE;
    const struct Chord chord_96 PROGMEM = {H_BOT4 + H_BOT5, SYM, &state_96, NULL, KC_LGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_97 = IDLE;
    const struct Chord chord_97 PROGMEM = {H_BOT5 + H_BOT6, SYM, &state_97, NULL, KC_LCTL, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_98 = IDLE;
    const struct Chord chord_98 PROGMEM = {H_BOT7 + H_BOT8, SYM, &state_98, NULL, KC_RCTL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_99 = IDLE;
    const struct Chord chord_99 PROGMEM = {H_BOT8 + H_BOT9, SYM, &state_99, NULL, KC_RGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_100 = IDLE;
    const struct Chord chord_100 PROGMEM = {H_BOT9 + H_BOT10, SYM, &state_100, NULL, KC_RALT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_101 = IDLE;
    const struct Chord chord_101 PROGMEM = {H_TOP3, MOVE, &state_101, NULL, KC_HOME, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_102 = IDLE;
    const struct Chord chord_102 PROGMEM = {H_TOP4, MOVE, &state_102, NULL, KC_UP, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_103 = IDLE;
    const struct Chord chord_103 PROGMEM = {H_TOP5, MOVE, &state_103, NULL, KC_PGUP, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_104 = IDLE;
    const struct Chord chord_104 PROGMEM = {H_TOP1 + H_BOT1, MOVE, &state_104, NULL, KC_INS, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_105 = IDLE;
    const struct Chord chord_105 PROGMEM = {H_TOP3 + H_BOT3, MOVE, &state_105, NULL, KC_END, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_106 = IDLE;
    const struct Chord chord_106 PROGMEM = {H_TOP5 + H_BOT5, MOVE, &state_106, NULL, KC_PGDN, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_107 = IDLE;
    const struct Chord chord_107 PROGMEM = {H_BOT1, MOVE, &state_107, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_108 = IDLE;
    const struct Chord chord_108 PROGMEM = {H_BOT3, MOVE, &state_108, NULL, KC_LEFT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_109 = IDLE;
    const struct Chord chord_109 PROGMEM = {H_BOT4, MOVE, &state_109, NULL, KC_DOWN, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_110 = IDLE;
    const struct Chord chord_110 PROGMEM = {H_BOT5, MOVE, &state_110, NULL, KC_RIGHT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_111 = IDLE;
    const struct Chord chord_111 PROGMEM = {H_BOT12, MOVE, &state_111, NULL, KC_RSFT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_112 = IDLE;
    const struct Chord chord_112 PROGMEM = {H_BOT3 + H_BOT4, MOVE, &state_112, NULL, KC_LALT, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_113 = IDLE;
    const struct Chord chord_113 PROGMEM = {H_BOT4 + H_BOT5, MOVE, &state_113, NULL, KC_LGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_114 = IDLE;
    const struct Chord chord_114 PROGMEM = {H_BOT5 + H_BOT6, MOVE, &state_114, NULL, KC_LCTL, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_115 = IDLE;
    const struct Chord chord_115 PROGMEM = {H_BOT7 + H_BOT8, MOVE, &state_115, NULL, KC_RCTL, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_116 = IDLE;
    const struct Chord chord_116 PROGMEM = {H_BOT8 + H_BOT9, MOVE, &state_116, NULL, KC_RGUI, 0, single_dance};
    

            
        
    

    
    

    
        
        
        
            
                
                
    
    uint8_t state_117 = IDLE;
    const struct Chord chord_117 PROGMEM = {H_BOT9 + H_BOT10, MOVE, &state_117, NULL, KC_RALT, 0, single_dance};
    

            
        
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

    
    

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
    
        &chord_44,
    
        &chord_45,
    
        &chord_46,
    
        &chord_47,
    
        &chord_48,
    
        &chord_49,
    
        &chord_50,
    
        &chord_51,
    
        &chord_52,
    
        &chord_53,
    
        &chord_54,
    
        &chord_55,
    
        &chord_56,
    
        &chord_57,
    
        &chord_58,
    
        &chord_59,
    
        &chord_60,
    
        &chord_61,
    
        &chord_62,
    
        &chord_63,
    
        &chord_64,
    
        &chord_65,
    
        &chord_66,
    
        &chord_67,
    
        &chord_68,
    
        &chord_69,
    
        &chord_70,
    
        &chord_71,
    
        &chord_72,
    
        &chord_73,
    
        &chord_74,
    
        &chord_75,
    
        &chord_76,
    
        &chord_77,
    
        &chord_78,
    
        &chord_79,
    
        &chord_80,
    
        &chord_81,
    
        &chord_82,
    
        &chord_83,
    
        &chord_84,
    
        &chord_85,
    
        &chord_86,
    
        &chord_87,
    
        &chord_88,
    
        &chord_89,
    
        &chord_90,
    
        &chord_91,
    
        &chord_92,
    
        &chord_93,
    
        &chord_94,
    
        &chord_95,
    
        &chord_96,
    
        &chord_97,
    
        &chord_98,
    
        &chord_99,
    
        &chord_100,
    
        &chord_101,
    
        &chord_102,
    
        &chord_103,
    
        &chord_104,
    
        &chord_105,
    
        &chord_106,
    
        &chord_107,
    
        &chord_108,
    
        &chord_109,
    
        &chord_110,
    
        &chord_111,
    
        &chord_112,
    
        &chord_113,
    
        &chord_114,
    
        &chord_115,
    
        &chord_116,
    
        &chord_117,
    
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
    
    for (int i = 0; i < 118; i++) {
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
    for (int i = 0; i < 118; i++) {
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
    for (int i = 0; i < 118; i++) {
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
    for (int i = 0; i < 118; i++) {
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {
            continue;
        }
        
        for (int j = 0; j < 118; j++) {
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
    for (int i = 0; i < 118; i++) {
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
    for (int i = 0; i < 118; i++) {
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
        for (int i = 0; i < 118; i++) {
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
