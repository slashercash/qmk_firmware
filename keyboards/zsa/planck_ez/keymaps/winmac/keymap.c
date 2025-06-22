#include QMK_KEYBOARD_H
#ifdef AUDIO_ENABLE
#include "muse.h"
#endif
#include "eeprom.h"
#include "os_detection.h"

enum planck_keycodes {
  RGB_SLD = SAFE_RANGE,
  ST_MACRO_0,
  ST_MACRO_1,
  ST_MACRO_2,
  ST_MACRO_3,
  ST_MACRO_4,
  ST_MACRO_5,
  ST_MACRO_6,
  ST_MACRO_7,
  ST_MACRO_8,
  ST_MACRO_9,
};


enum planck_layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _LAYER4,
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
    KC_ESCAPE,      KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_Z,           KC_U,           KC_I,           KC_O,           KC_P,           KC_BSPC,        
    KC_TAB,         KC_A,           KC_S,           KC_D,           KC_F,           KC_G,           KC_H,           KC_J,           KC_K,           KC_L,           ST_MACRO_0,     KC_DELETE,      
    KC_LEFT_SHIFT,  KC_Y,           KC_X,           KC_C,           KC_V,           KC_B,           KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_PSCR,        KC_RIGHT_SHIFT, 
    KC_LEFT_CTRL,   KC_LEFT_ALT,    KC_LEFT_GUI,    MO(4),          MO(1),          KC_SPACE,       KC_NO,          MO(2),          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_ENTER
  ),

  [_LOWER] = LAYOUT_planck_grid(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_AT,          KC_HASH,        KC_PERC,        KC_AMPR,        KC_PIPE,        KC_EQUAL,       KC_DQUO,        KC_QUOTE,       KC_GRAVE,       KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TILD,        KC_CIRC,        KC_EXLM,        KC_QUES,        KC_PLUS,        KC_MINUS,       KC_LPRN,        KC_RPRN,        KC_LABK,        KC_RABK,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_BSLS,        KC_UNDS,        KC_DLR,         KC_ASTR,        KC_SLASH,       KC_LCBR,        KC_RCBR,        KC_LBRC,        KC_RBRC,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),

  [_RAISE] = LAYOUT_planck_grid(
    KC_TRANSPARENT, KC_TRANSPARENT,    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ST_MACRO_1,     KC_UP,             ST_MACRO_2,         KC_TRANSPARENT, ST_MACRO_3,     
    KC_TRANSPARENT, LALT(KC_LEFT_GUI), ST_MACRO_4,     ST_MACRO_5,     KC_LEFT_SHIFT,  KC_TRANSPARENT, KC_TRANSPARENT, KC_LEFT,        KC_DOWN,           KC_RIGHT,           ST_MACRO_6,     ST_MACRO_7,     
    KC_TRANSPARENT, KC_TRANSPARENT,    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ST_MACRO_8,     KC_AUDIO_VOL_UP,   KC_BRIGHTNESS_UP,   LALT(KC_TAB),   KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT,    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ST_MACRO_9,     KC_NO,          KC_TRANSPARENT, KC_AUDIO_VOL_DOWN, KC_BRIGHTNESS_DOWN, KC_TRANSPARENT, KC_TRANSPARENT
  ),

  [_ADJUST] = LAYOUT_planck_grid(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, RGB_HUD,        LED_LEVEL,      KC_TRANSPARENT, RGB_HUI,        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, RGB_MODE_FORWARD,KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),

  [_LAYER4] = LAYOUT_planck_grid(
    KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_F5,          KC_F6,          KC_F7,          KC_F8,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           
    KC_F9,          KC_F10,         KC_F11,         KC_F12,         KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_LEFT_SHIFT,  KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),

};

bool left_shift = false;
bool right_shift = false;
bool umlauts = false;
bool tab_switch = false;

bool is_apple(void) {
    os_variant_t host_os = detected_host_os();
    return host_os == OS_MACOS || host_os == OS_IOS;
}

uint8_t code_by_os(uint8_t apple, uint8_t other) {
    if (is_apple()) {
        return apple;
    }
    return other;
}

