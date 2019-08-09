$py(x = 0)



$macro(KC, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(KEYCODE), 0, single_dance};
    $py(x += 1)
$endmacro

$macro(KL, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE, TO_PSEUDOLAYER)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(KEYCODE), $(TO_PSEUDOLAYER), key_layer_dance};
    $py(x += 1)
$endmacro

$macro(KK, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE1, KEYCODE2)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(KEYCODE1), $(KEYCODE2), key_key_dance};
    $py(x += 1)
$endmacro

$macro(MO, ON_PSEUDOLAYER, KEYCODES_HASH, TO_PSEUDOLAYER)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(TO_PSEUDOLAYER), 0, temp_pseudolayer};
    $py(x += 1)
$endmacro

$macro(KC_LOCK, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, lock};
    $py(x += 1)
$endmacro

$macro(DF, ON_PSEUDOLAYER, KEYCODES_HASH, TO_PSEUDOLAYER)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(TO_PSEUDOLAYER), 0, perm_pseudolayer};
    $py(x += 1)
$endmacro

$macro(OSK, ON_PSEUDOLAYER, KEYCODES_HASH, KEYCODE)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(KEYCODE), 0, one_shot_key};
    $py(x += 1)
$endmacro

$macro(OSL, ON_PSEUDOLAYER, KEYCODES_HASH, PSEUDOLAYER)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, $(PSEUDOLAYER), 0, one_shot_layer};
    $py(x += 1)
$endmacro

$macro(CMD, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, command};
    $py(x += 1)
$endmacro

$macro(DM_RECORD, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, dynamic_macro_record};
    $py(x += 1)
$endmacro

$macro(DM_NEXT, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, dynamic_macro_next};
    $py(x += 1)
$endmacro

$macro(DM_END, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, dynamic_macro_end};
    $py(x += 1)
$endmacro

$macro(DM_PLAY, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, dynamic_macro_play};
    $py(x += 1)
$endmacro

$macro(LEAD, ON_PSEUDOLAYER, KEYCODES_HASH)
    $nonlocal(x)
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, leader};
    $py(x += 1)
$endmacro

$macro(STR, ON_PSEUDOLAYER, KEYCODES_HASH, STRING)
    $nonlocal(x)
    void str_$(x)(const struct Chord* self) {
        if (*self->state == ACTIVATED) {
            SEND_STRING($(STRING));
        }
    }
    uint8_t state_$(x) = IDLE;
    const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(ON_PSEUDOLAYER), &state_$(x), NULL, 0, 0, str_$(x)};
    $py(x += 1)
$endmacro

