#define mu_assert(message, test)     do {         if (!(test)) {             return message;         }     } while (0)
        
#define mu_run_test(test)     do {         char *message = test();         tests_run++;         if (message) {             return message;         }     } while (0)

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

int tests_run = 0;

enum ASSERT_TYPES {
    UINT,
    INT
};

#define BUFF_SIZE 1024
char buffer[BUFF_SIZE];

#define ASSERT_EQ(type, actual, expected)     do {         if (actual != expected) {             switch (type) {                 case UINT:                     snprintf(buffer, BUFF_SIZE, "%s "RED"FAILED"NC"\nline %d\nvar %s\nactual = %u\nexpected = %u\n", name, __LINE__, #actual, actual, expected);                     break;                 case INT:                     snprintf(buffer, BUFF_SIZE, "%s "RED"FAILED"NC"\nline %d\nvar %s\nactual = %d\nexpected = %d\n", name, __LINE__, #actual, actual, expected);                     break;                 default:                     snprintf(buffer, BUFF_SIZE, "%s "RED"FAILED"NC"\nline %d\nunsupported ASSERT_EQ type\n", name, __LINE__);                     break;             }             return buffer;         }     } while (0)

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
const struct Chord* pgm_read_word(const struct Chord* const* chord) {return *chord;}

typedef struct {
    uint8_t col;
    uint8_t row;
} keypos_t;

typedef struct {
    keypos_t key;
    bool     pressed;
    uint16_t time;
} keyevent_t;

typedef struct {
    bool    interrupted :1;
    bool    reserved2   :1;
    bool    reserved1   :1;
    bool    reserved0   :1;
    uint8_t count       :4;
} tap_t;

typedef struct {
    keyevent_t event;
    tap_t tap;
} keyrecord_t;

keyrecord_t pressed = {{{0,0},true,0}, {0,0,0,0,0}};
keyrecord_t depressed = {{{0,0},false,0}, {0,0,0,0,0}};

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
    KC_LEFT,
    KC_DOWN,
    KC_UP,
    KC_RIGHT,
    
    SAFE_RANGE
};


int16_t current_time;
uint8_t keyboard_history[20][SAFE_RANGE-1];
int16_t time_history[20];
uint8_t history_index;

void register_code(int16_t keycode) {
    history_index++;
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[history_index][j] = keyboard_history[history_index-1][j];
    }
    keyboard_history[history_index][keycode] = 1;
    time_history[history_index] = current_time;
};
void unregister_code(int16_t keycode) {
    history_index++;
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[history_index][j] = keyboard_history[history_index-1][j];
    }
    keyboard_history[history_index][keycode] = 0;
    time_history[history_index] = current_time;
};
void send_keyboard_report(void) { /*still don't know what this does*/ };
void matrix_scan_user (void);
void wait_ms(uint16_t ms) {
    current_time += ms;
};
uint16_t timer_read(void) {
    uint16_t result = current_time;
    return result;
};
uint16_t timer_elapsed(uint16_t timer) {
    uint16_t result = current_time - timer;
    return result;
};
void layer_move(int16_t layer) { /*ignoring for now*/ };
void clear_keyboard(void) {
    history_index++;
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[history_index][j] = 0;
    }
    time_history[history_index] = current_time;
};
void reset_keyboard(void) { /*ignoring for now*/ };

void pause_ms(uint16_t ms) {
    for (int i = 0; i < ms; i++) {
        current_time++;
        matrix_scan_user();
    }
};











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
            [0] = LAYOUT_butter(
                                    TOP1,
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
                                    BOT0
                ),
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
    uint8_t* state;
    uint8_t* counter;
    uint16_t value1;
    uint8_t value2;
    void (*function) (const struct Chord*);
};

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
            if (*self->counter == 1) {
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
    for (int i = 0; i < 5; i++) {
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
        for (int i = 0; i < 20; i++) {
            dynamic_macro_buffer[i] = 0;
        }
        dynamic_macro_mode = true;
        *self->state = IDLE;
    }
}

