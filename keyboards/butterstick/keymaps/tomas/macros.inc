$py(NUM_OF_CHORDS = 0)
$py(list_of_leader_combos = [])

$macro(KC, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE)
    $nonlocal(NUM_OF_CHORDS)
    uint8_t state_$(NUM_OF_CHORDS) = IDLE;
    const struct Chord chord_$(NUM_OF_CHORDS) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(NUM_OF_CHORDS), NULL, $(KEYCODE), 0, single_dance};
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
    if DEFINITION == ",":
        return "KC_COMMA"
    elif DEFINITION == ".":
        return "KC_DOT"
    elif DEFINITION == "/":
        return "KC_SLASH"
    elif DEFINITION == ";":
        return "KC_SCOLON"
    elif DEFINITION == "'":
        return "KC_QUOTE"
    elif DEFINITION == "[":
        return "KC_LBRACKET"
    elif DEFINITION == "]":
        return "KC_RBRACKET"
    elif DEFINITION == "\\":
        return "KC_BSLASH"
    elif DEFINITION == "-":
        return "KC_MINUS"
    elif DEFINITION == "=":
        return "KC_EQUAL"
    elif DEFINITION == "`":
        return "KC_GRAVE"
    elif DEFINITION[0:3] == "KC_":
        return DEFINITION
    else:
        return "KC_" + DEFINITION
)

$macro(D, ON_PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    $py(l = len(DEFINITION.split(', ')))
    void function_$(NUM_OF_CHORDS)(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                *self->counter = *self->counter + 1;
                break;
            case DEACTIVATED:
                break;
            case FINISHED:
            case FINISHED_FROM_ACTIVE:
                switch (*self->counter) {
                    $for(i in range(0, l))
                    $py(val = DEFINITION.split(', ')[i])
                    case $(i+1):
                        $py(code = expand_keycode_fnc(val))
                        register_code($(code));
                        break;
                    $endfor
                    default:
                        break;
                }
                send_keyboard_report();
                break;
            case RESTART:
                switch (*self->counter) {
                    $for(i in range(0, l))
                    $py(val = DEFINITION.split(', ')[i])
                    case $(i+1):
                        $py(code = expand_keycode_fnc(val))
                        unregister_code($(code));
                        break;
                    $endfor
                    default:
                        break;
                }
                send_keyboard_report();
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
        $py(type = split[0])
        $if(len(split) == 1)
            $if(type == "LOCK")
                $LOCK(PSEUDOLAYER, KEYCODES_HASH)
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
            $elif(type == "CLEAR")
                $CLEAR(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "RESET")
                $RESET(PSEUDOLAYER, KEYCODES_HASH)
            $elif(type == "RESET")
                $KC(PSEUDOLAYER, KEYCODES_HASH, "RESET")
            $else
                $py(code = expand_keycode_fnc(type))
                $KC(PSEUDOLAYER, KEYCODES_HASH, code)
            $endif
        $else
            $py(val = split[1][:-1])
            $if(type == "O")
                $O(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "D")
                $D(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "M")
                $py(fnc = val.split(', ')[0])
                $py(val1 = val.split(', ')[1])
                $py(val2 = val.split(', ')[2])
                $M(PSEUDOLAYER, KEYCODES_HASH, val1, val2, fnc)
            $elif(type == "KK")
                $py(val1 = val.split(', ')[0])
                $py(code1 = expand_keycode_fnc(val1))
                $py(val2 = val.split(', ')[1])
                $py(code2 = expand_keycode_fnc(val2))
                $KK(PSEUDOLAYER, KEYCODES_HASH, code1, code2)
            $elif(type == "KL")
                $py(val1 = val.split(', ')[0])
                $py(code1 = expand_keycode_fnc(val1))
                $py(val2 = val.split(', ')[1])
                $KL(PSEUDOLAYER, KEYCODES_HASH, code1, val2)
            $elif(type == "MO")
                $MO(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "DF")
                $DF(PSEUDOLAYER, KEYCODES_HASH, val)
            $elif(type == "STR")
                $STR(PSEUDOLAYER, KEYCODES_HASH, val)
            $endif
        $endif
    $endif
$endmacro

$macro(butterstick_rows, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30)
    $nonlocal(NUM_OF_CHORDS)
    $add_key(PSEUDOLAYER, "H_TOP1", K1)
    $add_key(PSEUDOLAYER, "H_TOP2", K2)
    $add_key(PSEUDOLAYER, "H_TOP3", K3)
    $add_key(PSEUDOLAYER, "H_TOP4", K4)
    $add_key(PSEUDOLAYER, "H_TOP5", K5)
    $add_key(PSEUDOLAYER, "H_TOP6", K6)
    $add_key(PSEUDOLAYER, "H_TOP7", K7)
    $add_key(PSEUDOLAYER, "H_TOP8", K8)
    $add_key(PSEUDOLAYER, "H_TOP9", K9)
    $add_key(PSEUDOLAYER, "H_TOP0", K10)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_BOT1", K11)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_BOT2", K12)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_BOT3", K13)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_BOT4", K14)
    $add_key(PSEUDOLAYER, "H_TOP5 + H_BOT5", K15)
    $add_key(PSEUDOLAYER, "H_TOP6 + H_BOT6", K16)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_BOT7", K17)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_BOT8", K18)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_BOT9", K19)
    $add_key(PSEUDOLAYER, "H_TOP0 + H_BOT0", K20)
    $add_key(PSEUDOLAYER, "H_BOT1", K21)
    $add_key(PSEUDOLAYER, "H_BOT2", K22)
    $add_key(PSEUDOLAYER, "H_BOT3", K23)
    $add_key(PSEUDOLAYER, "H_BOT4", K24)
    $add_key(PSEUDOLAYER, "H_BOT5", K25)
    $add_key(PSEUDOLAYER, "H_BOT6", K26)
    $add_key(PSEUDOLAYER, "H_BOT7", K27)
    $add_key(PSEUDOLAYER, "H_BOT8", K28)
    $add_key(PSEUDOLAYER, "H_BOT9", K29)
    $add_key(PSEUDOLAYER, "H_BOT0", K30)