$macro(add_key, PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(x)
    $if(len(DEFINITION) == 0)
    $elif(len(DEFINITION) == 1)
        $if(DEFINITION == ",")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_COMMA")
        $elif(DEFINITION == ".")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_DOT")
        $elif(DEFINITION == "/")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_SLASH")
        $elif(DEFINITION == ";")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_SCOLON")
        $elif(DEFINITION == "'")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_QUOTE")
        $elif(DEFINITION == "[")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_LBRACKET")
        $elif(DEFINITION == "]")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_RBRACKET")
        $elif(DEFINITION == "\\")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_BSLASH")
        $elif(DEFINITION == "-")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_MINUS")
        $elif(DEFINITION == "=")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_EQUAL")
        $elif(DEFINITION == "`")
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_GRAVE")
        $else
            $KC(PSEUDOLAYER, KEYCODES_HASH, "KC_" + DEFINITION)
        $endif
    $else
        $if(DEFINITION[0:2] == "T(")
        $elif(DEFINITION[0:2] == "O(")
            $if(DEFINITION[2:5] == "KC_")
                $OSK(PSEUDOLAYER, KEYCODES_HASH, DEFINITION[2:-1])
            $else
                $OSL(PSEUDOLAYER, KEYCODES_HASH, DEFINITION[2:-1])
            $endif
        $elif(DEFINITION[0:3] == "TH(")
        $elif(DEFINITION[0:2] == "D(")
            $py(l = len(DEFINITION[2:-1].split(', ')))
            void function_$(x)(const struct Chord* self) {
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
                            $py(val = DEFINITION[2:-1].split(', ')[i])
                            case $(i+1):
                                register_code($(val));
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
                            $py(val = DEFINITION[2:-1].split(', ')[i])
                            case $(i+1):
                                unregister_code($(val));
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
            uint8_t state_$(x) = IDLE;
            uint8_t counter_$(x) = 0;
            const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(PSEUDOLAYER), &state_$(x), &counter_$(x), 0, 0, function_$(x)};
            $py(x += 1)
        $elif(DEFINITION[0:2] == "M(")
            $py(val1 = DEFINITION[2:-1].split(', ')[1])
            $py(val2 = DEFINITION[2:-1].split(', ')[2])
            $py(fnc = DEFINITION[2:-1].split(', ')[0])
            uint8_t state_$(x) = IDLE;
            uint8_t counter_$(x) = 0;
            const struct Chord chord_$(x) PROGMEM = {$(KEYCODES_HASH), $(PSEUDOLAYER), &state_$(x), &counter_$(x), $(val1), $(val2), $(fnc)});
            $py(x += 1)
        $elif(DEFINITION == "LOCK")
            $KC_LOCK(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION == "CMD")
            $CMD(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION == "LEAD")
            $LEAD(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION == "DM_RECORD")
            $LEAD(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION == "DM_NEXT")
            $LEAD(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION == "DM_END")
            $LEAD(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION == "DM_PLAY")
            $LEAD(PSEUDOLAYER, KEYCODES_HASH)
        $elif(DEFINITION[0:3] == "KL(")
            $py(val1 = DEFINITION[3:-1].split(', ')[0])
            $py(val2 = DEFINITION[3:-1].split(', ')[1])
            $KL(PSEUDOLAYER, KEYCODES_HASH, $(val1), $(val2))
        $elif(DEFINITION[0:3] == "KL(")
            $py(val1 = DEFINITION[3:-1].split(', ')[0])
            $py(val2 = DEFINITION[3:-1].split(', ')[1])
            $KK(PSEUDOLAYER, KEYCODES_HASH, $(val1), $(val2))
        $else
            $if(DEFINITION[0:3] == "KC_")
                $KC(PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
            $elif(DEFINITION[0:3] == "MO(")
                $MO(PSEUDOLAYER, KEYCODES_HASH, DEFINITION[3:-1])
            $elif(DEFINITION[0:3] == "DF(")
                $DF(PSEUDOLAYER, KEYCODES_HASH, DEFINITION[3:-1])
            $elif(DEFINITION[0:4] == "STR(")
                $STR(PSEUDOLAYER, KEYCODES_HASH, DEFINITION[4:-1])
            $endif
        $endif
    $endif
$endmacro

$macro(vertical_ortho_layer, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30)
    $nonlocal(x)
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

$macro(horizontal_ortho_layer, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K11, K12, K13, K14, K15, K16, K17, K18, K19, K21, K22, K23, K24, K25, K26, K27, K28, K29)
    $nonlocal(x)
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
    $nonlocal(x)
    $add_key(PSEUDOLAYER,  "H_TOP1", K1)
    $add_key(PSEUDOLAYER,  "H_TOP2", K2)
    $add_key(PSEUDOLAYER,  "H_TOP3", K3)
    $add_key(PSEUDOLAYER,  "H_TOP4", K4)
    $add_key(PSEUDOLAYER,  "H_TOP7", K5)
    $add_key(PSEUDOLAYER,  "H_TOP8", K6)
    $add_key(PSEUDOLAYER,  "H_TOP9", K7)
    $add_key(PSEUDOLAYER,  "H_TOP0", K8)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP2", K9)
    $add_key(PSEUDOLAYER,  "H_TOP2 + H_TOP3", K10)
    $add_key(PSEUDOLAYER,  "H_TOP3 + H_TOP4", K11)
    $add_key(PSEUDOLAYER,  "H_TOP4 + H_TOP7", K12)
    $add_key(PSEUDOLAYER,  "H_TOP7 + H_TOP8", K13)
    $add_key(PSEUDOLAYER,  "H_TOP8 + H_TOP9", K14)
    $add_key(PSEUDOLAYER,  "H_TOP9 + H_TOP0", K15)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP3", K16)
    $add_key(PSEUDOLAYER,  "H_TOP2 + H_TOP4", K17)
    $add_key(PSEUDOLAYER,  "H_TOP3 + H_TOP7", K18)
    $add_key(PSEUDOLAYER,  "H_TOP4 + H_TOP8", K19)
    $add_key(PSEUDOLAYER,  "H_TOP7 + H_TOP9", K20)
    $add_key(PSEUDOLAYER,  "H_TOP8 + H_TOP0", K21)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP4", K22)
    $add_key(PSEUDOLAYER,  "H_TOP2 + H_TOP7", K23)
    $add_key(PSEUDOLAYER,  "H_TOP3 + H_TOP8", K24)
    $add_key(PSEUDOLAYER,  "H_TOP4 + H_TOP9", K25)
    $add_key(PSEUDOLAYER,  "H_TOP7 + H_TOP0", K26)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP7", K27)
    $add_key(PSEUDOLAYER,  "H_TOP2 + H_TOP8", K28)
    $add_key(PSEUDOLAYER,  "H_TOP3 + H_TOP9", K29)
    $add_key(PSEUDOLAYER,  "H_TOP4 + H_TOP0", K30)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP8", K31)
    $add_key(PSEUDOLAYER,  "H_TOP2 + H_TOP9", K32)
    $add_key(PSEUDOLAYER,  "H_TOP3 + H_TOP0", K33)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP9", K34)
    $add_key(PSEUDOLAYER,  "H_TOP2 + H_TOP0", K35)
    $add_key(PSEUDOLAYER,  "H_TOP1 + H_TOP0", K36)
$endmacro