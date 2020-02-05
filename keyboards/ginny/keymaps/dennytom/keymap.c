#include QMK_KEYBOARD_H

#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 200
#define LEADER_TIMEOUT 750
#define TAP_TIMEOUT 50
#define LONG_PRESS_MULTIPLIER 3
#define DYNAMIC_MACRO_MAX_LENGTH 20
#define COMMAND_MAX_LENGTH 5
#define STRING_MAX_LENGTH 16
#define LEADER_MAX_LENGTH 5
#define HASH_TYPE uint16_t
#define NUMBER_OF_KEYS 10
#define DEFAULT_PSEUDOLAYER BASE

#define H_L1 ((HASH_TYPE) 1 << 0)
#define H_L2 ((HASH_TYPE) 1 << 1)
#define H_L3 ((HASH_TYPE) 1 << 2)
#define H_L4 ((HASH_TYPE) 1 << 3)
#define H_R1 ((HASH_TYPE) 1 << 4)
#define H_R2 ((HASH_TYPE) 1 << 5)
#define H_R3 ((HASH_TYPE) 1 << 6)
#define H_R4 ((HASH_TYPE) 1 << 7)
#define H_LT ((HASH_TYPE) 1 << 8)
#define H_RT ((HASH_TYPE) 1 << 9)

enum internal_keycodes {
    L1 = SAFE_RANGE,
    L2, L3, L4, R1, R2, R3, R4, LT, RT,
    FIRST_INTERNAL_KEYCODE = L1,
    LAST_INTERNAL_KEYCODE = RT
};

enum pseudolayers {
    ALWAYS_ON, BASE, NUM, COMMANDS, USER
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ginny(L1, L2, L3, L4, R1, R2, R3, R4, LT, RT),
};
size_t keymapsCount = 1;

