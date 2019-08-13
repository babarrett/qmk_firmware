const struct Chord* const list_of_chords[] PROGMEM = {
    $for(i in range(0, NUM_OF_CHORDS))
        &chord_$(i),
    $endfor
};

const uint16_t leader_triggers[$(len(list_of_leader_combos))][$(LEADER_MAX_LENGTH)] PROGMEM = {
    $for(i in range(0, len(list_of_leader_combos)))
        $(list_of_leader_combos[i][0]),
    $endfor
};

void (*leader_functions[]) (void) = {
    $for(i in range(0, len(list_of_leader_combos)))
        $(list_of_leader_combos[i][1]),
    $endfor
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
    
    for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
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
    for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
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
    for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
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
    for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
        struct Chord* chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        struct Chord chord_storage;
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        struct Chord* chord = &chord_storage;
        
        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {
            continue;
        }
        
        for (int j = 0; j < $(NUM_OF_CHORDS); j++) {
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
    for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
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
    for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
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
    for (int i = 0; i < $(COMMAND_MAX_LENGTH); i++) {
        if (command_buffer[i]) {
            register_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    wait_ms(TAP_TIMEOUT);
    for (int i = 0; i < $(COMMAND_MAX_LENGTH); i++) {
        if (command_buffer[i]) {
            unregister_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    for (int i = 0; i < $(COMMAND_MAX_LENGTH); i++) {
        command_buffer[i] = 0;
    }
    command_ind = 0;
}

void process_leader(void) {
    in_leader_mode = false;
    for (int i = 0; i < $(len(list_of_leader_combos)); i++) {
        uint16_t trigger[5];
        memcpy_P(trigger, leader_triggers[i], $(LEADER_MAX_LENGTH) * sizeof(uint16_t));
        
        if (identical(leader_buffer, trigger)) {
            (*leader_functions[i])();
            break;
        }
    }
    for (int i = 0; i < $(LEADER_MAX_LENGTH); i++) {
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
        for (int i = 0; i < $(NUM_OF_CHORDS); i++) {
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