$py(NUM_OF_CHORDS = 0)
$py(list_of_leader_combos = [])

$macro(KC, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(KEYCODE), 0, single_dance};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(AS, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    uint8_t counter_$(NUM_OF_CHORDS) = 0;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), &counter_$(NUM_OF_CHORDS), $(KEYCODE), 0, autoshift_dance};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(AT, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, autoshift_toggle};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(KL, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE, TO_PSEUDOLAYER)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(KEYCODE), $(TO_PSEUDOLAYER), key_layer_dance};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(KK, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE1, KEYCODE2)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(KEYCODE1), $(KEYCODE2), key_key_dance};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(KM, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE, TO_PSEUDOLAYER)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(KEYCODE), $(TO_PSEUDOLAYER), key_mod_dance};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(MO, ON_PSEUDOLAYER, KEYCODES_HASH, TO_PSEUDOLAYER)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(TO_PSEUDOLAYER), 0, temp_pseudolayer};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(LOCK, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, lock};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(DF, ON_PSEUDOLAYER, KEYCODES_HASH, TO_PSEUDOLAYER)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(TO_PSEUDOLAYER), 0, perm_pseudolayer};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(TO, ON_PSEUDOLAYER, KEYCODES_HASH, TO_PSEUDOLAYER)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(TO_PSEUDOLAYER), 0, switch_layer};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(OSK, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(KEYCODE), 0, one_shot_key};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(OSL, ON_PSEUDOLAYER, KEYCODES_HASH, PSEUDOLAYER)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(PSEUDOLAYER), 0, one_shot_layer};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(CMD, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, command};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(DM_RECORD, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, dynamic_macro_record};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(DM_NEXT, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, dynamic_macro_next};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(DM_END, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, dynamic_macro_end};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(DM_PLAY, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, dynamic_macro_play};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(LEAD, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, leader};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(CLEAR, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, clear};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(RESET, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, reset};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(STR, ON_PSEUDOLAYER, KEYCODES_HASH, STRING)
    $nonlocal(NUM_OF_CHORDS)
    void str_$(NUM_OF_CHORDS)(const struct Chord* self) {
        if (*self->state == ACTIVATED) {
            SEND_STRING($(STRING));
        }
    }
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, 0, 0, str_$(NUM_OF_CHORDS)};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(M, ON_PSEUDOLAYER, KEYCODES_HASH, VALUE1, VALUE2, FNC)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    uint8_t counter_$(NUM_OF_CHORDS) = 0;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), &counter_$(NUM_OF_CHORDS), $(VALUE1), $(VALUE2), $(FNC)};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$py(
def expand_keycode_fnc(DEFINITION):
    if DEFINITION == "`":
        DEFINITION = "GRAVE"
    elif DEFINITION == "-":
        DEFINITION = "MINUS"
    elif DEFINITION == "=":
        DEFINITION = "EQUAL"
    elif DEFINITION == "[":
        DEFINITION = "LBRACKET"
    elif DEFINITION == "]":
        DEFINITION = "RBRACKET"
    elif DEFINITION == "\\":
        DEFINITION = "BSLASH"
    elif DEFINITION == ";":
        DEFINITION = "SCOLON"
    elif DEFINITION == "'":
        DEFINITION = "QUOTE"
    elif DEFINITION == ",":
        DEFINITION = "COMMA"
    elif DEFINITION == ".":
        DEFINITION = "DOT"
    elif DEFINITION == "/":
        DEFINITION = "SLASH"

    if DEFINITION in [
        "A", "a", "B", "b", "C", "c", "D", "d", "E", "e",
        "F", "f", "G", "g", "H", "h", "I", "i", "J", "j",
        "K", "k", "L", "l", "M", "m", "N", "n", "O", "o",
        "P", "p", "Q", "q", "R", "r", "S", "s", "T", "t",
        "U", "u", "V", "v", "W", "w", "X", "x", "Y", "y",
        "Z", "z", "1", "2", "3", "4", "5", "6", "7", "8",
        "9", "0", "F1", "F2", "F3", "F4", "F5", "F6", "F7",
        "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15",
        "F16", "F17", "F18", "F19", "F20", "F21", "F22",
        "F23", "F24", "ENTER", "ENT", "ESCAPE", "ESC",
        "BSPACE", "BSPC", "TAB", "SPACE", "SPC", "NONUS_HASH",
        "NUHS", "NONUS_BSLASH", "NUBS", "COMMA", "COMM",
        "DOT", "SLASH", "SLSH", "SCOLON", "SCLN", "QUOTE",
        "QUOT", "LBRACKET", "LBRC", "RBRACKET", "RBRC",
        "BSLASH", "BSLS", "MINUS", "MINS", "EQUAL", "EQL",
        "GRAVE", "GRV", "ZKHK", "CAPSLOCK", "CLCK", "CAPS",
        "SCROLLOCK", "SLCK", "BRMD", "NUMLOCK", "NLCK",
        "LOCKING_CAPS", "LCAP", "LOCKING_NUM", "LNUM",
        "LOCKING_SCROLL", "LSCR", "LCTRL", "LCTL", "LSHIFT",
        "LSFT", "LALT", "LGUI", "LCMD", "LWIN", "RCTRL",
        "RCTL", "RSHIFT", "RSFT", "RALT", "RGUI", "RCMD",
        "RWIN", "INT1", "RO", "INT2", "KANA", "INT3", "JYEN",
        "INT4", "HENK", "INT5", "MHEN", "INT6", "INT7",
        "INT8", "INT9", "LANG1", "HAEN", "LANG2", "HANJ",
        "LANG3", "LANG4", "LANG5", "LANG6", "LANG7", "LANG8",
        "LANG9", "PSCREEN", "PSCR", "PAUSE", "PAUS", "BRK",
        "BRMU", "INSERT", "INS", "HOME", "PGUP", "DELETE",
        "DEL", "END", "PGDOWN", "PGDN", "RIGHT", "RGHT",
        "LEFT", "DOWN", "UP", "APPLICATION", "APP", "POWER",
        "EXECUTE", "EXEC", "HELP", "MENU", "SELECT", "SLCT",
        "STOP", "AGAIN", "AGIN", "UNDO", "CUT", "COPY",
        "PASTE", "PSTE", "FIND", "MUTE", "VOLUP", "VOLDOWN",
        "ALT_ERASE", "ERAS", "SYSREQ", "CANCEL", "CLEAR",
        "CLR", "PRIOR", "RETURN", "SEPARATOR", "OUT", "OPER",
        "CLEAR_AGAIN", "CRSEL", "EXSEL", "SYSTEM_POWER",
        "PWR", "SYSTEM_SLEEP", "SLEP", "SYSTEM_WAKE", "WAKE",
        "AUDIO_MUTE", "MUTE", "AUDIO_VOL_UP", "VOLU",
        "AUDIO_VOL_DOWN", "VOLD", "MEDIA_NEXT_TRACK", "MNXT",
        "MEDIA_PREV_TRACK", "CPRV", "MEDIA_STOP", "MSTP",
        "MEDIA_PLAY_PAUSE", "MPLY", "MEDIA_SELECT", "MSEL",
        "MEDIA_EJECT", "EJCT", "MAIL", "CALCULATOR", "CALC",
        "MY_COMPUTER", "MYCM", "WWW_SEATCH", "WSCH", "WWW_HOME",
        "WHOM", "WWW_BACK", "WBAK", "WWW_FORWARD", "WFWD",
        "WWW_STOP", "WSTP", "WWW_REFRESH", "WREF",
        "WWW_FAVORITES", "WFAV", "MEDIA_FAST_FORWARD", "MFFD",
        "MEDIA_REWIND", "MRWD", "BRIGHTNESS_UP", "BRIU",
        "BRIGHTNESS_DOWN", "BRID", "KP_SLASH", "PSLS",
        "KP_ASTERISK", "PAST", "KP_MINUS", "PMNS", "KP_PLUS",
        "PPLS", "KP_ENTER", "PENT", "KP_1", "P1", "KP_2", "P2",
        "KP_3", "P3", "KP_4", "P4", "KP_5", "P5", "KP_6", "P6",
        "KP_7", "P7", "KP_8", "P8", "KP_9", "P9", "KP_0", "P0",
        "KP_DOT", "PDOT", "KP_EQUAL", "PEQL", "KP_COMMA", "PCMM",
        "MS_BTN1", "BTN1", "MS_BTN2", "BTN2", "MS_BTN3", "BTN3",
        "MS_BTN4", "BTN4", "MS_BTN5", "BTN5", "MS_BTN6", "BTN6",
        "MS_LEFT", "MS_L", "MS_DOWN", "MS_D", "MS_UP", "MS_U",
        "MS_RIGHT", "MS_R", "MS_WH_UP", "WH_U", "MS_WH_DOWN",
        "WH_D", "MS_WH_LEFT", "MS_WH_L", "MS_WH_RIGHT", "MS_WH_R",
        "KC_MS_ACCEL0", "ACL0", "KC_MS_ACCEL1", "ACL1",
        "KC_MS_ACCEL2", "ACL2"
        ]:
        return "KC_" + DEFINITION
    else:
        return DEFINITION
)

