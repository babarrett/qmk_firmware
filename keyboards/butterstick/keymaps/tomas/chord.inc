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
    if (command_mode == 1 && command_ind < $(COMMAND_MAX_LENGTH)) {
        command_buffer[command_ind] = keycode;
        command_ind++;
    } else if (in_leader_mode && leader_ind < $(LEADER_MAX_LENGTH)) {
        leader_buffer[leader_ind] = keycode;
        leader_ind++;
    } else if (dynamic_macro_mode && dynamic_macro_ind < $(DYNAMIC_MACRO_MAX_LENGTH)) {
        dynamic_macro_buffer[dynamic_macro_ind] = keycode;
        dynamic_macro_ind++;
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
        if (~handle_US_ANSI_shifted_keys(keycode, false)) {
            unregister_code(keycode);
        }
        send_keyboard_report();
    }
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
            key_in(self->value1);
            wait_ms(TAP_TIMEOUT);
            key_out(self->value1);
            send_keyboard_report();
            
            *self->state = IDLE;
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            current_pseudolayer = self->value2;
            a_key_went_through = false;
            break;
        case RESTART:
            if (!a_key_went_through) {
                key_in(self->value1);
                wait_ms(TAP_TIMEOUT);
                key_out(self->value1);
                send_keyboard_report();
            } else {
                current_pseudolayer = self->pseudolayer;
            }
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
            key_in(self->value1);
            wait_ms(TAP_TIMEOUT);
            key_out(self->value1);
            
            *self->state = IDLE;
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            key_in(self->value2);
            break;
        case RESTART:
            key_out(self->value2);
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
        case DEACTIVATED:
            *self->state = IN_ONE_SHOT;
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
        case DEACTIVATED:
            *self->state = IN_ONE_SHOT;
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
    for (int i = 0; i < $(LEADER_MAX_LENGTH); i++) {
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
        for (int i = 0; i < $(DYNAMIC_MACRO_MAX_LENGTH); i++) {
            dynamic_macro_buffer[i] = 0;
        }
        dynamic_macro_mode = true;
    }
}

void dynamic_macro_next(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode && dynamic_macro_ind < $(DYNAMIC_MACRO_MAX_LENGTH)) {
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
        while (ind_start < $(DYNAMIC_MACRO_MAX_LENGTH)) {
            for (int i = ind_start; i < $(DYNAMIC_MACRO_MAX_LENGTH); i++) {
                register_code(dynamic_macro_buffer[i]);
                if (dynamic_macro_buffer[i] == 0) {
                    break;
                }
            }
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            for (int i = ind_start; i < $(DYNAMIC_MACRO_MAX_LENGTH); i++) {
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