uint8_t keycodes_buffer_array[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t command_buffer[] = {
    0, 0, 0, 0, 0
};

uint16_t leader_buffer[] = {
    0, 0, 0, 0, 0
};

uint8_t dynamic_macro_buffer[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

enum chord_states {
    IDLE,
    READY,
    ACTIVATED,
    DEACTIVATED,
    PRESS_FROM_ACTIVE,
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

uint8_t current_pseudolayer = DEFAULT_PSEUDOLAYER;
bool lock_next = false;
uint16_t chord_timer = 0;
uint16_t dance_timer = 0;
bool autoshift_mode = true;
uint8_t keycode_index = 0;
uint8_t command_mode = 0;
uint8_t command_ind = 0;
bool in_leader_mode = false;
uint8_t leader_ind = 0;
uint16_t leader_timer = 0;
uint8_t dynamic_macro_mode = false;
uint8_t dynamic_macro_ind = 0;
bool a_key_went_through = false;
struct Chord* last_chord = NULL;

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
    if (command_mode == 1 && command_ind < COMMAND_MAX_LENGTH) {
        command_buffer[command_ind] = keycode;
        command_ind++;
        a_key_went_through = true;
    } else if (in_leader_mode && leader_ind < LEADER_MAX_LENGTH) {
        leader_buffer[leader_ind] = keycode;
        leader_ind++;
        a_key_went_through = true;
    } else if (dynamic_macro_mode && dynamic_macro_ind < DYNAMIC_MACRO_MAX_LENGTH) {
        dynamic_macro_buffer[dynamic_macro_ind] = keycode;
        dynamic_macro_ind++;
        a_key_went_through = true;
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
        if (!handle_US_ANSI_shifted_keys(keycode, false)) {
            if (command_mode == 0 && in_leader_mode == false && dynamic_macro_mode == false) {
                unregister_code(keycode);
            }
        }
        send_keyboard_report();
    }
}

void tap_key(int16_t keycode) {
    key_in(keycode);
    wait_ms(TAP_TIMEOUT);
    key_out(keycode);
}
const char string_0 [] PROGMEM = "hi";
const char string_1 [] PROGMEM = "hi";
const char string_2 [] PROGMEM = "hi";
const char string_3 [] PROGMEM = "hi";
const char string_4 [] PROGMEM = "hi";
const char string_5 [] PROGMEM = "hi";
const char string_6 [] PROGMEM = "hi";
const char string_7 [] PROGMEM = "look";
const char string_8 [] PROGMEM = "do";
const char string_9 [] PROGMEM = "any";
const char string_10 [] PROGMEM = "per";
const char string_11 [] PROGMEM = "ah";
const char string_12 [] PROGMEM = "him";
const char string_13 [] PROGMEM = "up";
const char string_14 [] PROGMEM = "eyes";
const char string_15 [] PROGMEM = "top";
const char string_16 [] PROGMEM = "its";
const char string_17 [] PROGMEM = "if";
const char string_18 [] PROGMEM = "my";
const char string_19 [] PROGMEM = "bit";
const char string_20 [] PROGMEM = "we";
const char string_21 [] PROGMEM = "all";
const char string_22 [] PROGMEM = "pass";
const char string_23 [] PROGMEM = "pool";
const char string_24 [] PROGMEM = "be";
const char string_25 [] PROGMEM = "ms";
const char string_26 [] PROGMEM = "lie";
const char string_27 [] PROGMEM = "pope";
const char string_28 [] PROGMEM = "you";
const char string_29 [] PROGMEM = "san";
const char string_30 [] PROGMEM = "cost";
const char string_31 [] PROGMEM = "his";
const char string_32 [] PROGMEM = "he";
const char string_33 [] PROGMEM = "pizza";
const char string_34 [] PROGMEM = "sip";
const char string_35 [] PROGMEM = "deep";
const char string_36 [] PROGMEM = "ever";
const char string_37 [] PROGMEM = "fact";
const char string_38 [] PROGMEM = "pat";
const char string_39 [] PROGMEM = "of";
const char string_40 [] PROGMEM = "ask";
const char string_41 [] PROGMEM = "man";
const char string_42 [] PROGMEM = "oh";
const char string_43 [] PROGMEM = "or";
const char string_44 [] PROGMEM = "lot";
const char string_45 [] PROGMEM = "tip";
const char string_46 [] PROGMEM = "us";
const char string_47 [] PROGMEM = "set";
const char string_48 [] PROGMEM = "are";
const char string_49 [] PROGMEM = "fan";
const char string_50 [] PROGMEM = "but";
const char string_51 [] PROGMEM = "'i";
const char string_52 [] PROGMEM = "did";
const char string_53 [] PROGMEM = "bc";
const char string_54 [] PROGMEM = "wow";
const char string_55 [] PROGMEM = "ix";
const char string_56 [] PROGMEM = "done";
const char string_57 [] PROGMEM = "yeah";
const char string_58 [] PROGMEM = "that";
const char string_59 [] PROGMEM = "pick";
const char string_60 [] PROGMEM = "miss";
const char string_61 [] PROGMEM = "mr";
const char string_62 [] PROGMEM = "said";
const char string_63 [] PROGMEM = "gap";
const char string_64 [] PROGMEM = "mao";
const char string_65 [] PROGMEM = "now";
const char string_66 [] PROGMEM = "i've";
const char string_67 [] PROGMEM = "jump";
const char string_68 [] PROGMEM = "put";
const char string_69 [] PROGMEM = "five";
const char string_70 [] PROGMEM = "about";
const char string_71 [] PROGMEM = "tried";
const char string_72 [] PROGMEM = "john";
const char string_73 [] PROGMEM = "soap";
const char string_74 [] PROGMEM = "fall";
const char string_75 [] PROGMEM = "she";
const char string_76 [] PROGMEM = "step";
const char string_77 [] PROGMEM = "past";
const char string_78 [] PROGMEM = "best";
const char string_79 [] PROGMEM = "your";
const char string_80 [] PROGMEM = "can";
const char string_81 [] PROGMEM = "wait";
const char string_82 [] PROGMEM = "over";
const char string_83 [] PROGMEM = "into";
const char string_84 [] PROGMEM = "many";
const char string_85 [] PROGMEM = "people";
const char string_86 [] PROGMEM = "will";
const char string_87 [] PROGMEM = "and";
const char string_88 [] PROGMEM = "has";
const char string_89 [] PROGMEM = "were";
const char string_90 [] PROGMEM = "the";
const char string_91 [] PROGMEM = "seemed";
const char string_92 [] PROGMEM = "part";
const char string_93 [] PROGMEM = "alex";
const char string_94 [] PROGMEM = "pain";
const char string_95 [] PROGMEM = "lips";
const char string_96 [] PROGMEM = "possessed";
const char string_97 [] PROGMEM = "hall";
const char string_98 [] PROGMEM = "like";
const char string_99 [] PROGMEM = "keep";
const char string_100 [] PROGMEM = "money";
const char string_101 [] PROGMEM = "stop";
const char string_102 [] PROGMEM = "two";
const char string_103 [] PROGMEM = "wood";
const char string_104 [] PROGMEM = "still";
const char string_105 [] PROGMEM = "lap";
const char string_106 [] PROGMEM = "mass";
const char string_107 [] PROGMEM = "thix";
const char string_108 [] PROGMEM = "next";
const char string_109 [] PROGMEM = "pilot";
const char string_110 [] PROGMEM = "passed";
const char string_111 [] PROGMEM = "for";
const char string_112 [] PROGMEM = "poor";
const char string_113 [] PROGMEM = "good";
const char string_114 [] PROGMEM = "i'm";
const char string_115 [] PROGMEM = "just";
const char string_116 [] PROGMEM = "tim";
const char string_117 [] PROGMEM = "anyone";
const char string_118 [] PROGMEM = "only";
const char string_119 [] PROGMEM = "mba";
const char string_120 [] PROGMEM = "human";
const char string_121 [] PROGMEM = "exposed";
const char string_122 [] PROGMEM = "before";
const char string_123 [] PROGMEM = "more";
const char string_124 [] PROGMEM = "pack";
const char string_125 [] PROGMEM = "camp";
const char string_126 [] PROGMEM = "such";
const char string_127 [] PROGMEM = "has";
const char string_128 [] PROGMEM = "with";
const char string_129 [] PROGMEM = "prove";
const char string_130 [] PROGMEM = "apple";
const char string_131 [] PROGMEM = "have";
const char string_132 [] PROGMEM = "told";
const char string_133 [] PROGMEM = "once";
const char string_134 [] PROGMEM = "made";
const char string_135 [] PROGMEM = "pillow";
const char string_136 [] PROGMEM = "them";
const char string_137 [] PROGMEM = "alone";
const char string_138 [] PROGMEM = "path";
const char string_139 [] PROGMEM = "some";
const char string_140 [] PROGMEM = "gasp";
const char string_141 [] PROGMEM = "it's";
const char string_142 [] PROGMEM = "we're";
const char string_143 [] PROGMEM = "smith";
const char string_144 [] PROGMEM = "these";
const char string_145 [] PROGMEM = "matter";
const char string_146 [] PROGMEM = "might";
const char string_147 [] PROGMEM = "home";
const char string_148 [] PROGMEM = "page";
const char string_149 [] PROGMEM = "spanish";
const char string_150 [] PROGMEM = "first";
const char string_151 [] PROGMEM = "life";
const char string_152 [] PROGMEM = "speak";
const char string_153 [] PROGMEM = "again";
const char string_154 [] PROGMEM = "politics";
const char string_155 [] PROGMEM = "who";
const char string_156 [] PROGMEM = "plato";
const char string_157 [] PROGMEM = "other";
const char string_158 [] PROGMEM = "most";
const char string_159 [] PROGMEM = "happy";
const char string_160 [] PROGMEM = "spoke";
const char string_161 [] PROGMEM = "mrs";
const char string_162 [] PROGMEM = "among";
const char string_163 [] PROGMEM = "process";
const char string_164 [] PROGMEM = "mind";
const char string_165 [] PROGMEM = "shop";
const char string_166 [] PROGMEM = "town";
const char string_167 [] PROGMEM = "woman";
const char string_168 [] PROGMEM = "last";
const char string_169 [] PROGMEM = "years";
const char string_170 [] PROGMEM = "well";
const char string_171 [] PROGMEM = "private";
const char string_172 [] PROGMEM = "words";
const char string_173 [] PROGMEM = "should";
const char string_174 [] PROGMEM = "down";
const char string_175 [] PROGMEM = "another";
const char string_176 [] PROGMEM = "women";
const char string_177 [] PROGMEM = "came";
const char string_178 [] PROGMEM = "remain";
const char string_179 [] PROGMEM = "simply";
const char string_180 [] PROGMEM = "from";
const char string_181 [] PROGMEM = "could";
const char string_182 [] PROGMEM = "don't";
const char string_183 [] PROGMEM = "much";
const char string_184 [] PROGMEM = "april";
const char string_185 [] PROGMEM = "please";
const char string_186 [] PROGMEM = "power";
const char string_187 [] PROGMEM = "work";
const char string_188 [] PROGMEM = "without";
const char string_189 [] PROGMEM = "replied";
const char string_190 [] PROGMEM = "make";
const char string_191 [] PROGMEM = "imagination";
const char string_192 [] PROGMEM = "mother";
const char string_193 [] PROGMEM = "captain";
const char string_194 [] PROGMEM = "small";
const char string_195 [] PROGMEM = "where";
const char string_196 [] PROGMEM = "paris";
const char string_197 [] PROGMEM = "famous";
const char string_198 [] PROGMEM = "didn't";
const char string_199 [] PROGMEM = "because";
const char string_200 [] PROGMEM = "play";
const char string_201 [] PROGMEM = "would";
const char string_202 [] PROGMEM = "political";
const char string_203 [] PROGMEM = "played";
const char string_204 [] PROGMEM = "that's";
const char string_205 [] PROGMEM = "family";
const char string_206 [] PROGMEM = "company";
const char string_207 [] PROGMEM = "almost";
const char string_208 [] PROGMEM = "already";
const char string_209 [] PROGMEM = "something";
const char string_210 [] PROGMEM = "place";
const char string_211 [] PROGMEM = "himself";
const char string_212 [] PROGMEM = "women";
const char string_213 [] PROGMEM = "came";
const char string_214 [] PROGMEM = "remain";
const char string_215 [] PROGMEM = "simply";
const char string_216 [] PROGMEM = "from";
const char string_217 [] PROGMEM = "could";
const char string_218 [] PROGMEM = "don't";
const char string_219 [] PROGMEM = "much";
const char string_220 [] PROGMEM = "april";
const char string_221 [] PROGMEM = "please";
const char string_222 [] PROGMEM = "power";
const char string_223 [] PROGMEM = "work";
const char string_224 [] PROGMEM = "without";
const char string_225 [] PROGMEM = "replied";
const char string_226 [] PROGMEM = "make";
const char string_227 [] PROGMEM = "imagination";
const char string_228 [] PROGMEM = "mother";
const char string_229 [] PROGMEM = "captain";
const char string_230 [] PROGMEM = "small";
const char string_231 [] PROGMEM = "where";
const char string_232 [] PROGMEM = "paris";
const char string_233 [] PROGMEM = "famous";
const char string_234 [] PROGMEM = "didn't";
const char string_235 [] PROGMEM = "because";
const char string_236 [] PROGMEM = "play";
const char string_237 [] PROGMEM = "would";
const char string_238 [] PROGMEM = "political";
const char string_239 [] PROGMEM = "played";
const char string_240 [] PROGMEM = "that's";
const char string_241 [] PROGMEM = "family";
const char string_242 [] PROGMEM = "company";
const char string_243 [] PROGMEM = "almost";
const char string_244 [] PROGMEM = "already";
const char string_245 [] PROGMEM = "something";
const char string_246 [] PROGMEM = "place";
const char string_247 [] PROGMEM = "himself";
const char string_248 [] PROGMEM = "women";
const char string_249 [] PROGMEM = "came";
const char string_250 [] PROGMEM = "remain";
const char string_251 [] PROGMEM = "simply";
const char string_252 [] PROGMEM = "from";
const char string_253 [] PROGMEM = "could";
const char string_254 [] PROGMEM = "don't";
const char string_255 [] PROGMEM = "much";
const char string_256 [] PROGMEM = "april";
const char string_257 [] PROGMEM = "please";
const char string_258 [] PROGMEM = "power";
const char string_259 [] PROGMEM = "work";
const char string_260 [] PROGMEM = "without";
const char string_261 [] PROGMEM = "replied";
const char string_262 [] PROGMEM = "make";
const char string_263 [] PROGMEM = "imagination";
const char string_264 [] PROGMEM = "mother";
const char string_265 [] PROGMEM = "captain";
const char string_266 [] PROGMEM = "small";
const char string_267 [] PROGMEM = "where";
const char string_268 [] PROGMEM = "paris";
const char string_269 [] PROGMEM = "famous";
const char string_270 [] PROGMEM = "didn't";
const char string_271 [] PROGMEM = "because";
const char string_272 [] PROGMEM = "play";
const char string_273 [] PROGMEM = "would";
const char string_274 [] PROGMEM = "political";
const char string_275 [] PROGMEM = "played";
const char string_276 [] PROGMEM = "that's";
const char string_277 [] PROGMEM = "family";
const char string_278 [] PROGMEM = "company";
const char string_279 [] PROGMEM = "almost";
const char string_280 [] PROGMEM = "already";
const char string_281 [] PROGMEM = "something";
const char string_282 [] PROGMEM = "place";
const char string_283 [] PROGMEM = "himself";
const char string_284 [] PROGMEM = "women";
const char string_285 [] PROGMEM = "came";
const char string_286 [] PROGMEM = "remain";
const char string_287 [] PROGMEM = "simply";
const char string_288 [] PROGMEM = "from";
const char string_289 [] PROGMEM = "could";
const char string_290 [] PROGMEM = "don't";
const char string_291 [] PROGMEM = "much";
const char string_292 [] PROGMEM = "april";
const char string_293 [] PROGMEM = "please";
const char string_294 [] PROGMEM = "power";
const char string_295 [] PROGMEM = "work";
const char string_296 [] PROGMEM = "without";
const char string_297 [] PROGMEM = "replied";
const char string_298 [] PROGMEM = "make";
const char string_299 [] PROGMEM = "imagination";
const char string_300 [] PROGMEM = "mother";
const char string_301 [] PROGMEM = "captain";
const char string_302 [] PROGMEM = "small";
const char string_303 [] PROGMEM = "where";
const char string_304 [] PROGMEM = "paris";
const char string_305 [] PROGMEM = "famous";
const char string_306 [] PROGMEM = "didn't";
const char string_307 [] PROGMEM = "because";
const char string_308 [] PROGMEM = "play";
const char string_309 [] PROGMEM = "would";
const char string_310 [] PROGMEM = "political";
const char string_311 [] PROGMEM = "played";
const char string_312 [] PROGMEM = "that's";
const char string_313 [] PROGMEM = "family";
const char string_314 [] PROGMEM = "company";
const char string_315 [] PROGMEM = "almost";
const char string_316 [] PROGMEM = "already";
const char string_317 [] PROGMEM = "something";
const char string_318 [] PROGMEM = "place";
const char string_319 [] PROGMEM = "himself";
const char string_320 [] PROGMEM = "women";
const char string_321 [] PROGMEM = "came";
const char string_322 [] PROGMEM = "remain";
const char string_323 [] PROGMEM = "simply";
const char string_324 [] PROGMEM = "from";
const char string_325 [] PROGMEM = "could";
const char string_326 [] PROGMEM = "don't";
const char string_327 [] PROGMEM = "much";
const char string_328 [] PROGMEM = "april";
const char string_329 [] PROGMEM = "please";
const char string_330 [] PROGMEM = "power";
const char string_331 [] PROGMEM = "work";
const char string_332 [] PROGMEM = "without";
const char string_333 [] PROGMEM = "replied";
const char string_334 [] PROGMEM = "make";
const char string_335 [] PROGMEM = "imagination";
const char string_336 [] PROGMEM = "mother";
const char string_337 [] PROGMEM = "captain";
const char string_338 [] PROGMEM = "small";
const char string_339 [] PROGMEM = "where";
const char string_340 [] PROGMEM = "paris";
const char string_341 [] PROGMEM = "famous";
const char string_342 [] PROGMEM = "didn't";
const char string_343 [] PROGMEM = "because";
const char string_344 [] PROGMEM = "play";
const char string_345 [] PROGMEM = "would";
const char string_346 [] PROGMEM = "political";
const char string_347 [] PROGMEM = "played";
const char string_348 [] PROGMEM = "that's";
const char string_349 [] PROGMEM = "family";
const char string_350 [] PROGMEM = "company";
const char string_351 [] PROGMEM = "almost";
const char string_352 [] PROGMEM = "already";
const char string_353 [] PROGMEM = "something";
const char string_354 [] PROGMEM = "place";
const char string_355 [] PROGMEM = "himself";
const char string_356 [] PROGMEM = "women";
const char string_357 [] PROGMEM = "came";
const char string_358 [] PROGMEM = "remain";
const char string_359 [] PROGMEM = "simply";
const char string_360 [] PROGMEM = "from";
const char string_361 [] PROGMEM = "could";
const char string_362 [] PROGMEM = "don't";
const char string_363 [] PROGMEM = "much";
const char string_364 [] PROGMEM = "april";
const char string_365 [] PROGMEM = "please";
const char string_366 [] PROGMEM = "power";
const char string_367 [] PROGMEM = "work";
const char string_368 [] PROGMEM = "without";
const char string_369 [] PROGMEM = "replied";
const char string_370 [] PROGMEM = "make";
const char string_371 [] PROGMEM = "imagination";
const char string_372 [] PROGMEM = "mother";
const char string_373 [] PROGMEM = "captain";
const char string_374 [] PROGMEM = "small";
const char string_375 [] PROGMEM = "where";
const char string_376 [] PROGMEM = "paris";
const char string_377 [] PROGMEM = "famous";
const char string_378 [] PROGMEM = "didn't";
const char string_379 [] PROGMEM = "because";
const char string_380 [] PROGMEM = "play";
const char string_381 [] PROGMEM = "would";
const char string_382 [] PROGMEM = "political";
const char string_383 [] PROGMEM = "played";
const char string_384 [] PROGMEM = "that's";
const char string_385 [] PROGMEM = "family";
const char string_386 [] PROGMEM = "company";
const char string_387 [] PROGMEM = "almost";
const char string_388 [] PROGMEM = "already";
const char string_389 [] PROGMEM = "something";
const char string_390 [] PROGMEM = "place";
const char string_391 [] PROGMEM = "himself";
const char string_392 [] PROGMEM = "where";
const char string_393 [] PROGMEM = "paris";
const char string_394 [] PROGMEM = "famous";
const char string_395 [] PROGMEM = "didn't";
const char string_396 [] PROGMEM = "because";
const char string_397 [] PROGMEM = "play";
const char string_398 [] PROGMEM = "would";
const char string_399 [] PROGMEM = "political";
const char string_400 [] PROGMEM = "played";
const char string_401 [] PROGMEM = "that's";
const char string_402 [] PROGMEM = "family";
const char string_403 [] PROGMEM = "company";
const char string_404 [] PROGMEM = "almost";
const char string_405 [] PROGMEM = "already";
const char string_406 [] PROGMEM = "something";
const char string_407 [] PROGMEM = "place";
const char string_408 [] PROGMEM = "himself";
const char string_409 [] PROGMEM = "where";
const char string_410 [] PROGMEM = "paris";
const char string_411 [] PROGMEM = "famous";
const char string_412 [] PROGMEM = "didn't";
const char string_413 [] PROGMEM = "because";
const char string_414 [] PROGMEM = "play";
const char string_415 [] PROGMEM = "would";
const char string_416 [] PROGMEM = "political";
const char string_417 [] PROGMEM = "played";
const char string_418 [] PROGMEM = "that's";
const char string_419 [] PROGMEM = "family";
const char string_420 [] PROGMEM = "company";
const char string_421 [] PROGMEM = "almost";
const char string_422 [] PROGMEM = "already";
const char string_423 [] PROGMEM = "something";
const char string_424 [] PROGMEM = "place";
const char string_425 [] PROGMEM = "himself";
const char string_426 [] PROGMEM = "where";
const char string_427 [] PROGMEM = "paris";
const char string_428 [] PROGMEM = "famous";
const char string_429 [] PROGMEM = "didn't";
const char string_430 [] PROGMEM = "because";
const char string_431 [] PROGMEM = "play";
const char string_432 [] PROGMEM = "would";
const char string_433 [] PROGMEM = "political";
const char string_434 [] PROGMEM = "played";
const char string_435 [] PROGMEM = "that's";
const char string_436 [] PROGMEM = "family";
const char string_437 [] PROGMEM = "company";
const char string_438 [] PROGMEM = "almost";
const char string_439 [] PROGMEM = "already";
const char string_440 [] PROGMEM = "something";
const char string_441 [] PROGMEM = "place";
const char string_442 [] PROGMEM = "himself";
const char string_443 [] PROGMEM = "hi";
const char string_444 [] PROGMEM = "hi";
const char string_445 [] PROGMEM = "hi";
const char string_446 [] PROGMEM = "hi";
const char string_447 [] PROGMEM = "hi";
const char string_448 [] PROGMEM = "hi";
const char string_449 [] PROGMEM = "hi";
const char string_450 [] PROGMEM = "hi";

const char * const strings[] PROGMEM = {
    string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10, string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19, string_20, string_21, string_22, string_23, string_24, string_25, string_26, string_27, string_28, string_29, string_30, string_31, string_32, string_33, string_34, string_35, string_36, string_37, string_38, string_39, string_40, string_41, string_42, string_43, string_44, string_45, string_46, string_47, string_48, string_49, string_50, string_51, string_52, string_53, string_54, string_55, string_56, string_57, string_58, string_59, string_60, string_61, string_62, string_63, string_64, string_65, string_66, string_67, string_68, string_69, string_70, string_71, string_72, string_73, string_74, string_75, string_76, string_77, string_78, string_79, string_80, string_81, string_82, string_83, string_84, string_85, string_86, string_87, string_88, string_89, string_90, string_91, string_92, string_93, string_94, string_95, string_96, string_97, string_98, string_99, string_100, string_101, string_102, string_103, string_104, string_105, string_106, string_107, string_108, string_109, string_110, string_111, string_112, string_113, string_114, string_115, string_116, string_117, string_118, string_119, string_120, string_121, string_122, string_123, string_124, string_125, string_126, string_127, string_128, string_129, string_130, string_131, string_132, string_133, string_134, string_135, string_136, string_137, string_138, string_139, string_140, string_141, string_142, string_143, string_144, string_145, string_146, string_147, string_148, string_149, string_150, string_151, string_152, string_153, string_154, string_155, string_156, string_157, string_158, string_159, string_160, string_161, string_162, string_163, string_164, string_165, string_166, string_167, string_168, string_169, string_170, string_171, string_172, string_173, string_174, string_175, string_176, string_177, string_178, string_179, string_180, string_181, string_182, string_183, string_184, string_185, string_186, string_187, string_188, string_189, string_190, string_191, string_192, string_193, string_194, string_195, string_196, string_197, string_198, string_199, string_200, string_201, string_202, string_203, string_204, string_205, string_206, string_207, string_208, string_209, string_210, string_211, string_212, string_213, string_214, string_215, string_216, string_217, string_218, string_219, string_220, string_221, string_222, string_223, string_224, string_225, string_226, string_227, string_228, string_229, string_230, string_231, string_232, string_233, string_234, string_235, string_236, string_237, string_238, string_239, string_240, string_241, string_242, string_243, string_244, string_245, string_246, string_247, string_248, string_249, string_250, string_251, string_252, string_253, string_254, string_255, string_256, string_257, string_258, string_259, string_260, string_261, string_262, string_263, string_264, string_265, string_266, string_267, string_268, string_269, string_270, string_271, string_272, string_273, string_274, string_275, string_276, string_277, string_278, string_279, string_280, string_281, string_282, string_283, string_284, string_285, string_286, string_287, string_288, string_289, string_290, string_291, string_292, string_293, string_294, string_295, string_296, string_297, string_298, string_299, string_300, string_301, string_302, string_303, string_304, string_305, string_306, string_307, string_308, string_309, string_310, string_311, string_312, string_313, string_314, string_315, string_316, string_317, string_318, string_319, string_320, string_321, string_322, string_323, string_324, string_325, string_326, string_327, string_328, string_329, string_330, string_331, string_332, string_333, string_334, string_335, string_336, string_337, string_338, string_339, string_340, string_341, string_342, string_343, string_344, string_345, string_346, string_347, string_348, string_349, string_350, string_351, string_352, string_353, string_354, string_355, string_356, string_357, string_358, string_359, string_360, string_361, string_362, string_363, string_364, string_365, string_366, string_367, string_368, string_369, string_370, string_371, string_372, string_373, string_374, string_375, string_376, string_377, string_378, string_379, string_380, string_381, string_382, string_383, string_384, string_385, string_386, string_387, string_388, string_389, string_390, string_391, string_392, string_393, string_394, string_395, string_396, string_397, string_398, string_399, string_400, string_401, string_402, string_403, string_404, string_405, string_406, string_407, string_408, string_409, string_410, string_411, string_412, string_413, string_414, string_415, string_416, string_417, string_418, string_419, string_420, string_421, string_422, string_423, string_424, string_425, string_426, string_427, string_428, string_429, string_430, string_431, string_432, string_433, string_434, string_435, string_436, string_437, string_438, string_439, string_440, string_441, string_442, string_443, string_444, string_445, string_446, string_447, string_448, string_449, string_450
};

void single_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            key_in(self->value1);
            break;
        case DEACTIVATED:
            key_out(self->value1);
            *self->state = IDLE;
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
            current_pseudolayer = self->value2;
            a_key_went_through = false;
            break;
        case DEACTIVATED:
        case RESTART:
            if (!a_key_went_through) {
                tap_key(self->value1);
            }
            current_pseudolayer = self->pseudolayer;
            *self->state = IDLE; // does not have effect if the state was RESTART
            break;
        default:
            break;
    }
}

