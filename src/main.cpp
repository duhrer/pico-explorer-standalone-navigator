#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico_explorer.hpp"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

#include "drivers/button/button.hpp"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"

using namespace pimoroni;

ST7789 st7789(PicoExplorer::WIDTH, PicoExplorer::HEIGHT, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));

// 8-bit colour
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

const Pen WHITE = graphics.create_pen(255, 255, 255);
const Pen BLACK = graphics.create_pen(0, 0, 0);
const Pen RED   = graphics.create_pen(255, 0, 0);
const Pen GREEN = graphics.create_pen(0, 255, 0);

const int CX = round(st7789.width / 2);
const int CY = round(st7789.height / 2);

const int HEARTBEAT_RADIUS = 15;
const int HEARTBEAT_LINE_WIDTH = 3;
const int HEARTBEAT_MARGIN = 5;
const int HEARTBEAT_X = st7789.width - (HEARTBEAT_MARGIN + HEARTBEAT_RADIUS);
const int HEARTBEAT_Y = st7789.height - (HEARTBEAT_MARGIN + HEARTBEAT_RADIUS);

static Point HEARTBEAT_LOCATION = Point(HEARTBEAT_X, HEARTBEAT_Y);

// The defaults are fine for these.
Button button_a(PicoExplorer::A);
Button button_b(PicoExplorer::B);

// Disable repeat.
Button button_x(PicoExplorer::X, Polarity::ACTIVE_LOW, 0);
Button button_y(PicoExplorer::Y, Polarity::ACTIVE_LOW, 0);

bool keep_running = true;

void draw_text(std::string to_draw, Pen pen=BLACK) {
    graphics.set_font("bitmap8");
    int text_width = graphics.measure_text(to_draw, 4, 1, true);

    int text_x = CX - round(text_width / 2);
    int text_y = CY - 16;

    Point textLocation = Point(text_x, text_y);
    Rect text_box(text_x, text_y, text_width, 32);

    graphics.set_pen(pen);
    graphics.text(to_draw, textLocation, text_box.w, 4.0, 0.0, 1.0, true);
}

void drawStatusCircle(Point location, int radius, int margin, int lineWidth, Pen fillColor) {
    graphics.set_pen(BLACK);
    graphics.circle(location, radius);
    graphics.set_pen(fillColor);
    graphics.circle(location, radius - lineWidth);
}

int hid_pass = 0;

void hid_task() {
    if ( !tud_hid_ready() ) return;

    // TODO: Somehow the buttons are constantly repeating even the ones we set not to.
    bool button_x_state = button_x.read();
    bool button_y_state = button_y.read();
    bool button_a_state = button_a.read();
    bool button_b_state = button_b.read();

    bool any_button_pressed = button_a_state || button_b_state || button_x_state || button_y_state;

    bool all_buttons_pressed = button_a_state && button_b_state && button_x_state && button_y_state;

    if ( tud_suspended() && any_button_pressed )
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }

    // Clear the screen every 40 passes, which isn't exactly every second, but is perceivable.
    if (hid_pass % 40 == 0) {
        graphics.set_pen(WHITE);
        graphics.clear();
    }
    // Draw a "heartbeat" dot in between screen refreshes.
    else if (hid_pass % 20 == 0) {
        drawStatusCircle(HEARTBEAT_LOCATION, HEARTBEAT_RADIUS, HEARTBEAT_MARGIN, HEARTBEAT_LINE_WIDTH, GREEN);
    }

    int modifier = 0;

    uint8_t keyCodes[6]= {0};

    if (all_buttons_pressed) {
        keep_running = false;
    }
    else if (any_button_pressed) {
        graphics.set_pen(WHITE);
        graphics.clear();

        // Only one of tab/shift+tab should be sent.
        if(button_a_state) {
            keyCodes[0] = HID_KEY_TAB;
            modifier = KEYBOARD_MODIFIER_LEFTSHIFT;

            draw_text("Shift+Tab");
        }
        else if(button_b_state) {
            keyCodes[0] = HID_KEY_TAB;
            draw_text("Tab");
        }
        else if(button_x_state) {
            keyCodes[0] = HID_KEY_ESCAPE;
            draw_text("Escape");
        }
        else if(button_y_state) {
            keyCodes[0] = HID_KEY_SPACE;
            draw_text("Space");
        }
    }

    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keyCodes);

    st7789.update(&graphics);

    hid_pass++;
}

// Invoked when sent REPORT successfully to host.  If you're sending more than six characters, you can use this
// to know when it's safe to send the next report.
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void) instance;
    (void) len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
}

int main () {
    board_init();
    tusb_init();

    st7789.set_backlight(75);
    graphics.set_pen(WHITE);
    graphics.clear();
    st7789.update(&graphics);

    while (keep_running) {
        // Device tasks.
        tud_task();

        // Our task that polls buttons and sends keys in response.
        hid_task();

        // The pico-examples project for HID calls `tud_task` as often as possible, but waits to read button
        // inputs every 10ms.  In informal testing, delaying here for 50ms results in the device not
        // showing up at the OS level, but using 10ms seems to work fine.
        sleep_ms(10);
    }
}