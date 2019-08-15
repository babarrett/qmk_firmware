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
    if DEFINITION == "A" or DEFINITION == "a":
        return "KC_A"
    elif DEFINITION == "B" or DEFINITION == "b":
        return "KC_B"
    elif DEFINITION == "C" or DEFINITION == "c":
        return "KC_C"
    elif DEFINITION == "D" or DEFINITION == "d":
        return "KC_D"
    elif DEFINITION == "E" or DEFINITION == "e":
        return "KC_E"
    elif DEFINITION == "F" or DEFINITION == "f":
        return "KC_F"
    elif DEFINITION == "G" or DEFINITION == "g":
        return "KC_G"
    elif DEFINITION == "H" or DEFINITION == "h":
        return "KC_H"
    elif DEFINITION == "I" or DEFINITION == "i":
        return "KC_I"
    elif DEFINITION == "J" or DEFINITION == "j":
        return "KC_J"
    elif DEFINITION == "K" or DEFINITION == "k":
        return "KC_K"
    elif DEFINITION == "L" or DEFINITION == "l":
        return "KC_L"
    elif DEFINITION == "M" or DEFINITION == "m":
        return "KC_M"
    elif DEFINITION == "N" or DEFINITION == "n":
        return "KC_N"
    elif DEFINITION == "O" or DEFINITION == "o":
        return "KC_O"
    elif DEFINITION == "P" or DEFINITION == "p":
        return "KC_P"
    elif DEFINITION == "Q" or DEFINITION == "q":
        return "KC_Q"
    elif DEFINITION == "R" or DEFINITION == "r":
        return "KC_R"
    elif DEFINITION == "S" or DEFINITION == "s":
        return "KC_S"
    elif DEFINITION == "T" or DEFINITION == "t":
        return "KC_T"
    elif DEFINITION == "U" or DEFINITION == "u":
        return "KC_U"
    elif DEFINITION == "V" or DEFINITION == "v":
        return "KC_V"
    elif DEFINITION == "W" or DEFINITION == "w":
        return "KC_W"
    elif DEFINITION == "X" or DEFINITION == "x":
        return "KC_X"
    elif DEFINITION == "Y" or DEFINITION == "y":
        return "KC_Y"
    elif DEFINITION == "Z" or DEFINITION == "z":
        return "KC_Z"
    elif DEFINITION == "1":
        return "KC_1"
    elif DEFINITION == "2":
        return "KC_2"
    elif DEFINITION == "3":
        return "KC_3"
    elif DEFINITION == "4":
        return "KC_4"
    elif DEFINITION == "5":
        return "KC_5"
    elif DEFINITION == "6":
        return "KC_6"
    elif DEFINITION == "7":
        return "KC_7"
    elif DEFINITION == "8":
        return "KC_8"
    elif DEFINITION == "9":
        return "KC_9"
    elif DEFINITION == "0":
        return "KC_0"
    elif DEFINITION == "F1":
        return "KC_F1"
    elif DEFINITION == "F2":
        return "KC_F2"
    elif DEFINITION == "F3":
        return "KC_F3"
    elif DEFINITION == "F4":
        return "KC_F4"
    elif DEFINITION == "F5":
        return "KC_F5"
    elif DEFINITION == "F6":
        return "KC_F6"
    elif DEFINITION == "F7":
        return "KC_F7"
    elif DEFINITION == "F8":
        return "KC_F8"
    elif DEFINITION == "F9":
        return "KC_F9"
    elif DEFINITION == "F10":
        return "KC_F10"
    elif DEFINITION == "F11":
        return "KC_F11"
    elif DEFINITION == "F12":
        return "KC_F12"
    elif DEFINITION == "F13":
        return "KC_F13"
    elif DEFINITION == "F14":
        return "KC_F14"
    elif DEFINITION == "F15":
        return "KC_F15"
    elif DEFINITION == "F16":
        return "KC_F16"
    elif DEFINITION == "F17":
        return "KC_F17"
    elif DEFINITION == "F18":
        return "KC_F18"
    elif DEFINITION == "F19":
        return "KC_F19"
    elif DEFINITION == "F20":
        return "KC_F20"
    elif DEFINITION == "F21":
        return "KC_F21"
    elif DEFINITION == "F22":
        return "KC_F22"
    elif DEFINITION == "F23":
        return "KC_F23"
    elif DEFINITION == "F24":
        return "KC_F24"
    elif DEFINITION == "ENTER" or DEFINITION == "ENT":
        return "KC_ENTER"
    elif DEFINITION == "ESCAPE" or DEFINITION == "ESC":
        return "KC_ESCAPE"
    elif DEFINITION == "BSPACE" or DEFINITION == "BSPC":
        return "KC_BSPACE"
    elif DEFINITION == "TAB":
        return "KC_TAB"
    elif DEFINITION == "SPACE" or DEFINITION == "SPC":
        return "KC_SPACE"
    elif DEFINITION == "NONUS_HASH" or DEFINITION == "NUHS":
        return "KC_NONUS_HASH"
    elif DEFINITION == "NONUS_BSLASH" or DEFINITION == "NUBS":
        return "KC_NONUS_BSLASH"
    elif DEFINITION == "," or DEFINITION == "COMMA" or DEFINITION == "COMM":
        return "KC_COMMA"
    elif DEFINITION == "." or DEFINITION == "DOT":
        return "KC_DOT"
    elif DEFINITION == "/" or DEFINITION == "SLASH" or DEFINITION == "SLSH":
        return "KC_SLASH"
    elif DEFINITION == ";" or DEFINITION == "SCOLON" or DEFINITION == "SCLN":
        return "KC_SCOLON"
    elif DEFINITION == "'" or DEFINITION == "QUOTE" or DEFINITION == "QUOT":
        return "KC_QUOTE"
    elif DEFINITION == "[" or DEFINITION == "LBRACKET" or DEFINITION == "LBRC":
        return "KC_LBRACKET"
    elif DEFINITION == "]" or DEFINITION == "RBRACKET" or DEFINITION == "RBRC":
        return "KC_RBRACKET"
    elif DEFINITION == "\\" or DEFINITION == "BSLASH" or DEFINITION == "BSLS":
        return "KC_BSLASH"
    elif DEFINITION == "-" or DEFINITION == "MINUS" or DEFINITION == "MINS":
        return "KC_MINUS"
    elif DEFINITION == "=" or DEFINITION == "EQUAL" or DEFINITION == "EQL":
        return "KC_EQUAL"
    elif DEFINITION == "`" or DEFINITION == "GRAVE" or DEFINITION == "GRV" or DEFINITION == "ZKHK":
        return "KC_GRAVE"
    elif DEFINITION == "CAPSLOCK" or DEFINITION == "CLCK" or DEFINITION == "CAPS":
        return "KC_CAPSLOCK"
    elif DEFINITION == "SCROLLOCK" or DEFINITION == "SLCK" or DEFINITION == "BRMD":
        return "KC_SCROLLOCK"
    elif DEFINITION == "NUMLOCK" or DEFINITION == "NLCK":
        return "KC_CAPSLOCK"
    elif DEFINITION == "LOCKING_CAPS" or DEFINITION == "LCAP":
        return "KC_LOCKING_CAPS"
    elif DEFINITION == "LOCKING_NUM" or DEFINITION == "LNUM":
        return "KC_LOCKING_NUM"
    elif DEFINITION == "LOCKING_SCROLL" or DEFINITION == "LSCR":
        return "KC_LOCKING_SCROLL"
    elif DEFINITION == "LCTRL" or DEFINITION == "LCTL":
        return "KC_LCTRL"
    elif DEFINITION == "LSHIFT" or DEFINITION == "LSFT":
        return "KC_LSHIFT"
    elif DEFINITION == "LALT":
        return "KC_LALT"
    elif DEFINITION == "LGUI" or DEFINITION == "LCMD" or DEFINITION == "LWIN":
        return "KC_LGUI"
    elif DEFINITION == "RCTRL" or DEFINITION == "RCTL":
        return "KC_RCTRL"
    elif DEFINITION == "RSHIFT" or DEFINITION == "RSFT":
        return "KC_RSHIFT"
    elif DEFINITION == "RALT":
        return "KC_RALT"
    elif DEFINITION == "RGUI" or DEFINITION == "RCMD" or DEFINITION == "RWIN":
        return "KC_RGUI"
    elif DEFINITION == "INT1" or DEFINITION == "RO":
        return "KC_INT1"
    elif DEFINITION == "INT2" or DEFINITION == "KANA":
        return "KC_INT2"
    elif DEFINITION == "INT3" or DEFINITION == "JYEN":
        return "KC_INT3"
    elif DEFINITION == "INT4" or DEFINITION == "HENK":
        return "KC_INT4"
    elif DEFINITION == "INT5" or DEFINITION == "MHEN":
        return "KC_INT5"
    elif DEFINITION == "INT6":
        return "KC_INT6"
    elif DEFINITION == "INT7":
        return "KC_INT7"
    elif DEFINITION == "INT8":
        return "KC_INT8"
    elif DEFINITION == "INT9":
        return "KC_INT9"
    elif DEFINITION == "LANG1" or DEFINITION == "HAEN":
        return "KC_LANG1"
    elif DEFINITION == "LANG2" or DEFINITION == "HANJ":
        return "KC_LANG2"
    elif DEFINITION == "LANG3":
        return "KC_LANG3"
    elif DEFINITION == "LANG4":
        return "KC_LANG4"
    elif DEFINITION == "LANG5":
        return "KC_LANG5"
    elif DEFINITION == "LANG6":
        return "KC_LANG6"
    elif DEFINITION == "LANG7":
        return "KC_LANG7"
    elif DEFINITION == "LANG8":
        return "KC_LANG8"
    elif DEFINITION == "LANG9":
        return "KC_LANG9"
    elif DEFINITION == "PSCREEN" or DEFINITION == "PSCR":
        return "KC_PSCREEN"
    elif DEFINITION == "PAUSE" or DEFINITION == "PAUS" or DEFINITION == "BRK" or DEFINITION == "BRMU":
        return "KC_PAUSE"
    elif DEFINITION == "INSERT" or DEFINITION == "INS":
        return "KC_INSERT"
    elif DEFINITION == "HOME":
        return "KC_HOME"
    elif DEFINITION == "PGUP":
        return "KC_PGUP"
    elif DEFINITION == "DELETE" or DEFINITION == "DEL":
        return "KC_DELETE"
    elif DEFINITION == "END":
        return "KC_END"
    elif DEFINITION == "PGDOWN" or DEFINITION == "PGDN":
        return "KC_PGDOWN"
    elif DEFINITION == "RIGHT" or DEFINITION == "RGHT":
        return "KC_RIGHT"
    elif DEFINITION == "LEFT":
        return "KC_LEFT"
    elif DEFINITION == "DOWN":
        return "KC_DOWN"
    elif DEFINITION == "UP":
        return "KC_UP"
    elif DEFINITION == "APPLICATION" or DEFINITION == "APP":
        return "KC_APP"
    elif DEFINITION == "POWER":
        return "KC_POWER"
    elif DEFINITION == "EXECUTE" or DEFINITION == "EXEC":
        return "KC_EXEC"
    elif DEFINITION == "HELP":
        return "KC_HELP"
    elif DEFINITION == "MENU":
        return "KC_MENU"
    elif DEFINITION == "SELECT" or DEFINITION == "SLCT":
        return "KC_SELECT"
    elif DEFINITION == "STOP":
        return "KC_STOP"
    elif DEFINITION == "AGAIN" or DEFINITION == "AGIN":
        return "KC_AGAIN"
    elif DEFINITION == "UNDO":
        return "KC_UNDO"
    elif DEFINITION == "CUT":
        return "KC_CUT"
    elif DEFINITION == "COPY":
        return "KC_COPY"
    elif DEFINITION == "PASTE" or DEFINITION == "PSTE":
        return "KC_PASTE"
    elif DEFINITION == "FIND":
        return "KC_FIND"
    elif DEFINITION == "MUTE":
        return "KC__MUTE"
    elif DEFINITION == "VOLUP":
        return "KC__VOLUP"
    elif DEFINITION == "VOLDOWN":
        return "KC__VOLDOWN"
    elif DEFINITION == "ALT_ERASE" or DEFINITION == "ERAS":
        return "KC_ALT_ERASE"
    elif DEFINITION == "SYSREQ":
        return "KC_SYSREQ"
    elif DEFINITION == "CANCEL":
        return "KC_CANCEL"
    elif DEFINITION == "CLEAR" or DEFINITION == "CLR":
        return "KC_CLEAR"
    elif DEFINITION == "PRIOR":
        return "KC_PRIOR"
    elif DEFINITION == "RETURN":
        return "KC_RETURN"
    elif DEFINITION == "SEPARATOR":
        return "KC_SEPARATOR"
    elif DEFINITION == "OUT":
        return "KC_OUT"
    elif DEFINITION == "OPER":
        return "KC_OPER"
    elif DEFINITION == "CLEAR_AGAIN":
        return "KC_CLEAR_AGAIN"
    elif DEFINITION == "CRSEL":
        return "KC_CRSEL"
    elif DEFINITION == "EXSEL":
        return "KC_EXSEL"
    elif DEFINITION == "SYSTEM_POWER" or DEFINITION == "PWR":
        return "KC_PWR"
    elif DEFINITION == "SYSTEM_SLEEP" or DEFINITION == "SLEP":
        return "KC_SLEP"
    elif DEFINITION == "SYSTEM_WAKE" or DEFINITION == "WAKE":
        return "KC_WAKE"
    elif DEFINITION == "AUDIO_MUTE" or DEFINITION == "MUTE":
        return "KC_MUTE"
    elif DEFINITION == "AUDIO_VOL_UP" or DEFINITION == "VOLU":
        return "KC_VOLU"
    elif DEFINITION == "AUDIO_VOL_DOWN" or DEFINITION == "VOLD":
        return "KC_VOLD"
    elif DEFINITION == "MEDIA_NEXT_TRACK" or DEFINITION == "MNXT":
        return "KC_MNXT"
    elif DEFINITION == "MEDIA_PREV_TRACK" or DEFINITION == "CPRV":
        return "KC_MPRV"
    elif DEFINITION == "MEDIA_STOP" or DEFINITION == "MSTP":
        return "KC_MSTP"
    elif DEFINITION == "MEDIA_PLAY_PAUSE" or DEFINITION == "MPLY":
        return "KC_MPLY"
    elif DEFINITION == "MEDIA_SELECT" or DEFINITION == "MSEL":
        return "KC_MSEL"
    elif DEFINITION == "MEDIA_EJECT" or DEFINITION == "EJCT":
        return "KC_EJCT"
    elif DEFINITION == "MAIL":
        return "KC_MAIL"
    elif DEFINITION == "CALCULATOR" or DEFINITION == "CALC":
        return "KC_CALC"
    elif DEFINITION == "MY_COMPUTER" or DEFINITION == "MYCM":
        return "KC_MYCM"
    elif DEFINITION == "WWW_SEATCH" or DEFINITION == "WSCH":
        return "KC_WSCH"
    elif DEFINITION == "WWW_HOME" or DEFINITION == "WHOM":
        return "KC_WHOM"
    elif DEFINITION == "WWW_BACK" or DEFINITION == "WBAK":
        return "KC_WBAK"
    elif DEFINITION == "WWW_FORWARD" or DEFINITION == "WFWD":
        return "KC_WFWD"
    elif DEFINITION == "WWW_STOP" or DEFINITION == "WSTP":
        return "KC_WSTP"
    elif DEFINITION == "WWW_REFRESH" or DEFINITION == "WREF":
        return "KC_WREF"
    elif DEFINITION == "WWW_FAVORITES" or DEFINITION == "WFAV":
        return "KC_WFAV"
    elif DEFINITION == "MEDIA_FAST_FORWARD" or DEFINITION == "MFFD":
        return "KC_MFFD"
    elif DEFINITION == "MEDIA_REWIND" or DEFINITION == "MRWD":
        return "KC_MRWD"
    elif DEFINITION == "BRIGHTNESS_UP" or DEFINITION == "BRIU":
        return "KC_BRIU"
    elif DEFINITION == "BRIGHTNESS_DOWN" or DEFINITION == "BRID":
        return "KC_BRID"
    elif DEFINITION == "KP_SLASH" or DEFINITION == "PSLS":
        return "KC_PSLS"
    elif DEFINITION == "KP_ASTERISK" or DEFINITION == "PAST":
        return "KC_PAST"
    elif DEFINITION == "KP_MINUS" or DEFINITION == "PMNS":
        return "KC_PMNS"
    elif DEFINITION == "KP_PLUS" or DEFINITION == "PPLS":
        return "KC_PPLS"
    elif DEFINITION == "KP_ENTER" or DEFINITION == "PENT":
        return "KC_PENT"
    elif DEFINITION == "KP_1" or DEFINITION == "P1":
        return "KC_P1"
    elif DEFINITION == "KP_2" or DEFINITION == "P2":
        return "KC_P2"
    elif DEFINITION == "KP_3" or DEFINITION == "P3":
        return "KC_P3"
    elif DEFINITION == "KP_4" or DEFINITION == "P4":
        return "KC_P4"
    elif DEFINITION == "KP_5" or DEFINITION == "P5":
        return "KC_P5"
    elif DEFINITION == "KP_6" or DEFINITION == "P6":
        return "KC_P6"
    elif DEFINITION == "KP_7" or DEFINITION == "P7":
        return "KC_P7"
    elif DEFINITION == "KP_8" or DEFINITION == "P8":
        return "KC_P8"
    elif DEFINITION == "KP_9" or DEFINITION == "P9":
        return "KC_P9"
    elif DEFINITION == "KP_0" or DEFINITION == "P0":
        return "KC_P0"
    elif DEFINITION == "KP_DOT" or DEFINITION == "PDOT":
        return "KC_PDOT"
    elif DEFINITION == "KP_EQUAL" or DEFINITION == "PEQL":
        return "KC_PEQL"
    elif DEFINITION == "KP_COMMA" or DEFINITION == "PCMM":
        return "KC_PCMM"
    elif DEFINITION == "MS_BTN1" or DEFINITION == "BTN1":
        return "KC_MS_BTN1"
    elif DEFINITION == "MS_BTN2" or DEFINITION == "BRN2":
        return "KC_MS_BTN2"
    elif DEFINITION == "MS_LEFT" or DEFINITION == "MS_L":
        return "KC_MS_LEFT"
    elif DEFINITION == "MS_DOWN" or DEFINITION == "MS_D":
        return "KC_MS_DOWN"
    elif DEFINITION == "MS_UP" or DEFINITION == "MS_U":
        return "KC_MS_UP"
    elif DEFINITION == "MS_RIGHT" or DEFINITION == "MS_R":
        return "KC_MS_RIGHT"
    elif DEFINITION == "MS_WH_UP" or DEFINITION == "WH_U":
        return "KC_MS_WH_UP"
    elif DEFINITION == "MS_WH_DOWN" or DEFINITION == "WH_D":
        return "KC_MS_WH_DOWN"
    else:
        return DEFINITION
)