void key_mod_dance(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            key_in(self->value2);
            a_key_went_through = false;
            break;
        case DEACTIVATED:
        case RESTART:
            key_out(self->value2);
            if (!a_key_went_through) {
                tap_key(self->value1);
            }
            *self->state = IDLE; // does not have effect if the state was RESTART
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
            tap_key(self->value1);
            *self->state = IDLE;
            break;
        case FINISHED:
        case PRESS_FROM_ACTIVE:
            key_in(self->value2);
            break;
        case RESTART:
            key_out(self->value2);
            break;
        default:
            break;
    }
}

void autoshift_dance_impl(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            *self->counter = 0;
            break;
        case DEACTIVATED:
        case RESTART:
            tap_key(self->value1);
            *self->state = IDLE;
            break;
        case FINISHED_FROM_ACTIVE:
            if (*self->counter == (LONG_PRESS_MULTIPLIER - 2)) {
                key_in(KC_LSFT);
                tap_key(self->value1);
                key_out(KC_LSFT);
                *self->state = IDLE;
                // the skip to IDLE is usually just a lag optimization,
                // in this case it has a logic function, on a short
                // press (still longer than a tap) the key does not get shifted
            } else {
                *self->counter += 1;
                *self->state = PRESS_FROM_ACTIVE;
                dance_timer = timer_read();
            }
            break;
        default:
            break;
    }
}

void autoshift_dance(const struct Chord* self) {
    if (autoshift_mode) {
        autoshift_dance_impl(self);
    } else {
        single_dance(self);
    }
}

void autoshift_toggle(const struct Chord* self){
    if (*self->state == ACTIVATED) {
        autoshift_mode = !autoshift_mode;
        *self->state = IDLE;
    }
}

void temp_pseudolayer(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            current_pseudolayer = self->value1;
            break;
        case DEACTIVATED:
            current_pseudolayer = self->pseudolayer;
            *self->state = IDLE;
            break;
        case RESTART:
            current_pseudolayer = self->pseudolayer;
            break;
        default:
            break;
    }
}

void temp_pseudolayer_alt(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            current_pseudolayer = self->value1;
            break;
        case DEACTIVATED:
            current_pseudolayer = self->value2;
            *self->state = IDLE;
            break;
        case RESTART:
            current_pseudolayer = self->value2;
            break;
        default:
            break;
    }
}

void perm_pseudolayer(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        current_pseudolayer = self->value1;
        *self->state = IDLE;
    }
}

void switch_layer(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        layer_move(self->value1);
        *self->state = IDLE;
    }
}

void lock(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        lock_next = true;
        *self->state = IDLE;
    }
}

void one_shot_key(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            break;
        case DEACTIVATED:
            key_in(self->value1);
            *self->state = IN_ONE_SHOT;
            break;
        case FINISHED:
        case PRESS_FROM_ACTIVE:
            key_in(self->value1);
            a_key_went_through = false;
            break;
        case RESTART:
            if (a_key_went_through) {
                key_out(self->value1);
            } else {
                *self->state = IN_ONE_SHOT;
            }
        default:
            break;
    }
}

void one_shot_layer(const struct Chord* self) {
    switch (*self->state) {
        case ACTIVATED:
            break;
        case DEACTIVATED:
            current_pseudolayer = self->value1;
            *self->state = IN_ONE_SHOT;
            break;
        case FINISHED:
        case PRESS_FROM_ACTIVE:
            current_pseudolayer = self->value1;
            a_key_went_through = false;
            break;
        case RESTART:
            if (a_key_went_through) {
                current_pseudolayer = self->pseudolayer;
            } else {
                *self->state = IN_ONE_SHOT;
            }
        default:
            break;
    }
}

void command(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        command_mode++;
        *self->state = IDLE;
    }
}

bool identical(uint16_t* buffer1, uint16_t* buffer2) {
    bool same = true;
    for (int i = 0; i < LEADER_MAX_LENGTH; i++) {
        same = same && (buffer1[i] == buffer2[i]);
    }
    return same;
}

void leader(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        in_leader_mode = true;
        *self->state = IDLE;
    }
}

void dynamic_macro_record(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        for (int i = 0; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
            dynamic_macro_buffer[i] = 0;
        }
        dynamic_macro_mode = true;
        *self->state = IDLE;
    }
}

void dynamic_macro_next(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode && dynamic_macro_ind < DYNAMIC_MACRO_MAX_LENGTH) {
            dynamic_macro_buffer[dynamic_macro_ind] = 0;
            dynamic_macro_ind++;
        }
        *self->state = IDLE;
    }
}

void dynamic_macro_end(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        if (dynamic_macro_mode) {
            dynamic_macro_mode = false;
        }
        *self->state = IDLE;
    }
}

void dynamic_macro_play(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        int ind_start = 0;
        while (ind_start < DYNAMIC_MACRO_MAX_LENGTH) {
            for (int i = ind_start; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
                if (dynamic_macro_buffer[i] == 0) {
                    break;
                }
                register_code(dynamic_macro_buffer[i]);
            }
            send_keyboard_report();
            wait_ms(TAP_TIMEOUT);
            for (int i = ind_start; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
                if (dynamic_macro_buffer[i] == 0) {
                    ind_start = i + 1;
                    break;
                }
                unregister_code(dynamic_macro_buffer[i]);
            }
            send_keyboard_report();
        }
        *self->state = IDLE;
    }
}

