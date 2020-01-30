import json
from functools import reduce
from chord import *
import sys

def add_comma(x1, x2):
    return str(x1) + ", " + str(x2)

if len(sys.argv) != 3:
    raise Exception("Call as 'python3 parser.py keymap.json keymap.c'")

input_path = sys.argv[1]
output_path = sys.argv[2]

number_of_keys = 0
hash_type = ""
output_buffer = ""
number_of_chords = 0
list_of_leader_combos = []

with open(input_path, "r") as read_file:
    data = json.load(read_file)
    with open(output_path, "w") as write_file:
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
            raise Exception("This implementation supports only up to 64 keys.")
        
        output_buffer += "#include QMK_KEYBOARD_H\n\n"
        
        output_buffer += "#define CHORD_TIMEOUT " + str(data["keyboard_parameters"]["chord_timeout"]) + "\n"
        output_buffer += "#define DANCE_TIMEOUT " + str(data["keyboard_parameters"]["dance_timeout"]) + "\n"
        output_buffer += "#define LEADER_TIMEOUT " + str(data["keyboard_parameters"]["leader_timeout"]) + "\n"
        output_buffer += "#define TAP_TIMEOUT " + str(data["keyboard_parameters"]["tap_timeout"]) + "\n"
        output_buffer += "#define DYNAMIC_MACRO_MAX_LENGTH " + str(data["keyboard_parameters"]["dynamic_macro_max_length"]) + "\n"
        output_buffer += "#define COMMAND_MAX_LENGTH " + str(data["keyboard_parameters"]["command_max_length"]) + "\n"
        output_buffer += "#define LEADER_MAX_LENGTH " + str(data["keyboard_parameters"]["leader_max_length"]) + "\n"
        output_buffer += "#define LONG_PRESS_MULTIPLIER " + str(data["keyboard_parameters"]["long_press_multiplier"]) + "\n"
        output_buffer += "\n"
        
        output_buffer += "enum pseudolayers {\n"
        output_buffer += "    " + reduce(add_comma, [layer["name"] for layer in data["pseudolayers"]]) + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "enum internal_keycodes {\n"
        output_buffer += "    " + data["keys"][0] + " = SAFE_RANGE,\n"
        output_buffer += "    " + reduce(add_comma, [key for key in data["keys"][1:]]) + ",\n"
        output_buffer += "    FIRST_INTERNAL_KEYCODE = " + data["keys"][0] + ",\n"
        output_buffer += "    LAST_INTERNAL_KEYCODE = " + data["keys"][-1] + "\n"
        output_buffer += "};\n"
        output_buffer += "\n"
        
        output_buffer += "#define NUMBER_OF_KEYS " + str(len(data["keys"])) + "\n\n"
        output_buffer += "const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {\n"
        output_buffer += "    [0] = "
        if len(data["keyboard_parameters"]["layout_function_name"]) > 0:
          output_buffer += data["keyboard_parameters"]["layout_function_name"] + "(" + reduce(add_comma, [key for key in data["keys"]]) + ")\n"
        else:
          output_buffer += reduce(add_comma, [key for key in data["keys"]]) + "\n"
        output_buffer += "};\n"
        output_buffer += "size_t keymapsCount = 1;\n"
        output_buffer += "\n"
        
        for key, counter in zip(data["keys"], range(0, len(data["keys"]))):
            output_buffer += "#define H_" + key + " ((" + hash_type + ") 1 << " + str(counter) + ")\n"
        output_buffer += "\n"
        
        output_buffer += "uint8_t keycodes_buffer_array[] = {"
        reduce(add_comma, ["0"] * len(data["keys"]))
        output_buffer += reduce(add_comma, ["0"] * len(data["keys"]))
        output_buffer += "};\n"
        
        output_buffer += "uint8_t command_buffer[] = {"
        output_buffer += reduce(add_comma, ["0"] * data["keyboard_parameters"]["command_max_length"])
        output_buffer += "};\n"
        
        output_buffer += "uint16_t leader_buffer[] = {"
        output_buffer += reduce(add_comma, ["0"] * data["keyboard_parameters"]["leader_max_length"])
        output_buffer += "};\n"
        
        output_buffer += "uint8_t dynamic_macro_buffer[] = {"
        output_buffer += reduce(add_comma, ["0"] * data["keyboard_parameters"]["dynamic_macro_max_length"])
        output_buffer += "};\n"
        
        output_buffer += "\n"
        
        with open("engine_part1.in", "r") as file:
            output_buffer += file.read()
            output_buffer += "\n\n"
        
        output_buffer += data["extra_code"] + "\n"
        
        if len(data["extra_dependencies"]) > 0:
            for dependecy in data["extra_dependencies"]:
                with open(dependecy, "r") as file:
                    output_buffer += file.read()
            output_buffer += "\n"
        
        if len(data["leader_sequences"]) > 0:
            for sequence in data["leader_sequences"]:
                definition = sequence["sequence"]
                definition += ["0"] * (data["keyboard_parameters"]["leader_max_length"] - len(definition))
                definition = "{" + reduce(add_comma, definition) + "}"
                list_of_leader_combos.append([definition, sequence["name"]])
                if len(sequence["function"]) > 0:
                    output_buffer += sequence["function"] + "\n"
            output_buffer += "\n"
        
        output_buffer += "#define NUMBER_OF_LEADER_COMBOS " + str(len(list_of_leader_combos)) + "\n\n"
        
        if len(list_of_leader_combos) > 0:
            output_buffer += "const uint16_t leader_triggers[NUMBER_OF_LEADER_COMBOS][LEADER_MAX_LENGTH] PROGMEM = {\n"
            output_buffer += reduce((lambda x, y: str(x) + ",\n" + str(y)), ["    " + combo[0] for combo in list_of_leader_combos])
            output_buffer += "\n};\n"
            output_buffer += "void (*leader_functions[]) (void) = {\n"
            output_buffer += reduce((lambda x, y: str(x) + ",\n" + str(y)), ["    " + combo[1] for combo in list_of_leader_combos])
            output_buffer += "\n};\n"
        else:
            output_buffer += "const uint16_t** const leader_triggers PROGMEM = NULL;\n"
            output_buffer += "void (*leader_functions[]) (void) = {};\n"
        output_buffer += "\n"
        
        for pseudolayer in data["pseudolayers"]:
            name = pseudolayer["name"]
            for chord_set in pseudolayer["chord_sets"]:
                ks = reduce(add_comma, [word for word in chord_set["keycodes"]])
                [output_buffer, number_of_chords] = add_chord_set(name, ks, chord_set["chord_set"], data, output_buffer, number_of_chords)
            
            for single_chord in pseudolayer["single_chords"]:
                if single_chord["type"] == "visual":
                    ks = reduce(add_comma, [word for word in single_chord["chord"]])
                    [output_buffer, number_of_chords] = secret_chord(name, single_chord["keycode"], ks, data, output_buffer, number_of_chords)
                elif single_chord["type"] == "simple":
                    ks = reduce((lambda x, y: x + " + " + y), ["H_" + word for word in single_chord["chord"]])
                    [output_buffer, number_of_chords] = add_key(name, ks, single_chord["keycode"], output_buffer, number_of_chords)
        output_buffer += "\n"
        
        if number_of_chords == 0:
            raise Exception("You didn't define a single chord")
        
        output_buffer += "const struct Chord* const list_of_chords[] PROGMEM = {\n"
        output_buffer += reduce((lambda x, y: str(x) + ",\n" + str(y)), ["    &chord_" + str(i) for i in range(0, number_of_chords)])
        output_buffer += "\n};\n\n"
        
        output_buffer += "#define HASH_TYPE " + hash_type + "\n"
        output_buffer += "#define NUMBER_OF_CHORDS " + str(number_of_chords) + "\n\n"
        
        with open("engine_part2.in", "r") as file:
            output_buffer += file.read()
        
        write_file.write(output_buffer)
