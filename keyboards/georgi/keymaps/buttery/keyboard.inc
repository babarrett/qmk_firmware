#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 400
$py(COMMAND_MAX_LENGTH = 5)
#define LEADER_TIMEOUT 500
$py(LEADER_MAX_LENGTH = 5)
#define TAP_TIMEOUT 50
$py(DYNAMIC_MACRO_MAX_LENGTH = 20)

$include(engine_path + "keyboard_macros.inc")

$internal_keycodes("TOP1, TOP2, TOP3, TOP4, TOP5, TOP6, TOP7, TOP8, TOP9, TOP10, TOP11, TOP12, BOT1, BOT2, BOT3, BOT4, BOT5, BOT6, BOT7, BOT8, BOT9, BOT10, BOT11, BOT12, THU1, THU2, THU3, THU4, THU5, THU6")

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