void string_in(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        char buffer[STRING_MAX_LENGTH];
        strcpy_P(buffer, (char*)pgm_read_word(&(strings[self->value1])));
        send_string(buffer);
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

uint8_t state_0 = IDLE;
const struct Chord chord_0 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, ALWAYS_ON, &state_0, NULL, NUM, BASE, temp_pseudolayer_alt};
uint8_t state_1 = IDLE;
const struct Chord chord_1 PROGMEM = {H_L2 + H_L3 + H_R2 + H_R3, ALWAYS_ON, &state_1, NULL, COMMANDS, BASE, temp_pseudolayer_alt};
uint8_t state_2 = IDLE;
const struct Chord chord_2 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4 + H_R1 + H_R2 + H_R3 + H_R4, ALWAYS_ON, &state_2, NULL, USER, BASE, temp_pseudolayer_alt};
uint8_t state_3 = IDLE;
const struct Chord chord_3 PROGMEM = {H_L3 + H_L4 + H_R1 + H_R2, ALWAYS_ON, &state_3, NULL, 0, 0, command};
uint8_t state_4 = IDLE;
const struct Chord chord_4 PROGMEM = {H_RT + H_L1 + H_L2, ALWAYS_ON, &state_4, NULL, -1, 0, string_in};
uint8_t state_5 = IDLE;
const struct Chord chord_5 PROGMEM = {H_RT + H_L2 + H_L3, ALWAYS_ON, &state_5, NULL, 0, 0, string_in};
uint8_t state_6 = IDLE;
const struct Chord chord_6 PROGMEM = {H_RT + H_L3 + H_L4, ALWAYS_ON, &state_6, NULL, 1, 0, string_in};
uint8_t state_7 = IDLE;
const struct Chord chord_7 PROGMEM = {H_RT + H_L4 + H_R1, ALWAYS_ON, &state_7, NULL, 2, 0, string_in};
uint8_t state_8 = IDLE;
const struct Chord chord_8 PROGMEM = {H_RT + H_R1 + H_R2, ALWAYS_ON, &state_8, NULL, 3, 0, string_in};
uint8_t state_9 = IDLE;
const struct Chord chord_9 PROGMEM = {H_RT + H_R2 + H_R3, ALWAYS_ON, &state_9, NULL, 4, 0, string_in};
uint8_t state_10 = IDLE;
const struct Chord chord_10 PROGMEM = {H_RT + H_R3 + H_R4, ALWAYS_ON, &state_10, NULL, 5, 0, string_in};
uint8_t state_11 = IDLE;
const struct Chord chord_11 PROGMEM = {H_L1, BASE, &state_11, NULL, KC_A, 0, single_dance};
uint8_t state_12 = IDLE;
const struct Chord chord_12 PROGMEM = {H_L2, BASE, &state_12, NULL, KC_S, 0, single_dance};
uint8_t state_13 = IDLE;
const struct Chord chord_13 PROGMEM = {H_L3, BASE, &state_13, NULL, KC_E, 0, single_dance};
uint8_t state_14 = IDLE;
const struct Chord chord_14 PROGMEM = {H_L4, BASE, &state_14, NULL, KC_T, 0, single_dance};
uint8_t state_15 = IDLE;
const struct Chord chord_15 PROGMEM = {H_R1, BASE, &state_15, NULL, KC_N, 0, single_dance};
uint8_t state_16 = IDLE;
const struct Chord chord_16 PROGMEM = {H_R2, BASE, &state_16, NULL, KC_I, 0, single_dance};
uint8_t state_17 = IDLE;
const struct Chord chord_17 PROGMEM = {H_R3, BASE, &state_17, NULL, KC_O, 0, single_dance};
uint8_t state_18 = IDLE;
const struct Chord chord_18 PROGMEM = {H_R4, BASE, &state_18, NULL, KC_P, 0, single_dance};
uint8_t state_19 = IDLE;
const struct Chord chord_19 PROGMEM = {H_L1 + H_L2, BASE, &state_19, NULL, KC_W, 0, single_dance};
uint8_t state_20 = IDLE;
const struct Chord chord_20 PROGMEM = {H_L2 + H_L3, BASE, &state_20, NULL, KC_X, 0, single_dance};
uint8_t state_21 = IDLE;
const struct Chord chord_21 PROGMEM = {H_L3 + H_L4, BASE, &state_21, NULL, KC_F, 0, single_dance};
uint8_t state_22 = IDLE;
const struct Chord chord_22 PROGMEM = {H_L4 + H_R1, BASE, &state_22, NULL, KC_Q, 0, single_dance};
uint8_t state_23 = IDLE;
const struct Chord chord_23 PROGMEM = {H_R1 + H_R2, BASE, &state_23, NULL, KC_Z, 0, single_dance};
uint8_t state_24 = IDLE;
const struct Chord chord_24 PROGMEM = {H_R3 + H_R4, BASE, &state_24, NULL, KC_R, 0, single_dance};
uint8_t state_25 = IDLE;
const struct Chord chord_25 PROGMEM = {H_L1 + H_L3, BASE, &state_25, NULL, KC_Y, 0, single_dance};
uint8_t state_26 = IDLE;
const struct Chord chord_26 PROGMEM = {H_L2 + H_L4, BASE, &state_26, NULL, KC_COMMA, 0, single_dance};
uint8_t state_27 = IDLE;
const struct Chord chord_27 PROGMEM = {H_L3 + H_R1, BASE, &state_27, NULL, KC_MINUS, 0, single_dance};
uint8_t state_28 = IDLE;
const struct Chord chord_28 PROGMEM = {H_L4 + H_R2, BASE, &state_28, NULL, KC_QUOTE, 0, single_dance};
uint8_t state_29 = IDLE;
const struct Chord chord_29 PROGMEM = {H_R1 + H_R3, BASE, &state_29, NULL, KC_B, 0, single_dance};
uint8_t state_30 = IDLE;
const struct Chord chord_30 PROGMEM = {H_R2 + H_R4, BASE, &state_30, NULL, KC_V, 0, single_dance};
uint8_t state_31 = IDLE;
const struct Chord chord_31 PROGMEM = {H_L1 + H_L4, BASE, &state_31, NULL, KC_G, 0, single_dance};
uint8_t state_32 = IDLE;
const struct Chord chord_32 PROGMEM = {H_L2 + H_R1, BASE, &state_32, NULL, KC_BSPC, 0, single_dance};
uint8_t state_33 = IDLE;
const struct Chord chord_33 PROGMEM = {H_L3 + H_R2, BASE, &state_33, NULL, KC_H, 0, single_dance};
uint8_t state_34 = IDLE;
const struct Chord chord_34 PROGMEM = {H_L4 + H_R3, BASE, &state_34, NULL, KC_U, 0, single_dance};
uint8_t state_35 = IDLE;
const struct Chord chord_35 PROGMEM = {H_R1 + H_R4, BASE, &state_35, NULL, KC_M, 0, single_dance};
uint8_t state_36 = IDLE;
const struct Chord chord_36 PROGMEM = {H_L1 + H_R1, BASE, &state_36, NULL, KC_L, 0, single_dance};
uint8_t state_37 = IDLE;
const struct Chord chord_37 PROGMEM = {H_L2 + H_R2, BASE, &state_37, NULL, KC_EXCLAIM, 0, single_dance};
uint8_t state_38 = IDLE;
const struct Chord chord_38 PROGMEM = {H_L3 + H_R3, BASE, &state_38, NULL, KC_SCOLON, 0, single_dance};
uint8_t state_39 = IDLE;
const struct Chord chord_39 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_39, NULL, 6, 0, string_in};
uint8_t state_40 = IDLE;
const struct Chord chord_40 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_40, NULL, 7, 0, string_in};
uint8_t state_41 = IDLE;
const struct Chord chord_41 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_41, NULL, 8, 0, string_in};
uint8_t state_42 = IDLE;
const struct Chord chord_42 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_42, NULL, 9, 0, string_in};
uint8_t state_43 = IDLE;
const struct Chord chord_43 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_43, NULL, 10, 0, string_in};
uint8_t state_44 = IDLE;
const struct Chord chord_44 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_44, NULL, 11, 0, string_in};
uint8_t state_45 = IDLE;
const struct Chord chord_45 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_45, NULL, 12, 0, string_in};
uint8_t state_46 = IDLE;
const struct Chord chord_46 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_46, NULL, 13, 0, string_in};
uint8_t state_47 = IDLE;
const struct Chord chord_47 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_47, NULL, 14, 0, string_in};
uint8_t state_48 = IDLE;
const struct Chord chord_48 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_48, NULL, 15, 0, string_in};
uint8_t state_49 = IDLE;
const struct Chord chord_49 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_49, NULL, 16, 0, string_in};
uint8_t state_50 = IDLE;
const struct Chord chord_50 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_50, NULL, 17, 0, string_in};
uint8_t state_51 = IDLE;
const struct Chord chord_51 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_51, NULL, 18, 0, string_in};
uint8_t state_52 = IDLE;
const struct Chord chord_52 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_52, NULL, 19, 0, string_in};
uint8_t state_53 = IDLE;
const struct Chord chord_53 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_53, NULL, 20, 0, string_in};
uint8_t state_54 = IDLE;
const struct Chord chord_54 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_54, NULL, 21, 0, string_in};
uint8_t state_55 = IDLE;
const struct Chord chord_55 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_55, NULL, 22, 0, string_in};
uint8_t state_56 = IDLE;
const struct Chord chord_56 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_56, NULL, 23, 0, string_in};
uint8_t state_57 = IDLE;
const struct Chord chord_57 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_57, NULL, 24, 0, string_in};
uint8_t state_58 = IDLE;
const struct Chord chord_58 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_58, NULL, 25, 0, string_in};
uint8_t state_59 = IDLE;
const struct Chord chord_59 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_59, NULL, 26, 0, string_in};
uint8_t state_60 = IDLE;
const struct Chord chord_60 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_60, NULL, 27, 0, string_in};
uint8_t state_61 = IDLE;
const struct Chord chord_61 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_61, NULL, 28, 0, string_in};
uint8_t state_62 = IDLE;
const struct Chord chord_62 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_62, NULL, 29, 0, string_in};
uint8_t state_63 = IDLE;
const struct Chord chord_63 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_63, NULL, 30, 0, string_in};
uint8_t state_64 = IDLE;
const struct Chord chord_64 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_64, NULL, 31, 0, string_in};
uint8_t state_65 = IDLE;
const struct Chord chord_65 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_65, NULL, 32, 0, string_in};
uint8_t state_66 = IDLE;
const struct Chord chord_66 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_66, NULL, 33, 0, string_in};
uint8_t state_67 = IDLE;
const struct Chord chord_67 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_67, NULL, 34, 0, string_in};
uint8_t state_68 = IDLE;
const struct Chord chord_68 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_68, NULL, 35, 0, string_in};
uint8_t state_69 = IDLE;
const struct Chord chord_69 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_69, NULL, 36, 0, string_in};
uint8_t state_70 = IDLE;
const struct Chord chord_70 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_70, NULL, 37, 0, string_in};
uint8_t state_71 = IDLE;
const struct Chord chord_71 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_71, NULL, 38, 0, string_in};
uint8_t state_72 = IDLE;
const struct Chord chord_72 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_72, NULL, 39, 0, string_in};
uint8_t state_73 = IDLE;
const struct Chord chord_73 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_73, NULL, 40, 0, string_in};
uint8_t state_74 = IDLE;
const struct Chord chord_74 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_74, NULL, 41, 0, string_in};
uint8_t state_75 = IDLE;
const struct Chord chord_75 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_75, NULL, 42, 0, string_in};
uint8_t state_76 = IDLE;
const struct Chord chord_76 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_76, NULL, 43, 0, string_in};
uint8_t state_77 = IDLE;
const struct Chord chord_77 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_77, NULL, 44, 0, string_in};
uint8_t state_78 = IDLE;
const struct Chord chord_78 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_78, NULL, 45, 0, string_in};
uint8_t state_79 = IDLE;
const struct Chord chord_79 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_79, NULL, 46, 0, string_in};
uint8_t state_80 = IDLE;
const struct Chord chord_80 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_80, NULL, 47, 0, string_in};
uint8_t state_81 = IDLE;
const struct Chord chord_81 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_81, NULL, 48, 0, string_in};
uint8_t state_82 = IDLE;
const struct Chord chord_82 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_82, NULL, 49, 0, string_in};
uint8_t state_83 = IDLE;
const struct Chord chord_83 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_83, NULL, 50, 0, string_in};
uint8_t state_84 = IDLE;
const struct Chord chord_84 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_84, NULL, 51, 0, string_in};
uint8_t state_85 = IDLE;
const struct Chord chord_85 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_85, NULL, 52, 0, string_in};
uint8_t state_86 = IDLE;
const struct Chord chord_86 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_86, NULL, 53, 0, string_in};
uint8_t state_87 = IDLE;
const struct Chord chord_87 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_87, NULL, 54, 0, string_in};
uint8_t state_88 = IDLE;
const struct Chord chord_88 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_88, NULL, 55, 0, string_in};
uint8_t state_89 = IDLE;
const struct Chord chord_89 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_89, NULL, 56, 0, string_in};
uint8_t state_90 = IDLE;
const struct Chord chord_90 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_90, NULL, 57, 0, string_in};
uint8_t state_91 = IDLE;
const struct Chord chord_91 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_91, NULL, 58, 0, string_in};
uint8_t state_92 = IDLE;
const struct Chord chord_92 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_92, NULL, 59, 0, string_in};
uint8_t state_93 = IDLE;
const struct Chord chord_93 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_93, NULL, 60, 0, string_in};
uint8_t state_94 = IDLE;
const struct Chord chord_94 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_94, NULL, 61, 0, string_in};
uint8_t state_95 = IDLE;
const struct Chord chord_95 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_95, NULL, 62, 0, string_in};
uint8_t state_96 = IDLE;
const struct Chord chord_96 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_96, NULL, 63, 0, string_in};
uint8_t state_97 = IDLE;
const struct Chord chord_97 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_97, NULL, 64, 0, string_in};
uint8_t state_98 = IDLE;
const struct Chord chord_98 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_98, NULL, 65, 0, string_in};
uint8_t state_99 = IDLE;
const struct Chord chord_99 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_99, NULL, 66, 0, string_in};
uint8_t state_100 = IDLE;
const struct Chord chord_100 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_100, NULL, KC_ENTER, 0, single_dance};
uint8_t state_101 = IDLE;
const struct Chord chord_101 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_101, NULL, 67, 0, string_in};
uint8_t state_102 = IDLE;
const struct Chord chord_102 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_102, NULL, 68, 0, string_in};
uint8_t state_103 = IDLE;
const struct Chord chord_103 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_103, NULL, 69, 0, string_in};
uint8_t state_104 = IDLE;
const struct Chord chord_104 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_104, NULL, 70, 0, string_in};
uint8_t state_105 = IDLE;
const struct Chord chord_105 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_105, NULL, 71, 0, string_in};
uint8_t state_106 = IDLE;
const struct Chord chord_106 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_106, NULL, 72, 0, string_in};
uint8_t state_107 = IDLE;
const struct Chord chord_107 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_107, NULL, 73, 0, string_in};
uint8_t state_108 = IDLE;
const struct Chord chord_108 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_108, NULL, 74, 0, string_in};
uint8_t state_109 = IDLE;
const struct Chord chord_109 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_109, NULL, 75, 0, string_in};
uint8_t state_110 = IDLE;
const struct Chord chord_110 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_110, NULL, 76, 0, string_in};
uint8_t state_111 = IDLE;
const struct Chord chord_111 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_111, NULL, 77, 0, string_in};
uint8_t state_112 = IDLE;
const struct Chord chord_112 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_112, NULL, 78, 0, string_in};
uint8_t state_113 = IDLE;
const struct Chord chord_113 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_113, NULL, 79, 0, string_in};
uint8_t state_114 = IDLE;
const struct Chord chord_114 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_114, NULL, 80, 0, string_in};
uint8_t state_115 = IDLE;
const struct Chord chord_115 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_115, NULL, 81, 0, string_in};
uint8_t state_116 = IDLE;
const struct Chord chord_116 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_116, NULL, 82, 0, string_in};
uint8_t state_117 = IDLE;
const struct Chord chord_117 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_117, NULL, 83, 0, string_in};
uint8_t state_118 = IDLE;
const struct Chord chord_118 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_118, NULL, 84, 0, string_in};
uint8_t state_119 = IDLE;
const struct Chord chord_119 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_119, NULL, 85, 0, string_in};
uint8_t state_120 = IDLE;
const struct Chord chord_120 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_120, NULL, 86, 0, string_in};
uint8_t state_121 = IDLE;
const struct Chord chord_121 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_121, NULL, 87, 0, string_in};
uint8_t state_122 = IDLE;
const struct Chord chord_122 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_122, NULL, 88, 0, string_in};
uint8_t state_123 = IDLE;
const struct Chord chord_123 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_123, NULL, 89, 0, string_in};
uint8_t state_124 = IDLE;
const struct Chord chord_124 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_124, NULL, 90, 0, string_in};
uint8_t state_125 = IDLE;
const struct Chord chord_125 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_125, NULL, 91, 0, string_in};
uint8_t state_126 = IDLE;
const struct Chord chord_126 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_126, NULL, 92, 0, string_in};
uint8_t state_127 = IDLE;
const struct Chord chord_127 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_127, NULL, 93, 0, string_in};
uint8_t state_128 = IDLE;
const struct Chord chord_128 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_128, NULL, 94, 0, string_in};
uint8_t state_129 = IDLE;
const struct Chord chord_129 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_129, NULL, 95, 0, string_in};
uint8_t state_130 = IDLE;
const struct Chord chord_130 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_130, NULL, 96, 0, string_in};
uint8_t state_131 = IDLE;
const struct Chord chord_131 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_131, NULL, 97, 0, string_in};
uint8_t state_132 = IDLE;
const struct Chord chord_132 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_132, NULL, 98, 0, string_in};
uint8_t state_133 = IDLE;
const struct Chord chord_133 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_133, NULL, 99, 0, string_in};
uint8_t state_134 = IDLE;
const struct Chord chord_134 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_134, NULL, 100, 0, string_in};
uint8_t state_135 = IDLE;
const struct Chord chord_135 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_135, NULL, 101, 0, string_in};
uint8_t state_136 = IDLE;
const struct Chord chord_136 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_136, NULL, 102, 0, string_in};
uint8_t state_137 = IDLE;
const struct Chord chord_137 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_137, NULL, 103, 0, string_in};
uint8_t state_138 = IDLE;
const struct Chord chord_138 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_138, NULL, 104, 0, string_in};
uint8_t state_139 = IDLE;
const struct Chord chord_139 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_139, NULL, 105, 0, string_in};
uint8_t state_140 = IDLE;
const struct Chord chord_140 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_140, NULL, 106, 0, string_in};
uint8_t state_141 = IDLE;
const struct Chord chord_141 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_141, NULL, 107, 0, string_in};
uint8_t state_142 = IDLE;
const struct Chord chord_142 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_142, NULL, 108, 0, string_in};
uint8_t state_143 = IDLE;
const struct Chord chord_143 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_143, NULL, 109, 0, string_in};
uint8_t state_144 = IDLE;
const struct Chord chord_144 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_144, NULL, 110, 0, string_in};
uint8_t state_145 = IDLE;
const struct Chord chord_145 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_145, NULL, 111, 0, string_in};
uint8_t state_146 = IDLE;
const struct Chord chord_146 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_146, NULL, 112, 0, string_in};
uint8_t state_147 = IDLE;
const struct Chord chord_147 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_147, NULL, 113, 0, string_in};
uint8_t state_148 = IDLE;
const struct Chord chord_148 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_148, NULL, 114, 0, string_in};
uint8_t state_149 = IDLE;
const struct Chord chord_149 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_149, NULL, 115, 0, string_in};
uint8_t state_150 = IDLE;
const struct Chord chord_150 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_150, NULL, 116, 0, string_in};
uint8_t state_151 = IDLE;
const struct Chord chord_151 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_151, NULL, 117, 0, string_in};
uint8_t state_152 = IDLE;
const struct Chord chord_152 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_152, NULL, 118, 0, string_in};
uint8_t state_153 = IDLE;
const struct Chord chord_153 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_153, NULL, 119, 0, string_in};
uint8_t state_154 = IDLE;
const struct Chord chord_154 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_154, NULL, 120, 0, string_in};
uint8_t state_155 = IDLE;
const struct Chord chord_155 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_155, NULL, 121, 0, string_in};
uint8_t state_156 = IDLE;
const struct Chord chord_156 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_156, NULL, 122, 0, string_in};
uint8_t state_157 = IDLE;
const struct Chord chord_157 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_157, NULL, 123, 0, string_in};
uint8_t state_158 = IDLE;
const struct Chord chord_158 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_158, NULL, 124, 0, string_in};
uint8_t state_159 = IDLE;
const struct Chord chord_159 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_159, NULL, 125, 0, string_in};
uint8_t state_160 = IDLE;
const struct Chord chord_160 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_160, NULL, 126, 0, string_in};
uint8_t state_161 = IDLE;
const struct Chord chord_161 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_161, NULL, 127, 0, string_in};
uint8_t state_162 = IDLE;
const struct Chord chord_162 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_162, NULL, 128, 0, string_in};
uint8_t state_163 = IDLE;
const struct Chord chord_163 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_163, NULL, 129, 0, string_in};
uint8_t state_164 = IDLE;
const struct Chord chord_164 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_164, NULL, 130, 0, string_in};
uint8_t state_165 = IDLE;
const struct Chord chord_165 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_165, NULL, 131, 0, string_in};
uint8_t state_166 = IDLE;
const struct Chord chord_166 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_166, NULL, 132, 0, string_in};
uint8_t state_167 = IDLE;
const struct Chord chord_167 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_167, NULL, 133, 0, string_in};
uint8_t state_168 = IDLE;
const struct Chord chord_168 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_168, NULL, 134, 0, string_in};
uint8_t state_169 = IDLE;
const struct Chord chord_169 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_169, NULL, 135, 0, string_in};
uint8_t state_170 = IDLE;
const struct Chord chord_170 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_170, NULL, 136, 0, string_in};
uint8_t state_171 = IDLE;
const struct Chord chord_171 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_171, NULL, 137, 0, string_in};
uint8_t state_172 = IDLE;
const struct Chord chord_172 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_172, NULL, 138, 0, string_in};
uint8_t state_173 = IDLE;
const struct Chord chord_173 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_173, NULL, 139, 0, string_in};
uint8_t state_174 = IDLE;
const struct Chord chord_174 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_174, NULL, 140, 0, string_in};
uint8_t state_175 = IDLE;
const struct Chord chord_175 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_175, NULL, 141, 0, string_in};
uint8_t state_176 = IDLE;
const struct Chord chord_176 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_176, NULL, 142, 0, string_in};
uint8_t state_177 = IDLE;
const struct Chord chord_177 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_177, NULL, 143, 0, string_in};
uint8_t state_178 = IDLE;
const struct Chord chord_178 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_178, NULL, 144, 0, string_in};
uint8_t state_179 = IDLE;
const struct Chord chord_179 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_179, NULL, 145, 0, string_in};
uint8_t state_180 = IDLE;
const struct Chord chord_180 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_180, NULL, 146, 0, string_in};
uint8_t state_181 = IDLE;
const struct Chord chord_181 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_181, NULL, 147, 0, string_in};
uint8_t state_182 = IDLE;
const struct Chord chord_182 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_182, NULL, 148, 0, string_in};
uint8_t state_183 = IDLE;
const struct Chord chord_183 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_183, NULL, 149, 0, string_in};
uint8_t state_184 = IDLE;
const struct Chord chord_184 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_184, NULL, 150, 0, string_in};
uint8_t state_185 = IDLE;
const struct Chord chord_185 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_185, NULL, 151, 0, string_in};
uint8_t state_186 = IDLE;
const struct Chord chord_186 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_186, NULL, 152, 0, string_in};
uint8_t state_187 = IDLE;
const struct Chord chord_187 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_187, NULL, 153, 0, string_in};
uint8_t state_188 = IDLE;
const struct Chord chord_188 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_188, NULL, 154, 0, string_in};
uint8_t state_189 = IDLE;
const struct Chord chord_189 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_189, NULL, 155, 0, string_in};
uint8_t state_190 = IDLE;
const struct Chord chord_190 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_190, NULL, 156, 0, string_in};
uint8_t state_191 = IDLE;
const struct Chord chord_191 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_191, NULL, 157, 0, string_in};
uint8_t state_192 = IDLE;
const struct Chord chord_192 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_192, NULL, 158, 0, string_in};
uint8_t state_193 = IDLE;
const struct Chord chord_193 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_193, NULL, 159, 0, string_in};
uint8_t state_194 = IDLE;
const struct Chord chord_194 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_194, NULL, 160, 0, string_in};
uint8_t state_195 = IDLE;
const struct Chord chord_195 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_195, NULL, 161, 0, string_in};
uint8_t state_196 = IDLE;
const struct Chord chord_196 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_196, NULL, 162, 0, string_in};
uint8_t state_197 = IDLE;
const struct Chord chord_197 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_197, NULL, 163, 0, string_in};
uint8_t state_198 = IDLE;
const struct Chord chord_198 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_198, NULL, 164, 0, string_in};
uint8_t state_199 = IDLE;
const struct Chord chord_199 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_199, NULL, 165, 0, string_in};
uint8_t state_200 = IDLE;
const struct Chord chord_200 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_200, NULL, 166, 0, string_in};
uint8_t state_201 = IDLE;
const struct Chord chord_201 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_201, NULL, 167, 0, string_in};
uint8_t state_202 = IDLE;
const struct Chord chord_202 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_202, NULL, 168, 0, string_in};
uint8_t state_203 = IDLE;
const struct Chord chord_203 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_203, NULL, 169, 0, string_in};
uint8_t state_204 = IDLE;
const struct Chord chord_204 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_204, NULL, 170, 0, string_in};
uint8_t state_205 = IDLE;
const struct Chord chord_205 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_205, NULL, 171, 0, string_in};
uint8_t state_206 = IDLE;
const struct Chord chord_206 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_206, NULL, 172, 0, string_in};
uint8_t state_207 = IDLE;
const struct Chord chord_207 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_207, NULL, 173, 0, string_in};
uint8_t state_208 = IDLE;
const struct Chord chord_208 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_208, NULL, 174, 0, string_in};
uint8_t state_209 = IDLE;
const struct Chord chord_209 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_209, NULL, 175, 0, string_in};
uint8_t state_210 = IDLE;
const struct Chord chord_210 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_210, NULL, 176, 0, string_in};
uint8_t state_211 = IDLE;
const struct Chord chord_211 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_211, NULL, 177, 0, string_in};
uint8_t state_212 = IDLE;
const struct Chord chord_212 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_212, NULL, 178, 0, string_in};
uint8_t state_213 = IDLE;
const struct Chord chord_213 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_213, NULL, 179, 0, string_in};
uint8_t state_214 = IDLE;
const struct Chord chord_214 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_214, NULL, 180, 0, string_in};
uint8_t state_215 = IDLE;
const struct Chord chord_215 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_215, NULL, 181, 0, string_in};
uint8_t state_216 = IDLE;
const struct Chord chord_216 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_216, NULL, 182, 0, string_in};
uint8_t state_217 = IDLE;
const struct Chord chord_217 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_217, NULL, 183, 0, string_in};
uint8_t state_218 = IDLE;
const struct Chord chord_218 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_218, NULL, 184, 0, string_in};
uint8_t state_219 = IDLE;
const struct Chord chord_219 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_219, NULL, 185, 0, string_in};
uint8_t state_220 = IDLE;
const struct Chord chord_220 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_220, NULL, 186, 0, string_in};
uint8_t state_221 = IDLE;
const struct Chord chord_221 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_221, NULL, 187, 0, string_in};
uint8_t state_222 = IDLE;
const struct Chord chord_222 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_222, NULL, 188, 0, string_in};
uint8_t state_223 = IDLE;
const struct Chord chord_223 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_223, NULL, 189, 0, string_in};
uint8_t state_224 = IDLE;
const struct Chord chord_224 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_224, NULL, 190, 0, string_in};
uint8_t state_225 = IDLE;
const struct Chord chord_225 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_225, NULL, 191, 0, string_in};
uint8_t state_226 = IDLE;
const struct Chord chord_226 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_226, NULL, 192, 0, string_in};
uint8_t state_227 = IDLE;
const struct Chord chord_227 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_227, NULL, 193, 0, string_in};
uint8_t state_228 = IDLE;
const struct Chord chord_228 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_228, NULL, 194, 0, string_in};
uint8_t state_229 = IDLE;
const struct Chord chord_229 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_229, NULL, 195, 0, string_in};
uint8_t state_230 = IDLE;
const struct Chord chord_230 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_230, NULL, 196, 0, string_in};
uint8_t state_231 = IDLE;
const struct Chord chord_231 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_231, NULL, 197, 0, string_in};
uint8_t state_232 = IDLE;
const struct Chord chord_232 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_232, NULL, 198, 0, string_in};
uint8_t state_233 = IDLE;
const struct Chord chord_233 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_233, NULL, 199, 0, string_in};
uint8_t state_234 = IDLE;
const struct Chord chord_234 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_234, NULL, 200, 0, string_in};
uint8_t state_235 = IDLE;
const struct Chord chord_235 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_235, NULL, 201, 0, string_in};
uint8_t state_236 = IDLE;
const struct Chord chord_236 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_236, NULL, 202, 0, string_in};
uint8_t state_237 = IDLE;
const struct Chord chord_237 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_237, NULL, 203, 0, string_in};
uint8_t state_238 = IDLE;
const struct Chord chord_238 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_238, NULL, 204, 0, string_in};
uint8_t state_239 = IDLE;
const struct Chord chord_239 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_239, NULL, 205, 0, string_in};
uint8_t state_240 = IDLE;
const struct Chord chord_240 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_240, NULL, 206, 0, string_in};
uint8_t state_241 = IDLE;
const struct Chord chord_241 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_241, NULL, 207, 0, string_in};
uint8_t state_242 = IDLE;
const struct Chord chord_242 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_242, NULL, 208, 0, string_in};
uint8_t state_243 = IDLE;
const struct Chord chord_243 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_243, NULL, 209, 0, string_in};
uint8_t state_244 = IDLE;
const struct Chord chord_244 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_244, NULL, 210, 0, string_in};
uint8_t state_245 = IDLE;
const struct Chord chord_245 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_245, NULL, 211, 0, string_in};
uint8_t state_246 = IDLE;
const struct Chord chord_246 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_246, NULL, 212, 0, string_in};
uint8_t state_247 = IDLE;
const struct Chord chord_247 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_247, NULL, 213, 0, string_in};
uint8_t state_248 = IDLE;
const struct Chord chord_248 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_248, NULL, 214, 0, string_in};
uint8_t state_249 = IDLE;
const struct Chord chord_249 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_249, NULL, 215, 0, string_in};
uint8_t state_250 = IDLE;
const struct Chord chord_250 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_250, NULL, 216, 0, string_in};
uint8_t state_251 = IDLE;
const struct Chord chord_251 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_251, NULL, 217, 0, string_in};
uint8_t state_252 = IDLE;
const struct Chord chord_252 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_252, NULL, 218, 0, string_in};
uint8_t state_253 = IDLE;
const struct Chord chord_253 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_253, NULL, 219, 0, string_in};
uint8_t state_254 = IDLE;
const struct Chord chord_254 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_254, NULL, 220, 0, string_in};
uint8_t state_255 = IDLE;
const struct Chord chord_255 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_255, NULL, 221, 0, string_in};
uint8_t state_256 = IDLE;
const struct Chord chord_256 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_256, NULL, 222, 0, string_in};
uint8_t state_257 = IDLE;
const struct Chord chord_257 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_257, NULL, 223, 0, string_in};
uint8_t state_258 = IDLE;
const struct Chord chord_258 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_258, NULL, 224, 0, string_in};
uint8_t state_259 = IDLE;
const struct Chord chord_259 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_259, NULL, 225, 0, string_in};
uint8_t state_260 = IDLE;
const struct Chord chord_260 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_260, NULL, 226, 0, string_in};
uint8_t state_261 = IDLE;
const struct Chord chord_261 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_261, NULL, 227, 0, string_in};
uint8_t state_262 = IDLE;
const struct Chord chord_262 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_262, NULL, 228, 0, string_in};
uint8_t state_263 = IDLE;
const struct Chord chord_263 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_263, NULL, 229, 0, string_in};
uint8_t state_264 = IDLE;
const struct Chord chord_264 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_264, NULL, 230, 0, string_in};
uint8_t state_265 = IDLE;
const struct Chord chord_265 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_265, NULL, 231, 0, string_in};
uint8_t state_266 = IDLE;
const struct Chord chord_266 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_266, NULL, 232, 0, string_in};
uint8_t state_267 = IDLE;
const struct Chord chord_267 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_267, NULL, 233, 0, string_in};
uint8_t state_268 = IDLE;
const struct Chord chord_268 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_268, NULL, 234, 0, string_in};
uint8_t state_269 = IDLE;
const struct Chord chord_269 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_269, NULL, 235, 0, string_in};
uint8_t state_270 = IDLE;
const struct Chord chord_270 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_270, NULL, 236, 0, string_in};
uint8_t state_271 = IDLE;
const struct Chord chord_271 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_271, NULL, 237, 0, string_in};
uint8_t state_272 = IDLE;
const struct Chord chord_272 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_272, NULL, 238, 0, string_in};
uint8_t state_273 = IDLE;
const struct Chord chord_273 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_273, NULL, 239, 0, string_in};
uint8_t state_274 = IDLE;
const struct Chord chord_274 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_274, NULL, 240, 0, string_in};
uint8_t state_275 = IDLE;
const struct Chord chord_275 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_275, NULL, 241, 0, string_in};
uint8_t state_276 = IDLE;
const struct Chord chord_276 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_276, NULL, 242, 0, string_in};
uint8_t state_277 = IDLE;
const struct Chord chord_277 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_277, NULL, 243, 0, string_in};
uint8_t state_278 = IDLE;
const struct Chord chord_278 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_278, NULL, 244, 0, string_in};
uint8_t state_279 = IDLE;
const struct Chord chord_279 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_279, NULL, 245, 0, string_in};
uint8_t state_280 = IDLE;
const struct Chord chord_280 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_280, NULL, 246, 0, string_in};
uint8_t state_281 = IDLE;
const struct Chord chord_281 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_281, NULL, 247, 0, string_in};
uint8_t state_282 = IDLE;
const struct Chord chord_282 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_282, NULL, 248, 0, string_in};
uint8_t state_283 = IDLE;
const struct Chord chord_283 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_283, NULL, 249, 0, string_in};
uint8_t state_284 = IDLE;
const struct Chord chord_284 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_284, NULL, 250, 0, string_in};
uint8_t state_285 = IDLE;
const struct Chord chord_285 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_285, NULL, 251, 0, string_in};
uint8_t state_286 = IDLE;
const struct Chord chord_286 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_286, NULL, 252, 0, string_in};
uint8_t state_287 = IDLE;
const struct Chord chord_287 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_287, NULL, 253, 0, string_in};
uint8_t state_288 = IDLE;
const struct Chord chord_288 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_288, NULL, 254, 0, string_in};
uint8_t state_289 = IDLE;
const struct Chord chord_289 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_289, NULL, 255, 0, string_in};
uint8_t state_290 = IDLE;
const struct Chord chord_290 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_290, NULL, 256, 0, string_in};
uint8_t state_291 = IDLE;
const struct Chord chord_291 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_291, NULL, 257, 0, string_in};
uint8_t state_292 = IDLE;
const struct Chord chord_292 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_292, NULL, 258, 0, string_in};
uint8_t state_293 = IDLE;
const struct Chord chord_293 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_293, NULL, 259, 0, string_in};
uint8_t state_294 = IDLE;
const struct Chord chord_294 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_294, NULL, 260, 0, string_in};
uint8_t state_295 = IDLE;
const struct Chord chord_295 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_295, NULL, 261, 0, string_in};
uint8_t state_296 = IDLE;
const struct Chord chord_296 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_296, NULL, 262, 0, string_in};
uint8_t state_297 = IDLE;
const struct Chord chord_297 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_297, NULL, 263, 0, string_in};
uint8_t state_298 = IDLE;
const struct Chord chord_298 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_298, NULL, 264, 0, string_in};
uint8_t state_299 = IDLE;
const struct Chord chord_299 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_299, NULL, 265, 0, string_in};
uint8_t state_300 = IDLE;
const struct Chord chord_300 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_300, NULL, 266, 0, string_in};
uint8_t state_301 = IDLE;
const struct Chord chord_301 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_301, NULL, 267, 0, string_in};
uint8_t state_302 = IDLE;
const struct Chord chord_302 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_302, NULL, 268, 0, string_in};
uint8_t state_303 = IDLE;
const struct Chord chord_303 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_303, NULL, 269, 0, string_in};
uint8_t state_304 = IDLE;
const struct Chord chord_304 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_304, NULL, 270, 0, string_in};
uint8_t state_305 = IDLE;
const struct Chord chord_305 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_305, NULL, 271, 0, string_in};
uint8_t state_306 = IDLE;
const struct Chord chord_306 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_306, NULL, 272, 0, string_in};
uint8_t state_307 = IDLE;
const struct Chord chord_307 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_307, NULL, 273, 0, string_in};
uint8_t state_308 = IDLE;
const struct Chord chord_308 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_308, NULL, 274, 0, string_in};
uint8_t state_309 = IDLE;
const struct Chord chord_309 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_309, NULL, 275, 0, string_in};
uint8_t state_310 = IDLE;
const struct Chord chord_310 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_310, NULL, 276, 0, string_in};
uint8_t state_311 = IDLE;
const struct Chord chord_311 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_311, NULL, 277, 0, string_in};
uint8_t state_312 = IDLE;
const struct Chord chord_312 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_312, NULL, 278, 0, string_in};
uint8_t state_313 = IDLE;
const struct Chord chord_313 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_313, NULL, 279, 0, string_in};
uint8_t state_314 = IDLE;
const struct Chord chord_314 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_314, NULL, 280, 0, string_in};
uint8_t state_315 = IDLE;
const struct Chord chord_315 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_315, NULL, 281, 0, string_in};
uint8_t state_316 = IDLE;
const struct Chord chord_316 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_316, NULL, 282, 0, string_in};
uint8_t state_317 = IDLE;
const struct Chord chord_317 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_317, NULL, 283, 0, string_in};
uint8_t state_318 = IDLE;
const struct Chord chord_318 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_318, NULL, 284, 0, string_in};
uint8_t state_319 = IDLE;
const struct Chord chord_319 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_319, NULL, 285, 0, string_in};
uint8_t state_320 = IDLE;
const struct Chord chord_320 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_320, NULL, 286, 0, string_in};
uint8_t state_321 = IDLE;
const struct Chord chord_321 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_321, NULL, 287, 0, string_in};
uint8_t state_322 = IDLE;
const struct Chord chord_322 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_322, NULL, 288, 0, string_in};
uint8_t state_323 = IDLE;
const struct Chord chord_323 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_323, NULL, 289, 0, string_in};
uint8_t state_324 = IDLE;
const struct Chord chord_324 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_324, NULL, 290, 0, string_in};
uint8_t state_325 = IDLE;
const struct Chord chord_325 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_325, NULL, 291, 0, string_in};
uint8_t state_326 = IDLE;
const struct Chord chord_326 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_326, NULL, 292, 0, string_in};
uint8_t state_327 = IDLE;
const struct Chord chord_327 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_327, NULL, 293, 0, string_in};
uint8_t state_328 = IDLE;
const struct Chord chord_328 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_328, NULL, 294, 0, string_in};
uint8_t state_329 = IDLE;
const struct Chord chord_329 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_329, NULL, 295, 0, string_in};
uint8_t state_330 = IDLE;
const struct Chord chord_330 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_330, NULL, 296, 0, string_in};
uint8_t state_331 = IDLE;
const struct Chord chord_331 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_331, NULL, 297, 0, string_in};
uint8_t state_332 = IDLE;
const struct Chord chord_332 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_332, NULL, 298, 0, string_in};
uint8_t state_333 = IDLE;
const struct Chord chord_333 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_333, NULL, 299, 0, string_in};
uint8_t state_334 = IDLE;
const struct Chord chord_334 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_334, NULL, 300, 0, string_in};
uint8_t state_335 = IDLE;
const struct Chord chord_335 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_335, NULL, 301, 0, string_in};
uint8_t state_336 = IDLE;
const struct Chord chord_336 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_336, NULL, 302, 0, string_in};
uint8_t state_337 = IDLE;
const struct Chord chord_337 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_337, NULL, 303, 0, string_in};
uint8_t state_338 = IDLE;
const struct Chord chord_338 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_338, NULL, 304, 0, string_in};
uint8_t state_339 = IDLE;
const struct Chord chord_339 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_339, NULL, 305, 0, string_in};
uint8_t state_340 = IDLE;
const struct Chord chord_340 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_340, NULL, 306, 0, string_in};
uint8_t state_341 = IDLE;
const struct Chord chord_341 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_341, NULL, 307, 0, string_in};
uint8_t state_342 = IDLE;
const struct Chord chord_342 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_342, NULL, 308, 0, string_in};
uint8_t state_343 = IDLE;
const struct Chord chord_343 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_343, NULL, 309, 0, string_in};
uint8_t state_344 = IDLE;
const struct Chord chord_344 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_344, NULL, 310, 0, string_in};
uint8_t state_345 = IDLE;
const struct Chord chord_345 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_345, NULL, 311, 0, string_in};
uint8_t state_346 = IDLE;
const struct Chord chord_346 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_346, NULL, 312, 0, string_in};
uint8_t state_347 = IDLE;
const struct Chord chord_347 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_347, NULL, 313, 0, string_in};
uint8_t state_348 = IDLE;
const struct Chord chord_348 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_348, NULL, 314, 0, string_in};
uint8_t state_349 = IDLE;
const struct Chord chord_349 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_349, NULL, 315, 0, string_in};
uint8_t state_350 = IDLE;
const struct Chord chord_350 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_350, NULL, 316, 0, string_in};
uint8_t state_351 = IDLE;
const struct Chord chord_351 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_351, NULL, 317, 0, string_in};
uint8_t state_352 = IDLE;
const struct Chord chord_352 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_352, NULL, 318, 0, string_in};
uint8_t state_353 = IDLE;
const struct Chord chord_353 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_353, NULL, 319, 0, string_in};
uint8_t state_354 = IDLE;
const struct Chord chord_354 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_354, NULL, 320, 0, string_in};
uint8_t state_355 = IDLE;
const struct Chord chord_355 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_355, NULL, 321, 0, string_in};
uint8_t state_356 = IDLE;
const struct Chord chord_356 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_356, NULL, 322, 0, string_in};
uint8_t state_357 = IDLE;
const struct Chord chord_357 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_357, NULL, 323, 0, string_in};
uint8_t state_358 = IDLE;
const struct Chord chord_358 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_358, NULL, 324, 0, string_in};
uint8_t state_359 = IDLE;
const struct Chord chord_359 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_359, NULL, 325, 0, string_in};
uint8_t state_360 = IDLE;
const struct Chord chord_360 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_360, NULL, 326, 0, string_in};
uint8_t state_361 = IDLE;
const struct Chord chord_361 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_361, NULL, 327, 0, string_in};
uint8_t state_362 = IDLE;
const struct Chord chord_362 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_362, NULL, 328, 0, string_in};
uint8_t state_363 = IDLE;
const struct Chord chord_363 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_363, NULL, 329, 0, string_in};
uint8_t state_364 = IDLE;
const struct Chord chord_364 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_364, NULL, 330, 0, string_in};
uint8_t state_365 = IDLE;
const struct Chord chord_365 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_365, NULL, 331, 0, string_in};
uint8_t state_366 = IDLE;
const struct Chord chord_366 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_366, NULL, 332, 0, string_in};
uint8_t state_367 = IDLE;
const struct Chord chord_367 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_367, NULL, 333, 0, string_in};
uint8_t state_368 = IDLE;
const struct Chord chord_368 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_368, NULL, 334, 0, string_in};
uint8_t state_369 = IDLE;
const struct Chord chord_369 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_369, NULL, 335, 0, string_in};
uint8_t state_370 = IDLE;
const struct Chord chord_370 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_370, NULL, 336, 0, string_in};
uint8_t state_371 = IDLE;
const struct Chord chord_371 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_371, NULL, 337, 0, string_in};
uint8_t state_372 = IDLE;
const struct Chord chord_372 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_372, NULL, 338, 0, string_in};
uint8_t state_373 = IDLE;
const struct Chord chord_373 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_373, NULL, 339, 0, string_in};
uint8_t state_374 = IDLE;
const struct Chord chord_374 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_374, NULL, 340, 0, string_in};
uint8_t state_375 = IDLE;
const struct Chord chord_375 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_375, NULL, 341, 0, string_in};
uint8_t state_376 = IDLE;
const struct Chord chord_376 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_376, NULL, 342, 0, string_in};
uint8_t state_377 = IDLE;
const struct Chord chord_377 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_377, NULL, 343, 0, string_in};
uint8_t state_378 = IDLE;
const struct Chord chord_378 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_378, NULL, 344, 0, string_in};
uint8_t state_379 = IDLE;
const struct Chord chord_379 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_379, NULL, 345, 0, string_in};
uint8_t state_380 = IDLE;
const struct Chord chord_380 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_380, NULL, 346, 0, string_in};
uint8_t state_381 = IDLE;
const struct Chord chord_381 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_381, NULL, 347, 0, string_in};
uint8_t state_382 = IDLE;
const struct Chord chord_382 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_382, NULL, 348, 0, string_in};
uint8_t state_383 = IDLE;
const struct Chord chord_383 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_383, NULL, 349, 0, string_in};
uint8_t state_384 = IDLE;
const struct Chord chord_384 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_384, NULL, 350, 0, string_in};
uint8_t state_385 = IDLE;
const struct Chord chord_385 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_385, NULL, 351, 0, string_in};
uint8_t state_386 = IDLE;
const struct Chord chord_386 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_386, NULL, 352, 0, string_in};
uint8_t state_387 = IDLE;
const struct Chord chord_387 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_387, NULL, 353, 0, string_in};
uint8_t state_388 = IDLE;
const struct Chord chord_388 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_388, NULL, 354, 0, string_in};
uint8_t state_389 = IDLE;
const struct Chord chord_389 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_389, NULL, 355, 0, string_in};
uint8_t state_390 = IDLE;
const struct Chord chord_390 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_390, NULL, 356, 0, string_in};
uint8_t state_391 = IDLE;
const struct Chord chord_391 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_391, NULL, 357, 0, string_in};
uint8_t state_392 = IDLE;
const struct Chord chord_392 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_392, NULL, 358, 0, string_in};
uint8_t state_393 = IDLE;
const struct Chord chord_393 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_393, NULL, 359, 0, string_in};
uint8_t state_394 = IDLE;
const struct Chord chord_394 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_394, NULL, 360, 0, string_in};
uint8_t state_395 = IDLE;
const struct Chord chord_395 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_395, NULL, 361, 0, string_in};
uint8_t state_396 = IDLE;
const struct Chord chord_396 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_396, NULL, 362, 0, string_in};
uint8_t state_397 = IDLE;
const struct Chord chord_397 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_397, NULL, 363, 0, string_in};
uint8_t state_398 = IDLE;
const struct Chord chord_398 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_398, NULL, 364, 0, string_in};
uint8_t state_399 = IDLE;
const struct Chord chord_399 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_399, NULL, 365, 0, string_in};
uint8_t state_400 = IDLE;
const struct Chord chord_400 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_400, NULL, 366, 0, string_in};
uint8_t state_401 = IDLE;
const struct Chord chord_401 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_401, NULL, 367, 0, string_in};
uint8_t state_402 = IDLE;
const struct Chord chord_402 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_402, NULL, 368, 0, string_in};
uint8_t state_403 = IDLE;
const struct Chord chord_403 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_403, NULL, 369, 0, string_in};
uint8_t state_404 = IDLE;
const struct Chord chord_404 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_404, NULL, 370, 0, string_in};
uint8_t state_405 = IDLE;
const struct Chord chord_405 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_405, NULL, 371, 0, string_in};
uint8_t state_406 = IDLE;
const struct Chord chord_406 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_406, NULL, 372, 0, string_in};
uint8_t state_407 = IDLE;
const struct Chord chord_407 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_407, NULL, 373, 0, string_in};
uint8_t state_408 = IDLE;
const struct Chord chord_408 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_408, NULL, 374, 0, string_in};
uint8_t state_409 = IDLE;
const struct Chord chord_409 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_409, NULL, 375, 0, string_in};
uint8_t state_410 = IDLE;
const struct Chord chord_410 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_410, NULL, 376, 0, string_in};
uint8_t state_411 = IDLE;
const struct Chord chord_411 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_411, NULL, 377, 0, string_in};
uint8_t state_412 = IDLE;
const struct Chord chord_412 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_412, NULL, 378, 0, string_in};
uint8_t state_413 = IDLE;
const struct Chord chord_413 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_413, NULL, 379, 0, string_in};
uint8_t state_414 = IDLE;
const struct Chord chord_414 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_414, NULL, 380, 0, string_in};
uint8_t state_415 = IDLE;
const struct Chord chord_415 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_415, NULL, 381, 0, string_in};
uint8_t state_416 = IDLE;
const struct Chord chord_416 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_416, NULL, 382, 0, string_in};
uint8_t state_417 = IDLE;
const struct Chord chord_417 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_417, NULL, 383, 0, string_in};
uint8_t state_418 = IDLE;
const struct Chord chord_418 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_418, NULL, 384, 0, string_in};
uint8_t state_419 = IDLE;
const struct Chord chord_419 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_419, NULL, 385, 0, string_in};
uint8_t state_420 = IDLE;
const struct Chord chord_420 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_420, NULL, 386, 0, string_in};
uint8_t state_421 = IDLE;
const struct Chord chord_421 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_421, NULL, 387, 0, string_in};
uint8_t state_422 = IDLE;
const struct Chord chord_422 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_422, NULL, 388, 0, string_in};
uint8_t state_423 = IDLE;
const struct Chord chord_423 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_423, NULL, 389, 0, string_in};
uint8_t state_424 = IDLE;
const struct Chord chord_424 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_424, NULL, 390, 0, string_in};
uint8_t state_425 = IDLE;
const struct Chord chord_425 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_425, NULL, 391, 0, string_in};
uint8_t state_426 = IDLE;
const struct Chord chord_426 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_426, NULL, 392, 0, string_in};
uint8_t state_427 = IDLE;
const struct Chord chord_427 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_427, NULL, 393, 0, string_in};
uint8_t state_428 = IDLE;
const struct Chord chord_428 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_428, NULL, 394, 0, string_in};
uint8_t state_429 = IDLE;
const struct Chord chord_429 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_429, NULL, 395, 0, string_in};
uint8_t state_430 = IDLE;
const struct Chord chord_430 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_430, NULL, 396, 0, string_in};
uint8_t state_431 = IDLE;
const struct Chord chord_431 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_431, NULL, 397, 0, string_in};
uint8_t state_432 = IDLE;
const struct Chord chord_432 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_432, NULL, 398, 0, string_in};
uint8_t state_433 = IDLE;
const struct Chord chord_433 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_433, NULL, 399, 0, string_in};
uint8_t state_434 = IDLE;
const struct Chord chord_434 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_434, NULL, 400, 0, string_in};
uint8_t state_435 = IDLE;
const struct Chord chord_435 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_435, NULL, 401, 0, string_in};
uint8_t state_436 = IDLE;
const struct Chord chord_436 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_436, NULL, 402, 0, string_in};
uint8_t state_437 = IDLE;
const struct Chord chord_437 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_437, NULL, 403, 0, string_in};
uint8_t state_438 = IDLE;
const struct Chord chord_438 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_438, NULL, 404, 0, string_in};
uint8_t state_439 = IDLE;
const struct Chord chord_439 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_439, NULL, 405, 0, string_in};
uint8_t state_440 = IDLE;
const struct Chord chord_440 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_440, NULL, 406, 0, string_in};
uint8_t state_441 = IDLE;
const struct Chord chord_441 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_441, NULL, 407, 0, string_in};
uint8_t state_442 = IDLE;
const struct Chord chord_442 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_442, NULL, 408, 0, string_in};
uint8_t state_443 = IDLE;
const struct Chord chord_443 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_443, NULL, 409, 0, string_in};
uint8_t state_444 = IDLE;
const struct Chord chord_444 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_444, NULL, 410, 0, string_in};
uint8_t state_445 = IDLE;
const struct Chord chord_445 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_445, NULL, 411, 0, string_in};
uint8_t state_446 = IDLE;
const struct Chord chord_446 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_446, NULL, 412, 0, string_in};
uint8_t state_447 = IDLE;
const struct Chord chord_447 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_447, NULL, 413, 0, string_in};
uint8_t state_448 = IDLE;
const struct Chord chord_448 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_448, NULL, 414, 0, string_in};
uint8_t state_449 = IDLE;
const struct Chord chord_449 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_449, NULL, 415, 0, string_in};
uint8_t state_450 = IDLE;
const struct Chord chord_450 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_450, NULL, 416, 0, string_in};
uint8_t state_451 = IDLE;
const struct Chord chord_451 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_451, NULL, 417, 0, string_in};
uint8_t state_452 = IDLE;
const struct Chord chord_452 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_452, NULL, 418, 0, string_in};
uint8_t state_453 = IDLE;
const struct Chord chord_453 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_453, NULL, 419, 0, string_in};
uint8_t state_454 = IDLE;
const struct Chord chord_454 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_454, NULL, 420, 0, string_in};
uint8_t state_455 = IDLE;
const struct Chord chord_455 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_455, NULL, 421, 0, string_in};
uint8_t state_456 = IDLE;
const struct Chord chord_456 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_456, NULL, 422, 0, string_in};
uint8_t state_457 = IDLE;
const struct Chord chord_457 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_457, NULL, 423, 0, string_in};
uint8_t state_458 = IDLE;
const struct Chord chord_458 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_458, NULL, 424, 0, string_in};
uint8_t state_459 = IDLE;
const struct Chord chord_459 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_459, NULL, 425, 0, string_in};
uint8_t state_460 = IDLE;
const struct Chord chord_460 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_460, NULL, 426, 0, string_in};
uint8_t state_461 = IDLE;
const struct Chord chord_461 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_461, NULL, 427, 0, string_in};
uint8_t state_462 = IDLE;
const struct Chord chord_462 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_462, NULL, 428, 0, string_in};
uint8_t state_463 = IDLE;
const struct Chord chord_463 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_463, NULL, 429, 0, string_in};
uint8_t state_464 = IDLE;
const struct Chord chord_464 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_464, NULL, 430, 0, string_in};
uint8_t state_465 = IDLE;
const struct Chord chord_465 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_465, NULL, 431, 0, string_in};
uint8_t state_466 = IDLE;
const struct Chord chord_466 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_466, NULL, 432, 0, string_in};
uint8_t state_467 = IDLE;
const struct Chord chord_467 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_467, NULL, 433, 0, string_in};
uint8_t state_468 = IDLE;
const struct Chord chord_468 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_468, NULL, 434, 0, string_in};
uint8_t state_469 = IDLE;
const struct Chord chord_469 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_469, NULL, 435, 0, string_in};
uint8_t state_470 = IDLE;
const struct Chord chord_470 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_470, NULL, 436, 0, string_in};
uint8_t state_471 = IDLE;
const struct Chord chord_471 PROGMEM = {H_L1 + H_L4 + H_R1 + H_R4, BASE, &state_471, NULL, 437, 0, string_in};
uint8_t state_472 = IDLE;
const struct Chord chord_472 PROGMEM = {H_L1 + H_L2 + H_L3, BASE, &state_472, NULL, 438, 0, string_in};
uint8_t state_473 = IDLE;
const struct Chord chord_473 PROGMEM = {H_L1 + H_L3 + H_L4, BASE, &state_473, NULL, 439, 0, string_in};
uint8_t state_474 = IDLE;
const struct Chord chord_474 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_474, NULL, 440, 0, string_in};
uint8_t state_475 = IDLE;
const struct Chord chord_475 PROGMEM = {H_L1 + H_L2 + H_L3 + H_L4, BASE, &state_475, NULL, 441, 0, string_in};
uint8_t state_476 = IDLE;
const struct Chord chord_476 PROGMEM = {H_L1, NUM, &state_476, NULL, KC_1, 0, single_dance};
uint8_t state_477 = IDLE;
const struct Chord chord_477 PROGMEM = {H_L2, NUM, &state_477, NULL, KC_2, 0, single_dance};
uint8_t state_478 = IDLE;
const struct Chord chord_478 PROGMEM = {H_L3, NUM, &state_478, NULL, KC_3, 0, single_dance};
uint8_t state_479 = IDLE;
const struct Chord chord_479 PROGMEM = {H_L4, NUM, &state_479, NULL, KC_4, 0, single_dance};
uint8_t state_480 = IDLE;
const struct Chord chord_480 PROGMEM = {H_R1, NUM, &state_480, NULL, KC_7, 0, single_dance};
uint8_t state_481 = IDLE;
const struct Chord chord_481 PROGMEM = {H_R2, NUM, &state_481, NULL, KC_8, 0, single_dance};
uint8_t state_482 = IDLE;
const struct Chord chord_482 PROGMEM = {H_R3, NUM, &state_482, NULL, KC_9, 0, single_dance};
uint8_t state_483 = IDLE;
const struct Chord chord_483 PROGMEM = {H_R4, NUM, &state_483, NULL, KC_0, 0, single_dance};
uint8_t state_484 = IDLE;
const struct Chord chord_484 PROGMEM = {H_L1 + H_L2, NUM, &state_484, NULL, KC_SPC, 0, single_dance};
uint8_t state_485 = IDLE;
const struct Chord chord_485 PROGMEM = {H_L2 + H_L3, NUM, &state_485, NULL, KC_GRAVE, 0, single_dance};
uint8_t state_486 = IDLE;
const struct Chord chord_486 PROGMEM = {H_L3 + H_L4, NUM, &state_486, NULL, KC_LBRACKET, 0, single_dance};
uint8_t state_487 = IDLE;
const struct Chord chord_487 PROGMEM = {H_L4 + H_R1, NUM, &state_487, NULL, KC_SPC, 0, single_dance};
uint8_t state_488 = IDLE;
const struct Chord chord_488 PROGMEM = {H_R1 + H_R2, NUM, &state_488, NULL, KC_EXCLAIM, 0, single_dance};
uint8_t state_489 = IDLE;
const struct Chord chord_489 PROGMEM = {H_R3 + H_R4, NUM, &state_489, NULL, KC_5, 0, single_dance};
uint8_t state_490 = IDLE;
const struct Chord chord_490 PROGMEM = {H_L1 + H_L3, NUM, &state_490, NULL, KC_HOME, 0, single_dance};
uint8_t state_491 = IDLE;
const struct Chord chord_491 PROGMEM = {H_L2 + H_L4, NUM, &state_491, NULL, KC_COMMA, 0, single_dance};
uint8_t state_492 = IDLE;
const struct Chord chord_492 PROGMEM = {H_L3 + H_R1, NUM, &state_492, NULL, KC_MINUS, 0, single_dance};
uint8_t state_493 = IDLE;
const struct Chord chord_493 PROGMEM = {H_L4 + H_R2, NUM, &state_493, NULL, KC_QUOTE, 0, single_dance};
uint8_t state_494 = IDLE;
const struct Chord chord_494 PROGMEM = {H_L2 + H_R1, NUM, &state_494, NULL, KC_BSPC, 0, single_dance};
uint8_t state_495 = IDLE;
const struct Chord chord_495 PROGMEM = {H_L3 + H_R2, NUM, &state_495, NULL, KC_6, 0, single_dance};
uint8_t state_496 = IDLE;
const struct Chord chord_496 PROGMEM = {H_R1 + H_R4, NUM, &state_496, NULL, KC_RBRACKET, 0, single_dance};
uint8_t state_497 = IDLE;
const struct Chord chord_497 PROGMEM = {H_L1 + H_R1, NUM, &state_497, NULL, KC_EQUAL, 0, single_dance};
uint8_t state_498 = IDLE;
const struct Chord chord_498 PROGMEM = {H_L3 + H_R3, NUM, &state_498, NULL, KC_SCOLON, 0, single_dance};
uint8_t state_499 = IDLE;
const struct Chord chord_499 PROGMEM = {H_R1 + H_R2 + H_R3 + H_R4, NUM, &state_499, NULL, KC_TAB, 0, single_dance};
uint8_t state_500 = IDLE;
const struct Chord chord_500 PROGMEM = {H_L1, COMMANDS, &state_500, NULL, KC_LCTL, 0, single_dance};
uint8_t state_501 = IDLE;
const struct Chord chord_501 PROGMEM = {H_L2, COMMANDS, &state_501, NULL, KC_LSFT, 0, single_dance};
uint8_t state_502 = IDLE;
const struct Chord chord_502 PROGMEM = {H_L3, COMMANDS, &state_502, NULL, KC_LALT, 0, single_dance};
uint8_t state_503 = IDLE;
const struct Chord chord_503 PROGMEM = {H_L4, COMMANDS, &state_503, NULL, KC_LGUI, 0, single_dance};
uint8_t state_504 = IDLE;
const struct Chord chord_504 PROGMEM = {H_R1, COMMANDS, &state_504, NULL, KC_LEFT, 0, single_dance};
uint8_t state_505 = IDLE;
const struct Chord chord_505 PROGMEM = {H_R2, COMMANDS, &state_505, NULL, KC_DOWN, 0, single_dance};
uint8_t state_506 = IDLE;
const struct Chord chord_506 PROGMEM = {H_R3, COMMANDS, &state_506, NULL, KC_UP, 0, single_dance};
uint8_t state_507 = IDLE;
const struct Chord chord_507 PROGMEM = {H_R4, COMMANDS, &state_507, NULL, KC_RIGHT, 0, single_dance};
uint8_t state_508 = IDLE;
const struct Chord chord_508 PROGMEM = {H_L1 + H_R4, COMMANDS, &state_508, NULL, KC_F1, 0, single_dance};
uint8_t state_509 = IDLE;
const struct Chord chord_509 PROGMEM = {H_L1 + H_R3, COMMANDS, &state_509, NULL, KC_F2, 0, single_dance};
uint8_t state_510 = IDLE;
const struct Chord chord_510 PROGMEM = {H_L1 + H_R3 + H_R4, COMMANDS, &state_510, NULL, KC_F3, 0, single_dance};
uint8_t state_511 = IDLE;
const struct Chord chord_511 PROGMEM = {H_L1 + H_R2, COMMANDS, &state_511, NULL, KC_F4, 0, single_dance};
uint8_t state_512 = IDLE;
const struct Chord chord_512 PROGMEM = {H_L1 + H_R2 + H_R4, COMMANDS, &state_512, NULL, KC_F5, 0, single_dance};
uint8_t state_513 = IDLE;
const struct Chord chord_513 PROGMEM = {H_L1 + H_R2 + H_R3, COMMANDS, &state_513, NULL, KC_F6, 0, single_dance};
uint8_t state_514 = IDLE;
const struct Chord chord_514 PROGMEM = {H_L1 + H_R2 + H_R3 + H_R4, COMMANDS, &state_514, NULL, KC_F7, 0, single_dance};
uint8_t state_515 = IDLE;
const struct Chord chord_515 PROGMEM = {H_L1 + H_R1, COMMANDS, &state_515, NULL, KC_F8, 0, single_dance};
uint8_t state_516 = IDLE;
const struct Chord chord_516 PROGMEM = {H_L1 + H_R1 + H_R4, COMMANDS, &state_516, NULL, KC_F9, 0, single_dance};
uint8_t state_517 = IDLE;
const struct Chord chord_517 PROGMEM = {H_L1 + H_R1 + H_R3, COMMANDS, &state_517, NULL, KC_F10, 0, single_dance};
uint8_t state_518 = IDLE;
const struct Chord chord_518 PROGMEM = {H_L1 + H_R1 + H_R3 + H_R4, COMMANDS, &state_518, NULL, KC_F11, 0, single_dance};
uint8_t state_519 = IDLE;
const struct Chord chord_519 PROGMEM = {H_L1 + H_R1 + H_R2, COMMANDS, &state_519, NULL, KC_F12, 0, single_dance};
uint8_t state_520 = IDLE;
const struct Chord chord_520 PROGMEM = {H_L2 + H_R1, COMMANDS, &state_520, NULL, KC_HOME, 0, single_dance};
uint8_t state_521 = IDLE;
const struct Chord chord_521 PROGMEM = {H_L2 + H_R2, COMMANDS, &state_521, NULL, KC_INS, 0, single_dance};
uint8_t state_522 = IDLE;
const struct Chord chord_522 PROGMEM = {H_L2 + H_R3, COMMANDS, &state_522, NULL, KC_DEL, 0, single_dance};
uint8_t state_523 = IDLE;
const struct Chord chord_523 PROGMEM = {H_L2 + H_R4, COMMANDS, &state_523, NULL, KC_END, 0, single_dance};
uint8_t state_524 = IDLE;
const struct Chord chord_524 PROGMEM = {H_LT, COMMANDS, &state_524, NULL, KC_PGDN, 0, single_dance};
uint8_t state_525 = IDLE;
const struct Chord chord_525 PROGMEM = {H_RT, COMMANDS, &state_525, NULL, KC_PGUP, 0, single_dance};
uint8_t state_526 = IDLE;
const struct Chord chord_526 PROGMEM = {H_L1, USER, &state_526, NULL, 442, 0, string_in};
uint8_t state_527 = IDLE;
const struct Chord chord_527 PROGMEM = {H_L2, USER, &state_527, NULL, 443, 0, string_in};
uint8_t state_528 = IDLE;
const struct Chord chord_528 PROGMEM = {H_L3, USER, &state_528, NULL, 444, 0, string_in};
uint8_t state_529 = IDLE;
const struct Chord chord_529 PROGMEM = {H_L4, USER, &state_529, NULL, 445, 0, string_in};
uint8_t state_530 = IDLE;
const struct Chord chord_530 PROGMEM = {H_R1, USER, &state_530, NULL, 446, 0, string_in};
uint8_t state_531 = IDLE;
const struct Chord chord_531 PROGMEM = {H_R2, USER, &state_531, NULL, 447, 0, string_in};
uint8_t state_532 = IDLE;
const struct Chord chord_532 PROGMEM = {H_R3, USER, &state_532, NULL, 448, 0, string_in};
uint8_t state_533 = IDLE;
const struct Chord chord_533 PROGMEM = {H_R4, USER, &state_533, NULL, 449, 0, string_in};

