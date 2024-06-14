#include QMK_KEYBOARD_H
#ifdef AUDIO_ENABLE
#include "muse.h"
#endif
#include "eeprom.h"
#include "os_detection.h"

enum planck_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
  ST_MACRO_0,
  ST_MACRO_1,
  ST_MACRO_2,
  ST_MACRO_3,
  ST_MACRO_4,
  ST_MACRO_5,
  ST_MACRO_6,
  ST_MACRO_7,
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
    KC_TRANSPARENT, KC_A,           KC_S,           KC_D,           KC_F,           KC_G,           KC_H,           KC_J,           KC_K,           KC_L,           ST_MACRO_0,     KC_DELETE,      
    KC_LEFT_SHIFT,  KC_Y,           KC_X,           KC_C,           KC_V,           KC_B,           KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_TRANSPARENT, KC_RIGHT_SHIFT, 
    KC_LEFT_CTRL,   KC_LEFT_ALT,    KC_LEFT_GUI,    MO(4),          MO(1),          KC_SPACE,       KC_NO,          MO(2),          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_ENTER
  ),

  [_LOWER] = LAYOUT_planck_grid(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_AT,          KC_HASH,        KC_PERC,        KC_AMPR,        KC_PIPE,        KC_EQUAL,       KC_DQUO,        KC_QUOTE,       KC_GRAVE,       KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_CIRC,        KC_EXLM,        KC_QUES,        KC_PLUS,        KC_MINUS,       KC_LPRN,        KC_RPRN,        KC_LABK,        KC_RABK,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TILD,        KC_BSLS,        KC_UNDS,        KC_DLR,         KC_ASTR,        KC_SLASH,       KC_LCBR,        KC_RCBR,        KC_LBRC,        KC_RBRC,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_SPACE,       KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),

  [_RAISE] = LAYOUT_planck_grid(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ST_MACRO_1,     KC_UP,          ST_MACRO_2,     KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, ST_MACRO_3,     KC_LEFT_SHIFT,  ST_MACRO_4,     KC_TRANSPARENT, KC_TRANSPARENT, KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_TRANSPARENT, ST_MACRO_5,     
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, LSFT(KC_TAB),   KC_TRANSPARENT, KC_TRANSPARENT, KC_TAB,         KC_AUDIO_VOL_UP,KC_BRIGHTNESS_UP,KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ST_MACRO_6,     ST_MACRO_7,     KC_NO,          KC_TRANSPARENT, KC_AUDIO_VOL_DOWN,KC_BRIGHTNESS_DOWN,KC_TRANSPARENT, KC_TRANSPARENT
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
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),

};

bool left_shift_pressed = false;
bool right_shift_pressed = false;
bool umlauts = false;
bool tab_switch = false;

