#include QMK_KEYBOARD_H

#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 200
#define LEADER_TIMEOUT 750
#define TAP_TIMEOUT 50
#define LONG_PRESS_MULTIPLIER 3
#define DYNAMIC_MACRO_MAX_LENGTH 20
#define COMMAND_MAX_LENGTH 5
#define LEADER_MAX_LENGTH 5
#define HASH_TYPE uint16_t
#define NUMBER_OF_KEYS 10
#define DEFAULT_PSEUDOLAYER BASE

#define H_L1 ((HASH_TYPE) 1 << 0)
#define H_L2 ((HASH_TYPE) 1 << 1)
#define H_L3 ((HASH_TYPE) 1 << 2)
#define H_L4 ((HASH_TYPE) 1 << 3)
#define H_R1 ((HASH_TYPE) 1 << 4)
#define H_R2 ((HASH_TYPE) 1 << 5)
#define H_R3 ((HASH_TYPE) 1 << 6)
#define H_R4 ((HASH_TYPE) 1 << 7)
#define H_LT ((HASH_TYPE) 1 << 8)
#define H_RT ((HASH_TYPE) 1 << 9)

enum internal_keycodes {
    L1 = SAFE_RANGE,
    L2, L3, L4, R1, R2, R3, R4, LT, RT,
    FIRST_INTERNAL_KEYCODE = L1,
    LAST_INTERNAL_KEYCODE = RT
};

enum pseudolayers {
    ALWAYS_ON, BASE, NUM, COMMANDS, USER
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ginny(L1, L2, L3, L4, R1, R2, R3, R4, LT, RT),
};
size_t keymapsCount = 1;

uint8_t keycodes_buffer_array[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t command_buffer[] = {
    0, 0, 0, 0, 0
};

uint16_t leader_buffer[] = {
    0, 0, 0, 0, 0
};

uint8_t dynamic_macro_buffer[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

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
    uint8_t* state;
    uint8_t* counter;
    uint16_t value1;
    uint8_t value2;
    void (*function) (const struct Chord*);
};

uint8_t current_pseudolayer = DEFAULT_PSEUDOLAYER;
bool lock_next = false;
uint16_t chord_timer = 0;
uint16_t dance_timer = 0;
bool autoshift_mode = true;
uint8_t keycode_index = 0;
uint8_t command_mode = 0;
uint8_t command_ind = 0;
bool in_leader_mode = false;
uint8_t leader_ind = 0;
uint16_t leader_timer = 0;
uint8_t dynamic_macro_mode = false;
uint8_t dynamic_macro_ind = 0;
bool a_key_went_through = false;
struct Chord* last_chord = NULL;

bool handle_US_ANSI_shifted_keys(int16_t keycode, bool in) {
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
            register_code(KC_LSFT);
            register_code(regular_keycode);
        } else {
            unregister_code(regular_keycode);
            unregister_code(KC_LSFT);
        }
    }
    return is_US_ANSI_shifted;
}

void key_in(int16_t keycode) {
    if (command_mode == 1 && command_ind < COMMAND_MAX_LENGTH) {
        command_buffer[command_ind] = keycode;
        command_ind++;
        a_key_went_through = true;
    } else if (in_leader_mode && leader_ind < LEADER_MAX_LENGTH) {
        leader_buffer[leader_ind] = keycode;
        leader_ind++;
        a_key_went_through = true;
    } else if (dynamic_macro_mode && dynamic_macro_ind < DYNAMIC_MACRO_MAX_LENGTH) {
        dynamic_macro_buffer[dynamic_macro_ind] = keycode;
        dynamic_macro_ind++;
        a_key_went_through = true;
    } else {
        if (!handle_US_ANSI_shifted_keys(keycode, true)) {
            register_code(keycode);
        }
        send_keyboard_report();
        a_key_went_through = true;
    }
}

void key_out(int16_t keycode) {
    if (command_mode == 0) {
        if (!handle_US_ANSI_shifted_keys(keycode, false)) {
            if (command_mode == 0 && in_leader_mode == false && dynamic_macro_mode == false) {
                unregister_code(keycode);
            }
        }
        send_keyboard_report();
    }
}

void tap_key(int16_t keycode) {
    key_in(keycode);
    wait_ms(TAP_TIMEOUT);
    key_out(keycode);
}
void single_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            key_in(self->value1);
            break;
        case DEACTIVATED:
            key_out(self->value1);
            *self->state = IDLE;
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
            current_pseudolayer = self->value2;
            a_key_went_through = false;
            break;
        case DEACTIVATED:
        case RESTART:
            if (!a_key_went_through) {
                tap_key(self->value1);
            }
            current_pseudolayer = self->pseudolayer;
            *self->state = IDLE; // does not have effect if the state was RESTART
            break;
        default:
            break;
    }
}

void key_mod_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            key_in(self->value2);
            a_key_went_through = false;
            break;
        case DEACTIVATED:
        case RESTART:
            key_out(self->value2);
            if (!a_key_went_through) {
                tap_key(self->value1);
            }
            *self->state = IDLE; // does not have effect if the state was RESTART
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
            tap_key(self->value1);
            *self->state = IDLE;
            break;
        case FINISHED:
        case PRESS_FROM_ACTIVE:
            key_in(self->value2);
            break;
        case RESTART:
            key_out(self->value2);
            break;
        default:
            break;
    }
}

void autoshift_dance_impl(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            *self->counter = 0;
            break;
        case DEACTIVATED:
        case RESTART:
            tap_key(self->value1);
            *self->state = IDLE;
            break;
        case FINISHED_FROM_ACTIVE:
            if (*self->counter == (LONG_PRESS_MULTIPLIER - 2)) {
                key_in(KC_LSFT);
                tap_key(self->value1);
                key_out(KC_LSFT);
                *self->state = IDLE;
                // the skip to IDLE is usually just a lag optimization,
                // in this case it has a logic function, on a short
                // press (still longer than a tap) the key does not get shifted
            } else {
                *self->counter += 1;
                *self->state = PRESS_FROM_ACTIVE;
                dance_timer = timer_read();
            }
            break;
        default:
            break;
    }
}

void autoshift_dance(const struct Chord* self) {
    if (autoshift_mode) {
        autoshift_dance_impl(self);
    } else {
        single_dance(self);
    }
}

void autoshift_toggle(const struct Chord* self){
    if (*self->state == ACTIVATED) {
        autoshift_mode = !autoshift_mode;
        *self->state = IDLE;
    }
}

void temp_pseudolayer(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            current_pseudolayer = self->value1;
            break;
        case DEACTIVATED:
            current_pseudolayer = self->pseudolayer;
            *self->state = IDLE;
            break;
        case RESTART:
            current_pseudolayer = self->pseudolayer;
            break;
        default:
            break;
    }
}

void temp_pseudolayer_alt(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            current_pseudolayer = self->value1;
            break;
        case DEACTIVATED:
            current_pseudolayer = self->value2;
            *self->state = IDLE;
            break;
        case RESTART:
            current_pseudolayer = self->value2;
            break;
        default:
            break;
    }
}

void perm_pseudolayer(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        current_pseudolayer = self->value1;
        *self->state = IDLE;
    }
}

void switch_layer(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        layer_move(self->value1);
        *self->state = IDLE;
    }
}

void lock(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        lock_next = true;
        *self->state = IDLE;
    }
}

void one_shot_key(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            break;
        case DEACTIVATED:
            key_in(self->value1);
            *self->state = IN_ONE_SHOT;
            break;
        case FINISHED:
        case PRESS_FROM_ACTIVE:
            key_in(self->value1);
            a_key_went_through = false;
            break;
        case RESTART:
            if (a_key_went_through) {
                key_out(self->value1);
            } else {
                *self->state = IN_ONE_SHOT;
            }
        default:
            break;
    }
}