const struct Chord* const list_of_chords[] PROGMEM = {
    &chord_0, &chord_1, &chord_2, &chord_3, &chord_4, &chord_5, &chord_6, &chord_7, &chord_8, &chord_9, &chord_10, &chord_11, &chord_12, &chord_13, &chord_14, &chord_15, &chord_16, &chord_17, &chord_18, &chord_19, &chord_20, &chord_21, &chord_22, &chord_23, &chord_24, &chord_25, &chord_26, &chord_27, &chord_28, &chord_29, &chord_30, &chord_31, &chord_32, &chord_33, &chord_34, &chord_35, &chord_36, &chord_37, &chord_38, &chord_39, &chord_40, &chord_41, &chord_42, &chord_43, &chord_44, &chord_45, &chord_46, &chord_47, &chord_48, &chord_49, &chord_50, &chord_51, &chord_52, &chord_53, &chord_54, &chord_55, &chord_56, &chord_57, &chord_58, &chord_59, &chord_60, &chord_61, &chord_62, &chord_63, &chord_64, &chord_65, &chord_66, &chord_67, &chord_68, &chord_69, &chord_70, &chord_71, &chord_72, &chord_73, &chord_74, &chord_75, &chord_76, &chord_77, &chord_78, &chord_79, &chord_80, &chord_81, &chord_82, &chord_83, &chord_84, &chord_85, &chord_86, &chord_87, &chord_88, &chord_89, &chord_90, &chord_91, &chord_92, &chord_93, &chord_94, &chord_95, &chord_96, &chord_97, &chord_98, &chord_99, &chord_100, &chord_101, &chord_102, &chord_103, &chord_104, &chord_105, &chord_106, &chord_107, &chord_108, &chord_109, &chord_110, &chord_111, &chord_112, &chord_113, &chord_114, &chord_115, &chord_116, &chord_117, &chord_118, &chord_119, &chord_120, &chord_121, &chord_122, &chord_123, &chord_124, &chord_125, &chord_126, &chord_127, &chord_128, &chord_129, &chord_130, &chord_131, &chord_132, &chord_133, &chord_134, &chord_135, &chord_136, &chord_137, &chord_138, &chord_139, &chord_140, &chord_141, &chord_142, &chord_143, &chord_144, &chord_145, &chord_146, &chord_147, &chord_148, &chord_149, &chord_150, &chord_151, &chord_152, &chord_153, &chord_154, &chord_155, &chord_156, &chord_157, &chord_158, &chord_159, &chord_160, &chord_161, &chord_162, &chord_163, &chord_164, &chord_165, &chord_166, &chord_167, &chord_168, &chord_169, &chord_170, &chord_171, &chord_172, &chord_173, &chord_174, &chord_175, &chord_176, &chord_177, &chord_178, &chord_179, &chord_180, &chord_181, &chord_182, &chord_183, &chord_184, &chord_185, &chord_186, &chord_187, &chord_188, &chord_189, &chord_190, &chord_191, &chord_192, &chord_193, &chord_194, &chord_195, &chord_196, &chord_197, &chord_198, &chord_199, &chord_200, &chord_201, &chord_202, &chord_203, &chord_204, &chord_205, &chord_206, &chord_207, &chord_208, &chord_209, &chord_210, &chord_211, &chord_212, &chord_213, &chord_214, &chord_215, &chord_216, &chord_217, &chord_218, &chord_219, &chord_220, &chord_221, &chord_222, &chord_223, &chord_224, &chord_225, &chord_226, &chord_227, &chord_228, &chord_229, &chord_230, &chord_231, &chord_232, &chord_233, &chord_234, &chord_235, &chord_236, &chord_237, &chord_238, &chord_239, &chord_240, &chord_241, &chord_242, &chord_243, &chord_244, &chord_245, &chord_246, &chord_247, &chord_248, &chord_249, &chord_250, &chord_251, &chord_252, &chord_253, &chord_254, &chord_255, &chord_256, &chord_257, &chord_258, &chord_259, &chord_260, &chord_261, &chord_262, &chord_263, &chord_264, &chord_265, &chord_266, &chord_267, &chord_268, &chord_269, &chord_270, &chord_271, &chord_272, &chord_273, &chord_274, &chord_275, &chord_276, &chord_277, &chord_278, &chord_279, &chord_280, &chord_281, &chord_282, &chord_283, &chord_284, &chord_285, &chord_286, &chord_287, &chord_288, &chord_289, &chord_290, &chord_291, &chord_292, &chord_293, &chord_294, &chord_295, &chord_296, &chord_297, &chord_298, &chord_299, &chord_300, &chord_301, &chord_302, &chord_303, &chord_304, &chord_305, &chord_306, &chord_307, &chord_308, &chord_309, &chord_310, &chord_311, &chord_312, &chord_313, &chord_314, &chord_315, &chord_316, &chord_317, &chord_318, &chord_319, &chord_320, &chord_321, &chord_322, &chord_323, &chord_324, &chord_325, &chord_326, &chord_327, &chord_328, &chord_329, &chord_330, &chord_331, &chord_332, &chord_333, &chord_334, &chord_335, &chord_336, &chord_337, &chord_338, &chord_339, &chord_340, &chord_341, &chord_342, &chord_343, &chord_344, &chord_345, &chord_346, &chord_347, &chord_348, &chord_349, &chord_350, &chord_351, &chord_352, &chord_353, &chord_354, &chord_355, &chord_356, &chord_357, &chord_358, &chord_359, &chord_360, &chord_361, &chord_362, &chord_363, &chord_364, &chord_365, &chord_366, &chord_367, &chord_368, &chord_369, &chord_370, &chord_371, &chord_372, &chord_373, &chord_374, &chord_375, &chord_376, &chord_377, &chord_378, &chord_379, &chord_380, &chord_381, &chord_382, &chord_383, &chord_384, &chord_385, &chord_386, &chord_387, &chord_388, &chord_389, &chord_390, &chord_391, &chord_392, &chord_393, &chord_394, &chord_395, &chord_396, &chord_397, &chord_398, &chord_399, &chord_400, &chord_401, &chord_402, &chord_403, &chord_404, &chord_405, &chord_406, &chord_407, &chord_408, &chord_409, &chord_410, &chord_411, &chord_412, &chord_413, &chord_414, &chord_415, &chord_416, &chord_417, &chord_418, &chord_419, &chord_420, &chord_421, &chord_422, &chord_423, &chord_424, &chord_425, &chord_426, &chord_427, &chord_428, &chord_429, &chord_430, &chord_431, &chord_432, &chord_433, &chord_434, &chord_435, &chord_436, &chord_437, &chord_438, &chord_439, &chord_440, &chord_441, &chord_442, &chord_443, &chord_444, &chord_445, &chord_446, &chord_447, &chord_448, &chord_449, &chord_450, &chord_451, &chord_452, &chord_453, &chord_454, &chord_455, &chord_456, &chord_457, &chord_458, &chord_459, &chord_460, &chord_461, &chord_462, &chord_463, &chord_464, &chord_465, &chord_466, &chord_467, &chord_468, &chord_469, &chord_470, &chord_471, &chord_472, &chord_473, &chord_474, &chord_475, &chord_476, &chord_477, &chord_478, &chord_479, &chord_480, &chord_481, &chord_482, &chord_483, &chord_484, &chord_485, &chord_486, &chord_487, &chord_488, &chord_489, &chord_490, &chord_491, &chord_492, &chord_493, &chord_494, &chord_495, &chord_496, &chord_497, &chord_498, &chord_499, &chord_500, &chord_501, &chord_502, &chord_503, &chord_504, &chord_505, &chord_506, &chord_507, &chord_508, &chord_509, &chord_510, &chord_511, &chord_512, &chord_513, &chord_514, &chord_515, &chord_516, &chord_517, &chord_518, &chord_519, &chord_520, &chord_521, &chord_522, &chord_523, &chord_524, &chord_525, &chord_526, &chord_527, &chord_528, &chord_529, &chord_530, &chord_531, &chord_532, &chord_533
};