bool is_apple(void) {
    os_variant_t host_os = detected_host_os();
    return host_os == OS_MACOS || host_os == OS_IOS;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case ST_MACRO_0: // UMLAUTS
    if (is_apple()) {
        if (record->event.pressed) {
            register_code16(LALT(KC_U));
        } else {
            unregister_code16(LALT(KC_U));
        }
    } else {
        umlauts = true;
    }
    return false;

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
    return false;

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
    return false;

    case ST_MACRO_3: // SWITCH_TAB
    if (record->event.pressed) {
        tab_switch = true;
    } else {
        tab_switch = false;
    }
    break;

    case ST_MACRO_4: // JUMP_WORD
    ;
    uint8_t code1 = 0;
    if (is_apple()) {
        code1 = KC_LEFT_ALT;
    } else {
        code1 = KC_LEFT_CTRL;
    }
    if (record->event.pressed) {
        register_code(code1);
    } else {
        unregister_code(code1);
    }
    break;

    case ST_MACRO_5: // CLOSE_APP
    ;
    uint16_t code = 0;
    if (is_apple()) {
        code = LGUI(KC_Q);
    } else {
        code = LALT(KC_F4);
    }
    if (record->event.pressed) {
        register_code16(code);
    } else {
        unregister_code16(code);
    }
    break;

    case ST_MACRO_6: // SWITCH_APP
    ;
    uint8_t code2 = 0;
    if (is_apple()) {
        code2 = KC_LEFT_GUI;
    } else {
        code2 = KC_LEFT_ALT;
    }
    if (record->event.pressed) {
        register_code(code2);
    } else {
        unregister_code(code2);
    }
    break;

    case ST_MACRO_7: // SNAPP_APP
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

    case KC_LEFT:
    case KC_RIGHT:
    if (tab_switch) {
        uint16_t code = 0;
        if (keycode == KC_LEFT) {
            code = LCTL(KC_PAGE_UP);
        } else {
            code = LCTL(KC_PAGE_DOWN);
        }
        if (record->event.pressed) {
            register_code16(code);
        } else {
            unregister_code16(code);
        }
    } else if (record->event.pressed) {
        register_code(keycode);
    } else {
        unregister_code(keycode);
    }
    return false;

    case KC_LEFT_SHIFT:
    if (record->event.pressed) {
        left_shift_pressed = true;
        register_code(KC_LEFT_SHIFT);
    } else {
        left_shift_pressed = false;
        unregister_code(KC_LEFT_SHIFT);
    }
    break;

    case KC_RIGHT_SHIFT:
    if (record->event.pressed) {
        right_shift_pressed = true;
        register_code(KC_RIGHT_SHIFT);
    } else {
        right_shift_pressed = false;
        unregister_code(KC_RIGHT_SHIFT);
    }
    break;

    case KC_COMMA:
    if (record->event.pressed) {
        if (left_shift_pressed || right_shift_pressed) {
            unregister_code(KC_LEFT_SHIFT);
            unregister_code(KC_RIGHT_SHIFT);
            unregister_code(KC_COMMA);
            register_code(KC_SCLN);
        } else {
            register_code(KC_COMMA);
        }
    } else {
        if (left_shift_pressed) {
            register_code(KC_LEFT_SHIFT);
        }
        if (right_shift_pressed) {
            register_code(KC_RIGHT_SHIFT);
        }
        unregister_code(KC_COMMA);
        unregister_code(KC_SCLN);
    }
    return false;

    case KC_DOT:
    if (record->event.pressed) {
        if (left_shift_pressed || right_shift_pressed) {
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
    uint8_t code3 = 0;
    if (is_apple()) {
        code3 = KC_LEFT_GUI;
    } else {
        code3 = KC_LEFT_CTRL;
    }
    if (record->event.pressed) {
        register_code(code3);
    } else {
        unregister_code(code3);
    }
    return false;

    case KC_LEFT_GUI:
    ;
    uint8_t code4 = 0;
    if (is_apple()) {
        code4 = KC_LEFT_CTRL;
    } else {
        code4 = KC_LEFT_GUI;
    }
    if (record->event.pressed) {
        register_code(code4);
    } else {
        unregister_code(code4);
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
        if (left_shift_pressed || right_shift_pressed) {
            unregister_code(KC_LEFT_SHIFT);
            unregister_code(KC_RIGHT_SHIFT);
            switch (keycode) {
                case KC_A: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_9) SS_TAP(X_KP_6) )); // Ä
                break;
                case KC_O: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_1) SS_TAP(X_KP_4) )); // Ö
                break;
                case KC_U: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_0) )); // Ü
                break;
            }
        } else {
            switch (keycode) {
                case KC_A: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_8) )); // ä
                break;
                case KC_O: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_6) )); // ö
                break;
                case KC_U: SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_5) SS_TAP(X_KP_2) )); // ü
                break;
            }
        }
    } else {
        if (left_shift_pressed) {
            register_code(KC_LEFT_SHIFT);
        }
        if (right_shift_pressed) {
            register_code(KC_RIGHT_SHIFT);
        }
        umlauts = false;
    }
    return false;

    case RGB_SLD:
        if (rawhid_state.rgb_control) {
            return false;
        }
        if (record->event.pressed) {
            rgblight_mode(1);
        }
        return false;
  }
  return true;
}

#ifdef AUDIO_ENABLE
bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update(bool clockwise) {
    if (muse_mode) {
        if (IS_LAYER_ON(_RAISE)) {
            if (clockwise) {
                muse_offset++;
            } else {
                muse_offset--;
            }
        } else {
            if (clockwise) {
                muse_tempo+=1;
            } else {
                muse_tempo-=1;
            }
        }
    } else {
        if (clockwise) {
        #ifdef MOUSEKEY_ENABLE
            register_code(KC_MS_WH_DOWN);
            unregister_code(KC_MS_WH_DOWN);
        #else
            register_code(KC_PGDN);
            unregister_code(KC_PGDN);
        #endif
        } else {
        #ifdef MOUSEKEY_ENABLE
            register_code(KC_MS_WH_UP);
            unregister_code(KC_MS_WH_UP);
        #else
            register_code(KC_PGUP);
            unregister_code(KC_PGUP);
        #endif
        }
    }
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
    case RAISE:
    case LOWER:
        return false;
    default:
        return true;
    }
}
#endif

uint8_t layer_state_set_user(uint8_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