void one_shot_layer(const struct Chord* self) {
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

void command(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        command_mode++;
        *self->state = IDLE;
    }
}

bool identical(uint16_t* buffer1, uint16_t* buffer2) {
    bool same = true;
    for (int i = 0; i < LEADER_MAX_LENGTH; i++) {
        same = same && (buffer1[i] == buffer2[i]);
    }
    return same;
}

void leader(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        in_leader_mode = true;
        *self->state = IDLE;
    }
}

void dynamic_macro_record(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        for (int i = 0; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
            dynamic_macro_buffer[i] = 0;
        }
        dynamic_macro_mode = true;
        *self->state = IDLE;
    }
}

void dynamic_macro_next(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode && dynamic_macro_ind < DYNAMIC_MACRO_MAX_LENGTH) {
            dynamic_macro_buffer[dynamic_macro_ind] = 0;
            dynamic_macro_ind++;
        }
        *self->state = IDLE;
    }
}

void dynamic_macro_end(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode) {
            dynamic_macro_mode = false;
        }
        *self->state = IDLE;
    }
}

void dynamic_macro_play(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        int ind_start = 0;
        while (ind_start < DYNAMIC_MACRO_MAX_LENGTH) {
            for (int i = ind_start; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
                if (dynamic_macro_buffer[i] == 0) {
                    break;
                }
                register_code(dynamic_macro_buffer[i]);
            }
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            for (int i = ind_start; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
                if (dynamic_macro_buffer[i] == 0) {
                    ind_start = i + 1;
                    break;
                }
                unregister_code(dynamic_macro_buffer[i]);
            }
            send_keyboard_report();
        }
        *self->state = IDLE;
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

uint8_t state_0 = IDLE;
const struct Chord chord_0 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, ALWAYS_ON, &state_0, NULL, NUM, BASE, temp_pseudolayer_alt};
uint8_t state_1 = IDLE;
const struct Chord chord_1 PROGMEM = {H_L2 + H_L3 + H_R2 + H_R3, ALWAYS_ON, &state_1, NULL, COMMANDS, BASE, temp_pseudolayer_alt};
uint8_t state_2 = IDLE;
const struct Chord chord_2 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4 + H_R1 + H_R2 + H_R3 + H_R4, ALWAYS_ON, &state_2, NULL, USER, BASE, temp_pseudolayer_alt};
uint8_t state_3 = IDLE;
const struct Chord chord_3 PROGMEM = {H_L3 + H_L4 + H_R1 + H_R2, ALWAYS_ON, &state_3, NULL, 0, 0, command};
uint8_t state_4 = IDLE;
const struct Chord chord_4 PROGMEM = {H_L1, BASE, &state_4, NULL, KC_A, 0, single_dance};
uint8_t state_5 = IDLE;
const struct Chord chord_5 PROGMEM = {H_L2, BASE, &state_5, NULL, KC_S, 0, single_dance};
uint8_t state_6 = IDLE;
const struct Chord chord_6 PROGMEM = {H_L3, BASE, &state_6, NULL, KC_E, 0, single_dance};
uint8_t state_7 = IDLE;
const struct Chord chord_7 PROGMEM = {H_L4, BASE, &state_7, NULL, KC_T, 0, single_dance};
uint8_t state_8 = IDLE;
const struct Chord chord_8 PROGMEM = {H_R1, BASE, &state_8, NULL, KC_N, 0, single_dance};
uint8_t state_9 = IDLE;
const struct Chord chord_9 PROGMEM = {H_R2, BASE, &state_9, NULL, KC_I, 0, single_dance};
uint8_t state_10 = IDLE;
const struct Chord chord_10 PROGMEM = {H_R3, BASE, &state_10, NULL, KC_O, 0, single_dance};
uint8_t state_11 = IDLE;
const struct Chord chord_11 PROGMEM = {H_R4, BASE, &state_11, NULL, KC_P, 0, single_dance};
uint8_t state_12 = IDLE;
const struct Chord chord_12 PROGMEM = {H_L1 + H_L2, BASE, &state_12, NULL, KC_W, 0, single_dance};
uint8_t state_13 = IDLE;
const struct Chord chord_13 PROGMEM = {H_L2 + H_L3, BASE, &state_13, NULL, KC_X, 0, single_dance};
uint8_t state_14 = IDLE;
const struct Chord chord_14 PROGMEM = {H_L3 + H_L4, BASE, &state_14, NULL, KC_F, 0, single_dance};
uint8_t state_15 = IDLE;
const struct Chord chord_15 PROGMEM = {H_L4 + H_R1, BASE, &state_15, NULL, KC_Q, 0, single_dance};
uint8_t state_16 = IDLE;
const struct Chord chord_16 PROGMEM = {H_R1 + H_R2, BASE, &state_16, NULL, KC_Z, 0, single_dance};
uint8_t state_17 = IDLE;
const struct Chord chord_17 PROGMEM = {H_R3 + H_R4, BASE, &state_17, NULL, KC_R, 0, single_dance};
uint8_t state_18 = IDLE;
const struct Chord chord_18 PROGMEM = {H_L1 + H_L3, BASE, &state_18, NULL, KC_Y, 0, single_dance};
uint8_t state_19 = IDLE;
const struct Chord chord_19 PROGMEM = {H_L2 + H_L4, BASE, &state_19, NULL, KC_COMMA, 0, single_dance};
uint8_t state_20 = IDLE;
const struct Chord chord_20 PROGMEM = {H_L3 + H_R1, BASE, &state_20, NULL, KC_MINUS, 0, single_dance};
uint8_t state_21 = IDLE;
const struct Chord chord_21 PROGMEM = {H_L4 + H_R2, BASE, &state_21, NULL, KC_QUOTE, 0, single_dance};
uint8_t state_22 = IDLE;
const struct Chord chord_22 PROGMEM = {H_R1 + H_R3, BASE, &state_22, NULL, KC_B, 0, single_dance};
uint8_t state_23 = IDLE;
const struct Chord chord_23 PROGMEM = {H_R2 + H_R4, BASE, &state_23, NULL, KC_V, 0, single_dance};
uint8_t state_24 = IDLE;
const struct Chord chord_24 PROGMEM = {H_L1 + H_L4, BASE, &state_24, NULL, KC_G, 0, single_dance};
uint8_t state_25 = IDLE;
const struct Chord chord_25 PROGMEM = {H_L2 + H_R1, BASE, &state_25, NULL, KC_BSPC, 0, single_dance};
uint8_t state_26 = IDLE;
const struct Chord chord_26 PROGMEM = {H_L3 + H_R2, BASE, &state_26, NULL, KC_H, 0, single_dance};
uint8_t state_27 = IDLE;
const struct Chord chord_27 PROGMEM = {H_L4 + H_R3, BASE, &state_27, NULL, KC_U, 0, single_dance};
uint8_t state_28 = IDLE;
const struct Chord chord_28 PROGMEM = {H_R1 + H_R4, BASE, &state_28, NULL, KC_M, 0, single_dance};
uint8_t state_29 = IDLE;
const struct Chord chord_29 PROGMEM = {H_L1 + H_R1, BASE, &state_29, NULL, KC_L, 0, single_dance};
uint8_t state_30 = IDLE;
const struct Chord chord_30 PROGMEM = {H_L2 + H_R2, BASE, &state_30, NULL, KC_EXCLAIM, 0, single_dance};
uint8_t state_31 = IDLE;
const struct Chord chord_31 PROGMEM = {H_L3 + H_R3, BASE, &state_31, NULL, KC_SCOLON, 0, single_dance};
void str_32(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_32 = IDLE;
const struct Chord chord_32 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_32, NULL, 0, 0, str_32};
void str_33(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_33 = IDLE;
const struct Chord chord_33 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_33, NULL, 0, 0, str_33};
void str_34(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_34 = IDLE;
const struct Chord chord_34 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_34, NULL, 0, 0, str_34};
void str_35(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_35 = IDLE;
const struct Chord chord_35 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_35, NULL, 0, 0, str_35};
void str_36(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_36 = IDLE;
const struct Chord chord_36 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_36, NULL, 0, 0, str_36};
void str_37(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_37 = IDLE;
const struct Chord chord_37 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_37, NULL, 0, 0, str_37};
void str_38(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_38 = IDLE;
const struct Chord chord_38 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_38, NULL, 0, 0, str_38};
void str_39(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_39 = IDLE;
const struct Chord chord_39 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_39, NULL, 0, 0, str_39};
void str_40(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_40 = IDLE;
const struct Chord chord_40 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_40, NULL, 0, 0, str_40};
void str_41(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_41 = IDLE;
const struct Chord chord_41 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_41, NULL, 0, 0, str_41};
void str_42(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_42 = IDLE;
const struct Chord chord_42 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_42, NULL, 0, 0, str_42};
void str_43(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_43 = IDLE;
const struct Chord chord_43 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_43, NULL, 0, 0, str_43};
void str_44(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_44 = IDLE;
const struct Chord chord_44 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_44, NULL, 0, 0, str_44};
void str_45(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_45 = IDLE;
const struct Chord chord_45 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_45, NULL, 0, 0, str_45};
void str_46(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_46 = IDLE;
const struct Chord chord_46 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_46, NULL, 0, 0, str_46};
void str_47(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_47 = IDLE;
const struct Chord chord_47 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_47, NULL, 0, 0, str_47};
void str_48(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_48 = IDLE;
const struct Chord chord_48 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_48, NULL, 0, 0, str_48};
void str_49(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_49 = IDLE;
const struct Chord chord_49 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_49, NULL, 0, 0, str_49};
void str_50(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_50 = IDLE;
const struct Chord chord_50 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_50, NULL, 0, 0, str_50};
void str_51(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_51 = IDLE;
const struct Chord chord_51 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_51, NULL, 0, 0, str_51};
void str_52(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_52 = IDLE;
const struct Chord chord_52 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_52, NULL, 0, 0, str_52};
void str_53(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_53 = IDLE;
const struct Chord chord_53 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_53, NULL, 0, 0, str_53};
void str_54(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_54 = IDLE;
const struct Chord chord_54 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_54, NULL, 0, 0, str_54};
void str_55(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_55 = IDLE;
const struct Chord chord_55 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_55, NULL, 0, 0, str_55};
void str_56(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_56 = IDLE;
const struct Chord chord_56 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_56, NULL, 0, 0, str_56};
void str_57(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_57 = IDLE;
const struct Chord chord_57 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_57, NULL, 0, 0, str_57};
void str_58(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_58 = IDLE;
const struct Chord chord_58 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_58, NULL, 0, 0, str_58};
void str_59(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_59 = IDLE;
const struct Chord chord_59 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_59, NULL, 0, 0, str_59};
void str_60(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_60 = IDLE;
const struct Chord chord_60 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_60, NULL, 0, 0, str_60};
void str_61(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_61 = IDLE;
const struct Chord chord_61 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_61, NULL, 0, 0, str_61};
void str_62(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_62 = IDLE;
const struct Chord chord_62 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_62, NULL, 0, 0, str_62};
void str_63(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_63 = IDLE;
const struct Chord chord_63 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_63, NULL, 0, 0, str_63};
void str_64(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_64 = IDLE;
const struct Chord chord_64 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_64, NULL, 0, 0, str_64};
void str_65(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_65 = IDLE;
const struct Chord chord_65 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_65, NULL, 0, 0, str_65};
void str_66(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_66 = IDLE;
const struct Chord chord_66 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_66, NULL, 0, 0, str_66};
void str_67(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_67 = IDLE;
const struct Chord chord_67 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_67, NULL, 0, 0, str_67};
void str_68(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_68 = IDLE;
const struct Chord chord_68 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_68, NULL, 0, 0, str_68};
void str_69(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_69 = IDLE;
const struct Chord chord_69 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_69, NULL, 0, 0, str_69};
void str_70(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_70 = IDLE;
const struct Chord chord_70 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_70, NULL, 0, 0, str_70};
void str_71(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_71 = IDLE;
const struct Chord chord_71 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_71, NULL, 0, 0, str_71};
void str_72(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_72 = IDLE;
const struct Chord chord_72 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_72, NULL, 0, 0, str_72};
void str_73(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_73 = IDLE;
const struct Chord chord_73 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_73, NULL, 0, 0, str_73};
void str_74(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_74 = IDLE;
const struct Chord chord_74 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_74, NULL, 0, 0, str_74};
void str_75(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_75 = IDLE;
const struct Chord chord_75 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_75, NULL, 0, 0, str_75};
void str_76(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_76 = IDLE;
const struct Chord chord_76 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_76, NULL, 0, 0, str_76};
void str_77(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_77 = IDLE;
const struct Chord chord_77 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_77, NULL, 0, 0, str_77};
void str_78(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_78 = IDLE;
const struct Chord chord_78 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_78, NULL, 0, 0, str_78};
void str_79(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_79 = IDLE;
const struct Chord chord_79 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_79, NULL, 0, 0, str_79};
void str_80(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_80 = IDLE;
const struct Chord chord_80 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_80, NULL, 0, 0, str_80};
void str_81(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_81 = IDLE;
const struct Chord chord_81 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_81, NULL, 0, 0, str_81};
void str_82(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_82 = IDLE;
const struct Chord chord_82 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_82, NULL, 0, 0, str_82};
void str_83(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_83 = IDLE;
const struct Chord chord_83 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_83, NULL, 0, 0, str_83};
void str_84(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_84 = IDLE;
const struct Chord chord_84 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_84, NULL, 0, 0, str_84};
void str_85(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_85 = IDLE;
const struct Chord chord_85 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_85, NULL, 0, 0, str_85};
void str_86(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_86 = IDLE;
const struct Chord chord_86 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_86, NULL, 0, 0, str_86};
void str_87(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_87 = IDLE;
const struct Chord chord_87 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_87, NULL, 0, 0, str_87};
void str_88(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_88 = IDLE;
const struct Chord chord_88 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_88, NULL, 0, 0, str_88};
void str_89(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_89 = IDLE;
const struct Chord chord_89 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_89, NULL, 0, 0, str_89};
void str_90(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_90 = IDLE;
const struct Chord chord_90 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_90, NULL, 0, 0, str_90};
void str_91(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_91 = IDLE;
const struct Chord chord_91 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_91, NULL, 0, 0, str_91};
void str_92(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_92 = IDLE;
const struct Chord chord_92 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_92, NULL, 0, 0, str_92};
void str_93(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_93 = IDLE;
const struct Chord chord_93 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_93, NULL, 0, 0, str_93};
void str_94(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_94 = IDLE;
const struct Chord chord_94 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_94, NULL, 0, 0, str_94};
void str_95(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_95 = IDLE;
const struct Chord chord_95 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_95, NULL, 0, 0, str_95};
void str_96(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_96 = IDLE;
const struct Chord chord_96 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_96, NULL, 0, 0, str_96};
void str_97(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_97 = IDLE;
const struct Chord chord_97 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_97, NULL, 0, 0, str_97};
void str_98(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_98 = IDLE;
const struct Chord chord_98 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_98, NULL, 0, 0, str_98};
void str_99(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_99 = IDLE;
const struct Chord chord_99 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_99, NULL, 0, 0, str_99};
void str_100(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_100 = IDLE;
const struct Chord chord_100 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_100, NULL, 0, 0, str_100};
void str_101(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_101 = IDLE;
const struct Chord chord_101 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_101, NULL, 0, 0, str_101};
void str_102(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_102 = IDLE;
const struct Chord chord_102 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_102, NULL, 0, 0, str_102};
void str_103(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_103 = IDLE;
const struct Chord chord_103 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_103, NULL, 0, 0, str_103};
void str_104(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_104 = IDLE;
const struct Chord chord_104 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_104, NULL, 0, 0, str_104};
void str_105(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_105 = IDLE;
const struct Chord chord_105 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_105, NULL, 0, 0, str_105};
void str_106(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_106 = IDLE;
const struct Chord chord_106 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_106, NULL, 0, 0, str_106};
void str_107(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_107 = IDLE;
const struct Chord chord_107 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_107, NULL, 0, 0, str_107};
void str_108(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_108 = IDLE;
const struct Chord chord_108 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_108, NULL, 0, 0, str_108};
void str_109(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_109 = IDLE;
const struct Chord chord_109 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_109, NULL, 0, 0, str_109};
void str_110(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_110 = IDLE;
const struct Chord chord_110 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_110, NULL, 0, 0, str_110};
void str_111(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_111 = IDLE;
const struct Chord chord_111 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_111, NULL, 0, 0, str_111};
void str_112(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_112 = IDLE;
const struct Chord chord_112 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_112, NULL, 0, 0, str_112};
void str_113(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_113 = IDLE;
const struct Chord chord_113 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_113, NULL, 0, 0, str_113};
void str_114(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_114 = IDLE;
const struct Chord chord_114 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_114, NULL, 0, 0, str_114};
void str_115(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_115 = IDLE;
const struct Chord chord_115 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_115, NULL, 0, 0, str_115};
void str_116(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_116 = IDLE;
const struct Chord chord_116 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_116, NULL, 0, 0, str_116};
void str_117(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_117 = IDLE;
const struct Chord chord_117 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_117, NULL, 0, 0, str_117};
void str_118(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_118 = IDLE;
const struct Chord chord_118 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_118, NULL, 0, 0, str_118};
void str_119(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_119 = IDLE;
const struct Chord chord_119 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_119, NULL, 0, 0, str_119};
void str_120(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_120 = IDLE;
const struct Chord chord_120 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_120, NULL, 0, 0, str_120};
void str_121(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_121 = IDLE;
const struct Chord chord_121 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_121, NULL, 0, 0, str_121};
void str_122(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_122 = IDLE;
const struct Chord chord_122 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_122, NULL, 0, 0, str_122};
void str_123(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_123 = IDLE;
const struct Chord chord_123 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_123, NULL, 0, 0, str_123};
void str_124(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_124 = IDLE;
const struct Chord chord_124 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_124, NULL, 0, 0, str_124};
void str_125(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_125 = IDLE;
const struct Chord chord_125 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_125, NULL, 0, 0, str_125};
void str_126(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_126 = IDLE;
const struct Chord chord_126 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_126, NULL, 0, 0, str_126};
void str_127(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_127 = IDLE;
const struct Chord chord_127 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_127, NULL, 0, 0, str_127};
void str_128(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_128 = IDLE;
const struct Chord chord_128 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_128, NULL, 0, 0, str_128};
void str_129(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_129 = IDLE;
const struct Chord chord_129 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_129, NULL, 0, 0, str_129};
void str_130(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_130 = IDLE;
const struct Chord chord_130 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_130, NULL, 0, 0, str_130};
void str_131(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_131 = IDLE;
const struct Chord chord_131 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_131, NULL, 0, 0, str_131};
void str_132(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_132 = IDLE;
const struct Chord chord_132 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_132, NULL, 0, 0, str_132};
void str_133(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_133 = IDLE;
const struct Chord chord_133 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_133, NULL, 0, 0, str_133};
void str_134(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_134 = IDLE;
const struct Chord chord_134 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_134, NULL, 0, 0, str_134};
void str_135(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_135 = IDLE;
const struct Chord chord_135 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_135, NULL, 0, 0, str_135};
void str_136(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_136 = IDLE;
const struct Chord chord_136 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_136, NULL, 0, 0, str_136};
void str_137(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_137 = IDLE;
const struct Chord chord_137 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_137, NULL, 0, 0, str_137};
void str_138(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_138 = IDLE;
const struct Chord chord_138 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_138, NULL, 0, 0, str_138};
void str_139(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_139 = IDLE;
const struct Chord chord_139 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_139, NULL, 0, 0, str_139};
void str_140(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_140 = IDLE;
const struct Chord chord_140 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_140, NULL, 0, 0, str_140};
void str_141(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_141 = IDLE;
const struct Chord chord_141 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_141, NULL, 0, 0, str_141};
void str_142(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_142 = IDLE;
const struct Chord chord_142 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_142, NULL, 0, 0, str_142};
void str_143(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_143 = IDLE;
const struct Chord chord_143 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_143, NULL, 0, 0, str_143};
void str_144(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_144 = IDLE;
const struct Chord chord_144 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_144, NULL, 0, 0, str_144};
void str_145(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_145 = IDLE;
const struct Chord chord_145 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_145, NULL, 0, 0, str_145};
void str_146(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_146 = IDLE;
const struct Chord chord_146 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_146, NULL, 0, 0, str_146};
void str_147(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_147 = IDLE;
const struct Chord chord_147 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_147, NULL, 0, 0, str_147};
void str_148(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_148 = IDLE;
const struct Chord chord_148 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_148, NULL, 0, 0, str_148};
void str_149(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_149 = IDLE;
const struct Chord chord_149 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_149, NULL, 0, 0, str_149};
void str_150(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_150 = IDLE;
const struct Chord chord_150 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_150, NULL, 0, 0, str_150};
void str_151(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_151 = IDLE;
const struct Chord chord_151 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_151, NULL, 0, 0, str_151};
void str_152(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_152 = IDLE;
const struct Chord chord_152 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_152, NULL, 0, 0, str_152};
void str_153(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_153 = IDLE;
const struct Chord chord_153 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_153, NULL, 0, 0, str_153};
void str_154(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_154 = IDLE;
const struct Chord chord_154 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_154, NULL, 0, 0, str_154};
void str_155(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_155 = IDLE;
const struct Chord chord_155 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_155, NULL, 0, 0, str_155};
void str_156(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_156 = IDLE;
const struct Chord chord_156 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_156, NULL, 0, 0, str_156};
void str_157(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_157 = IDLE;
const struct Chord chord_157 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_157, NULL, 0, 0, str_157};
void str_158(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_158 = IDLE;
const struct Chord chord_158 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_158, NULL, 0, 0, str_158};
void str_159(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_159 = IDLE;
const struct Chord chord_159 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_159, NULL, 0, 0, str_159};
void str_160(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_160 = IDLE;
const struct Chord chord_160 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_160, NULL, 0, 0, str_160};
void str_161(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_161 = IDLE;
const struct Chord chord_161 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_161, NULL, 0, 0, str_161};
void str_162(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_162 = IDLE;
const struct Chord chord_162 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_162, NULL, 0, 0, str_162};
void str_163(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_163 = IDLE;
const struct Chord chord_163 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_163, NULL, 0, 0, str_163};
void str_164(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_164 = IDLE;
const struct Chord chord_164 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_164, NULL, 0, 0, str_164};
void str_165(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_165 = IDLE;
const struct Chord chord_165 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_165, NULL, 0, 0, str_165};
void str_166(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_166 = IDLE;
const struct Chord chord_166 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_166, NULL, 0, 0, str_166};
void str_167(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_167 = IDLE;
const struct Chord chord_167 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_167, NULL, 0, 0, str_167};
void str_168(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_168 = IDLE;
const struct Chord chord_168 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_168, NULL, 0, 0, str_168};
void str_169(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_169 = IDLE;
const struct Chord chord_169 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_169, NULL, 0, 0, str_169};
void str_170(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_170 = IDLE;
const struct Chord chord_170 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_170, NULL, 0, 0, str_170};
void str_171(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_171 = IDLE;
const struct Chord chord_171 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_171, NULL, 0, 0, str_171};
void str_172(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_172 = IDLE;
const struct Chord chord_172 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_172, NULL, 0, 0, str_172};
void str_173(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_173 = IDLE;
const struct Chord chord_173 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_173, NULL, 0, 0, str_173};
void str_174(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_174 = IDLE;
const struct Chord chord_174 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_174, NULL, 0, 0, str_174};
void str_175(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_175 = IDLE;
const struct Chord chord_175 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_175, NULL, 0, 0, str_175};
void str_176(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_176 = IDLE;
const struct Chord chord_176 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_176, NULL, 0, 0, str_176};
void str_177(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_177 = IDLE;
const struct Chord chord_177 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_177, NULL, 0, 0, str_177};
void str_178(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_178 = IDLE;
const struct Chord chord_178 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_178, NULL, 0, 0, str_178};
void str_179(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_179 = IDLE;
const struct Chord chord_179 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_179, NULL, 0, 0, str_179};
void str_180(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_180 = IDLE;
const struct Chord chord_180 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_180, NULL, 0, 0, str_180};
void str_181(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_181 = IDLE;
const struct Chord chord_181 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_181, NULL, 0, 0, str_181};
void str_182(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_182 = IDLE;
const struct Chord chord_182 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_182, NULL, 0, 0, str_182};
void str_183(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_183 = IDLE;
const struct Chord chord_183 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_183, NULL, 0, 0, str_183};
void str_184(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_184 = IDLE;
const struct Chord chord_184 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_184, NULL, 0, 0, str_184};
void str_185(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_185 = IDLE;
const struct Chord chord_185 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_185, NULL, 0, 0, str_185};
void str_186(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_186 = IDLE;
const struct Chord chord_186 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_186, NULL, 0, 0, str_186};
void str_187(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_187 = IDLE;
const struct Chord chord_187 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_187, NULL, 0, 0, str_187};
void str_188(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_188 = IDLE;
const struct Chord chord_188 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_188, NULL, 0, 0, str_188};
void str_189(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_189 = IDLE;
const struct Chord chord_189 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_189, NULL, 0, 0, str_189};
void str_190(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_190 = IDLE;
const struct Chord chord_190 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_190, NULL, 0, 0, str_190};
void str_191(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_191 = IDLE;
const struct Chord chord_191 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_191, NULL, 0, 0, str_191};
void str_192(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_192 = IDLE;
const struct Chord chord_192 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_192, NULL, 0, 0, str_192};
void str_193(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_193 = IDLE;
const struct Chord chord_193 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_193, NULL, 0, 0, str_193};
void str_194(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_194 = IDLE;
const struct Chord chord_194 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_194, NULL, 0, 0, str_194};
void str_195(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_195 = IDLE;
const struct Chord chord_195 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_195, NULL, 0, 0, str_195};
void str_196(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_196 = IDLE;
const struct Chord chord_196 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_196, NULL, 0, 0, str_196};
void str_197(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_197 = IDLE;
const struct Chord chord_197 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_197, NULL, 0, 0, str_197};
void str_198(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_198 = IDLE;
const struct Chord chord_198 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_198, NULL, 0, 0, str_198};
void str_199(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_199 = IDLE;
const struct Chord chord_199 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_199, NULL, 0, 0, str_199};
void str_200(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_200 = IDLE;
const struct Chord chord_200 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_200, NULL, 0, 0, str_200};
void str_201(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_201 = IDLE;
const struct Chord chord_201 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_201, NULL, 0, 0, str_201};
void str_202(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_202 = IDLE;
const struct Chord chord_202 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_202, NULL, 0, 0, str_202};
void str_203(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_203 = IDLE;
const struct Chord chord_203 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_203, NULL, 0, 0, str_203};
void str_204(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_204 = IDLE;
const struct Chord chord_204 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_204, NULL, 0, 0, str_204};
void str_205(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_205 = IDLE;
const struct Chord chord_205 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_205, NULL, 0, 0, str_205};
void str_206(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_206 = IDLE;
const struct Chord chord_206 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_206, NULL, 0, 0, str_206};
void str_207(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_207 = IDLE;
const struct Chord chord_207 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_207, NULL, 0, 0, str_207};
void str_208(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_208 = IDLE;
const struct Chord chord_208 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_208, NULL, 0, 0, str_208};
void str_209(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_209 = IDLE;
const struct Chord chord_209 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_209, NULL, 0, 0, str_209};
void str_210(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_210 = IDLE;
const struct Chord chord_210 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_210, NULL, 0, 0, str_210};
void str_211(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_211 = IDLE;
const struct Chord chord_211 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_211, NULL, 0, 0, str_211};
void str_212(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_212 = IDLE;
const struct Chord chord_212 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_212, NULL, 0, 0, str_212};
void str_213(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_213 = IDLE;
const struct Chord chord_213 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_213, NULL, 0, 0, str_213};
void str_214(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_214 = IDLE;
const struct Chord chord_214 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_214, NULL, 0, 0, str_214};
void str_215(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_215 = IDLE;
const struct Chord chord_215 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_215, NULL, 0, 0, str_215};
void str_216(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_216 = IDLE;
const struct Chord chord_216 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_216, NULL, 0, 0, str_216};
void str_217(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_217 = IDLE;
const struct Chord chord_217 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_217, NULL, 0, 0, str_217};
void str_218(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_218 = IDLE;
const struct Chord chord_218 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_218, NULL, 0, 0, str_218};
void str_219(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_219 = IDLE;
const struct Chord chord_219 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_219, NULL, 0, 0, str_219};
void str_220(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_220 = IDLE;
const struct Chord chord_220 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_220, NULL, 0, 0, str_220};
void str_221(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_221 = IDLE;
const struct Chord chord_221 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_221, NULL, 0, 0, str_221};
void str_222(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_222 = IDLE;
const struct Chord chord_222 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_222, NULL, 0, 0, str_222};
void str_223(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_223 = IDLE;
const struct Chord chord_223 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_223, NULL, 0, 0, str_223};
void str_224(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_224 = IDLE;
const struct Chord chord_224 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_224, NULL, 0, 0, str_224};
void str_225(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_225 = IDLE;
const struct Chord chord_225 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_225, NULL, 0, 0, str_225};
void str_226(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_226 = IDLE;
const struct Chord chord_226 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_226, NULL, 0, 0, str_226};
void str_227(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_227 = IDLE;
const struct Chord chord_227 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_227, NULL, 0, 0, str_227};
void str_228(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_228 = IDLE;
const struct Chord chord_228 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_228, NULL, 0, 0, str_228};
void str_229(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_229 = IDLE;
const struct Chord chord_229 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_229, NULL, 0, 0, str_229};
void str_230(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_230 = IDLE;
const struct Chord chord_230 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_230, NULL, 0, 0, str_230};
void str_231(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_231 = IDLE;
const struct Chord chord_231 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_231, NULL, 0, 0, str_231};
void str_232(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_232 = IDLE;
const struct Chord chord_232 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_232, NULL, 0, 0, str_232};
void str_233(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_233 = IDLE;
const struct Chord chord_233 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_233, NULL, 0, 0, str_233};
void str_234(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("we");
    }
}
uint8_t state_234 = IDLE;
const struct Chord chord_234 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_234, NULL, 0, 0, str_234};
void str_235(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("are");
    }
}
uint8_t state_235 = IDLE;
const struct Chord chord_235 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_235, NULL, 0, 0, str_235};
void str_236(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_236 = IDLE;
const struct Chord chord_236 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_236, NULL, 0, 0, str_236};
void str_237(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        SEND_STRING("were");
    }
}
uint8_t state_237 = IDLE;
const struct Chord chord_237 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_237, NULL, 0, 0, str_237};
uint8_t state_238 = IDLE;
const struct Chord chord_238 PROGMEM = {H_L1, NUM, &state_238, NULL, KC_1, 0, single_dance};
uint8_t state_239 = IDLE;
const struct Chord chord_239 PROGMEM = {H_L2, NUM, &state_239, NULL, KC_2, 0, single_dance};
uint8_t state_240 = IDLE;
const struct Chord chord_240 PROGMEM = {H_L3, NUM, &state_240, NULL, KC_3, 0, single_dance};
uint8_t state_241 = IDLE;
const struct Chord chord_241 PROGMEM = {H_L4, NUM, &state_241, NULL, KC_4, 0, single_dance};
uint8_t state_242 = IDLE;
const struct Chord chord_242 PROGMEM = {H_R1, NUM, &state_242, NULL, KC_7, 0, single_dance};
uint8_t state_243 = IDLE;
const struct Chord chord_243 PROGMEM = {H_R2, NUM, &state_243, NULL, KC_8, 0, single_dance};
uint8_t state_244 = IDLE;
const struct Chord chord_244 PROGMEM = {H_R3, NUM, &state_244, NULL, KC_9, 0, single_dance};
uint8_t state_245 = IDLE;
const struct Chord chord_245 PROGMEM = {H_R4, NUM, &state_245, NULL, KC_0, 0, single_dance};
uint8_t state_246 = IDLE;
const struct Chord chord_246 PROGMEM = {H_L1 + H_L2, NUM, &state_246, NULL, KC_SPC, 0, single_dance};
uint8_t state_247 = IDLE;
const struct Chord chord_247 PROGMEM = {H_L2 + H_L3, NUM, &state_247, NULL, KC_GRAVE, 0, single_dance};
uint8_t state_248 = IDLE;
const struct Chord chord_248 PROGMEM = {H_L3 + H_L4, NUM, &state_248, NULL, KC_LBRACKET, 0, single_dance};
uint8_t state_249 = IDLE;
const struct Chord chord_249 PROGMEM = {H_L4 + H_R1, NUM, &state_249, NULL, KC_SPC, 0, single_dance};
uint8_t state_250 = IDLE;
const struct Chord chord_250 PROGMEM = {H_R1 + H_R2, NUM, &state_250, NULL, KC_EXCLAIM, 0, single_dance};
uint8_t state_251 = IDLE;
const struct Chord chord_251 PROGMEM = {H_R3 + H_R4, NUM, &state_251, NULL, KC_5, 0, single_dance};
uint8_t state_252 = IDLE;
const struct Chord chord_252 PROGMEM = {H_L1 + H_L3, NUM, &state_252, NULL, KC_HOME, 0, single_dance};
uint8_t state_253 = IDLE;
const struct Chord chord_253 PROGMEM = {H_L2 + H_L4, NUM, &state_253, NULL, KC_COMMA, 0, single_dance};
uint8_t state_254 = IDLE;
const struct Chord chord_254 PROGMEM = {H_L3 + H_R1, NUM, &state_254, NULL, KC_MINUS, 0, single_dance};
uint8_t state_255 = IDLE;
const struct Chord chord_255 PROGMEM = {H_L4 + H_R2, NUM, &state_255, NULL, KC_QUOTE, 0, single_dance};
uint8_t state_256 = IDLE;
const struct Chord chord_256 PROGMEM = {H_L2 + H_R1, NUM, &state_256, NULL, KC_BSPC, 0, single_dance};
uint8_t state_257 = IDLE;
const struct Chord chord_257 PROGMEM = {H_L3 + H_R2, NUM, &state_257, NULL, KC_6, 0, single_dance};
uint8_t state_258 = IDLE;
const struct Chord chord_258 PROGMEM = {H_R1 + H_R4, NUM, &state_258, NULL, KC_RBRACKET, 0, single_dance};
uint8_t state_259 = IDLE;
const struct Chord chord_259 PROGMEM = {H_L1 + H_R1, NUM, &state_259, NULL, KC_EQUAL, 0, single_dance};
uint8_t state_260 = IDLE;
const struct Chord chord_260 PROGMEM = {H_L3 + H_R3, NUM, &state_260, NULL, KC_SCOLON, 0, single_dance};
uint8_t state_261 = IDLE;
const struct Chord chord_261 PROGMEM = {H_R1 + H_R2 + H_R3 + H_R4, NUM, &state_261, NULL, KC_TAB, 0, single_dance};
uint8_t state_262 = IDLE;
const struct Chord chord_262 PROGMEM = {H_L1, COMMANDS, &state_262, NULL, KC_LCTL, 0, single_dance};
uint8_t state_263 = IDLE;
const struct Chord chord_263 PROGMEM = {H_L2, COMMANDS, &state_263, NULL, KC_LSFT, 0, single_dance};
uint8_t state_264 = IDLE;
const struct Chord chord_264 PROGMEM = {H_L3, COMMANDS, &state_264, NULL, KC_LALT, 0, single_dance};
uint8_t state_265 = IDLE;
const struct Chord chord_265 PROGMEM = {H_L4, COMMANDS, &state_265, NULL, KC_LGUI, 0, single_dance};
uint8_t state_266 = IDLE;
const struct Chord chord_266 PROGMEM = {H_R1, COMMANDS, &state_266, NULL, KC_LEFT, 0, single_dance};
uint8_t state_267 = IDLE;
const struct Chord chord_267 PROGMEM = {H_R2, COMMANDS, &state_267, NULL, KC_DOWN, 0, single_dance};
uint8_t state_268 = IDLE;
const struct Chord chord_268 PROGMEM = {H_R3, COMMANDS, &state_268, NULL, KC_UP, 0, single_dance};
uint8_t state_269 = IDLE;
const struct Chord chord_269 PROGMEM = {H_R4, COMMANDS, &state_269, NULL, KC_RIGHT, 0, single_dance};
uint8_t state_270 = IDLE;
const struct Chord chord_270 PROGMEM = {H_L1 + H_R4, COMMANDS, &state_270, NULL, KC_F1, 0, single_dance};
uint8_t state_271 = IDLE;
const struct Chord chord_271 PROGMEM = {H_L1 + H_R3, COMMANDS, &state_271, NULL, KC_F2, 0, single_dance};
uint8_t state_272 = IDLE;
const struct Chord chord_272 PROGMEM = {H_L1 + H_R3 + H_R4, COMMANDS, &state_272, NULL, KC_F3, 0, single_dance};
uint8_t state_273 = IDLE;
const struct Chord chord_273 PROGMEM = {H_L1 + H_R2, COMMANDS, &state_273, NULL, KC_F4, 0, single_dance};
uint8_t state_274 = IDLE;
const struct Chord chord_274 PROGMEM = {H_L1 + H_R2 + H_R4, COMMANDS, &state_274, NULL, KC_F5, 0, single_dance};
uint8_t state_275 = IDLE;
const struct Chord chord_275 PROGMEM = {H_L1 + H_R2 + H_R3, COMMANDS, &state_275, NULL, KC_F6, 0, single_dance};
uint8_t state_276 = IDLE;
const struct Chord chord_276 PROGMEM = {H_L1 + H_R2 + H_R3 + H_R4, COMMANDS, &state_276, NULL, KC_F7, 0, single_dance};
uint8_t state_277 = IDLE;
const struct Chord chord_277 PROGMEM = {H_L1 + H_R1, COMMANDS, &state_277, NULL, KC_F8, 0, single_dance};
uint8_t state_278 = IDLE;
const struct Chord chord_278 PROGMEM = {H_L1 + H_R1 + H_R4, COMMANDS, &state_278, NULL, KC_F9, 0, single_dance};
uint8_t state_279 = IDLE;
const struct Chord chord_279 PROGMEM = {H_L1 + H_R1 + H_R3, COMMANDS, &state_279, NULL, KC_F10, 0, single_dance};
uint8_t state_280 = IDLE;
const struct Chord chord_280 PROGMEM = {H_L1 + H_R1 + H_R3 + H_R4, COMMANDS, &state_280, NULL, KC_F11, 0, single_dance};
uint8_t state_281 = IDLE;
const struct Chord chord_281 PROGMEM = {H_L1 + H_R1 + H_R2, COMMANDS, &state_281, NULL, KC_F12, 0, single_dance};
uint8_t state_282 = IDLE;
const struct Chord chord_282 PROGMEM = {H_L2 + H_R1, COMMANDS, &state_282, NULL, KC_HOME, 0, single_dance};
uint8_t state_283 = IDLE;
const struct Chord chord_283 PROGMEM = {H_L2 + H_R2, COMMANDS, &state_283, NULL, KC_INS, 0, single_dance};
uint8_t state_284 = IDLE;
const struct Chord chord_284 PROGMEM = {H_L2 + H_R3, COMMANDS, &state_284, NULL, KC_DEL, 0, single_dance};
uint8_t state_285 = IDLE;
const struct Chord chord_285 PROGMEM = {H_L2 + H_R4, COMMANDS, &state_285, NULL, KC_END, 0, single_dance};
uint8_t state_286 = IDLE;
const struct Chord chord_286 PROGMEM = {H_LT, COMMANDS, &state_286, NULL, KC_PGDN, 0, single_dance};
uint8_t state_287 = IDLE;
const struct Chord chord_287 PROGMEM = {H_RT, COMMANDS, &state_287, NULL, KC_PGUP, 0, single_dance};