$macro(MK_backup, ON_PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    $py(l = len(DEFINITION.split(', ')))
    void function_$(NUM_OF_CHORDS)(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                $for(i in range(0, l))\
                    $py(val = DEFINITION.split(',')[i].strip())
                    $py(code = expand_keycode_fnc(val))
                    key_in($(code));
                $endfor\
                break;
            case DEACTIVATED:
                $for(i in range(0, l))\
                    $py(val = DEFINITION.split(',')[i].strip())
                    $py(code = expand_keycode_fnc(val))
                    key_out($(code));
                $endfor\
                *self->state = IDLE;
                break;
            case RESTART:
                $for(i in range(0, l))\
                    $py(val = DEFINITION.split(',')[i].strip())
                    $py(code = expand_keycode_fnc(val))
                    key_out($(code));
                $endfor\
                break;
            default:
                break;
        }
    }
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    uint8_t counter_$(NUM_OF_CHORDS) = 0;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(PSEUDOLAYER), &state_$(NUM_OF_CHORDS), &counter_$(NUM_OF_CHORDS), 0, 0, function_$(NUM_OF_CHORDS)};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(MK, ON_PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    $py(l = len(DEFINITION.split(', ')))
    void function_$(NUM_OF_CHORDS)(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                $for(i in range(0, l))\
                    $py(val = DEFINITION.split(',')[i].strip())
                    $py(code = expand_keycode_fnc(val))
                    key_in($(code));
                $endfor\
                break;
            case DEACTIVATED:
                $for(i in range(0, l))\
                    $py(val = DEFINITION.split(',')[i].strip())
                    $py(code = expand_keycode_fnc(val))
                    key_out($(code));
                $endfor\
                *self->state = IDLE;
                break;
            case RESTART:
                $for(i in range(0, l))\
                    $py(val = DEFINITION.split(',')[i].strip())
                    $py(code = expand_keycode_fnc(val))
                    key_out($(code));
                $endfor\
                break;
            default:
                break;
        }
    }
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    uint8_t counter_$(NUM_OF_CHORDS) = 0;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(PSEUDOLAYER), &state_$(NUM_OF_CHORDS), &counter_$(NUM_OF_CHORDS), 0, 0, function_$(NUM_OF_CHORDS)};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(D, ON_PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    $py(l = len(DEFINITION.split(',')))
    void function_$(NUM_OF_CHORDS)(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                *self->counter = *self->counter + 1;
                break;
            case PRESS_FROM_ACTIVE:
                switch (*self->counter) {
                    $for(i in range(0, l))\
                        $py(val = DEFINITION.split(',')[i].strip())
                        case $(i + 1):
                            $py(code = expand_keycode_fnc(val))
                            key_in($(code));
                            break;
                    $endfor\
                    default:
                        break;
                }
                *self->state = FINISHED_FROM_ACTIVE;
                break;
            case FINISHED:
                switch (*self->counter) {
                    $for(i in range(0, l))\
                        $py(val = DEFINITION.split(',')[i].strip())
                        case $(i + 1):
                            $py(code = expand_keycode_fnc(val))
                            tap_key($(code));
                            break;
                    $endfor\
                    default:
                        break;
                }
                *self->counter = 0;
                *self->state = IDLE;
                break;
            case RESTART:
                switch (*self->counter) {
                    $for(i in range(0, l))\
                        $py(val = DEFINITION.split(',')[i].strip())
                        case $(i + 1):
                            $py(code = expand_keycode_fnc(val))
                            key_out($(code));
                            break;
                    $endfor\
                    default:
                        break;
                }
                *self->counter = 0;
                break;
            default:
                break;
        }
    }
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    uint8_t counter_$(NUM_OF_CHORDS) = 0;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(PSEUDOLAYER), &state_$(NUM_OF_CHORDS), &counter_$(NUM_OF_CHORDS), 0, 0, function_$(NUM_OF_CHORDS)};
    $py(NUM_OF_CHORDS += 1)
