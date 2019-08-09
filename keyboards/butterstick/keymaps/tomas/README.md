# README

## About

This keymap is using a custom chording engine and general purpose preprocessor pyexpander.

Pure QMK combos were not sufficient as they do not support subcombos. If you define 3 combos `(KC_Q, KC_W)`, `(KC_Z, KC_X)` and `(KC_Q, KC_W, KC_Z, KC_X)` and press Q, W, Z and X at the same time, all three combos activate. The default butterstick keymap solves this by relying on modified stenografic engine. However, this doesn't allow for comfortable typing in the traditional way. The steno chord activates only when *all* keys are lifted and makes it difficult to implement some advanced features.

The reason behind general purpose preprocessor is abstraction when defining the keymap. Every function on this keymap is a chord (combo). Meaning you have to follow syntax similar to pure QMK combos. The resulting keymap file is long, difficult to navigate and even more difficult to modify. It is *nearly impossible* to write C preprocessor macros that make it as easy as pure QMK keymap. Furthermore, the general purpose preprocessor makes it easy to define advanced macros for simple addition of functions like a tap dance calling advanced keycodes not just basic ones.

## Features

The chording engine completely sidesteps QMK's key event processing. Most of QMK's features are reimplemented. A list with short description follow, examples and further details follow later in this README.

### Chords

Once again, *everything* on this keymap is a chord. Even sending `KC_Q` is done by pressing a single key chord. Chord gets activated after all it's keys get pressed. Only the longest chord gets activated (after each key gets pressed, a timer is updated and the engine wait to see if more keys will be pressed in case a longer chord should be activated). The order in which the keys get pressed *does not matter*. An active chord gets deactivated if any of it's keys gets depressed. To activate the same single chord again, *all* it's keys have to be depressed and pressed again. With a few exceptions chords are independent of each other, while some chords are currently active and some not, others can be activated or deactivated without affecting each other's state.

### Tap-Dance

All chords are implemented in the same manner. They are relatively simple state machines that execute a specific function every time they change state. For simplicity and optimization purposes, there are a few prewritten functions that implement common features like 'send a single key' or 'lock'. Any number of chords can be "in dance" at any given moment without affecting each other's state. Custom dances can be easily added.

### Pseudolayers

Only one QMK layer is used. Following the default keymap's example, the chording engine is using pseudolayers. The main difference to QMK's layers is that only one pseudolayer can be active at each time. Chords can be activated only if they are on the currently active pseudolayer. Chords that are currently active do not get deactivated if the pseudolayer changes. Locked chords (see below) and chords on the `ALWAYS_ON` pseudolayer can be activated anytime.

### Lock

Similarly to QMK's lock, the next chord activated after the Lock chord will not deactivated on release but on the next activation. Any number of chords can be locked at the same time. To make sure a locked chord can be unlocked, it can activate no matter the current pseudolayer.

### One shots

Chords that send keycodes and chords that turn on pseudolayers can be one shots. If tapped, they will lock (stay active) until the *next keycode gets sent*. If held, they will deactivate on release *even if no keycode got sent*.

### Tap-Hold

Either sends a defined keycode on tap, temporarily switches pseudolayer on hold *or* sends two different keycodes on tap and hold.

### Command mode

Works the similarly as default keymap's. After getting activated for the first time, the keyboard switches to command mode. All keycodes that would get registered get buffered instead. After activating the Command mode chord for the second time, all buffered keycodes get released at the same time allowing for key combination that would be hard or impossible to register. The Command mode only affects keycodes. It is therefore possible to change pseudolayers or activate / deactivate other chords while in Command mode. While multiple Command mode chords can be defined, they would not be independent of each other.

### Leader key

Just like pure QMK's Leader key, this allows you to add functions that execute if the Leader key gets pressed and then in a short time a specific sequence of keycodes gets registered, for example `:wq` can send `Ctrl+S` and `Ctrl+W` in a quick succession. While multiple Leader keys can be defined, they all would access the same list of sequences.

### Dynamic macro

A sequence of keycodes can be recorded and stored in the RAM of the keyboard and replayed.

## Examples and Details

I do not have experience with stenography, so the the steno keycodes are hard for me to remember. That is why the keymap is using new keycodes TOP1, TOP2, ... TOP9, TOP0, BOT1, BOT2, ... BOT9 and BOT0. To keep track which keys are pressed and have not been processed yet and to track which keys need to be pressed to activate a chord, each key has assigned a bit in a uint32_t variable. Macros H_TOP1, H_TOP2, ... provide a translation for these bits.  *If your keyboard has more than 20 keys, you need to add more keycodes and their translation. If you have more than 32 keys, you also have to upgrade the buffer and chord's keycodes_hash.*

Each chord is defined by a constant structure, a function and two `int`s keeping the track of the chord's state:

```c
struct Chord {
    uint32_t keycodes_hash;
    uint8_t pseudolayer;
    uint8_t* state;
    uint8_t* counter;
    uint16_t value1;
    uint8_t value2;
    void (*function) (const struct Chord*);
};

uint8_t state_0 = IDLE;
uint8_t counter_0 = 0;
void function_0(struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            register_code(self->value1);
            break;
        case DEACTIVATED:
            unregister_code(self->value1);
            break;
        case FINISHED:
        case FINISHED_FROM_ACTIVE:
            break;
        case RESTART:
            unregister_code(self->value1);
            break;
        default:
            break;
    }
}
const struct Chord chord_0 PROGMEM = {H_TOP1, QWERTY, &state_0, &counter_0, KC_Q, 0, function_0};
```

All chords have to be added to `list_of_chord` array that gets regularly scanned and processed. The file `macros.inc` contains pyexpander macros that simplify adding the chords. The same chord can be added using this line: `$KC("QWERTY", "H_TOP1", "KC_Q")`.

Macros `vertical_ortho_layer` and `horizontal_ortho_layer` (in process of getting cleaned up) allow to add all standard butterstick combos in a syntax similar to QMK's layer syntax:

```c
$vertical_ortho_layer("QWERTY",
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";",
    "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/")
$horizontal_ortho_layer("QWERTY",
    "KC_ESC", "MO(MOV)", "KC_TAB", "", "O(KC_RGUI)", "", "KC_INS", "KC_DEL", "KC_BSPC",
    'STR("HELLO")', "", "", "", "CMD", "", "", "", "KC_ENTER",
    "O(KC_LSFT)", "O(KC_LCTL)", "O(KC_LALT)", "MO(NUM)", "O(KC_LGUI)", "MO(NUM)", "O(KC_RALT)", "O(KC_RCTL)", "O(KC_RSFT)")
```

The first macro defines single key chords and the logical middle row. The second one defines the logical columns (`TOP1 + TOP2 = KC_ESC`, `TOP9 + TOP0 + BOT9 + BOT0 = KC_ENTER` ).

You might notice that the macros try to do a few clever things:

* If the keycode would be just a character basic keycode, it tries to allow the use of shortcuts. `Q` will get replaced with `KC_Q`. `,` will get replaced with `KC_COMMA`. This really works only for alphas, numbers and punctuation. `KC_ESC` still has to be fully spelled out.
* `MO()` and `DF()` macros work the same way for pseudolayers as they would for layers in pure QMK.
* `O()` define a one shot key but it also supports pseudolayers!
* `STR("...")` sends a string. Careful with quoting.
* Special chords like Command mode have their own codes like `CMD`.
* The empty strings `""` get ignored.

These two macros take care of most chords, I need to manually add only chords with non-standard (from butterstick's point of view) keys like `$KC("QWERTY", "H_BOT1 + H_BOT0", "KC_SPACE")`. I also have a macro for ASETNIOP style layout but that one is much more WIP.

The complete list of strings that these two macros can accept is:

* `KC_X`: Send code `X` just like a normal keyboard would (including repetition).
* `STR("X")`: Send string "x" on each activation of the chord.
* `MO(X)`: Temporary switch to pseudolayer `X`.
* `DF(X)`: Permanent switch to pseudolayer `X`.
* `O(X)`: One-shot key `X` (if `X` starts with "KC_") or one-shot layer `X` (otherwise) .
* `LOCK`: The lock key. Since tap-dances of chords are independent, it is possible to lock a chord *anywhere in it's dance if you time it right!*.
* `CMD`: The command mode. The number of keycodes that can be buffered is defined in `keyboard.inc` in `COMMAND_MAX_LENGTH` (works but needs cleanup).
* `LEAD`: The leader key. The number of leader combos and their max length need to be defined in macros (works but needs cleanup).
* `M(X, VALUE1, VALUE2)` A custom macro. Adds a chord that will use function `X` and with `chord.value1 = VALUE1; chord.value2 = VALUE2;`.
* `D(X1, X2, ...)`: A basic keycode dance. If tapped (or held), registers `X1`. If tapped and then tapped again (or held), registers `X2`, ... It *cannot* recognize between tapping and holding to register different keycodes (however holding will result in repeat). You can put in as many basic keycodes as you want, but the macro will break if you go beyond 256. Advanced keycodes are not *yet* supported.
* `DM_RECORD`, `DM_NEXT`, `DM_END`, `DM_PLAY`: Start recording a dynamic macro. Once you start recording, basic keycodes will get stored. When replaying the macro, all keys you press before `DM_NEXT` or `DM_END` will get pressed at the same time. For example the sequence `DM_RECORD`, `KC_CTRL`, `KC_A`, `DM_NEXT`, `KC_BSPC`, `DM_END` will record a macro that when played will execute the sequence Ctrl+a, Backspace.

Each chord stores as much as possible in `PROGMEM` and unless it needs it, doesn't allocate `counter`. However it still has to store it's `state` and sometimes the `counter` in RAM. If you keep adding more chords, at one point you will run out. If your firmware fits in the memory and your keyboard crashes, try removing some chords.

Also, the code is not perfect. I keep testing it, but can not guarantee that it is stable. Some functions take (very short but still) time and if you happen to create keypress event when the keyboard can not see it, a chord can get stuck in a funny state. That is especially fun if the pseudolayer changes and you can not immediately press it again. Just restart the keyboard or push the key a few times. 