const struct Chord* const list_of_chords[] PROGMEM = {
    &chord_0, &chord_1, &chord_2, &chord_3, &chord_4, &chord_5, &chord_6, &chord_7, &chord_8, &chord_9, &chord_10, &chord_11, &chord_12, &chord_13, &chord_14, &chord_15, &chord_16, &chord_17, &chord_18, &chord_19, &chord_20, &chord_21, &chord_22, &chord_23, &chord_24, &chord_25, &chord_26, &chord_27, &chord_28, &chord_29, &chord_30, &chord_31, &chord_32, &chord_33, &chord_34, &chord_35, &chord_36, &chord_37, &chord_38, &chord_39, &chord_40, &chord_41, &chord_42, &chord_43, &chord_44, &chord_45, &chord_46, &chord_47, &chord_48, &chord_49, &chord_50, &chord_51, &chord_52, &chord_53, &chord_54, &chord_55, &chord_56, &chord_57, &chord_58, &chord_59, &chord_60, &chord_61, &chord_62, &chord_63, &chord_64, &chord_65, &chord_66, &chord_67, &chord_68, &chord_69, &chord_70, &chord_71, &chord_72, &chord_73, &chord_74, &chord_75, &chord_76, &chord_77, &chord_78, &chord_79, &chord_80, &chord_81, &chord_82, &chord_83, &chord_84, &chord_85, &chord_86, &chord_87, &chord_88, &chord_89, &chord_90, &chord_91, &chord_92, &chord_93, &chord_94, &chord_95, &chord_96, &chord_97, &chord_98, &chord_99, &chord_100, &chord_101, &chord_102, &chord_103, &chord_104, &chord_105, &chord_106, &chord_107, &chord_108, &chord_109, &chord_110, &chord_111, &chord_112, &chord_113, &chord_114, &chord_115, &chord_116, &chord_117, &chord_118, &chord_119, &chord_120, &chord_121, &chord_122, &chord_123, &chord_124, &chord_125, &chord_126, &chord_127, &chord_128, &chord_129, &chord_130, &chord_131, &chord_132, &chord_133, &chord_134, &chord_135, &chord_136, &chord_137, &chord_138, &chord_139, &chord_140, &chord_141, &chord_142, &chord_143, &chord_144, &chord_145, &chord_146, &chord_147, &chord_148, &chord_149, &chord_150, &chord_151, &chord_152, &chord_153, &chord_154, &chord_155, &chord_156, &chord_157, &chord_158, &chord_159, &chord_160, &chord_161, &chord_162, &chord_163, &chord_164, &chord_165, &chord_166, &chord_167, &chord_168, &chord_169, &chord_170, &chord_171, &chord_172, &chord_173, &chord_174, &chord_175, &chord_176, &chord_177, &chord_178, &chord_179, &chord_180, &chord_181, &chord_182, &chord_183, &chord_184, &chord_185, &chord_186, &chord_187, &chord_188, &chord_189, &chord_190, &chord_191, &chord_192, &chord_193, &chord_194, &chord_195, &chord_196, &chord_197, &chord_198, &chord_199, &chord_200, &chord_201, &chord_202, &chord_203, &chord_204, &chord_205, &chord_206, &chord_207, &chord_208, &chord_209, &chord_210, &chord_211, &chord_212, &chord_213, &chord_214, &chord_215, &chord_216, &chord_217, &chord_218, &chord_219, &chord_220, &chord_221, &chord_222, &chord_223, &chord_224, &chord_225, &chord_226, &chord_227, &chord_228, &chord_229, &chord_230, &chord_231, &chord_232, &chord_233, &chord_234, &chord_235, &chord_236, &chord_237, &chord_238, &chord_239, &chord_240, &chord_241, &chord_242, &chord_243, &chord_244, &chord_245, &chord_246, &chord_247, &chord_248, &chord_249, &chord_250, &chord_251, &chord_252, &chord_253, &chord_254, &chord_255, &chord_256, &chord_257, &chord_258, &chord_259, &chord_260, &chord_261, &chord_262, &chord_263, &chord_264, &chord_265, &chord_266, &chord_267, &chord_268, &chord_269, &chord_270, &chord_271, &chord_272, &chord_273, &chord_274, &chord_275, &chord_276, &chord_277, &chord_278, &chord_279, &chord_280, &chord_281, &chord_282, &chord_283, &chord_284, &chord_285, &chord_286, &chord_287
};