$endmacro

$macro(butterstick_cols, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K11, K12, K13, K14, K15, K16, K17, K18, K19, K21, K22, K23, K24, K25, K26, K27, K28, K29)
    $nonlocal(NUM_OF_CHORDS)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP2", K1)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP3", K2)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP4", K3)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_TOP5", K4)
    $add_key(PSEUDOLAYER, "H_TOP5 + H_TOP6", K5)
    $add_key(PSEUDOLAYER, "H_TOP6 + H_TOP7", K6)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_TOP8", K7)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_TOP9", K8)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_TOP0", K9)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_BOT1 + H_TOP2 + H_BOT2", K11)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_BOT2 + H_TOP3 + H_BOT3", K12)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_BOT3 + H_TOP4 + H_BOT4", K13)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_BOT4 + H_TOP5 + H_BOT5", K14)
    $add_key(PSEUDOLAYER, "H_TOP5 + H_BOT5 + H_TOP6 + H_BOT6", K15)
    $add_key(PSEUDOLAYER, "H_TOP6 + H_BOT6 + H_TOP7 + H_BOT7", K16)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_BOT7 + H_TOP8 + H_BOT8", K17)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_BOT8 + H_TOP9 + H_BOT9", K18)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_BOT9 + H_TOP0 + H_BOT0", K19)
    $add_key(PSEUDOLAYER, "H_BOT1 + H_BOT2", K21)
    $add_key(PSEUDOLAYER, "H_BOT2 + H_BOT3", K22)
    $add_key(PSEUDOLAYER, "H_BOT3 + H_BOT4", K23)
    $add_key(PSEUDOLAYER, "H_BOT4 + H_BOT5", K24)
    $add_key(PSEUDOLAYER, "H_BOT5 + H_BOT6", K25)
    $add_key(PSEUDOLAYER, "H_BOT6 + H_BOT7", K26)
    $add_key(PSEUDOLAYER, "H_BOT7 + H_BOT8", K27)
    $add_key(PSEUDOLAYER, "H_BOT8 + H_BOT9", K28)
    $add_key(PSEUDOLAYER, "H_BOT9 + H_BOT0", K29)