$endmacro

$macro(O, ON_PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    $if(DEFINITION[0:3] == "KC_")
        $OSK(PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $else
        $OSL(PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $endif
$endmacro

$macro(add_key, PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    
    $if(DEFINITION == "")
    $else
        $py(split = DEFINITION.split("("))
        $py(type = split[0].strip())
        $if(len(split) == 1)
            $if(type == "LOCK")
                $LOCK(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "AT")
                $AT(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "CMD")
                $CMD(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "LEAD")
                $LEAD(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "DM_RECORD")
                $DM_RECORD(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "DM_NEXT")
                $DM_NEXT(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "DM_END")
                $DM_END(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "DM_PLAY")
                $DM_PLAY(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "CLEAR_KB")
                $CLEAR(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "RESET")
                $RESET(PSEUDOLAYER, KEYCODES_HASH)
            $else
                $py(code = expand_keycode_fnc(type))
                $KC(PSEUDOLAYER, KEYCODES_HASH, code)
            $endif
        $else
            $py(val = split[1][:-1].strip())
            $if(type == "O")
                $py(code = expand_keycode_fnc(val))
                $O(PSEUDOLAYER, KEYCODES_HASH, code)
            $elif(type == "D")
                $D(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "MK")
                $MK(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "M")
                $py(fnc = val.split(',')[0].strip())
                $py(val1 = val.split(',')[1].strip())
                $py(val2 = val.split(',')[2].strip())
                $M(PSEUDOLAYER, KEYCODES_HASH, val1, val2, fnc)
            $elif(type == "KK")
                $py(val1 = val.split(',')[0].strip())
                $py(code1 = expand_keycode_fnc(val1))
                $py(val2 = val.split(',')[1].strip())
                $py(code2 = expand_keycode_fnc(val2))
                $KK(PSEUDOLAYER, KEYCODES_HASH, code1, code2)
            $elif(type == "KL")
                $py(val1 = val.split(',')[0].strip())
                $py(code1 = expand_keycode_fnc(val1))
                $py(val2 = val.split(',')[1].strip())
                $KL(PSEUDOLAYER, KEYCODES_HASH, code1, val2)
            $elif(type == "KM")
                $py(val1 = val.split(',')[0].strip())
                $py(code1 = expand_keycode_fnc(val1))
                $py(val2 = val.split(',')[1].strip())
                $py(code2 = expand_keycode_fnc(val2))
                $KM(PSEUDOLAYER, KEYCODES_HASH, code1, code2)
            $elif(type == "AS")
                $py(code = expand_keycode_fnc(val))
                $AS(PSEUDOLAYER, KEYCODES_HASH, code)
            $elif(type == "MO")
                $MO(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "DF")
                $DF(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "TO")
                $TO(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "STR")
                $STR(PSEUDOLAYER, KEYCODES_HASH, val)
            $endif
        $endif
    $endif
$endmacro

$macro(add_leader_combo, DEFINITION, FUNCTION)
    $nonlocal(list_of_leader_combos)
    $py(list_of_leader_combos.append([DEFINITION, FUNCTION]))
$endmacro