const uint16_t** const leader_triggers PROGMEM = NULL;
void (*leader_functions[]) (void) = {};

#define NUMBER_OF_CHORDS 288
#define NUMBER_OF_LEADER_COMBOS 0

bool are_hashed_keycodes_in_sound(HASH_TYPE keycodes_hash, HASH_TYPE sound) {
    return (keycodes_hash & sound) == keycodes_hash;
}

uint8_t keycode_to_index(uint16_t keycode) {
    return keycode - FIRST_INTERNAL_KEYCODE;
}

void sound_keycode_array(uint16_t keycode) {
    uint8_t index = keycode_to_index(keycode);
    keycode_index++;
    keycodes_buffer_array[index] = keycode_index;
}

void silence_keycode_hash_array(HASH_TYPE keycode_hash) {
    for (int i = 0; i < NUMBER_OF_KEYS; i++) {
        bool index_in_hash = ((HASH_TYPE) 1 << i) & keycode_hash;
        if (index_in_hash) {
            uint8_t current_val = keycodes_buffer_array[i];
            keycodes_buffer_array[i] = 0;
            for (int j = 0; j < NUMBER_OF_KEYS; j++) {
                if (keycodes_buffer_array[j] > current_val) {
                    keycodes_buffer_array[j]--;
                }
            }
            keycode_index--;
        }
    }
}

