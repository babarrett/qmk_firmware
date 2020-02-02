import json
from functools import reduce 
from chord import *
import sys

if len(sys.argv) != 3:
    raise Exception("Wrong number of arguments.\n\nUsage: python parser.py keymap.json keymap.c")

input_filepath = sys.argv[1]
output_filepath = sys.argv[2]

comma_separator = (lambda x, y: str(x) + ", " + str(y))
string_sum = (lambda x, y: str(x) + " + " + str(y))
newline_separator = (lambda x, y: str(x) + "\n" + str(y))

number_of_keys = 0
hash_type = ""
output_buffer = ""
number_of_chords = 0

with open(input_filepath, "r") as read_file:
    data = json.load(read_file)
    with open(output_filepath, "w") as write_file:
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
            raise Exception("The engine currently supports only up to 64 keys.")
        
        if not ("do_not_include_QMK" in data["parameters"] and data["parameters"]["do_not_include_QMK"] == True):
            output_buffer += "#include QMK_KEYBOARD_H\n"
        if len(data["extra_dependencies"]) > 0:
            for dependecy in data["extra_dependencies"]:
                output_buffer += '#include "' + dependecy + '"\n'
        output_buffer += "\n"
        
        output_buffer += "#define CHORD_TIMEOUT " + str(data["parameters"]["chord_timeout"]) + "\n"
        output_buffer += "#define DANCE_TIMEOUT " + str(data["parameters"]["dance_timeout"]) + "\n"
        output_buffer += "#define LEADER_TIMEOUT " + str(data["parameters"]["leader_timeout"]) + "\n"
        output_buffer += "#define TAP_TIMEOUT " + str(data["parameters"]["tap_timeout"]) + "\n"
        output_buffer += "#define LONG_PRESS_MULTIPLIER " + str(data["parameters"]["long_press_multiplier"]) + "\n"
        output_buffer += "#define DYNAMIC_MACRO_MAX_LENGTH " + str(data["parameters"]["dynamic_macro_max_length"]) + "\n"
        output_buffer += "#define COMMAND_MAX_LENGTH " + str(data["parameters"]["command_max_length"]) + "\n"
        output_buffer += "#define LEADER_MAX_LENGTH " + str(data["parameters"]["leader_max_length"]) + "\n"
        output_buffer += "#define HASH_TYPE " + hash_type + "\n"
        output_buffer += "#define NUMBER_OF_KEYS " + str(len(data["keys"])) + "\n"
        output_buffer += "#define DEFAULT_PSEUDOLAYER " + data["parameters"]["default_pseudolayer"] + "\n"
        output_buffer += "\n"
        
        for key, counter in zip(data["keys"], range(0, len(data["keys"]))):
            output_buffer += "#define H_" + key + " ((HASH_TYPE) 1 << " + str(counter) + ")\n"
        output_buffer += "\n"
        
        output_buffer += "enum internal_keycodes {\n"
        output_buffer += "    " + data["keys"][0] + " = SAFE_RANGE,\n"
        output_buffer += "    " + reduce(comma_separator, [key for key in data["keys"][1:]]) + ",\n"
        output_buffer += "    FIRST_INTERNAL_KEYCODE = " + data["keys"][0] + ",\n"
        output_buffer += "    LAST_INTERNAL_KEYCODE = " + data["keys"][-1] + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "enum pseudolayers {\n"
        output_buffer += "    " + reduce(comma_separator, [layer["name"] for layer in data["pseudolayers"]]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {\n"
        for layer, counter in zip(data["layers"], range(0,len(data["layers"]))):
            if layer["type"] == "auto":
                output_buffer += "    [" + str(counter) + "] = " + data["parameters"]["layout_function_name"] + "(" + reduce(comma_separator, [key for key in data["keys"]]) + "),\n"
            else:
                output_buffer += "    [" + str(counter) + "] = " + data["parameters"]["layout_function_name"] + "(" + reduce(comma_separator, [key for key in layer["keycodes"]]) + "),\n"
        output_buffer += "};\n"
        output_buffer += "size_t keymapsCount = " + str(len(data["layers"])) + ";\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t keycodes_buffer_array[] = {\n"
        output_buffer += "    " + reduce(comma_separator, ["0"] * len(data["keys"])) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t command_buffer[] = {\n"
        output_buffer += "    " + reduce(comma_separator, ["0"] * data["parameters"]["command_max_length"]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "uint16_t leader_buffer[] = {\n"
        output_buffer += "    " + reduce(comma_separator, ["0"] * data["parameters"]["leader_max_length"]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t dynamic_macro_buffer[] = {\n"
        output_buffer += "    " + reduce(comma_separator, ["0"] * data["parameters"]["dynamic_macro_max_length"]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        with open("engine.part.1", "r") as file:
            output_buffer += file.read()
        output_buffer += "\n"
        
        if len(data["extra_code"]) > 0:
            output_buffer += data["extra_code"] + "\n"
        
        with open("engine.part.2", "r") as file:
            output_buffer += file.read()
        output_buffer += "\n"
        
        for pseudolayer in data["pseudolayers"]:
            name = pseudolayer["name"]
            for chord_set in pseudolayer["chord_sets"]:
                keycodes = reduce(comma_separator, [word for word in chord_set["keycodes"]])
                [output_buffer, number_of_chords] = add_chord_set(name, keycodes, chord_set["chord_set"], data, output_buffer, number_of_chords)
            
            for single_chord in pseudolayer["single_chords"]:
                if single_chord["type"] == "visual":
                    keycodes = reduce(comma_separator, [word for word in single_chord["chord"]])
                    [output_buffer, number_of_chords] = secret_chord(name, single_chord["keycode"], keycodes, data, output_buffer, number_of_chords)
                elif single_chord["type"] == "simple":
                    keycodes = reduce(string_sum, ["H_" + word for word in single_chord["chord"]])
                    [output_buffer, number_of_chords] = add_key(name, keycodes, single_chord["keycode"], output_buffer, number_of_chords)
        output_buffer += "\n"
        
        output_buffer += "const struct Chord* const list_of_chords[] PROGMEM = {\n"
        output_buffer += "    " + reduce(comma_separator, ["&chord_" + str(i) for i in range(0, number_of_chords)]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        if len(data["leader_sequences"]) > 0:
            output_buffer += reduce(newline_separator, [sequence["function"] for sequence in data["leader_sequences"]]) + "\n\n"
            output_buffer += "const uint16_t leader_triggers[][LEADER_MAX_LENGTH] PROGMEM = {\n"
            for sequence in data["leader_sequences"]:
                output_buffer += "    {" + reduce(comma_separator, sequence["sequence"] + ["0"] * (data["parameters"]["leader_max_length"] - len(sequence["sequence"]))) + "},\n"
            output_buffer += "};\n\n"
            output_buffer += "void (*leader_functions[]) (void) = {\n"
            output_buffer += "    " + reduce(comma_separator, ["&" + sequence["name"] for sequence in data["leader_sequences"]]) + "\n"
            output_buffer += "};\n"
        else:
            output_buffer += "const uint16_t** const leader_triggers PROGMEM = NULL;\n"
            output_buffer += "void (*leader_functions[]) (void) = {};\n"
        output_buffer += "\n"
        
        output_buffer += "#define NUMBER_OF_CHORDS " + str(number_of_chords) + "\n"
        output_buffer += "#define NUMBER_OF_LEADER_COMBOS " + str(len(data["leader_sequences"])) + "\n"
        output_buffer += "\n"
        
        with open("engine.part.3", "r") as file:
            output_buffer += file.read()
        
        write_file.write(output_buffer)