const uint16_t** const leader_triggers PROGMEM = NULL;
void (*leader_functions[]) (void) = {};

#define NUMBER_OF_CHORDS 534
#define NUMBER_OF_LEADER_COMBOS 0

bool are_hashed_keycodes_in_sound(HASH_TYPE keycodes_hash, HASH_TYPE sound) {
    return (keycodes_hash & sound) == keycodes_hash;
}

uint8_t keycode_to_index(uint16_t keycode) {
    return keycode - FIRST_INTERNAL_KEYCODE;
}

void sound_keycode_array(uint16_t keycode) {
    uint8_t index = keycode_to_index(keycode);
    keycode_index++;
    keycodes_buffer_array[index] = keycode_index;
}

void silence_keycode_hash_array(HASH_TYPE keycode_hash) {
    for (int i = 0; i < NUMBER_OF_KEYS; i++) {
        bool index_in_hash = ((HASH_TYPE) 1 << i) & keycode_hash;
        if (index_in_hash) {
            uint8_t current_val = keycodes_buffer_array[i];
            keycodes_buffer_array[i] = 0;
            for (int j = 0; j < NUMBER_OF_KEYS; j++) {
                if (keycodes_buffer_array[j] > current_val) {
                    keycodes_buffer_array[j]--;
                }
            }
            keycode_index--;
        }
    }
}