bool are_hashed_keycodes_in_array(HASH_TYPE keycode_hash) {
    for (int i = 0; i < NUMBER_OF_KEYS; i++) {
        bool index_in_hash = ((HASH_TYPE) 1 << i) & keycode_hash;
        bool index_in_array = (bool) keycodes_buffer_array[i];
        if (index_in_hash && !index_in_array) {
            return false;
        }
    }
    return true;
}

void kill_one_shots(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (*chord->state == IN_ONE_SHOT) {
            *chord->state = RESTART;
            chord->function(chord);
            if (*chord->state == RESTART) {
                *chord->state = IDLE;
            }
        }
    }
}

void process_finished_dances(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (*chord->state == ACTIVATED) {
            *chord->state = PRESS_FROM_ACTIVE;
            chord->function(chord);
            if (a_key_went_through) {
                kill_one_shots();
            }
            dance_timer = timer_read();
        } else if (*chord->state == IDLE_IN_DANCE) {
            *chord->state = FINISHED;
            chord->function(chord);
            if (*chord->state == FINISHED) {
                *chord->state = RESTART;
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
            }
        } else if (*chord->state == PRESS_FROM_ACTIVE) {
            *chord->state = FINISHED_FROM_ACTIVE;
            chord->function(chord);
            if (a_key_went_through) {
                kill_one_shots();
            }
            dance_timer = timer_read();
        }
    }
}

