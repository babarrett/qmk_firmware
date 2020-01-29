import json
from functools import reduce 

def top_level_split(s):
    """
    Split `s` by top-level commas only. Commas within parentheses are ignored.
    """
    
    # Parse the string tracking whether the current character is within
    # parentheses.
    balance = 0
    parts = []
    part = ""
    
    for i in range(len(s)):
        c = s[i]
        part += c
        if c == '(':
            balance += 1
        elif c == ')':
            balance -= 1
        elif c == ',' and balance == 0 and not s[i+1] == ',':
            part = part[:-1].strip()
            parts.append(part)
            part = ""
    
    # Capture last part
    if len(part):
        parts.append(part.strip())
    
    return parts

def KC(on_pseudolayer, keycodes_hash, keycode, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + keycode + ", 0, single_dance};\n"
    index += 1
    return [output_buffer, index]

def AS(on_pseudolayer, keycodes_hash, keycode, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "uint8_t counter_" + str(index) + " = 0;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", &counter_" + str(index) + ", " + keycode + ", 0, autoshift_dance};\n"
    index += 1
    return [output_buffer, index]

def AT(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, autoshift_toggle};\n"
    index += 1
    return [output_buffer, index]

def KL(on_pseudolayer, keycodes_hash, keycode, to_pseudolayer, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + keycode + ", " + to_pseudolayer + ", key_layer_dance};\n"
    index += 1
    return [output_buffer, index]

def KK(on_pseudolayer, keycodes_hash, keycode1, keycode2, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + keycode1 + ", " + keycode2 + ", key_key_dance};\n"
    index += 1
    return [output_buffer, index]

def KM(on_pseudolayer, keycodes_hash, keycode, to_pseudolayer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + keycode + ", " + to_pseudolayer + ", key_mod_dance};\n"
    index += 1
    return [output_buffer, index]

def MO(on_pseudolayer, keycodes_hash, to_pseudolayer, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + to_pseudolayer + ", 0, temp_pseudolayer};\n"
    index += 1
    return [output_buffer, index]

def LOCK(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, lock};\n"
    index += 1
    return [output_buffer, index]

def DF(on_pseudolayer, keycodes_hash, to_pseudolayer, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + to_pseudolayer + ", 0, perm_pseudolayer};\n"
    index += 1
    return [output_buffer, index]

def TO(on_pseudolayer, keycodes_hash, to_pseudolayer, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + to_pseudolayer + ", 0, switch_layer};\n"
    index += 1
    return [output_buffer, index]

def OSK(on_pseudolayer, keycodes_hash, keycode, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + keycode + ", 0, one_shot_key};\n"
    index += 1
    return [output_buffer, index]

def OSL(on_pseudolayer, keycodes_hash, to_pseudolayer, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, " + to_pseudolayer + ", 0, one_shot_layer};\n"
    index += 1
    return [output_buffer, index]

def CMD(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, command};\n"
    index += 1
    return [output_buffer, index]

def DM_RECORD(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, dynamic_macro_record};\n"
    index += 1
    return [output_buffer, index]

def DM_NEXT(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, dynamic_macro_next};\n"
    index += 1
    return [output_buffer, index]

def DM_END(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, dynamic_macro_end};\n"
    index += 1
    return [output_buffer, index]

def DM_PLAY(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, dynamic_macro_play};\n"
    index += 1
    return [output_buffer, index]

def LEAD(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, leader};\n"
    index += 1
    return [output_buffer, index]

def CLEAR(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, clear};\n"
    index += 1
    return [output_buffer, index]

def RESET(on_pseudolayer, keycodes_hash, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, reset};\n"
    index += 1
    return [output_buffer, index]

def STR(on_pseudolayer, keycodes_hash, string_input, output_buffer, index):
    output_buffer += "void str_" + str(index) + "(const struct Chord* self) {\n"
    output_buffer += "    if (*self->state == ACTIVATED) {\n"
    output_buffer += '        SEND_STRING("' + string_input + '");\n'
    output_buffer += "    }\n"
    output_buffer += "}\n"
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", NULL, 0, 0, str_" + str(index) + "};\n"
    index += 1
    return [output_buffer, index]

def M(on_pseudolayer, keycodes_hash, value1, value2, fnc, output_buffer, index):
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "uint8_t counter_" + str(index) + " = 0;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", &counter_" + str(index) + ", " + value1 + ", " + value2 + ", " + fnc + "};\n"
    index += 1
    return [output_buffer, index]

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

def MK(on_pseudolayer, keycodes_hash, definition, output_buffer, index):
    l = len(DEFINITION.split(', '))
    output_buffer += "void function_" + str(index) + "(const struct Chord* self) {\n"
    output_buffer += "    switch (*self->state) {\n"
    output_buffer += "        case ACTIVATED:\n"
    for i in range(0, l):
        val = definition.split(',')[i].strip()
        code = expand_keycode_fnc(val)
        output_buffer += "            key_in(" + code + "):\n"
    output_buffer += "            break;\n"
    output_buffer += "        case DEACTIVATED:\n"
    for i in range(0, l):
        val = definition.split(',')[i].strip()
        code = expand_keycode_fnc(val)
        output_buffer += "            key_out(" + code + "):\n"
    output_buffer += "            *self->state = IDLE;\n"
    output_buffer += "            break;\n"
    output_buffer += "        case RESTART:\n"
    for i in range(0, l):
        val = definition.split(',')[i].strip()
        code = expand_keycode_fnc(val)
        output_buffer += "            key_out(" + code + "):\n"
    output_buffer += "            break;\n"
    output_buffer += "        default:\n"
    output_buffer += "            break;\n"
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "uint8_t counter_" + str(index) + " = 0;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", &counter_" + str(index) + ", 0, 0, function_" + str(index) + ")};\n"
    index += 1
    return [output_buffer, index]

def D(on_pseudolayer, keycodes_hash, DEFINITION, output_buffer, index):
    l = len(DEFINITION.split(','))
    output_buffer += "void function_" + str(index) + "(const struct Chord* self) {\n"
    output_buffer += "    switch (*self->state) {\n"
    output_buffer += "        case ACTIVATED:\n"
    output_buffer += "            *self->counter = *self->counter + 1;\n"
    output_buffer += "            break;\n"
    output_buffer += "        case PRESS_FROM_ACTIVE:\n"
    output_buffer += "            switch (*self->counter) {\n"
    for i in range(0, l):
        val = DEFINITION.split(',')[i].strip()
        code = expand_keycode_fnc(val)
        output_buffer += "                case " + str(i + 1) + ":\n"
        output_buffer += "                    key_in( " + code + ");\n"
        output_buffer += "                    break;\n"
    output_buffer += "                default:\n"
    output_buffer += "                    break;\n"
    output_buffer += "            }\n"
    output_buffer += "            *self->state = FINISHED_FROM_ACTIVE;\n"
    output_buffer += "            break;\n"
    output_buffer += "        case FINISHED:\n"
    output_buffer += "            switch (*self->counter) {\n"
    for i in range(0, l):
        val = DEFINITION.split(',')[i].strip()
        code = expand_keycode_fnc(val)
        output_buffer += "                case " + str(i + 1) + ":\n"
        output_buffer += "                    tap_key( " + code + ");\n"
        output_buffer += "                    break;\n"
    output_buffer += "                default:\n"
    output_buffer += "                    break;\n"
    output_buffer += "            }\n"
    output_buffer += "            *self->counter = 0;\n"
    output_buffer += "            *self->state = IDLE;\n"
    output_buffer += "            break;\n"
    output_buffer += "        case RESTART:\n"
    output_buffer += "            switch (*self->counter) {\n"
    for i in range(0, l):
        val = DEFINITION.split(',')[i].strip()
        code = expand_keycode_fnc(val)
        output_buffer += "                case " + str(i + 1) + ":\n"
        output_buffer += "                    key_out( " + code + ");\n"
        output_buffer += "                    break;\n"
    output_buffer += "                default:\n"
    output_buffer += "                    break;\n"
    output_buffer += "            }\n"
    output_buffer += "            *self->counter = 0;\n"
    output_buffer += "            break;\n"
    output_buffer += "        default:\n"
    output_buffer += "            break;\n"
    output_buffer += "    }\n"
    output_buffer += "}\n"
    output_buffer += "uint8_t state_" + str(index) + " = IDLE;\n"
    output_buffer += "uint8_t counter_" + str(index) + " = 0;\n"
    output_buffer += "const struct Chord chord_" + str(index) + " PROGMEM = {" + keycodes_hash + ", " + on_pseudolayer + ", &state_" + str(index) + ", &counter_" + str(index) + ", 0, 0, function_" + str(index) + ")};\n"
    index += 1
    return [output_buffer, index]

def O(on_pseudolayer, keycodes_hash, DEFINITION, output_buffer, index):
    if DEFINITION[0:3] == "KC_":
        return OSK(on_pseudolayer, keycodes_hash, DEFINITION, output_buffer, index)
    else:
        return OSL(on_pseudolayer, keycodes_hash, DEFINITION, output_buffer, index)

def add_key(PSEUDOLAYER, KEYCODES_HASH, DEFINITION, output_buffer, index):
    if(DEFINITION == ""):
        return [output_buffer, index]
    else:
        split = DEFINITION.split("(")
        type = split[0].strip()
        if len(split) == 1:
            if type == "LOCK":
                [output_buffer, index] = LOCK(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "AT":
                [output_buffer, index] = AT(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "CMD":
                [output_buffer, index] = CMD(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "LEAD":
                [output_buffer, index] = LEAD(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "DM_RECORD":
                [output_buffer, index] = DM_RECORD(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "DM_NEXT":
                [output_buffer, index] = DM_NEXT(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "DM_END":
                [output_buffer, index] = DM_END(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "DM_PLAY":
                [output_buffer, index] = DM_PLAY(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "CLEAR_KB":
                [output_buffer, index] = CLEAR(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            elif type == "RESET":
                [output_buffer, index] = RESET(PSEUDOLAYER, KEYCODES_HASH, output_buffer, index)
            else:
                code = expand_keycode_fnc(type)
                [output_buffer, index] = KC(PSEUDOLAYER, KEYCODES_HASH, code, output_buffer, index)
        else:
            val = split[1][:-1].strip()
            if type == "O":
                code = expand_keycode_fnc(val)
                [output_buffer, index] = O(PSEUDOLAYER, KEYCODES_HASH, code, output_buffer, index)
            elif type == "D":
                [output_buffer, index] = D(PSEUDOLAYER, KEYCODES_HASH, val, output_buffer, index)
            elif type == "MK":
                [output_buffer, index] = MK(PSEUDOLAYER, KEYCODES_HASH, val, output_buffer, index)
            elif type == "M":
                fnc = val.split(',')[0].strip()
                val1 = val.split(',')[1].strip()
                val2 = val.split(',')[2].strip()
                [output_buffer, index] = M(PSEUDOLAYER, KEYCODES_HASH, val1, val2, fnc, output_buffer, index)
            elif type == "KK":
                val1 = val.split(',')[0].strip()
                code1 = expand_keycode_fnc(val1)
                val2 = val.split(',')[1].strip()
                code2 = expand_keycode_fnc(val2)
                [output_buffer, index] = KK(PSEUDOLAYER, KEYCODES_HASH, code1, code2, output_buffer, index)
            elif type == "KL":
                val1 = val.split(',')[0].strip()
                code1 = expand_keycode_fnc(val1)
                val2 = val.split(',')[1].strip()
                [output_buffer, index] = KL(PSEUDOLAYER, KEYCODES_HASH, code1, val2, output_buffer, index)
            elif type == "KM":
                val1 = val.split(',')[0].strip()
                code1 = expand_keycode_fnc(val1)
                val2 = val.split(',')[1].strip()
                code2 = expand_keycode_fnc(val2)
                [output_buffer, index] = KM(PSEUDOLAYER, KEYCODES_HASH, code1, code2, output_buffer, index)
            elif type == "AS":
                code = expand_keycode_fnc(val)
                [output_buffer, index] = AS(PSEUDOLAYER, KEYCODES_HASH, code, output_buffer, index)
            elif type == "MO":
                [output_buffer, index] = MO(PSEUDOLAYER, KEYCODES_HASH, val, output_buffer, index)
            elif type == "DF":
                [output_buffer, index] = DF(PSEUDOLAYER, KEYCODES_HASH, val, output_buffer, index)
            elif type == "TO":
                [output_buffer, index] = TO(PSEUDOLAYER, KEYCODES_HASH, val, output_buffer, index)
            elif type == "STR":
                [output_buffer, index] = STR(PSEUDOLAYER, KEYCODES_HASH, val, output_buffer, index)
    return [output_buffer, index]

def add_leader_combo(DEFINITION, FUNCTION):
    return list_of_leader_combos.append([DEFINITION, FUNCTION])

def add_chord_set(PSEUDOLAYER, INPUT_STRING, TYPE, data, output_buffer, index):
    chord_set = {}
    for set in data["chord_sets"]:
        if set["name"] == TYPE:
            chord_set = set["chords"]
            break
    
    separated_string = top_level_split(INPUT_STRING)
    for word, chord in zip(separated_string, chord_set):
        chord_hash = reduce((lambda x, y: str(x) + " + " + str(y)), ["H_" + key for key in chord])
        [output_buffer, index] = add_key(PSEUDOLAYER, chord_hash, word, output_buffer, index)
    
    return [output_buffer, index]

def secret_chord(PSEUDOLAYER, ACTION, INPUT_STRING, output_buffer, index):
    separated_string = top_level_split(INPUT_STRING)
    hash = ""
    if separated_string[0] != "":
        hash = hash + " + H_TOP1"
    if separated_string[1] != "":
        hash = hash + " + H_TOP2"
    if separated_string[2] != "":
        hash = hash + " + H_TOP3"
    if separated_string[3] != "":
        hash = hash + " + H_TOP4"
    if separated_string[4] != "":
        hash = hash + " + H_TOP5"
    if separated_string[5] != "":
        hash = hash + " + H_TOP6"
    if separated_string[6] != "":
        hash = hash + " + H_TOP7"
    if separated_string[7] != "":
        hash = hash + " + H_TOP8"
    if separated_string[8] != "":
        hash = hash + " + H_TOP9"
    if separated_string[9] != "":
        hash = hash + " + H_TOP0"
    if separated_string[10] != "":
        hash = hash + " + H_BOT1"
    if separated_string[11] != "":
        hash = hash + " + H_BOT2"
    if separated_string[12] != "":
        hash = hash + " + H_BOT3"
    if separated_string[13] != "":
        hash = hash + " + H_BOT4"
    if separated_string[14] != "":
        hash = hash + " + H_BOT5"
    if separated_string[15] != "":
        hash = hash + " + H_BOT6"
    if separated_string[16] != "":
        hash = hash + " + H_BOT7"
    if separated_string[17] != "":
        hash = hash + " + H_BOT8"
    if separated_string[18] != "":
        hash = hash + " + H_BOT9"
    if separated_string[19] != "":
        hash = hash + " + H_BOT0"
    hash = hash[3:]
    if hash != "":
        return add_key(PSEUDOLAYER, hash, ACTION, output_buffer, index)

number_of_keys = 0
hash_type = ""
output_buffer = ""
number_of_chords = 0
list_of_leader_combos = []

with open("keymap_def.json", "r") as read_file:
    data = json.load(read_file)
    with open("keymap.c", "w") as write_file:
        number_of_keys = len(data["keys"])
        if number_of_keys <= 8:
            hash_type = "uint8_t"
        elif number_of_keys <= 16:
            hash_type = "uint16_t"
        elif number_of_keys <= 32:
            hash_type = "uint32_t"
        elif number_of_keys <= 64:
            hash_type = "uint64_t"
        else:
            raise Exception()
        
        output_buffer += "#include QMK_KEYBOARD_H\n"
        output_buffer += "\n"
        
        if len(data["extra_dependencies"]) > 0:
            for dependecy in data["extra_dependencies"]:
                output_buffer += '#include "' + dependecy + '"\n'
        output_buffer += "\n"
        
        output_buffer += "#define CHORD_TIMEOUT " + str(data["keyboard_parameters"]["chord_timeout"]) + "\n"
        output_buffer += "#define DANCE_TIMEOUT " + str(data["keyboard_parameters"]["dance_timeout"]) + "\n"
        output_buffer += "#define LEADER_TIMEOUT " + str(data["keyboard_parameters"]["leader_timeout"]) + "\n"
        output_buffer += "#define TAP_TIMEOUT " + str(data["keyboard_parameters"]["tap_timeout"]) + "\n"
        output_buffer += "#define DYNAMIC_MACRO_MAX_LENGTH " + str(data["keyboard_parameters"]["dynamic_macro_max_length"]) + "\n"
        output_buffer += "#define COMMAND_MAX_LENGTH " + str(data["keyboard_parameters"]["command_max_length"]) + "\n"
        output_buffer += "#define LEADER_MAX_LENGTH " + str(data["keyboard_parameters"]["leader_max_length"]) + "\n"
        output_buffer += "\n"
        
        output_buffer += "enum pseudolayers {\n"
        output_buffer += "    " + reduce((lambda x, y: str(x) + ", " + str(y)), [layer["name"] for layer in data["pseudolayers"]]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "enum internal_keycodes {\n"
        output_buffer += "    " + data["keys"][0] + " = SAFE_RANGE,\n"
        output_buffer += "    " + reduce((lambda x, y: str(x) + ", " + str(y)), [key for key in data["keys"][1:]]) + ",\n"
        output_buffer += "    FIRST_INTERNAL_KEYCODE = " + data["keys"][0] + ",\n"
        output_buffer += "    LAST_INTERNAL_KEYCODE = " + data["keys"][-1] + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {\n"
        output_buffer += "    [0] = LAYOUT_butter(" + reduce((lambda x, y: str(x) + ", " + str(y)), [key for key in data["keys"]]) + ")\n"
        output_buffer += "};\n"
        output_buffer += "size_t keymapsCount = 1;\n"
        output_buffer += "\n"
        
        counter = 0
        for key in data["keys"]:
            output_buffer += "#define H_" + key + " ((" + hash_type + ") 1 << " + str(counter) + ")\n"
            counter += 1
        output_buffer += "\n"
        
        output_buffer += "uint8_t current_pseudolayer = 1;\n"
        output_buffer += "bool lock_next = false;\n"
        output_buffer += "uint16_t chord_timer = 0;\n"
        output_buffer += "uint16_t dance_timer = 0;\n"
        output_buffer += "bool autoshift_mode = true;\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t keycodes_buffer_array[] = {\n"
        output_buffer += "    " + ("0, " * len(data["keys"])) + "\n"
        output_buffer += "};\n"
        output_buffer += "uint8_t keycode_index = 0;\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t command_mode = 0;\n"
        output_buffer += "uint8_t command_buffer[] = {\n"
        output_buffer += "    " + ("0, " * data["keyboard_parameters"]["command_max_length"]) + "\n"
        output_buffer += "};\n"
        output_buffer += "uint8_t command_ind = 0;\n"
        output_buffer += "\n"
        
        output_buffer += "bool in_leader_mode = false;\n"
        output_buffer += "uint16_t leader_buffer[] = {\n"
        output_buffer += "    " + ("0, " * data["keyboard_parameters"]["leader_max_length"]) + "\n"
        output_buffer += "};\n"
        output_buffer += "uint8_t leader_ind = 0;\n"
        output_buffer += "uint16_t leader_timer = 0;\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t dynamic_macro_mode = false;\n"
        output_buffer += "uint8_t dynamic_macro_buffer[] = {\n"
        output_buffer += "    " + ("0, " * data["keyboard_parameters"]["dynamic_macro_max_length"]) + "\n"
        output_buffer += "};\n"
        output_buffer += "uint8_t dynamic_macro_ind = 0;\n"
        output_buffer += "\n"
        
        output_buffer += "bool a_key_went_through = false;\n"
        output_buffer += "\n"
        output_buffer += "enum chord_states {\n"
        output_buffer += "    IDLE,\n"
        output_buffer += "    READY,\n"
        output_buffer += "    ACTIVATED,\n"
        output_buffer += "    DEACTIVATED,\n"
        output_buffer += "    PRESS_FROM_ACTIVE,\n"
        output_buffer += "    FINISHED_FROM_ACTIVE,\n"
        output_buffer += "    IDLE_IN_DANCE,\n"
        output_buffer += "    READY_IN_DANCE,\n"
        output_buffer += "    FINISHED,\n"
        output_buffer += "    LOCKED,\n"
        output_buffer += "    READY_LOCKED,\n"
        output_buffer += "    RESTART,\n"
        output_buffer += "    IN_ONE_SHOT\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        output_buffer += "struct Chord {\n"
        output_buffer += "    uint32_t keycodes_hash;\n"
        output_buffer += "    uint8_t pseudolayer;\n"
        output_buffer += "    uint8_t* state;\n"
        output_buffer += "    uint8_t* counter;\n"
        output_buffer += "    uint16_t value1;\n"
        output_buffer += "    uint8_t value2;\n"
        output_buffer += "    void (*function) (const struct Chord*);\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        output_buffer += "bool handle_US_ANSI_shifted_keys(int16_t keycode, bool in) {\n"
        output_buffer += "    bool is_US_ANSI_shifted = true;\n"
        output_buffer += "    int16_t regular_keycode = KC_NO;\n"
        output_buffer += "    switch (keycode) {\n"
        output_buffer += "        case KC_TILDE:\n"
        output_buffer += "            regular_keycode = KC_GRAVE;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_EXCLAIM:\n"
        output_buffer += "            regular_keycode = KC_1;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_AT:\n"
        output_buffer += "            regular_keycode = KC_2;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_HASH:\n"
        output_buffer += "            regular_keycode = KC_3;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_DOLLAR:\n"
        output_buffer += "            regular_keycode = KC_4;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_PERCENT:\n"
        output_buffer += "            regular_keycode = KC_5;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_CIRCUMFLEX:\n"
        output_buffer += "            regular_keycode = KC_6;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_AMPERSAND:\n"
        output_buffer += "            regular_keycode = KC_7;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_ASTERISK:\n"
        output_buffer += "            regular_keycode = KC_8;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_LEFT_PAREN:\n"
        output_buffer += "            regular_keycode = KC_9;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_RIGHT_PAREN:\n"
        output_buffer += "            regular_keycode = KC_0;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_UNDERSCORE:\n"
        output_buffer += "            regular_keycode = KC_MINUS;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_PLUS:\n"
        output_buffer += "            regular_keycode = KC_EQUAL;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_LEFT_CURLY_BRACE:\n"
        output_buffer += "            regular_keycode = KC_LBRACKET;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_RIGHT_CURLY_BRACE:\n"
        output_buffer += "            regular_keycode = KC_RBRACKET;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_PIPE:\n"
        output_buffer += "            regular_keycode = KC_BSLASH;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_COLON:\n"
        output_buffer += "            regular_keycode = KC_SCOLON;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_DOUBLE_QUOTE:\n"
        output_buffer += "            regular_keycode = KC_QUOTE;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_LEFT_ANGLE_BRACKET:\n"
        output_buffer += "            regular_keycode = KC_COMMA;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_RIGHT_ANGLE_BRACKET:\n"
        output_buffer += "            regular_keycode = KC_DOT;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case KC_QUESTION:\n"
        output_buffer += "            regular_keycode = KC_SLASH;\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            is_US_ANSI_shifted = false;\n"
        output_buffer += "    }\n"
        output_buffer += "    if (is_US_ANSI_shifted) {\n"
        output_buffer += "        if (in) {\n"
        output_buffer += "            register_code(KC_LSFT);\n"
        output_buffer += "            register_code(regular_keycode);\n"
        output_buffer += "        } else {\n"
        output_buffer += "            unregister_code(regular_keycode);\n"
        output_buffer += "            unregister_code(KC_LSFT);\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "    return is_US_ANSI_shifted;\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void key_in(int16_t keycode) {\n"
        output_buffer += "    if (command_mode == 1 && command_ind < COMMAND_MAX_LENGTH) {\n"
        output_buffer += "        command_buffer[command_ind] = keycode;\n"
        output_buffer += "        command_ind++;\n"
        output_buffer += "        a_key_went_through = true;\n"
        output_buffer += "    } else if (in_leader_mode && leader_ind < LEADER_MAX_LENGTH) {\n"
        output_buffer += "        leader_buffer[leader_ind] = keycode;\n"
        output_buffer += "        leader_ind++;\n"
        output_buffer += "        a_key_went_through = true;\n"
        output_buffer += "    } else if (dynamic_macro_mode && dynamic_macro_ind < DYNAMIC_MACRO_MAX_LENGTH) {\n"
        output_buffer += "        dynamic_macro_buffer[dynamic_macro_ind] = keycode;\n"
        output_buffer += "        dynamic_macro_ind++;\n"
        output_buffer += "        a_key_went_through = true;\n"
        output_buffer += "    } else {\n"
        output_buffer += "        if (!handle_US_ANSI_shifted_keys(keycode, true)) {\n"
        output_buffer += "            register_code(keycode);\n"
        output_buffer += "        }\n"
        output_buffer += "        send_keyboard_report();\n"
        output_buffer += "        a_key_went_through = true;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void key_out(int16_t keycode) {\n"
        output_buffer += "    if (command_mode == 0) {\n"
        output_buffer += "        if (!handle_US_ANSI_shifted_keys(keycode, false)) {\n"
        output_buffer += "            if (command_mode == 0 && in_leader_mode == false && dynamic_macro_mode == false) {\n"
        output_buffer += "                unregister_code(keycode);\n"
        output_buffer += "            }\n"
        output_buffer += "        }\n"
        output_buffer += "        send_keyboard_report();\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void tap_key(int16_t keycode) {\n"
        output_buffer += "    key_in(keycode);\n"
        output_buffer += "    wait_ms(TAP_TIMEOUT);\n"
        output_buffer += "    key_out(keycode);\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void single_dance(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            key_in(self->value1);\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "            key_out(self->value1);\n"
        output_buffer += "            *self->state = IDLE;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            key_out(self->value1);\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void key_layer_dance(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            current_pseudolayer = self->value2;\n"
        output_buffer += "            a_key_went_through = false;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            if (!a_key_went_through) {\n"
        output_buffer += "                tap_key(self->value1);\n"
        output_buffer += "            }\n"
        output_buffer += "            current_pseudolayer = self->pseudolayer;\n"
        output_buffer += "            *self->state = IDLE; // does not have effect if the state was RESTART\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void key_mod_dance(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            key_in(self->value2);\n"
        output_buffer += "            a_key_went_through = false;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            key_out(self->value2);\n"
        output_buffer += "            if (!a_key_went_through) {\n"
        output_buffer += "                tap_key(self->value1);\n"
        output_buffer += "            }\n"
        output_buffer += "            *self->state = IDLE; // does not have effect if the state was RESTART\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void key_key_dance(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "            tap_key(self->value1);\n"
        output_buffer += "            *self->state = IDLE;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case FINISHED:\n"
        output_buffer += "        case PRESS_FROM_ACTIVE:\n"
        output_buffer += "            key_in(self->value2);\n"
        output_buffer += "            break;\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            key_out(self->value2);\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void autoshift_dance_impl(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            *self->counter = 0;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            tap_key(self->value1);\n"
        output_buffer += "            *self->state = IDLE;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case FINISHED_FROM_ACTIVE:\n"
        output_buffer += "            if (*self->counter == " + str(data["keyboard_parameters"]["long_press_multiplier"] - 2) + ") {\n"
        output_buffer += "                key_in(KC_LSFT);\n"
        output_buffer += "                tap_key(self->value1);\n"
        output_buffer += "                key_out(KC_LSFT);\n"
        output_buffer += "                *self->state = IDLE;\n"
        output_buffer += "                // the skip to IDLE is usually just a lag optimization,\n"
        output_buffer += "                // in this case it has a logic function, on a short\n"
        output_buffer += "                // press (still longer than a tap) the key does not get shifted\n"
        output_buffer += "            } else {\n"
        output_buffer += "                *self->counter += 1;\n"
        output_buffer += "                *self->state = PRESS_FROM_ACTIVE;\n"
        output_buffer += "                dance_timer = timer_read();\n"
        output_buffer += "            }\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void autoshift_dance(const struct Chord* self) {\n"
        output_buffer += "    if (autoshift_mode) {\n"
        output_buffer += "        autoshift_dance_impl(self);\n"
        output_buffer += "    } else {\n"
        output_buffer += "        single_dance(self);\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void autoshift_toggle(const struct Chord* self){\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        autoshift_mode = !autoshift_mode;\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void temp_pseudolayer(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            current_pseudolayer = self->value1;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "            current_pseudolayer = self->pseudolayer;\n"
        output_buffer += "            *self->state = IDLE;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            current_pseudolayer = self->pseudolayer;\n"
        output_buffer += "            break;\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void perm_pseudolayer(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        current_pseudolayer = self->value1;\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void switch_layer(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        layer_move(self->value1);\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void lock(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        lock_next = true;\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void one_shot_key(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "            key_in(self->value1);\n"
        output_buffer += "            *self->state = IN_ONE_SHOT;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case FINISHED:\n"
        output_buffer += "        case PRESS_FROM_ACTIVE:\n"
        output_buffer += "            key_in(self->value1);\n"
        output_buffer += "            a_key_went_through = false;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            if (a_key_went_through) {\n"
        output_buffer += "                key_out(self->value1);\n"
        output_buffer += "            } else {\n"
        output_buffer += "                *self->state = IN_ONE_SHOT;\n"
        output_buffer += "            }\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void one_shot_layer(const struct Chord* self) {\n"
        output_buffer += "    switch (*self->state) {\n"
        output_buffer += "        case ACTIVATED:\n"
        output_buffer += "            break;\n"
        output_buffer += "        case DEACTIVATED:\n"
        output_buffer += "            current_pseudolayer = self->value1;\n"
        output_buffer += "            *self->state = IN_ONE_SHOT;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case FINISHED:\n"
        output_buffer += "        case PRESS_FROM_ACTIVE:\n"
        output_buffer += "            current_pseudolayer = self->value1;\n"
        output_buffer += "            a_key_went_through = false;\n"
        output_buffer += "            break;\n"
        output_buffer += "        case RESTART:\n"
        output_buffer += "            if (a_key_went_through) {\n"
        output_buffer += "                current_pseudolayer = self->pseudolayer;\n"
        output_buffer += "            } else {\n"
        output_buffer += "                *self->state = IN_ONE_SHOT;\n"
        output_buffer += "            }\n"
        output_buffer += "        default:\n"
        output_buffer += "            break;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void command(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        command_mode++;\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "bool identical(uint16_t* buffer1, uint16_t* buffer2) {\n"
        output_buffer += "    bool same = true;\n"
        output_buffer += "    for (int i = 0; i < LEADER_MAX_LENGTH; i++) {\n"
        output_buffer += "        same = same && (buffer1[i] == buffer2[i]);\n"
        output_buffer += "    }\n"
        output_buffer += "    return same;\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void leader(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        in_leader_mode = true;\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void dynamic_macro_record(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        for (int i = 0; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {\n"
        output_buffer += "            dynamic_macro_buffer[i] = 0;\n"
        output_buffer += "        }\n"
        output_buffer += "        dynamic_macro_mode = true;\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void dynamic_macro_next(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        if (dynamic_macro_mode && dynamic_macro_ind < DYNAMIC_MACRO_MAX_LENGTH) {\n"
        output_buffer += "            dynamic_macro_buffer[dynamic_macro_ind] = 0;\n"
        output_buffer += "            dynamic_macro_ind++;\n"
        output_buffer += "        }\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void dynamic_macro_end(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        if (dynamic_macro_mode) {\n"
        output_buffer += "            dynamic_macro_mode = false;\n"
        output_buffer += "        }\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void dynamic_macro_play(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        int ind_start = 0;\n"
        output_buffer += "        while (ind_start < DYNAMIC_MACRO_MAX_LENGTH) {\n"
        output_buffer += "            for (int i = ind_start; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {\n"
        output_buffer += "                if (dynamic_macro_buffer[i] == 0) {\n"
        output_buffer += "                    break;\n"
        output_buffer += "                }\n"
        output_buffer += "                register_code(dynamic_macro_buffer[i]);\n"
        output_buffer += "            }\n"
        output_buffer += "            send_keyboard_report();\n"
        output_buffer += "            wait_ms(TAP_TIMEOUT);\n"
        output_buffer += "            for (int i = ind_start; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {\n"
        output_buffer += "                if (dynamic_macro_buffer[i] == 0) {\n"
        output_buffer += "                    ind_start = i + 1;\n"
        output_buffer += "                    break;\n"
        output_buffer += "                }\n"
        output_buffer += "                unregister_code(dynamic_macro_buffer[i]);\n"
        output_buffer += "            }\n"
        output_buffer += "            send_keyboard_report();\n"
        output_buffer += "        }\n"
        output_buffer += "        *self->state = IDLE;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void clear(const struct Chord* self);\n"
        output_buffer += "\n"
        output_buffer += "void reset_keyboard_kb(void){\n"
        output_buffer += "#ifdef WATCHDOG_ENABLE\n"
        output_buffer += "    MCUSR = 0;\n"
        output_buffer += "    wdt_disable();\n"
        output_buffer += "    wdt_reset();\n"
        output_buffer += "#endif\n"
        output_buffer += "    reset_keyboard();\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void reset(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        reset_keyboard_kb();\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        
        output_buffer += "struct Chord* last_chord = NULL;\n\n"
        
        for pseudolayer in data["pseudolayers"]:
            name = pseudolayer["name"]
            for chord_set in pseudolayer["chord_sets"]:
                ks = reduce((lambda x, y: str(x) + ", " + str(y)), [word for word in chord_set["keycodes"]])
                [output_buffer, number_of_chords] = add_chord_set(name, ks, chord_set["chord_set"], data, output_buffer, number_of_chords)
            
            for single_chord in pseudolayer["single_chords"]:
                if single_chord["type"] == "visual":
                    ks = reduce((lambda x, y: str(x) + ", " + str(y)), [word for word in single_chord["chord"]])
                    [output_buffer, number_of_chords] = secret_chord(name, single_chord["keycode"], ks, output_buffer, number_of_chords)
                elif single_chord["type"] == "simple":
                    ks = reduce((lambda x, y: str(x) + " + " + str(y)), ["H_" + word for word in single_chord["chord"]])
                    [output_buffer, number_of_chords] = add_key(name, ks, single_chord["keycode"], output_buffer, number_of_chords)
        output_buffer += "\n"
        
        output_buffer += "const struct Chord* const list_of_chords[] PROGMEM = {\n"
        for i in range (0, number_of_chords):
            output_buffer += "    &chord_" + str(i) + ",\n"
        output_buffer += "};\n\n"
        
        if len(list_of_leader_combos) > 0:
            output_buffer += "const uint16_t leader_triggers[" + str((len(list_of_leader_combos))) + "][LEADER_MAX_LENGTH] PROGMEM = {\n"
            for i in range(0, len(list_of_leader_combos)):
                output_buffer += list_of_leader_combos[i][0] + ", "
            output_buffer += "};"
        else:
            output_buffer += "const uint16_t** const leader_triggers PROGMEM = NULL;\n"
        
        output_buffer += "void (*leader_functions[]) (void) = {\n"
        for i in range(0, len(list_of_leader_combos)):
            output_buffer += list_of_leader_combos[i][1] + ", "
        output_buffer += "};\n\n"
        
        output_buffer += "bool are_hashed_keycodes_in_sound(" + hash_type + " keycodes_hash, " + hash_type + " sound) {\n"
        output_buffer += "    return (keycodes_hash & sound) == keycodes_hash;\n"
        output_buffer += "}\n\n"
        
        output_buffer += "uint8_t keycode_to_index(uint16_t keycode) {\n"
        output_buffer += "    return keycode - FIRST_INTERNAL_KEYCODE;\n"
        output_buffer += "}\n\n"
        
        output_buffer += "void sound_keycode_array(uint16_t keycode) {\n"
        output_buffer += "    uint8_t index = keycode_to_index(keycode);\n"
        output_buffer += "    keycode_index++;\n"
        output_buffer += "    keycodes_buffer_array[index] = keycode_index;\n"
        output_buffer += "}\n\n"
        
        output_buffer += "void silence_keycode_hash_array(" + hash_type + " keycode_hash) {\n"
        output_buffer += "    for (int i = 0; i < " + str(number_of_keys) + "; i++) {\n"
        output_buffer += "        bool index_in_hash = ((" + hash_type + ") 1 << i) & keycode_hash;\n"
        output_buffer += "        if (index_in_hash) {\n"
        output_buffer += "            uint8_t current_val = keycodes_buffer_array[i];\n"
        output_buffer += "            keycodes_buffer_array[i] = 0;\n"
        output_buffer += "            for (int j = 0; j < " + str(number_of_keys) + "; j++) {\n"
        output_buffer += "                if (keycodes_buffer_array[j] > current_val) {\n"
        output_buffer += "                    keycodes_buffer_array[j]--;\n"
        output_buffer += "                }\n"
        output_buffer += "            }\n"
        output_buffer += "            keycode_index--;\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "}\n\n"
        
        output_buffer += "bool are_hashed_keycodes_in_array(" + hash_type + " keycode_hash) {\n"
        output_buffer += "    for (int i = 0; i < " + str(number_of_keys) + "; i++) {\n"
        output_buffer += "        bool index_in_hash = ((" + hash_type + ") 1 << i) & keycode_hash;\n"
        output_buffer += "        bool index_in_array = (bool) keycodes_buffer_array[i];\n"
        output_buffer += "        if (index_in_hash && !index_in_array) {\n"
        output_buffer += "            return false;\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "    return true;\n"
        output_buffer += "}\n\n"
        
        output_buffer += "void kill_one_shots(void) {\n"
        output_buffer += "    struct Chord chord_storage;\n"
        output_buffer += "    struct Chord* chord_ptr;\n"
        output_buffer += "    struct Chord* chord;\n"
        output_buffer += "    \n"
        output_buffer += "    for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "        chord = &chord_storage;\n"
        output_buffer += "        \n"
        output_buffer += "        if (*chord->state == IN_ONE_SHOT) {\n"
        output_buffer += "            *chord->state = RESTART;\n"
        output_buffer += "            chord->function(chord);\n"
        output_buffer += "            if (*chord->state == RESTART) {\n"
        output_buffer += "                *chord->state = IDLE;\n"
        output_buffer += "            }\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "}\n\n"
        
        output_buffer += "void process_finished_dances(void) {\n"
        output_buffer += "    struct Chord chord_storage;\n"
        output_buffer += "    struct Chord* chord_ptr;\n"
        output_buffer += "    struct Chord* chord;\n"
        output_buffer += "    \n"
        output_buffer += "    for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "        chord = &chord_storage;\n"
        output_buffer += "        \n"
        output_buffer += "        if (*chord->state == ACTIVATED) {\n"
        output_buffer += "            *chord->state = PRESS_FROM_ACTIVE;\n"
        output_buffer += "            chord->function(chord);\n"
        output_buffer += "            if (a_key_went_through) {\n"
        output_buffer += "                kill_one_shots();\n"
        output_buffer += "            }\n"
        output_buffer += "            dance_timer = timer_read();\n"
        output_buffer += "        } else if (*chord->state == IDLE_IN_DANCE) {\n"
        output_buffer += "            *chord->state = FINISHED;\n"
        output_buffer += "            chord->function(chord);\n"
        output_buffer += "            if (*chord->state == FINISHED) {\n"
        output_buffer += "                *chord->state = RESTART;\n"
        output_buffer += "                if (*chord->state == RESTART) {\n"
        output_buffer += "                    *chord->state = IDLE;\n"
        output_buffer += "                }\n"
        output_buffer += "            }\n"
        output_buffer += "        } else if (*chord->state == PRESS_FROM_ACTIVE) {\n"
        output_buffer += "            *chord->state = FINISHED_FROM_ACTIVE;\n"
        output_buffer += "            chord->function(chord);\n"
        output_buffer += "            if (a_key_went_through) {\n"
        output_buffer += "                kill_one_shots();\n"
        output_buffer += "            }\n"
        output_buffer += "            dance_timer = timer_read();\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "}\n\n"
        
        output_buffer += "uint8_t keycodes_buffer_array_min(uint8_t* first_keycode_index) {\n"
        output_buffer += "    for (int i = 0; i < " +str(number_of_keys) + "; i++) {\n"
        output_buffer += "        if (keycodes_buffer_array[i] == 1) {\n"
        output_buffer += "            if (first_keycode_index != NULL) {\n"
        output_buffer += "                *first_keycode_index = (uint8_t) i;\n"
        output_buffer += "            }\n"
        output_buffer += "            return 1;\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "    return 0;\n"
        output_buffer += "}\n\n"
        
        output_buffer += "void remove_subchords(void) {\n"
        output_buffer += "    struct Chord chord_storage;\n"
        output_buffer += "    struct Chord* chord_ptr;\n"
        output_buffer += "    struct Chord* chord;\n"
        output_buffer += "    \n"
        output_buffer += "    for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "        chord = &chord_storage;\n"
        output_buffer += "        \n"
        output_buffer += "        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {\n"
        output_buffer += "            continue;\n"
        output_buffer += "        }\n"
        output_buffer += "        \n"
        output_buffer += "        struct Chord chord_storage_2;\n"
        output_buffer += "        struct Chord* chord_ptr_2;\n"
        output_buffer += "        struct Chord* chord_2;\n"
        output_buffer += "        for (int j = 0; j < " + str(number_of_chords) + "; j++) {\n"
        output_buffer += "            if (i == j) {continue;}\n"
        output_buffer += "            \n"
        output_buffer += "            chord_ptr_2 = (struct Chord*) pgm_read_word (&list_of_chords[j]);\n"
        output_buffer += "            memcpy_P(&chord_storage_2, chord_ptr_2, sizeof(struct Chord));\n"
        output_buffer += "            chord_2 = &chord_storage_2;\n"
        output_buffer += "            \n"
        output_buffer += "            if (are_hashed_keycodes_in_sound(chord_2->keycodes_hash, chord->keycodes_hash)) {\n"
        output_buffer += "                if (*chord_2->state == READY) {\n"
        output_buffer += "                    *chord_2->state = IDLE;\n"
        output_buffer += "                }\n"
        output_buffer += "                if (*chord_2->state == READY_IN_DANCE) {\n"
        output_buffer += "                    *chord_2->state = IDLE_IN_DANCE;\n"
        output_buffer += "                }\n"
        output_buffer += "                if (*chord_2->state == READY_LOCKED) {\n"
        output_buffer += "                    *chord_2->state = LOCKED;\n"
        output_buffer += "                }\n"
        output_buffer += "            }\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "}\n\n"
        
        output_buffer += "void process_ready_chords(void) {\n"
        output_buffer += "    uint8_t first_keycode_index = 0;\n"
        output_buffer += "    while (keycodes_buffer_array_min(&first_keycode_index)) {\n"
        output_buffer += "        // find ready chords\n"
        output_buffer += "        struct Chord chord_storage;\n"
        output_buffer += "        struct Chord* chord_ptr;\n"
        output_buffer += "        struct Chord* chord;\n"
        output_buffer += "        \n"
        output_buffer += "        for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "            chord = &chord_storage;\n"
        output_buffer += "            \n"
        output_buffer += "            // if the chord does not contain the first keycode\n"
        output_buffer += "            bool contains_first_keycode = ((uint32_t) 1 << first_keycode_index) & chord->keycodes_hash;\n"
        output_buffer += "            if (!contains_first_keycode) {\n"
        output_buffer += "                continue;\n"
        output_buffer += "            }\n"
        output_buffer += "            \n"
        output_buffer += "            if (!are_hashed_keycodes_in_array(chord->keycodes_hash)){\n"
        output_buffer += "                continue;\n"
        output_buffer += "            }\n"
        output_buffer += "            \n"
        output_buffer += "            if (*chord->state == LOCKED) {\n"
        output_buffer += "                *chord->state = READY_LOCKED;\n"
        output_buffer += "                continue;\n"
        output_buffer += "            }\n"
        output_buffer += "            \n"
        output_buffer += "            if (!(chord->pseudolayer == current_pseudolayer || chord->pseudolayer == ALWAYS_ON)) {\n"
        output_buffer += "                continue;\n"
        output_buffer += "            }\n"
        output_buffer += "            \n"
        output_buffer += "            if (*chord->state == IDLE) {\n"
        output_buffer += "                *chord->state = READY;\n"
        output_buffer += "                continue;\n"
        output_buffer += "            }\n"
        output_buffer += "            \n"
        output_buffer += "            if (*chord->state == IDLE_IN_DANCE) {\n"
        output_buffer += "                *chord->state = READY_IN_DANCE;\n"
        output_buffer += "            }\n"
        output_buffer += "        }\n"
        output_buffer += "        \n"
        output_buffer += "        // remove subchords\n"
        output_buffer += "        remove_subchords();\n"
        output_buffer += "        \n"
        output_buffer += "        // execute logic\n"
        output_buffer += "        // this should be only one chord\n"
        output_buffer += "        for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "            chord = &chord_storage;\n"
        output_buffer += "            \n"
        output_buffer += "            if (*chord->state == READY_LOCKED) {\n"
        output_buffer += "                *chord->state = RESTART;\n"
        output_buffer += "                chord->function(chord);\n"
        output_buffer += "                if (*chord->state == RESTART) {\n"
        output_buffer += "                    *chord->state = IDLE;\n"
        output_buffer += "                }\n"
        output_buffer += "                break;\n"
        output_buffer += "            }\n"
        output_buffer += "            \n"
        output_buffer += "            if (*chord->state == READY || *chord->state == READY_IN_DANCE) {\n"
        output_buffer += "                if (last_chord && last_chord != chord) {\n"
        output_buffer += "                    process_finished_dances();\n"
        output_buffer += "                }\n"
        output_buffer += "                \n"
        output_buffer += "                bool lock_next_prev_state = lock_next;\n"
        output_buffer += "                \n"
        output_buffer += "                *chord->state = ACTIVATED;\n"
        output_buffer += "                chord->function(chord);\n"
        output_buffer += "                dance_timer = timer_read();\n"
        output_buffer += "                \n"
        output_buffer += "                if (lock_next && lock_next == lock_next_prev_state) {\n"
        output_buffer += "                    lock_next = false;\n"
        output_buffer += "                    *chord->state = PRESS_FROM_ACTIVE;\n"
        output_buffer += "                    chord->function(chord);\n"
        output_buffer += "                    if (*chord->state == PRESS_FROM_ACTIVE) {\n"
        output_buffer += "                        *chord->state = LOCKED;\n"
        output_buffer += "                    }\n"
        output_buffer += "                    if (a_key_went_through) {\n"
        output_buffer += "                        kill_one_shots();\n"
        output_buffer += "                    }\n"
        output_buffer += "                }\n"
        output_buffer += "                break;\n"
        output_buffer += "            }\n"
        output_buffer += "        }\n"
        output_buffer += "        \n"
        output_buffer += "        // silence notes\n"
        output_buffer += "        silence_keycode_hash_array(chord->keycodes_hash);\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void deactivate_active_chords(uint16_t keycode) {\n"
        output_buffer += "    " + hash_type + " hash = (" + hash_type + ")1 << (keycode - SAFE_RANGE);\n"
        output_buffer += "    bool broken;\n"
        output_buffer += "    struct Chord chord_storage;\n"
        output_buffer += "    struct Chord* chord_ptr;\n"
        output_buffer += "    struct Chord* chord;\n"
        output_buffer += "    \n"
        output_buffer += "    for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "        chord = &chord_storage;\n"
        output_buffer += "        \n"
        output_buffer += "        broken = are_hashed_keycodes_in_sound(hash, chord->keycodes_hash);\n"
        output_buffer += "        if (!broken) {\n"
        output_buffer += "            continue;\n"
        output_buffer += "        }\n"
        output_buffer += "        \n"
        output_buffer += "        switch (*chord->state) {\n"
        output_buffer += "            case ACTIVATED:\n"
        output_buffer += "                *chord->state = DEACTIVATED;\n"
        output_buffer += "                chord->function(chord);\n"
        output_buffer += "                \n"
        output_buffer += "                if (*chord->state == DEACTIVATED) {\n"
        output_buffer += "                    dance_timer = timer_read();\n"
        output_buffer += "                    *chord->state = IDLE_IN_DANCE;\n"
        output_buffer += "                }\n"
        output_buffer += "                if (*chord->state != IN_ONE_SHOT) {\n"
        output_buffer += "                    kill_one_shots();\n"
        output_buffer += "                }\n"
        output_buffer += "                break;\n"
        output_buffer += "            case PRESS_FROM_ACTIVE:\n"
        output_buffer += "            case FINISHED_FROM_ACTIVE:\n"
        output_buffer += "                *chord->state = RESTART;\n"
        output_buffer += "                chord->function(chord);\n"
        output_buffer += "                if (*chord->state == RESTART) {\n"
        output_buffer += "                    *chord->state = IDLE;\n"
        output_buffer += "                }\n"
        output_buffer += "                kill_one_shots();\n"
        output_buffer += "                break;\n"
        output_buffer += "            default:\n"
        output_buffer += "                break;\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "    \n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void process_command(void) {\n"
        output_buffer += "    command_mode = 0;\n"
        output_buffer += "    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {\n"
        output_buffer += "        if (command_buffer[i]) {\n"
        output_buffer += "            register_code(command_buffer[i]);\n"
        output_buffer += "        }\n"
        output_buffer += "        send_keyboard_report();\n"
        output_buffer += "    }\n"
        output_buffer += "    wait_ms(TAP_TIMEOUT);\n"
        output_buffer += "    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {\n"
        output_buffer += "        if (command_buffer[i]) {\n"
        output_buffer += "            unregister_code(command_buffer[i]);\n"
        output_buffer += "        }\n"
        output_buffer += "        send_keyboard_report();\n"
        output_buffer += "    }\n"
        output_buffer += "    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {\n"
        output_buffer += "        command_buffer[i] = 0;\n"
        output_buffer += "    }\n"
        output_buffer += "    command_ind = 0;\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void process_leader(void) {\n"
        output_buffer += "    in_leader_mode = false;\n"
        output_buffer += "    for (int i = 0; i < " + str(len(list_of_leader_combos)) + "; i++) {\n"
        output_buffer += "        uint16_t trigger[5];\n"
        output_buffer += "        memcpy_P(trigger, leader_triggers[i], LEADER_MAX_LENGTH * sizeof(uint16_t));\n"
        output_buffer += "        \n"
        output_buffer += "        if (identical(leader_buffer, trigger)) {\n"
        output_buffer += "            (*leader_functions[i])();\n"
        output_buffer += "            break;\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "    for (int i = 0; i < LEADER_MAX_LENGTH; i++) {\n"
        output_buffer += "        leader_buffer[i] = 0;\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "bool process_record_user(uint16_t keycode, keyrecord_t *record) {\n"
        output_buffer += "    if (keycode < FIRST_INTERNAL_KEYCODE || keycode > LAST_INTERNAL_KEYCODE) {\n"
        output_buffer += "        return true;\n"
        output_buffer += "    }\n"
        output_buffer += "    \n"
        output_buffer += "    if (record->event.pressed) {\n"
        output_buffer += "        sound_keycode_array(keycode);\n"
        output_buffer += "    } else {\n"
        output_buffer += "        process_ready_chords();\n"
        output_buffer += "        deactivate_active_chords(keycode);\n"
        output_buffer += "    }\n"
        output_buffer += "    chord_timer = timer_read();\n"
        output_buffer += "    leader_timer = timer_read();\n"
        output_buffer += "    \n"
        output_buffer += "    return false;\n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void matrix_scan_user(void) {\n"
        output_buffer += "    bool chord_timer_expired = timer_elapsed(chord_timer) > CHORD_TIMEOUT;\n"
        output_buffer += "    if (chord_timer_expired && keycodes_buffer_array_min(NULL)) {\n"
        output_buffer += "        process_ready_chords();\n"
        output_buffer += "    }\n"
        output_buffer += "    \n"
        output_buffer += "    bool dance_timer_expired = timer_elapsed(dance_timer) > DANCE_TIMEOUT;\n"
        output_buffer += "    if (dance_timer_expired) { // would love to have && in_dance but not sure how\n"
        output_buffer += "        process_finished_dances();\n"
        output_buffer += "    }\n"
        output_buffer += "    \n"
        output_buffer += "    bool in_command_mode = command_mode == 2;\n"
        output_buffer += "    if (in_command_mode) {\n"
        output_buffer += "        process_command();\n"
        output_buffer += "    }\n"
        output_buffer += "    \n"
        output_buffer += "    bool leader_timer_expired = timer_elapsed(leader_timer) > LEADER_TIMEOUT;\n"
        output_buffer += "    if (leader_timer_expired && in_leader_mode) {\n"
        output_buffer += "        process_leader();\n"
        output_buffer += "    }\n"
        output_buffer += "    \n"
        output_buffer += "}\n"
        output_buffer += "\n"
        output_buffer += "void clear(const struct Chord* self) {\n"
        output_buffer += "    if (*self->state == ACTIVATED) {\n"
        output_buffer += "        // kill all chords\n"
        output_buffer += "        struct Chord chord_storage;\n"
        output_buffer += "        struct Chord* chord_ptr;\n"
        output_buffer += "        struct Chord* chord;\n"
        output_buffer += "        \n"
        output_buffer += "        for (int i = 0; i < " + str(number_of_chords) + "; i++) {\n"
        output_buffer += "            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);\n"
        output_buffer += "            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));\n"
        output_buffer += "            chord = &chord_storage;\n"
        output_buffer += "            \n"
        output_buffer += "            *chord->state = IDLE;\n"
        output_buffer += "            \n"
        output_buffer += "            if (chord->counter) {\n"
        output_buffer += "                *chord->counter = 0;\n"
        output_buffer += "            }\n"
        output_buffer += "        }\n"
        output_buffer += "        \n"
        output_buffer += "        // clear keyboard\n"
        output_buffer += "        clear_keyboard();\n"
        output_buffer += "        send_keyboard_report();\n"
        output_buffer += "        \n"
        output_buffer += "        // switch to default pseudolayer\n"
        output_buffer += "        current_pseudolayer = 1;\n"
        output_buffer += "        \n"
        output_buffer += "        // clear all keyboard states\n"
        output_buffer += "        lock_next = false;\n"
        output_buffer += "        autoshift_mode = true;\n"
        output_buffer += "        command_mode = 0;\n"
        output_buffer += "        in_leader_mode = false;\n"
        output_buffer += "        leader_ind = 0;\n"
        output_buffer += "        dynamic_macro_mode = false;\n"
        output_buffer += "        a_key_went_through = false;\n"
        output_buffer += "        \n"
        output_buffer += "        for (int i = 0; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {\n"
        output_buffer += "            dynamic_macro_buffer[i] = 0;\n"
        output_buffer += "        }\n"
        output_buffer += "    }\n"
        output_buffer += "}\n"



        write_file.write(output_buffer)