bool are_hashed_keycodes_in_array(HASH_TYPE keycode_hash) {
    for (int i = 0; i < NUMBER_OF_KEYS; i++) {
        bool index_in_hash = ((HASH_TYPE) 1 << i) & keycode_hash;
        bool index_in_array = (bool) keycodes_buffer_array[i];
        if (index_in_hash && !index_in_array) {
            return false;
        }
    }
    return true;
}

void kill_one_shots(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (*chord->state == IN_ONE_SHOT) {
            *chord->state = RESTART;
            chord->function(chord);
            if (*chord->state == RESTART) {
                *chord->state = IDLE;
            }
        }
    }
}

void process_finished_dances(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (*chord->state == ACTIVATED) {
            *chord->state = PRESS_FROM_ACTIVE;
            chord->function(chord);
            if (a_key_went_through) {
                kill_one_shots();
            }
            dance_timer = timer_read();
        } else if (*chord->state == IDLE_IN_DANCE) {
            *chord->state = FINISHED;
            chord->function(chord);
            if (*chord->state == FINISHED) {
                *chord->state = RESTART;
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
            }
        } else if (*chord->state == PRESS_FROM_ACTIVE) {
            *chord->state = FINISHED_FROM_ACTIVE;
            chord->function(chord);
            if (a_key_went_through) {
                kill_one_shots();
            }
            dance_timer = timer_read();
        }
    }
}