// return true;  -> Let QMK send the enter press/release events
// return false; -> Skip all further processing of this key
// break;        -> Breaks out of the switch statement, therefore returns true
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (umlauts && keycode != KC_A && keycode != KC_O && keycode != KC_U && keycode != KC_LEFT_SHIFT && keycode != KC_RIGHT_SHIFT) {
    umlauts = false;
  }

  switch (keycode) {
    case ST_MACRO_0: // UMLAUTS
    if (is_apple()) {
        if (record->event.pressed) {
            register_code16(LALT(KC_U));
            unregister_code16(LALT(KC_U));
        } else {
            unregister_code16(LALT(KC_U));
        }
    } else {
        umlauts = true;
    }
    break;

    case ST_MACRO_1: // JUMP_SOL
    if (is_apple()) {
        if (record->event.pressed) {
            register_code16(LGUI(KC_LEFT));
        } else {
            unregister_code16(LGUI(KC_LEFT));
        }
    } else {
        if (record->event.pressed) {
            register_code(KC_HOME);
        } else {
            unregister_code(KC_HOME);
        }
    }
    break;

    case ST_MACRO_2: // JUMP_EOL
    if (is_apple()) {
        if (record->event.pressed) {
            register_code16(LGUI(KC_RIGHT));
        } else {
            unregister_code16(LGUI(KC_RIGHT));
        }
    } else {
        if (record->event.pressed) {
            register_code(KC_END);
        } else {
            unregister_code(KC_END);
        }
    }
    break;

    case ST_MACRO_3: // LOCK
    if (is_apple()) {
        if (record->event.pressed) {
            register_code16(LCTL(LGUI(KC_Q)));
        } else {
            unregister_code16(LCTL(LGUI(KC_Q)));
        }
    } else {
        if (record->event.pressed) {
            register_code16(LGUI(KC_L));
        } else {
            unregister_code16(LGUI(KC_L));
        }
    }
    break;

    case ST_MACRO_4: // SWITCH_TAB
    if (record->event.pressed) {
        tab_switch = true;
    } else {
        unregister_code16(LCTL(KC_PAGE_UP));
        unregister_code16(LCTL(KC_PAGE_DOWN));
        tab_switch = false;
    }
    break;

    case ST_MACRO_5: // JUMP_WORD
    ;
    uint8_t KC_JUMP_WORD = code_by_os(KC_LEFT_ALT, KC_LEFT_CTRL);
    if (record->event.pressed) {
        register_code(KC_JUMP_WORD);
    } else {
        unregister_code(KC_JUMP_WORD);
    }
    break;

    case ST_MACRO_6: // EXPOSE_APP
    if (is_apple()) {
        if (record->event.pressed) {
            register_code16(LALT(KC_DOWN));
        } else {
            unregister_code16(LALT(KC_DOWN));
        }
    } else {
        if (record->event.pressed) {
            register_code16(LGUI(KC_TAB));
        } else {
            unregister_code16(LGUI(KC_TAB));
        }
    }
    break;

    case ST_MACRO_7: // CLOSE_APP
    ;
    uint16_t KC_CLOSE_APP = 0;
    if (is_apple()) {
        KC_CLOSE_APP = LGUI(KC_Q);
    } else {
        KC_CLOSE_APP = LALT(KC_F4);
    }
    if (record->event.pressed) {
        register_code16(KC_CLOSE_APP);
    } else {
        unregister_code16(KC_CLOSE_APP);
    }
    break;

    case ST_MACRO_8: // SWITCH_APP
    ;
    uint8_t KC_SWITCH_APP = code_by_os(KC_LEFT_GUI, KC_LEFT_ALT);
    if (record->event.pressed) {
        register_code(KC_SWITCH_APP);
        register_code(KC_TAB);
    } else {
        unregister_code(KC_TAB);
    }
    break;

    case ST_MACRO_9: // SNAPP_APP
    if (is_apple()) {
        if (record->event.pressed) {
            register_code(KC_LEFT_CTRL);
            register_code(KC_LEFT_ALT);
        } else {
            unregister_code(KC_LEFT_CTRL);
            unregister_code(KC_LEFT_ALT);
        }
    } else {
        if (record->event.pressed) {
            register_code(KC_LEFT_GUI);
        } else {
            unregister_code(KC_LEFT_GUI);
        }
    }
    break;

    case MO(2):
    if (!record->event.pressed) {
        unregister_code(code_by_os(KC_LEFT_GUI, KC_LEFT_ALT));
    }
    break;

    case KC_LEFT:
    case KC_RIGHT:
    if (tab_switch) {
        uint16_t KC_TAB_SWITCH = 0;
        if (keycode == KC_LEFT) {
            KC_TAB_SWITCH = LCTL(KC_PAGE_UP);
        } else {
            KC_TAB_SWITCH = LCTL(KC_PAGE_DOWN);
        }
        if (record->event.pressed) {
            register_code16(KC_TAB_SWITCH);
        } else {
            unregister_code16(KC_TAB_SWITCH);
        }
        break;
    } else if (record->event.pressed) {
        register_code(keycode);
    } else {
        unregister_code(keycode);
    }
    return false;

    case KC_LEFT_SHIFT:
    if (record->event.pressed) {
        left_shift = true;
        register_code(KC_LEFT_SHIFT);
    } else {
        left_shift = false;
        unregister_code(KC_LEFT_SHIFT);
    }
    break;

    case KC_RIGHT_SHIFT:
    if (record->event.pressed) {
        right_shift = true;
        register_code(KC_RIGHT_SHIFT);
    } else {
        right_shift = false;
        unregister_code(KC_RIGHT_SHIFT);
    }
    break;

    case KC_COMMA:
    if (record->event.pressed) {
        if (left_shift || right_shift) {
            unregister_code(KC_LEFT_SHIFT);
            unregister_code(KC_RIGHT_SHIFT);
            unregister_code(KC_COMMA);
            register_code(KC_SCLN);
        } else {
            register_code(KC_COMMA);
        }
    } else {
        if (left_shift) {
            register_code(KC_LEFT_SHIFT);
        }
        if (right_shift) {
            register_code(KC_RIGHT_SHIFT);
        }
        unregister_code(KC_COMMA);
        unregister_code(KC_SCLN);
    }
    return false;

    case KC_DOT:
    if (record->event.pressed) {
        if (left_shift || right_shift) {
            register_code(KC_SCLN);
        } else {
            register_code(KC_DOT);
        }
    } else {
        unregister_code(KC_SCLN);
        unregister_code(KC_DOT);
    }
    return false;

    case KC_LEFT_CTRL:
    ;
    uint8_t KC_LEFT_CTRL_BY_OS = code_by_os(KC_LEFT_GUI, KC_LEFT_CTRL);
    if (record->event.pressed) {
        register_code(KC_LEFT_CTRL_BY_OS);
    } else {
        unregister_code(KC_LEFT_CTRL_BY_OS);
    }
    return false;

    case KC_LEFT_GUI:
    ;
    uint8_t KC_LEFT_GUI_BY_OS = code_by_os(KC_LEFT_CTRL, KC_LEFT_GUI);
    if (record->event.pressed) {
        register_code(KC_LEFT_GUI_BY_OS);
    } else {
        unregister_code(KC_LEFT_GUI_BY_OS);
    }
    return false;

    case KC_A:
    case KC_O:
    case KC_U:
    if (!umlauts || is_apple()) {
        if (record->event.pressed) {
            register_code(keycode);
        } else {
            unregister_code(keycode);
        }
    } else if (record->event.pressed) {
        if (left_shift || right_shift) {
            unregister_code(KC_LEFT_SHIFT);
            unregister_code(KC_RIGHT_SHIFT);
            switch (keycode) {
                case KC_A: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_9) SS_TAP(X_KP_6) )); break; // Ä
                case KC_O: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_1) SS_TAP(X_KP_4) )); break; // Ö
                case KC_U: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_0) )); break; // Ü
            }
        } else {
            switch (keycode) {
                case KC_A: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_8) )); break; // ä
                case KC_O: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_6) )); break; // ö
                case KC_U: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_5) SS_TAP(X_KP_2) )); break; // ü
            }
        }
    } else {
        if (left_shift) {
            register_code(KC_LEFT_SHIFT);
        }
        if (right_shift) {
            register_code(KC_RIGHT_SHIFT);
        }
        umlauts = false;
    }
    return false;

//    case RGB_SLD:
//        if (layer_state.rgb_control) {
//            return false;
//        }
//        if (record->event.pressed) {
//            rgblight_mode(1);
//        }
//        return false;
  }
  return true;
}

uint8_t layer_state_set_user(uint8_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
