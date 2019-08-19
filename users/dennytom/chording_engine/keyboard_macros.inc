$py(type = "")
$py(NUM_OF_KEYS = 0)

$macro(internal_keycodes, keycodes_string)
    $nonlocal(type)
    $nonlocal(NUM_OF_KEYS)
    $py(keycodes = keycodes_string.split(','))
    $py(NUM_OF_KEYS = len(keycodes))
    enum internal_keycodes {
        $(keycodes[0].lstrip()) = SAFE_RANGE,
        $for(i in range(1,len(keycodes)))
            $(keycodes[i].lstrip()),
        $endfor

        FIRST_INTERNAL_KEYCODE = $(keycodes[0].lstrip()),
        LAST_INTERNAL_KEYCODE = $(keycodes[-1].lstrip())
    };

    const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT_georgi(
            $for(i in range(0, len(keycodes) - 1))
                $(keycodes[i].lstrip()),
            $endfor
            $(keycodes[-1].lstrip())
            )
    };

    // "Don't fuck with this, thanks." -- germ
    // Sorry, it has been fucked with.
    size_t keymapsCount = 1;

    $if(len(keycodes) <= 8)
        $py(type = "uint8_t")
    $elif(len(keycodes) <= 16)
        $py(type = "uint16_t")
    $elif(len(keycodes) <= 32)
        $py(type = "uint32_t")
    $elif(len(keycodes) <= 64)
        $py(type = "uint64_t")
    $else
        $py(raise Exception("The current implementation supports up to 64 keys."))
    $endif
    $for(i in range( 0, len(keycodes)))
        #define H_$(keycodes[i].lstrip()) (($(type)) 1 << $(i))
    $endfor
$endmacro