$macro(MK, ON_PSEUDOLAYER, KEYCODES_HASH, DEFINITION)
    $nonlocal(NUM_OF_CHORDS)
    $py(l = len(DEFINITION.split(', ')))
    void function_$(NUM_OF_CHORDS)(const struct Chord* self) {
        switch (*self->state) {
            case ACTIVATED:
                $for(i in range(0, l))
                    $py(val = DEFINITION.split(', ')[i])
                    $py(code = expand_keycode_fnc(val))
                    register_code($(code));
                $endfor
                send_keyboard_report();
                break;
            case DEACTIVATED:
                break;
            case FINISHED:
            case FINISHED_FROM_ACTIVE:
                $for(i in range(0, l))
                    $py(val = DEFINITION.split(', ')[i])
                    $py(code = expand_keycode_fnc(val))
                    register_code($(code));
                $endfor
                send_keyboard_report();
                break;
            case RESTART:
                $for(i in range(0, l))
                    $py(val = DEFINITION.split(', ')[i])
                    $py(code = expand_keycode_fnc(val))
                    unregister_code($(code));
                $endfor
                send_keyboard_report();
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
            $elif(type == "CLEAR_KB")
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
            $elif(type == "MK")
                $MK(PSEUDOLAYER, KEYCODES_HASH, val)
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

$macro(butterstick_rows, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K40, K41, K42)
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
    $add_key(PSEUDOLAYER, "H_TOP10", K10)
    $add_key(PSEUDOLAYER, "H_TOP11", K11)
    $add_key(PSEUDOLAYER, "H_TOP12", K12)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_BOT1", K13)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_BOT2", K14)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_BOT3", K15)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_BOT4", K16)
    $add_key(PSEUDOLAYER, "H_TOP5 + H_BOT5", K17)
    $add_key(PSEUDOLAYER, "H_TOP6 + H_BOT6", K18)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_BOT7", K19)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_BOT8", K20)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_BOT9", K21)
    $add_key(PSEUDOLAYER, "H_TOP10 + H_BOT10", K22)
    $add_key(PSEUDOLAYER, "H_TOP11 + H_BOT11", K23)
    $add_key(PSEUDOLAYER, "H_TOP12 + H_BOT12", K24)
    $add_key(PSEUDOLAYER, "H_BOT1", K25)
    $add_key(PSEUDOLAYER, "H_BOT2", K26)
    $add_key(PSEUDOLAYER, "H_BOT3", K27)
    $add_key(PSEUDOLAYER, "H_BOT4", K28)
    $add_key(PSEUDOLAYER, "H_BOT5", K29)
    $add_key(PSEUDOLAYER, "H_BOT6", K30)
    $add_key(PSEUDOLAYER, "H_BOT7", K31)
    $add_key(PSEUDOLAYER, "H_BOT8", K32)
    $add_key(PSEUDOLAYER, "H_BOT9", K33)
    $add_key(PSEUDOLAYER, "H_BOT10", K34)
    $add_key(PSEUDOLAYER, "H_BOT11", K35)
    $add_key(PSEUDOLAYER, "H_BOT12", K36)
    $add_key(PSEUDOLAYER, "H_THU1", K37)
    $add_key(PSEUDOLAYER, "H_THU2", K38)
    $add_key(PSEUDOLAYER, "H_THU3", K39)
    $add_key(PSEUDOLAYER, "H_THU4", K40)
    $add_key(PSEUDOLAYER, "H_THU5", K41)
    $add_key(PSEUDOLAYER, "H_THU6", K42)