uint8_t keycodes_buffer_array_min(uint8_t* first_keycode_index) {
    for (int i = 0; i < NUMBER_OF_KEYS; i++) {
        if (keycodes_buffer_array[i] == 1) {
            if (first_keycode_index != NULL) {
                *first_keycode_index = (uint8_t) i;
            }
            return 1;
        }
    }
    return 0;
}

void remove_subchords(void) {
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        if (!(*chord->state == READY || *chord->state == READY_IN_DANCE || *chord->state == READY_LOCKED)) {
            continue;
        }
        
        struct Chord chord_storage_2;
        struct Chord* chord_ptr_2;
        struct Chord* chord_2;
        for (int j = 0; j < NUMBER_OF_CHORDS; j++) {
            if (i == j) {continue;}
            
            chord_ptr_2 = (struct Chord*) pgm_read_word (&list_of_chords[j]);
            memcpy_P(&chord_storage_2, chord_ptr_2, sizeof(struct Chord));
            chord_2 = &chord_storage_2;
            
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
}

void process_ready_chords(void) {
    uint8_t first_keycode_index = 0;
    while (keycodes_buffer_array_min(&first_keycode_index)) {
        // find ready chords
        struct Chord chord_storage;
        struct Chord* chord_ptr;
        struct Chord* chord;
        
        for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            chord = &chord_storage;
            
            // if the chord does not contain the first keycode
            bool contains_first_keycode = ((uint32_t) 1 << first_keycode_index) & chord->keycodes_hash;
            if (!contains_first_keycode) {
                continue;
            }
            
            if (!are_hashed_keycodes_in_array(chord->keycodes_hash)){
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
        remove_subchords();
        
        // execute logic
        // this should be only one chord
        for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            chord = &chord_storage;
            
            if (*chord->state == READY_LOCKED) {
                *chord->state = RESTART;
                chord->function(chord);
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
                break;
            }
            
            if (*chord->state == READY || *chord->state == READY_IN_DANCE) {
                if (last_chord && last_chord != chord) {
                    process_finished_dances();
                }
                
                bool lock_next_prev_state = lock_next;
                
                *chord->state = ACTIVATED;
                chord->function(chord);
                dance_timer = timer_read();
                
                if (lock_next && lock_next == lock_next_prev_state) {
                    lock_next = false;
                    *chord->state = PRESS_FROM_ACTIVE;
                    chord->function(chord);
                    if (*chord->state == PRESS_FROM_ACTIVE) {
                        *chord->state = LOCKED;
                    }
                    if (a_key_went_through) {
                        kill_one_shots();
                    }
                }
                break;
            }
        }
        
        // silence notes
        silence_keycode_hash_array(chord->keycodes_hash);
    }
}

void deactivate_active_chords(uint16_t keycode) {
    HASH_TYPE hash = (HASH_TYPE)1 << (keycode - SAFE_RANGE);
    bool broken;
    struct Chord chord_storage;
    struct Chord* chord_ptr;
    struct Chord* chord;
    
    for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
        chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
        memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
        chord = &chord_storage;
        
        broken = are_hashed_keycodes_in_sound(hash, chord->keycodes_hash);
        if (!broken) {
            continue;
        }
        
        switch (*chord->state) {
            case ACTIVATED:
                *chord->state = DEACTIVATED;
                chord->function(chord);
                
                if (*chord->state == DEACTIVATED) {
                    dance_timer = timer_read();
                    *chord->state = IDLE_IN_DANCE;
                }
                if (*chord->state != IN_ONE_SHOT) {
                    kill_one_shots();
                }
                break;
            case PRESS_FROM_ACTIVE:
            case FINISHED_FROM_ACTIVE:
                *chord->state = RESTART;
                chord->function(chord);
                if (*chord->state == RESTART) {
                    *chord->state = IDLE;
                }
                kill_one_shots();
                break;
            default:
                break;
        }
    }
    
}

void process_command(void) {
    command_mode = 0;
    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {
        if (command_buffer[i]) {
            register_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    wait_ms(TAP_TIMEOUT);
    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {
        if (command_buffer[i]) {
            unregister_code(command_buffer[i]);
        }
        send_keyboard_report();
    }
    for (int i = 0; i < COMMAND_MAX_LENGTH; i++) {
        command_buffer[i] = 0;
    }
    command_ind = 0;
}

void process_leader(void) {
    in_leader_mode = false;
    for (int i = 0; i < NUMBER_OF_LEADER_COMBOS; i++) {
        uint16_t trigger[LEADER_MAX_LENGTH];
        memcpy_P(trigger, leader_triggers[i], LEADER_MAX_LENGTH * sizeof(uint16_t));
        
        if (identical(leader_buffer, trigger)) {
            (*leader_functions[i])();
            break;
        }
    }
    for (int i = 0; i < LEADER_MAX_LENGTH; i++) {
        leader_buffer[i] = 0;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode < FIRST_INTERNAL_KEYCODE || keycode > LAST_INTERNAL_KEYCODE) {
        return true;
    }
    
    if (record->event.pressed) {
        sound_keycode_array(keycode);
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
    if (chord_timer_expired && keycodes_buffer_array_min(NULL)) {
        process_ready_chords();
    }
    
    bool dance_timer_expired = timer_elapsed(dance_timer) > DANCE_TIMEOUT;
    if (dance_timer_expired) { // would love to have && in_dance but not sure how
        process_finished_dances();
    }
    
    bool in_command_mode = command_mode == 2;
    if (in_command_mode) {
        process_command();
    }
    
    bool leader_timer_expired = timer_elapsed(leader_timer) > LEADER_TIMEOUT;
    if (leader_timer_expired && in_leader_mode) {
        process_leader();
    }
    
}

void clear(const struct Chord* self) {
    if (*self->state == ACTIVATED) {
        // kill all chords
        struct Chord chord_storage;
        struct Chord* chord_ptr;
        struct Chord* chord;
        
        for (int i = 0; i < NUMBER_OF_CHORDS; i++) {
            chord_ptr = (struct Chord*) pgm_read_word (&list_of_chords[i]);
            memcpy_P(&chord_storage, chord_ptr, sizeof(struct Chord));
            chord = &chord_storage;
            
            *chord->state = IDLE;
            
            if (chord->counter) {
                *chord->counter = 0;
            }
        }
        
        // clear keyboard
        clear_keyboard();
        send_keyboard_report();
        
        // switch to default pseudolayer
        current_pseudolayer = DEFAULT_PSEUDOLAYER;
        
        // clear all keyboard states
        lock_next = false;
        autoshift_mode = true;
        command_mode = 0;
        in_leader_mode = false;
        leader_ind = 0;
        dynamic_macro_mode = false;
        a_key_went_through = false;
        
        for (int i = 0; i < DYNAMIC_MACRO_MAX_LENGTH; i++) {
            dynamic_macro_buffer[i] = 0;
        }
    }
}