uint8_t keycodes_buffer_array_min(uint8_t* first_keycode_index) {
    for (int i = 0; i < NUMBER_OF_KEYS; i++) {
        if (keycodes_buffer_array[i] == 1) {
            if (first_keycode_index != NULL) {
                *first_keycode_index = (uint8_t) i;
            }
            return 1;
        }
    }
    return 0;
}

void remove_subchords(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {
            continue;
        }
        
        struct Chord chord_storage_2;
        struct Chord* chord_ptr_2;
        struct Chord* chord_2;
        for (int j = 0; j < NUMBER_OF_CHORDS; j++) {
            if (i == j) {continue;}
            
            chord_ptr_2 = (struct Chord*) pgm_read_word (&list_of_chords[j]);
            memcpy_P(&chord_storage_2, chord_ptr_2, sizeof(struct Chord));
            chord_2 = &chord_storage_2;
            
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
}

void process_ready_chords(void) {
    uint8_t first_keycode_index = 0;
    while (keycodes_buffer_array_min(&first_keycode_index)) {
        // find ready chords
        struct Chord chord_storage;
        struct Chord* chord_ptr;
        struct Chord* chord;
        
        for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            chord = &chord_storage;
            
            // if the chord does not contain the first keycode
            bool contains_first_keycode = ((uint32_t) 1 << first_keycode_index) & chord->keycodes_hash;
            if (!contains_first_keycode) {
                continue;
            }
            
            if (!are_hashed_keycodes_in_array(chord->keycodes_hash)){
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
        remove_subchords();
        
        // execute logic
        // this should be only one chord
        for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            chord = &chord_storage;
            
            if (*chord->state == READY_LOCKED) {
                *chord->state = RESTART;
                chord->function(chord);
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
                break;
            }
            
            if (*chord->state == READY || *chord->state == READY_IN_DANCE) {
                if (last_chord && last_chord != chord) {
                    process_finished_dances();
                }
                
                bool lock_next_prev_state = lock_next;
                
                *chord->state = ACTIVATED;
                chord->function(chord);
                dance_timer = timer_read();
                
                if (lock_next && lock_next == lock_next_prev_state) {
                    lock_next = false;
                    *chord->state = PRESS_FROM_ACTIVE;
                    chord->function(chord);
                    if (*chord->state == PRESS_FROM_ACTIVE) {
                        *chord->state = LOCKED;
                    }
                    if (a_key_went_through) {
                        kill_one_shots();
                    }
                }
                break;
            }
        }
        
        // silence notes
        silence_keycode_hash_array(chord->keycodes_hash);
    }
}

void deactivate_active_chords(uint16_t keycode) {
    HASH_TYPE hash = (HASH_TYPE)1 << (keycode - SAFE_RANGE);
    bool broken;
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        broken = are_hashed_keycodes_in_sound(hash, chord->keycodes_hash);
        if (!broken) {
            continue;
        }
        
        switch (*chord->state) {
            case ACTIVATED:
                *chord->state = DEACTIVATED;
                chord->function(chord);
                
                if (*chord->state == DEACTIVATED) {
                    dance_timer = timer_read();
                    *chord->state = IDLE_IN_DANCE;
                }
                if (*chord->state != IN_ONE_SHOT) {
                    kill_one_shots();
                }
                break;
            case PRESS_FROM_ACTIVE:
            case FINISHED_FROM_ACTIVE:
                *chord->state = RESTART;
                chord->function(chord);
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
                kill_one_shots();
                break;
            default:
                break;
        }
    }
    
}