$endmacro

$macro(butterstick_cols, PSEUDOLAYER, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30, K31, K32, K33, K34, K35, K36, K37, K38)
    $nonlocal(NUM_OF_CHORDS)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_TOP2", K1)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_TOP3", K2)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_TOP4", K3)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_TOP5", K4)
    $add_key(PSEUDOLAYER, "H_TOP5 + H_TOP6", K5)
    $add_key(PSEUDOLAYER, "H_TOP6 + H_TOP7", K6)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_TOP8", K7)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_TOP9", K8)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_TOP10", K9)
    $add_key(PSEUDOLAYER, "H_TOP10 + H_TOP11", K10)
    $add_key(PSEUDOLAYER, "H_TOP11 + H_TOP12", K11)
    $add_key(PSEUDOLAYER, "H_TOP1 + H_BOT1 + H_TOP2 + H_BOT2", K12)
    $add_key(PSEUDOLAYER, "H_TOP2 + H_BOT2 + H_TOP3 + H_BOT3", K13)
    $add_key(PSEUDOLAYER, "H_TOP3 + H_BOT3 + H_TOP4 + H_BOT4", K14)
    $add_key(PSEUDOLAYER, "H_TOP4 + H_BOT4 + H_TOP5 + H_BOT5", K15)
    $add_key(PSEUDOLAYER, "H_TOP5 + H_BOT5 + H_TOP6 + H_BOT6", K16)
    $add_key(PSEUDOLAYER, "H_TOP6 + H_BOT6 + H_TOP7 + H_BOT7", K17)
    $add_key(PSEUDOLAYER, "H_TOP7 + H_BOT7 + H_TOP8 + H_BOT8", K18)
    $add_key(PSEUDOLAYER, "H_TOP8 + H_BOT8 + H_TOP9 + H_BOT9", K19)
    $add_key(PSEUDOLAYER, "H_TOP9 + H_BOT9 + H_TOP10 + H_BOT10", K20)
    $add_key(PSEUDOLAYER, "H_TOP10 + H_BOT10 + H_TOP11 + H_BOT11", K21)
    $add_key(PSEUDOLAYER, "H_TOP11 + H_BOT11 + H_TOP12 + H_BOT12", K22)
    $add_key(PSEUDOLAYER, "H_BOT1 + H_BOT2", K23)
    $add_key(PSEUDOLAYER, "H_BOT2 + H_BOT3", K24)
    $add_key(PSEUDOLAYER, "H_BOT3 + H_BOT4", K25)
    $add_key(PSEUDOLAYER, "H_BOT4 + H_BOT5", K26)
    $add_key(PSEUDOLAYER, "H_BOT5 + H_BOT6", K27)
    $add_key(PSEUDOLAYER, "H_BOT6 + H_BOT7", K28)
    $add_key(PSEUDOLAYER, "H_BOT7 + H_BOT8", K29)
    $add_key(PSEUDOLAYER, "H_BOT8 + H_BOT9", K30)
    $add_key(PSEUDOLAYER, "H_BOT9 + H_BOT10", K31)
    $add_key(PSEUDOLAYER, "H_BOT10 + H_BOT11", K32)
    $add_key(PSEUDOLAYER, "H_BOT11 + H_BOT12", K33)
    $add_key(PSEUDOLAYER, "H_THU1 + H_THU2", K34)
    $add_key(PSEUDOLAYER, "H_THU2 + H_THU3", K35)
    $add_key(PSEUDOLAYER, "H_THU3 + H_THU4", K36)
    $add_key(PSEUDOLAYER, "H_THU4 + H_THU5", K37)
    $add_key(PSEUDOLAYER, "H_THU5 + H_THU6", K38)
$endmacro

$macro(secret_chord, PSEUDOLAYER, ACTION, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, TH1, TH2, TH3, TH4, TH5, TH6)
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
    $if(T10 != "")
        $py(hash = hash + " + H_TOP10")
    $endif
    $if(T11 != "")
        $py(hash = hash + " + H_TOP11")
    $endif
    $if(T12 != "")
        $py(hash = hash + " + H_TOP12")
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
    $if(B10 != "")
        $py(hash = hash + " + H_BOT10")
    $endif
    $if(B11 != "")
        $py(hash = hash + " + H_BOT11")
    $endif
    $if(B12 != "")
        $py(hash = hash + " + H_BOT12")
    $endif
    $if(TH1 != "")
        $py(hash = hash + " + H_THU1")
    $endif
    $if(TH2 != "")
        $py(hash = hash + " + H_THU2")
    $endif
    $if(TH3 != "")
        $py(hash = hash + " + H_THU3")
    $endif
    $if(TH4 != "")
        $py(hash = hash + " + H_THU4")
    $endif
    $if(TH5 != "")
        $py(hash = hash + " + H_THU5")
    $endif
    $if(TH6 != "")
        $py(hash = hash + " + H_THU6")
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
