#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 500
$py(COMMAND_MAX_LENGTH = 5)\
#define LEADER_TIMEOUT 500
$py(LEADER_MAX_LENGTH = 5)\
#define TAP_TIMEOUT 50
$py(DYNAMIC_MACRO_MAX_LENGTH = 20)\
$py(layout_name = "LAYOUT_butter")

$include(engine_path + "keyboard_macros.inc")

$internal_keycodes("TOP1, TOP2, TOP3, TOP4, TOP5, TOP6, TOP7, TOP8, TOP9, TOP0, BOT1, BOT2, BOT3, BOT4, BOT5, BOT6, BOT7, BOT8, BOT9, BOT0")

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