$endmacro

$macro(asetniop_layer, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30, K31, K32, K33, K34, K35, K36)
    $nonlocal(NUM_OF_CHORDS)
    $add_key(PSEUDOLAYER, "H_TOP1", K1)
    $add_key(PSEUDOLAYER, "H_TOP2", K2)
    $add_key(PSEUDOLAYER, "H_TOP3", K3)
    $add_key(PSEUDOLAYER, "H_TOP4", K4)
    $add_key(PSEUDOLAYER, "H_TOP7", K5)
    $add_key(PSEUDOLAYER, "H_TOP8", K6)
    $add_key(PSEUDOLAYER, "H_TOP9", K7)
    $add_key(PSEUDOLAYER, "H_TOP0", K8)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP2", K9)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP3", K10)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP4", K11)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_TOP7", K12)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_TOP8", K13)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_TOP9", K14)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_TOP0", K15)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP3", K16)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP4", K17)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP7", K18)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_TOP8", K19)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_TOP9", K20)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_TOP0", K21)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP4", K22)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP7", K23)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP8", K24)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_TOP9", K25)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_TOP0", K26)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP7", K27)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP8", K28)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP9", K29)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_TOP0", K30)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP8", K31)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP9", K32)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP0", K33)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP9", K34)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP0", K35)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP0", K36)
$endmacro

$macro(secret_chord, PSEUDOLAYER, ACTION, T1, T2, T3, T4, T5, T6, T7, T8, T9, T0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B0)
    $nonlocal(NUM_OF_CHORDS)
    $py(hash = "")
    $if(T1 != "")
        $py(hash = hash + " + H_TOP1")
    $endif
    $if(T2 != "")
        $py(hash = hash + " + H_TOP2")
    $endif
    $if(T3 != "")
        $py(hash = hash + " + H_TOP3")
    $endif
    $if(T4 != "")
        $py(hash = hash + " + H_TOP4")
    $endif
    $if(T5 != "")
        $py(hash = hash + " + H_TOP5")
    $endif
    $if(T6 != "")
        $py(hash = hash + " + H_TOP6")
    $endif
    $if(T7 != "")
        $py(hash = hash + " + H_TOP7")
    $endif
    $if(T8 != "")
        $py(hash = hash + " + H_TOP8")
    $endif
    $if(T9 != "")
        $py(hash = hash + " + H_TOP9")
    $endif
    $if(T0 != "")
        $py(hash = hash + " + H_TOP0")
    $endif
    $if(B1 != "")
        $py(hash = hash + " + H_BOT1")
    $endif
    $if(B2 != "")
        $py(hash = hash + " + H_BOT2")
    $endif
    $if(B3 != "")
        $py(hash = hash + " + H_BOT3")
    $endif
    $if(B4 != "")
        $py(hash = hash + " + H_BOT4")
    $endif
    $if(B5 != "")
        $py(hash = hash + " + H_BOT5")
    $endif
    $if(B6 != "")
        $py(hash = hash + " + H_BOT6")
    $endif
    $if(B7 != "")
        $py(hash = hash + " + H_BOT7")
    $endif
    $if(B8 != "")
        $py(hash = hash + " + H_BOT8")
    $endif
    $if(B9 != "")
        $py(hash = hash + " + H_BOT9")
    $endif
    $if(B0 != "")
        $py(hash = hash + " + H_BOT0")
    $endif
    $py(hash = hash[3:])
    $if(hash != "")
        $add_key(PSEUDOLAYER, hash, ACTION)
    $endif
$endmacro

$macro(add_leader_combo, DEFINITION, FUNCTION)
    $nonlocal(list_of_leader_combos)
    $py(list_of_leader_combos.append([DEFINITION, FUNCTION]))
$endmacro