void dynamic_macro_next(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode && dynamic_macro_ind < 20) {
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
        while (ind_start < 20) {
            for (int i = ind_start; i < 20; i++) {
                if (dynamic_macro_buffer[i] == 0) {
                    break;
                }
                register_code(dynamic_macro_buffer[i]);
            }
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            for (int i = ind_start; i < 20; i++) {
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

void fnc_L1(void) {
    key_in(KC_A);
    clear_keyboard();
}

    
    


void fnc_L2(void) {
    key_in(KC_S);
    clear_keyboard();
}

    
    


void double_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            *self->counter = (*self->counter + 1) % 2;
            break;
        case PRESS_FROM_ACTIVE:
            if (*self->counter == 1) {
                key_in(self->value1);
            } else {
                key_in(self->value2);
            }
            *self->state = FINISHED_FROM_ACTIVE;
            break;
        case FINISHED:
            if (*self->counter == 1) {
                tap_key(self->value1);
            } else {
                tap_key(self->value2);
            }
            *self->counter = 0;
            *self->state = IDLE;
            break;
        case RESTART:
            if (*self->counter == 1) {
                key_out(self->value1);
            } else {
                key_out(self->value2);
            }
            *self->counter = 0;
            break;
        default:
            break;
    }
}


    
    
    
    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_0 = IDLE;
    const struct Chord chord_0 PROGMEM = {H_TOP1, QWERTY, &state_0, NULL, KC_Q, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_1 = IDLE;
    const struct Chord chord_1 PROGMEM = {H_TOP2, QWERTY, &state_1, NULL, KC_W, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_2 = IDLE;
    const struct Chord chord_2 PROGMEM = {H_TOP3, QWERTY, &state_2, NULL, KC_E, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_3 = IDLE;
    const struct Chord chord_3 PROGMEM = {H_TOP4, QWERTY, &state_3, NULL, KC_R, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_4 = IDLE;
    const struct Chord chord_4 PROGMEM = {H_TOP5, QWERTY, &state_4, NULL, KC_T, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_5 = IDLE;
    const struct Chord chord_5 PROGMEM = {H_TOP6, QWERTY, &state_5, NULL, KC_Y, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_6 = IDLE;
    const struct Chord chord_6 PROGMEM = {H_TOP7, QWERTY, &state_6, NULL, KC_U, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_7 = IDLE;
    const struct Chord chord_7 PROGMEM = {H_TOP8, QWERTY, &state_7, NULL, KC_I, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_8 = IDLE;
    const struct Chord chord_8 PROGMEM = {H_TOP9, QWERTY, &state_8, NULL, KC_O, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_9 = IDLE;
    const struct Chord chord_9 PROGMEM = {H_TOP0, QWERTY, &state_9, NULL, KC_P, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_10 = IDLE;
    const struct Chord chord_10 PROGMEM = {H_TOP1 + H_BOT1, QWERTY, &state_10, NULL, KC_A, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_11 = IDLE;
    const struct Chord chord_11 PROGMEM = {H_TOP2 + H_BOT2, QWERTY, &state_11, NULL, KC_S, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_12 = IDLE;
    const struct Chord chord_12 PROGMEM = {H_TOP3 + H_BOT3, QWERTY, &state_12, NULL, KC_D, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_13 = IDLE;
    const struct Chord chord_13 PROGMEM = {H_TOP4 + H_BOT4, QWERTY, &state_13, NULL, KC_F, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_14 = IDLE;
    const struct Chord chord_14 PROGMEM = {H_TOP5 + H_BOT5, QWERTY, &state_14, NULL, KC_G, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_15 = IDLE;
    const struct Chord chord_15 PROGMEM = {H_TOP6 + H_BOT6, QWERTY, &state_15, NULL, KC_H, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_16 = IDLE;
    const struct Chord chord_16 PROGMEM = {H_TOP7 + H_BOT7, QWERTY, &state_16, NULL, KC_J, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_17 = IDLE;
    const struct Chord chord_17 PROGMEM = {H_TOP8 + H_BOT8, QWERTY, &state_17, NULL, KC_K, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_18 = IDLE;
    const struct Chord chord_18 PROGMEM = {H_TOP9 + H_BOT9, QWERTY, &state_18, NULL, KC_L, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_19 = IDLE;
    const struct Chord chord_19 PROGMEM = {H_TOP0 + H_BOT0, QWERTY, &state_19, NULL, KC_SCOLON, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
                
    
    uint8_t state_20 = IDLE;
    uint8_t counter_20 = 0;
    const struct Chord chord_20 PROGMEM = {H_BOT1, QWERTY, &state_20, &counter_20, KC_Z, 0, autoshift_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
                
                
                
                
    
    uint8_t state_21 = IDLE;
    const struct Chord chord_21 PROGMEM = {H_BOT2, QWERTY, &state_21, NULL, KC_X, KC_LCTL, key_key_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
                
                
                
    
    uint8_t state_22 = IDLE;
    const struct Chord chord_22 PROGMEM = {H_BOT3, QWERTY, &state_22, NULL, KC_C, NUM, key_layer_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
                
                
                
                
    
    uint8_t state_23 = IDLE;
    const struct Chord chord_23 PROGMEM = {H_BOT4, QWERTY, &state_23, NULL, KC_V, KC_LALT, key_mod_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_24 = IDLE;
    const struct Chord chord_24 PROGMEM = {H_BOT5, QWERTY, &state_24, NULL, KC_B, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_25 = IDLE;
    const struct Chord chord_25 PROGMEM = {H_BOT6, QWERTY, &state_25, NULL, KC_N, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_26 = IDLE;
    const struct Chord chord_26 PROGMEM = {H_BOT7, QWERTY, &state_26, NULL, KC_M, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_27 = IDLE;
    const struct Chord chord_27 PROGMEM = {H_BOT8, QWERTY, &state_27, NULL, KC_COMMA, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_28 = IDLE;
    const struct Chord chord_28 PROGMEM = {H_BOT9, QWERTY, &state_28, NULL, KC_DOT, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
                
                
                
    
    uint8_t state_29 = IDLE;
    uint8_t counter_29 = 0;
    const struct Chord chord_29 PROGMEM = {H_BOT0, QWERTY, &state_29, &counter_29, KC_9, KC_0, double_dance};
    

            
        
    




    
    
    
    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_30 = IDLE;
    const struct Chord chord_30 PROGMEM = {H_TOP1 + H_TOP2, QWERTY, &state_30, NULL, KC_ESC, 0, single_dance};
    

            
        
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_31 = IDLE;
    const struct Chord chord_31 PROGMEM = {H_TOP1 + H_BOT1 + H_TOP2 + H_BOT2, QWERTY, &state_31, NULL, 0, 0, lock};
    

            
        
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_32 = IDLE;
    const struct Chord chord_32 PROGMEM = {H_TOP2 + H_BOT2 + H_TOP3 + H_BOT3, QWERTY, &state_32, NULL, 0, 0, leader};
    

            
        
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_33 = IDLE;
    const struct Chord chord_33 PROGMEM = {H_TOP5 + H_BOT5 + H_TOP6 + H_BOT6, QWERTY, &state_33, NULL, 0, 0, command};
    

            
        
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_34 = IDLE;
    const struct Chord chord_34 PROGMEM = {H_BOT1 + H_BOT2, QWERTY, &state_34, NULL, KC_LSFT, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
                
    
    
        
    
    uint8_t state_35 = IDLE;
    const struct Chord chord_35 PROGMEM = {H_BOT2 + H_BOT3, QWERTY, &state_35, NULL, KC_LSFT, 0, one_shot_key};
    

    

            
        
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
        
        
        
            
            
                
                
    
    
        
    
    uint8_t state_36 = IDLE;
    const struct Chord chord_36 PROGMEM = {H_BOT6 + H_BOT7, QWERTY, &state_36, NULL, NUM, 0, one_shot_layer};
    

    

            
        
    

    
    
    
    
        
        
        
            
            
                
    
    uint8_t state_37 = IDLE;
    const struct Chord chord_37 PROGMEM = {H_BOT7 + H_BOT8, QWERTY, &state_37, NULL, NUM, 0, temp_pseudolayer};
    

            
        
    

    
    
    
    
    

    
    
    
    
        
        
        
            
            
                
    
    uint8_t state_38 = IDLE;
    const struct Chord chord_38 PROGMEM = {H_BOT9 + H_BOT0, QWERTY, &state_38, NULL, NUM, 0, perm_pseudolayer};
    

            
        
    




    
    
    
    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_39 = IDLE;
    const struct Chord chord_39 PROGMEM = {H_TOP1, NUM, &state_39, NULL, KC_1, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_40 = IDLE;
    const struct Chord chord_40 PROGMEM = {H_TOP2, NUM, &state_40, NULL, KC_2, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_41 = IDLE;
    const struct Chord chord_41 PROGMEM = {H_TOP3, NUM, &state_41, NULL, KC_3, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_42 = IDLE;
    const struct Chord chord_42 PROGMEM = {H_TOP4, NUM, &state_42, NULL, KC_4, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_43 = IDLE;
    const struct Chord chord_43 PROGMEM = {H_TOP5, NUM, &state_43, NULL, KC_5, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_44 = IDLE;
    const struct Chord chord_44 PROGMEM = {H_TOP6, NUM, &state_44, NULL, KC_6, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_45 = IDLE;
    const struct Chord chord_45 PROGMEM = {H_TOP7, NUM, &state_45, NULL, KC_7, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_46 = IDLE;
    const struct Chord chord_46 PROGMEM = {H_TOP8, NUM, &state_46, NULL, KC_8, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_47 = IDLE;
    const struct Chord chord_47 PROGMEM = {H_TOP9, NUM, &state_47, NULL, KC_9, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_48 = IDLE;
    const struct Chord chord_48 PROGMEM = {H_TOP0, NUM, &state_48, NULL, KC_0, 0, single_dance};
    

            
        
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
        
        
        
            
            
                
    
    
    void function_49(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                                    
                    
                    key_in(KC_LCTL);
                                    
                    
                    key_in(KC_LSFT);
                                break;
            case DEACTIVATED:
                                    
                    
                    key_out(KC_LCTL);
                                    
                    
                    key_out(KC_LSFT);
                                *self->state = IDLE;
                break;
            case RESTART:
                                    
                    
                    key_out(KC_LCTL);
                                    
                    
                    key_out(KC_LSFT);
                                break;
            default:
                break;
        }
    }
    uint8_t state_49 = IDLE;
    uint8_t counter_49 = 0;
    const struct Chord chord_49 PROGMEM = {H_BOT1, NUM, &state_49, &counter_49, 0, 0, function_49};
    

            
        
    

    
    
    
    
        
        
        
            
                
                
    
    uint8_t state_50 = IDLE;
    const struct Chord chord_50 PROGMEM = {H_BOT2, NUM, &state_50, NULL, KC_LEFT, 0, single_dance};
    

            
        
    

    
    
    
    
        
        
        
            
            
                
    
    
    void function_51(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                *self->counter = *self->counter + 1;
                break;
            case PRESS_FROM_ACTIVE:
                switch (*self->counter) {
                                            
                        case 1:
                            
                            key_in(KC_1);
                            break;
                                            
                        case 2:
                            
                            key_in(KC_2);
                            break;
                                            
                        case 3:
                            
                            key_in(KC_3);
                            break;
                                        default:
                        break;
                }
                *self->state = FINISHED_FROM_ACTIVE;
                break;
            case FINISHED:
                switch (*self->counter) {
                                            
                        case 1:
                            
                            tap_key(KC_1);
                            break;
                                            
                        case 2:
                            
                            tap_key(KC_2);
                            break;
                                            
                        case 3:
                            
                            tap_key(KC_3);
                            break;
                                        default:
                        break;
                }
                *self->counter = 0;
                *self->state = IDLE;
                break;
            case RESTART:
                switch (*self->counter) {
                                            
                        case 1:
                            
                            key_out(KC_1);
                            break;
                                            
                        case 2:
                            
                            key_out(KC_2);
                            break;
                                            
                        case 3:
                            
                            key_out(KC_3);
                            break;
                                        default:
                        break;
                }
                *self->counter = 0;
                break;
            default:
                break;
        }
    }
    uint8_t state_51 = IDLE;
    uint8_t counter_51 = 0;
    const struct Chord chord_51 PROGMEM = {H_BOT3, NUM, &state_51, &counter_51, 0, 0, function_51};
    

            
        
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_52 = IDLE;
    const struct Chord chord_52 PROGMEM = {H_BOT7, NUM, &state_52, NULL, 0, 0, dynamic_macro_record};
    

            
        
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_53 = IDLE;
    const struct Chord chord_53 PROGMEM = {H_BOT8, NUM, &state_53, NULL, 0, 0, dynamic_macro_next};
    

            
        
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_54 = IDLE;
    const struct Chord chord_54 PROGMEM = {H_BOT9, NUM, &state_54, NULL, 0, 0, dynamic_macro_end};
    

            
        
    

    
    
    
    
        
        
        
            
                
    
    uint8_t state_55 = IDLE;
    const struct Chord chord_55 PROGMEM = {H_BOT0, NUM, &state_55, NULL, 0, 0, dynamic_macro_play};
    

            
        
    



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
    
};


const uint16_t leader_triggers[2][5] PROGMEM = {
            {KC_O, KC_P, 0, 0, 0},
            {KC_P, KC_O, 0, 0, 0},
    
};


void (*leader_functions[]) (void) = {
            fnc_L1,
            fnc_L2,
    
};

struct Chord* last_chord = NULL;

bool are_hashed_keycodes_in_sound(uint32_t keycodes_hash, uint32_t sound) {
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

void silence_keycode_hash_array(uint32_t keycode_hash) {
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

bool are_hashed_keycodes_in_array(uint32_t keycode_hash) {
    for (int i = 0; i < 20; i++) {
        bool index_in_hash = ((uint32_t) 1 << i) & keycode_hash;
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
    
    for (int i = 0; i < 56; i++) {
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
    
    for (int i = 0; i < 56; i++) {
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

void remove_subchords(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < 56; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {
            continue;
        }
        
        struct Chord chord_storage_2;
        struct Chord* chord_ptr_2;
        struct Chord* chord_2;
        for (int j = 0; j < 56; j++) {
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
        
        for (int i = 0; i < 56; i++) {
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
        for (int i = 0; i < 56; i++) {
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
    uint32_t hash = (uint32_t)1 << (keycode - SAFE_RANGE);
    bool broken;
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < 56; i++) {
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
    in_leader_mode = false;
    for (int i = 0; i < 2; i++) {
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

// for now here
void clear(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        // kill all chords
        struct Chord chord_storage;
        struct Chord* chord_ptr;
        struct Chord* chord;
        
        for (int i = 0; i < 56; i++) {
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
        current_pseudolayer = 1;
        
        // clear all keyboard states
        lock_next = false;
        autoshift_mode = true;
        command_mode = 0;
        in_leader_mode = false;
        leader_ind = 0;
        dynamic_macro_mode = false;
        a_key_went_through = false;

        for (int i = 0; i < 20; i++) {
            dynamic_macro_buffer[i] = 0;
        }

    }
}

// CLEAR_KB

    
    
    
    static char * test_clear() {     char name[] = "clear";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    for (int i = 0; i < 56; i++) {
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        *chord->state = READY;
        
        if (chord->counter) {
            *chord->counter = 1;
        }
    }
    
    history_index++;
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[history_index][j] = 1;
    }
    
    current_pseudolayer = 5;
    lock_next = true;
    autoshift_mode = false;
    command_mode = 1;
    in_leader_mode = true;
    dynamic_macro_mode = true;
    a_key_went_through = true;

    for (int i = 0; i < 20; i++) {
        dynamic_macro_buffer[i] = 1;
    }
    
    uint8_t clear_state = ACTIVATED;
    struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
    clear_chord.function(&clear_chord);
    
    for (int i = 0; i < 56; i++) {
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        ASSERT_EQ(UINT, *chord->state, IDLE);
        
        if (chord->counter) {
            ASSERT_EQ(UINT, *chord->counter, 0);
        }
    }
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        ASSERT_EQ(UINT, keyboard_history[history_index][j], 0);
    }
    
    ASSERT_EQ(UINT, current_pseudolayer, 1);
    ASSERT_EQ(UINT, lock_next, false);
    ASSERT_EQ(UINT, autoshift_mode, true);
    ASSERT_EQ(UINT, command_mode, 0);
    ASSERT_EQ(UINT, in_leader_mode, false);
    ASSERT_EQ(UINT, leader_ind, 0);
    ASSERT_EQ(UINT, dynamic_macro_mode, false);
    ASSERT_EQ(UINT, a_key_went_through, false);

    for (int i = 0; i < 20; i++) {
        ASSERT_EQ(UINT, dynamic_macro_buffer[i], 0);
    }

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_pause_ms() {     char name[] = "pause_ms";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    pause_ms(500);
    ASSERT_EQ(UINT, current_time, 500);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// KC

    
    
    
    static char * test_single_dance_held_states() {     char name[] = "single_dance_held_states";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, state_0, IDLE);
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT);
    ASSERT_EQ(UINT, state_0, IDLE);
    pause_ms(1);
    ASSERT_EQ(UINT, state_0, ACTIVATED);
    pause_ms(DANCE_TIMEOUT);
    ASSERT_EQ(UINT, state_0, ACTIVATED);
    pause_ms(1);
    ASSERT_EQ(UINT, state_0, PRESS_FROM_ACTIVE);
    pause_ms(DANCE_TIMEOUT);
    ASSERT_EQ(UINT, state_0, PRESS_FROM_ACTIVE);
    pause_ms(1);
    ASSERT_EQ(UINT, state_0, FINISHED_FROM_ACTIVE);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, state_0, IDLE);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_single_dance_held_codes() {     char name[] = "single_dance_held_codes";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    pause_ms(1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    pause_ms(DANCE_TIMEOUT);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    pause_ms(1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    pause_ms(DANCE_TIMEOUT);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    pause_ms(1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_single_dance_tapped_states() {     char name[] = "single_dance_tapped_states";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, state_0, IDLE);
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT);
    ASSERT_EQ(UINT, state_0, IDLE);
    pause_ms(1);
    ASSERT_EQ(UINT, state_0, ACTIVATED);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, state_0, IDLE);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_single_dance_tapped_codes() {     char name[] = "single_dance_tapped_codes";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    pause_ms(1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// I can not actually track the states if the tap is faster than chord timeout


    
    
    
    static char * test_single_dance_tapped_fast_codes() {     char name[] = "single_dance_tapped_fast_codes";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, state_0, IDLE);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[0][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_Q], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_subchords_are_ignored() {     char name[] = "subchords_are_ignored";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, state_0, IDLE);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_W], 0);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_ESC], 1);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_multiple_chords_at_once() {     char name[] = "multiple_chords_at_once";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, state_0, IDLE);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_E], 1);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// MO

    
    
    
    static char * test_momentary_layer() {     char name[] = "momentary_layer";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT7, &pressed);
    pause_ms(1);
    process_record_user(BOT8, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    process_record_user(BOT7, &depressed);
    pause_ms(1);
    process_record_user(BOT8, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_momentary_layer_reset() {     char name[] = "momentary_layer_reset";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT7, &pressed);
    pause_ms(1);
    process_record_user(BOT8, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    pause_ms(DANCE_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    process_record_user(BOT7, &depressed);
    pause_ms(1);
    process_record_user(BOT8, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// DF

    
    
    
    static char * test_permanent_layer() {     char name[] = "permanent_layer";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT9, &pressed);
    pause_ms(1);
    process_record_user(BOT0, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    process_record_user(BOT9, &depressed);
    pause_ms(1);
    process_record_user(BOT0, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    pause_ms(1000);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// AT

    
    
    
    static char * test_autoshift_toggle() {     char name[] = "autoshift_toggle";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, autoshift_mode, 1);
    uint8_t state = ACTIVATED;
    struct Chord chord PROGMEM = {0, QWERTY, &state, NULL, 0, 0, autoshift_toggle};
    chord.function(&chord);
    ASSERT_EQ(UINT, autoshift_mode, 0);
    state = ACTIVATED;
    chord.function(&chord);
    ASSERT_EQ(UINT, autoshift_mode, 1);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// AS

    
    
    
    static char * test_autoshift_tap() {     char name[] = "autoshift_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(BOT1, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_Z], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LSFT], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_autoshift_hold() {     char name[] = "autoshift_hold";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    
    process_record_user(BOT1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(3 * (DANCE_TIMEOUT + 1));
    process_record_user(BOT1, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_Z], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[4][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LSFT], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_autoshift_hold_off() {     char name[] = "autoshift_hold_off";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    autoshift_mode = 0;
    
    process_record_user(BOT1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(3 * (DANCE_TIMEOUT + 1));
    process_record_user(BOT1, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_Z], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_Z], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LSFT], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// LOCK

    
    
    
    static char * test_lock() {     char name[] = "lock";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT1, &pressed);
    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    process_record_user(BOT1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    pause_ms(1);
    process_record_user(BOT2, &depressed);
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    process_record_user(TOP2, &pressed);
    process_record_user(BOT1, &pressed);
    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    process_record_user(TOP2, &depressed);
    process_record_user(BOT1, &depressed);
    process_record_user(BOT2, &depressed);
    pause_ms(1);
    process_record_user(BOT1, &pressed);
    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    process_record_user(BOT1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    pause_ms(1);
    process_record_user(BOT2, &depressed);
    pause_ms(1000);
    process_record_user(BOT1, &pressed);
    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT1, &depressed);
    pause_ms(1);
    process_record_user(BOT2, &depressed);
    pause_ms(1000);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// OSK

    
    
    
    static char * test_one_shot_key_tap() {     char name[] = "one_shot_key_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT2, &pressed);
    process_record_user(BOT3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT2, &depressed);
    pause_ms(1);
    process_record_user(BOT3, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    pause_ms(1000);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_one_shot_key_hold() {     char name[] = "one_shot_key_hold";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT2, &pressed);
    process_record_user(BOT3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(DANCE_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    
    process_record_user(BOT2, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_one_shot_key_retrotapping() {     char name[] = "one_shot_key_retrotapping";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT2, &pressed);
    process_record_user(BOT3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(DANCE_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);
    
    pause_ms(1000);
    
    process_record_user(BOT2, &depressed);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 1);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// OSL

    
    
    
    static char * test_one_shot_layer_tap() {     char name[] = "one_shot_layer_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT6, &pressed);
    process_record_user(BOT7, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT6, &depressed);
    pause_ms(1);
    process_record_user(BOT7, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    pause_ms(1000);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_one_shot_layer_hold() {     char name[] = "one_shot_layer_hold";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT6, &pressed);
    process_record_user(BOT7, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(DANCE_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    
    process_record_user(BOT6, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_one_shot_layer_retrotapping() {     char name[] = "one_shot_layer_retrotapping";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT6, &pressed);
    process_record_user(BOT7, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(DANCE_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    
    pause_ms(1000);
    
    process_record_user(BOT6, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// CMD

    
    
    
    static char * test_command_mode() {     char name[] = "command_mode";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    // start recording
    process_record_user(TOP5, &pressed);
    process_record_user(TOP6, &pressed);
    process_record_user(BOT5, &pressed);
    process_record_user(BOT6, &pressed);
    pause_ms(1);
    process_record_user(TOP5, &depressed);
    process_record_user(TOP6, &depressed);
    process_record_user(BOT5, &depressed);
    process_record_user(BOT6, &depressed);
    
    ASSERT_EQ(UINT, command_mode, 1);
    
    // record shift+q
    process_record_user(BOT1, &pressed);
    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(BOT1, &depressed);
    process_record_user(BOT2, &depressed);
    pause_ms(1000);
    
    process_record_user(TOP1, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    process_record_user(TOP1, &depressed);
    pause_ms(1000);
    
    
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[history_index][KC_LSFT], 0);
    // execute
    process_record_user(TOP5, &pressed);
    process_record_user(TOP6, &pressed);
    process_record_user(BOT5, &pressed);
    process_record_user(BOT6, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    
    ASSERT_EQ(UINT, command_mode, 0);
    
    // test history
    ASSERT_EQ(UINT, keyboard_history[0][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[3][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[4][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[5][KC_Q], 255);
    ASSERT_EQ(UINT, keyboard_history[5][KC_LSFT], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// KK

    
    
    
    static char * test_key_key_dance_tap() {     char name[] = "key_key_dance_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    process_record_user(BOT2, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_X], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_LCTL], 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_X], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LCTL], 0);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_X], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LCTL], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_key_dance_hold() {     char name[] = "key_key_dance_hold";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    
    process_record_user(BOT2, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    pause_ms(DANCE_TIMEOUT + 1);
    process_record_user(BOT2, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_X], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_LCTL], 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_X], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LCTL], 1);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_X], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LCTL], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// KL

    
    
    
    static char * test_key_layer_tap() {     char name[] = "key_layer_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    process_record_user(BOT3, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_C], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_C], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_C], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_C], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_layer_retrotapping() {     char name[] = "key_layer_retrotapping";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT3, &pressed);
    pause_ms(1000);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    process_record_user(BOT3, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_C], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_C], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_C], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_C], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_layer_hold_quick_typist() {     char name[] = "key_layer_hold_quick_typist";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    pause_ms(1);
    
    process_record_user(BOT3, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[6][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[7][KC_1], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_layer_hold_slow_typist() {     char name[] = "key_layer_hold_slow_typist";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    process_record_user(BOT3, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    
    pause_ms(1000);
    process_record_user(TOP1, &pressed);
    pause_ms(1000);
    process_record_user(TOP1, &depressed);
    pause_ms(1000);
    process_record_user(TOP1, &pressed);
    pause_ms(1000);
    process_record_user(TOP1, &depressed);
    pause_ms(1000);
    process_record_user(TOP1, &pressed);
    pause_ms(1000);
    process_record_user(TOP1, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, NUM);
    pause_ms(1);
    
    process_record_user(BOT3, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[6][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[7][KC_1], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// KM

    
    
    
    static char * test_key_mod_tap() {     char name[] = "key_mod_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[0][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_V], 0);
    process_record_user(BOT4, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_V], 0);
    process_record_user(BOT4, &depressed);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_V], 0);
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_V], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_LALT], 255);
    ASSERT_EQ(UINT, keyboard_history[5][KC_V], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_mod_retrotapping() {     char name[] = "key_mod_retrotapping";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    ASSERT_EQ(UINT, keyboard_history[0][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_V], 0);
    process_record_user(BOT4, &pressed);
    pause_ms(1000);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_V], 0);
    process_record_user(BOT4, &depressed);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_V], 0);
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_V], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_LALT], 255);
    ASSERT_EQ(UINT, keyboard_history[5][KC_V], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_mod_hold_quick_typist() {     char name[] = "key_mod_hold_quick_typist";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT4, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    
    process_record_user(BOT4, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[3][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[5][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[6][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[6][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[7][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[7][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[8][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[8][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[9][KC_LALT], 255);
    ASSERT_EQ(UINT, keyboard_history[9][KC_V], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_key_mod_hold_slow_typist() {     char name[] = "key_mod_hold_slow_typist";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT4, &pressed);
    pause_ms(CHORD_TIMEOUT + 1);
    
    pause_ms(1000);
    process_record_user(TOP1, &pressed);
    pause_ms(1000);
    process_record_user(TOP1, &depressed);
    pause_ms(1000);
    process_record_user(TOP1, &pressed);
    pause_ms(1000);
    process_record_user(TOP1, &depressed);
    pause_ms(1000);
    process_record_user(TOP1, &pressed);
    pause_ms(1000);
    process_record_user(TOP1, &depressed);
    pause_ms(1000);
    
    process_record_user(BOT4, &depressed);
    ASSERT_EQ(UINT, current_pseudolayer, QWERTY);
    
    ASSERT_EQ(UINT, keyboard_history[0][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[0][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[3][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[3][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[5][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[5][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[6][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[6][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[6][KC_Q], 1);
    ASSERT_EQ(UINT, keyboard_history[7][KC_LALT], 1);
    ASSERT_EQ(UINT, keyboard_history[7][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[7][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[8][KC_LALT], 0);
    ASSERT_EQ(UINT, keyboard_history[8][KC_V], 0);
    ASSERT_EQ(UINT, keyboard_history[8][KC_Q], 0);
    ASSERT_EQ(UINT, keyboard_history[9][KC_LALT], 255);
    ASSERT_EQ(UINT, keyboard_history[9][KC_V], 255);
    ASSERT_EQ(UINT, keyboard_history[9][KC_Q], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// LEADER

    
    
    
    static char * test_leader_triggers_global() {     char name[] = "leader_triggers_global";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    uint8_t state = ACTIVATED;
    struct Chord chord PROGMEM = {0, QWERTY, &state, NULL, 0, 0, leader};
    chord.function(&chord);

    ASSERT_EQ(UINT, in_leader_mode, 1);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_leader_no_follow() {     char name[] = "leader_no_follow";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    uint8_t state = ACTIVATED;
    struct Chord chord PROGMEM = {0, QWERTY, &state, NULL, 0, 0, leader};
    chord.function(&chord);
    
    ASSERT_EQ(UINT, in_leader_mode, 1);

    pause_ms(1000);
    
    ASSERT_EQ(UINT, in_leader_mode, 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_O], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_leader_wrong_follow() {     char name[] = "leader_wrong_follow";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(TOP2, &pressed);
    process_record_user(TOP3, &pressed);
    process_record_user(BOT2, &pressed);
    process_record_user(BOT3, &pressed);
    pause_ms(1);
    process_record_user(TOP2, &depressed);
    process_record_user(TOP3, &depressed);
    process_record_user(BOT2, &depressed);
    process_record_user(BOT3, &depressed);

    ASSERT_EQ(UINT, in_leader_mode, 1);

    pause_ms(1);
    process_record_user(TOP1, &pressed);
    pause_ms(1);
    process_record_user(TOP1, &depressed);
    pause_ms(1);
    process_record_user(TOP2, &pressed);
    pause_ms(1);
    process_record_user(TOP2, &depressed);
    
    pause_ms(LEADER_TIMEOUT);
    pause_ms(1);
    
    ASSERT_EQ(UINT, in_leader_mode, 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_Q], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_leader_correct_follow() {     char name[] = "leader_correct_follow";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(TOP2, &pressed);
    process_record_user(TOP3, &pressed);
    process_record_user(BOT2, &pressed);
    process_record_user(BOT3, &pressed);
    pause_ms(1);
    process_record_user(TOP2, &depressed);
    process_record_user(TOP3, &depressed);
    process_record_user(BOT2, &depressed);
    process_record_user(BOT3, &depressed);

    ASSERT_EQ(UINT, in_leader_mode, 1);

    pause_ms(1);
    process_record_user(TOP0, &pressed);
    pause_ms(1);
    process_record_user(TOP0, &depressed);
    pause_ms(1);
    process_record_user(TOP9, &pressed);
    pause_ms(1);
    process_record_user(TOP9, &depressed);
    
    pause_ms(LEADER_TIMEOUT);
    ASSERT_EQ(UINT, in_leader_mode, 1);
    
    pause_ms(1);    
    ASSERT_EQ(UINT, in_leader_mode, 0);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_O], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_P], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_A], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_S], 1);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_O], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_P], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_A], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_S], 0);
    
    ASSERT_EQ(UINT, keyboard_history[5][KC_Q], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// DYNAMIC MACRO

    
    
    
    static char * test_dynamic_macro_record_mode() {     char name[] = "dynamic_macro_record_mode";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    // record
    ASSERT_EQ(UINT, dynamic_macro_mode, 0);
    process_record_user(BOT7, &pressed);
    process_record_user(BOT7, &depressed);
    ASSERT_EQ(UINT, dynamic_macro_mode, 1);
    pause_ms(1000);
    ASSERT_EQ(UINT, dynamic_macro_mode, 1);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dynamic_macro_record_mode_off() {     char name[] = "dynamic_macro_record_mode_off";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT7, &pressed);
    process_record_user(BOT7, &depressed);
    ASSERT_EQ(UINT, dynamic_macro_mode, 1);
    
    process_record_user(BOT9, &pressed);
    process_record_user(BOT9, &depressed);
    ASSERT_EQ(UINT, dynamic_macro_mode, 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dynamic_macro_record_one() {     char name[] = "dynamic_macro_record_one";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT7, &pressed);
    process_record_user(BOT7, &depressed);
    ASSERT_EQ(UINT, dynamic_macro_mode, 1);
    
    process_record_user(TOP1, &pressed);
    process_record_user(TOP1, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 255);
    
    process_record_user(BOT9, &pressed);
    process_record_user(BOT9, &depressed);
    
    pause_ms(1000);
    
    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 255);
    
    pause_ms(1000);
    
    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dynamic_macro_record_two() {     char name[] = "dynamic_macro_record_two";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT7, &pressed);
    process_record_user(BOT7, &depressed);
    ASSERT_EQ(UINT, dynamic_macro_mode, 1);
    
    process_record_user(TOP1, &pressed);
    process_record_user(TOP1, &depressed);
    
    process_record_user(TOP2, &pressed);
    process_record_user(TOP2, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 255);
    
    process_record_user(BOT9, &pressed);
    process_record_user(BOT9, &depressed);
    
    pause_ms(1000);
    
    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dynamic_macro_record_two_parts() {     char name[] = "dynamic_macro_record_two_parts";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT7, &pressed);
    process_record_user(BOT7, &depressed);
    ASSERT_EQ(UINT, dynamic_macro_mode, 1);
    
    process_record_user(TOP1, &pressed);
    process_record_user(TOP1, &depressed);
    
    process_record_user(TOP2, &pressed);
    process_record_user(TOP2, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 255);
    
    process_record_user(BOT8, &pressed);
    process_record_user(BOT8, &depressed);
    
    process_record_user(TOP3, &pressed);
    process_record_user(TOP3, &depressed);
    
    process_record_user(BOT9, &pressed);
    process_record_user(BOT9, &depressed);
    
    pause_ms(1000);
    
    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_3], 0);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_3], 0);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[3][KC_3], 0);
    
    ASSERT_EQ(UINT, keyboard_history[4][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_3], 0);
    
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_3], 1);
    
    ASSERT_EQ(UINT, keyboard_history[6][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[6][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[6][KC_3], 0);
    
    ASSERT_EQ(UINT, keyboard_history[7][KC_1], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// dance + M()


    
    
    
    static char * test_dance_tap() {     char name[] = "dance_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_9], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_0], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_9], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_0], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_9], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dance_hold() {     char name[] = "dance_hold";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT0, &pressed);
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_9], 1);
    
    process_record_user(BOT0, &depressed);
    ASSERT_EQ(UINT, keyboard_history[2][KC_9], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dance_tap_tap() {     char name[] = "dance_tap_tap";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_9], 0);
    ASSERT_EQ(UINT, keyboard_history[1][KC_0], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_9], 0);
    ASSERT_EQ(UINT, keyboard_history[2][KC_0], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_9], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dance_tap_hold() {     char name[] = "dance_tap_hold";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    process_record_user(BOT0, &pressed);
    process_record_user(BOT0, &depressed);
    pause_ms(1);
    process_record_user(BOT0, &pressed);
    pause_ms(1000);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_0], 1);
    
    process_record_user(BOT0, &depressed);
    ASSERT_EQ(UINT, keyboard_history[2][KC_0], 0);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// MK

    
    
    
    static char * test_multiple_keys() {     char name[] = "multiple_keys";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT1, &pressed);
    process_record_user(BOT1, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_LCTL], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_LCTL], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_LCTL], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[4][KC_LCTL], 0);
    ASSERT_EQ(UINT, keyboard_history[4][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[5][KC_LCTL], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_multiple_keys_interleaved() {     char name[] = "multiple_keys_interleaved";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT1, &pressed);
    pause_ms(CHORD_TIMEOUT+1);
    
    process_record_user(TOP1, &pressed);
    process_record_user(TOP1, &depressed);
    process_record_user(TOP1, &pressed);
    process_record_user(TOP1, &depressed);
    process_record_user(TOP1, &pressed);
    process_record_user(TOP1, &depressed);
    
    process_record_user(BOT1, &depressed);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_LCTL], 1);
    ASSERT_EQ(UINT, keyboard_history[1][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[2][KC_LCTL], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[6][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[7][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[8][KC_1], 0);
    
    ASSERT_EQ(UINT, keyboard_history[9][KC_LCTL], 0);
    ASSERT_EQ(UINT, keyboard_history[9][KC_LSFT], 1);
    
    ASSERT_EQ(UINT, keyboard_history[10][KC_LCTL], 0);
    ASSERT_EQ(UINT, keyboard_history[10][KC_LSFT], 0);
    
    ASSERT_EQ(UINT, keyboard_history[11][KC_LCTL], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// D

    
    
    
    static char * test_dance_one() {     char name[] = "dance_one";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 255);
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_1], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_1], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_1], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dance_two() {     char name[] = "dance_two";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_2], 255);
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_2], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dance_three() {     char name[] = "dance_three";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_3], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_3], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_3], 255);
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_3], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_3], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_3], 1);
    ASSERT_EQ(UINT, keyboard_history[4][KC_3], 0);
    ASSERT_EQ(UINT, keyboard_history[5][KC_3], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}



    
    
    
    static char * test_dance_two_held() {     char name[] = "dance_two_held";     
    do {
        uint8_t clear_state = ACTIVATED;
        struct Chord clear_chord PROGMEM = {0, QWERTY, &clear_state, NULL, 0, 0, clear};
        clear_chord.function(&clear_chord);
    } while (0);
    
    current_time = 0;
    history_index = 0;
    
    for (int j = 0; j < SAFE_RANGE-1; j++) {
        keyboard_history[0][j] = 0;
    }
    time_history[0] = 0;
    for (int i = 1; i < 20; i++) {
        for (int j = 0; j < SAFE_RANGE-1; j++) {
            keyboard_history[i][j] = -1;
        }
        time_history[i] = -1;
    }

    current_pseudolayer = NUM;
    
    process_record_user(BOT3, &pressed);
    process_record_user(BOT3, &depressed);
    process_record_user(BOT3, &pressed);
    
    pause_ms(CHORD_TIMEOUT+DANCE_TIMEOUT+2);
    
    ASSERT_EQ(UINT, keyboard_history[1][KC_2], 1);
    ASSERT_EQ(UINT, keyboard_history[2][KC_2], 255);
    
    process_record_user(BOT3, &depressed);
    ASSERT_EQ(UINT, keyboard_history[2][KC_2], 0);
    ASSERT_EQ(UINT, keyboard_history[3][KC_2], 255);

    printf("%s "GREEN"PASSED"NC"\n", name);
    return 0;
}


// These two are leaving the chording engine, they kinda have to be tested manually
// TO
// RESET




static char * all_tests() {
        mu_run_test(test_clear);
        mu_run_test(test_pause_ms);
        mu_run_test(test_single_dance_held_states);
        mu_run_test(test_single_dance_held_codes);
        mu_run_test(test_single_dance_tapped_states);
        mu_run_test(test_single_dance_tapped_codes);
        mu_run_test(test_single_dance_tapped_fast_codes);
        mu_run_test(test_subchords_are_ignored);
        mu_run_test(test_multiple_chords_at_once);
        mu_run_test(test_momentary_layer);
        mu_run_test(test_momentary_layer_reset);
        mu_run_test(test_permanent_layer);
        mu_run_test(test_autoshift_toggle);
        mu_run_test(test_autoshift_tap);
        mu_run_test(test_autoshift_hold);
        mu_run_test(test_autoshift_hold_off);
        mu_run_test(test_lock);
        mu_run_test(test_one_shot_key_tap);
        mu_run_test(test_one_shot_key_hold);
        mu_run_test(test_one_shot_key_retrotapping);
        mu_run_test(test_one_shot_layer_tap);
        mu_run_test(test_one_shot_layer_hold);
        mu_run_test(test_one_shot_layer_retrotapping);
        mu_run_test(test_command_mode);
        mu_run_test(test_key_key_dance_tap);
        mu_run_test(test_key_key_dance_hold);
        mu_run_test(test_key_layer_tap);
        mu_run_test(test_key_layer_retrotapping);
        mu_run_test(test_key_layer_hold_quick_typist);
        mu_run_test(test_key_layer_hold_slow_typist);
        mu_run_test(test_key_mod_tap);
        mu_run_test(test_key_mod_retrotapping);
        mu_run_test(test_key_mod_hold_quick_typist);
        mu_run_test(test_key_mod_hold_slow_typist);
        mu_run_test(test_leader_triggers_global);
        mu_run_test(test_leader_no_follow);
        mu_run_test(test_leader_wrong_follow);
        mu_run_test(test_leader_correct_follow);
        mu_run_test(test_dynamic_macro_record_mode);
        mu_run_test(test_dynamic_macro_record_mode_off);
        mu_run_test(test_dynamic_macro_record_one);
        mu_run_test(test_dynamic_macro_record_two);
        mu_run_test(test_dynamic_macro_record_two_parts);
        mu_run_test(test_dance_tap);
        mu_run_test(test_dance_hold);
        mu_run_test(test_dance_tap_tap);
        mu_run_test(test_dance_tap_hold);
        mu_run_test(test_multiple_keys);
        mu_run_test(test_multiple_keys_interleaved);
        mu_run_test(test_dance_one);
        mu_run_test(test_dance_two);
        mu_run_test(test_dance_three);
        mu_run_test(test_dance_two_held);
    
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("\n"GREEN"ALL TESTS PASSED"NC"\n");
    }
    printf("Tests run: %d / %d\n", tests_run, 53);

    return result != 0;
}

