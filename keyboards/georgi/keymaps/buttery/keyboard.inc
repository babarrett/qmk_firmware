#define CHORD_TIMEOUT 100
#define DANCE_TIMEOUT 250
$py(COMMAND_MAX_LENGTH = 5)
#define LEADER_TIMEOUT 500
$py(LEADER_MAX_LENGTH = 5)
#define TAP_TIMEOUT 50
$py(DYNAMIC_MACRO_MAX_LENGTH = 20)

uint8_t current_pseudolayer = 1;
bool lock_next = false;
uint32_t keycodes_buffer = 0;
uint16_t chord_timer = 0;
uint16_t dance_timer = 0;

uint8_t command_mode = 0;
uint16_t command_buffer[] = {
    $for(i in range(0, COMMAND_MAX_LENGTH))
        0,
    $endfor
    };
uint8_t command_ind = 0;

bool in_leader_mode = false;
uint16_t leader_buffer[] = {
    $for(i in range(0, LEADER_MAX_LENGTH))
        0,
    $endfor
    };
uint8_t leader_ind = 0;
uint16_t leader_timer = 0;

bool dynamic_macro_mode = false;
uint16_t dynamic_macro_buffer[] = {
    $for(i in range(0, DYNAMIC_MACRO_MAX_LENGTH))
        0,
    $endfor
    };
uint8_t dynamic_macro_ind;

bool a_key_went_through = false;