void process_command(void) {
    command_mode = 0;
    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {
        if (command_buffer[i]) {
            register_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    wait_ms(TAP_TIMEOUT);
    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {
        if (command_buffer[i]) {
            unregister_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {
        command_buffer[i] = 0;
    }
    command_ind = 0;
}

void process_leader(void) {
    in_leader_mode = false;
    for (int i = 0; i < NUMBER_OF_LEADER_COMBOS; i++) {
        uint16_t trigger[LEADER_MAX_LENGTH];
        memcpy_P(trigger, leader_triggers[i], LEADER_MAX_LENGTH * sizeof(uint16_t));
        
        if (identical(leader_buffer, trigger)) {
            (*leader_functions[i])();
            break;
        }
    }
    for (int i = 0; i < LEADER_MAX_LENGTH; i++) {
        leader_buffer[i] = 0;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode < FIRST_INTERNAL_KEYCODE || keycode > LAST_INTERNAL_KEYCODE) {
        return true;
    }
    
    if (record->event.pressed) {
        sound_keycode_array(keycode);
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
    if (chord_timer_expired && keycodes_buffer_array_min(NULL)) {
        process_ready_chords();
    }
    
    bool dance_timer_expired = timer_elapsed(dance_timer) > DANCE_TIMEOUT;
    if (dance_timer_expired) { // would love to have && in_dance but not sure how
        process_finished_dances();
    }
    
    bool in_command_mode = command_mode == 2;
    if (in_command_mode) {
        process_command();
    }
    
    bool leader_timer_expired = timer_elapsed(leader_timer) > LEADER_TIMEOUT;
    if (leader_timer_expired && in_leader_mode) {
        process_leader();
    }
    
}

void clear(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        // kill all chords
        struct Chord chord_storage;
        struct Chord* chord_ptr;
        struct Chord* chord;
        
        for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            chord = &chord_storage;
            
            *chord->state = IDLE;
            
            if (chord->counter) {
                *chord->counter = 0;
            }
        }
        
        // clear keyboard
        clear_keyboard();
        send_keyboard_report();
        
        // switch to default pseudolayer
        current_pseudolayer = DEFAULT_PSEUDOLAYER;
        
        // clear all keyboard states
        lock_next = false;
        autoshift_mode = true;
        command_mode = 0;
        in_leader_mode = false;
        leader_ind = 0;
        dynamic_macro_mode = false;
        a_key_went_through = false;
        
        for (int i = 0; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
            dynamic_macro_buffer[i] = 0;
        }
    }
}