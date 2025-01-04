#include <genQrCode.h>
#include <Arduino.h>

// Biến toàn cục
static lv_obj_t *qr;
static lv_obj_t *textarea;
static lv_obj_t *keyboard;  // Bàn phím
static lv_obj_t *label;
static lv_obj_t *btn_label;
bool accessQr = false;

// Callback để cập nhật QR code
void update_qr_code(lv_event_t *e) {
    if(accessQr == false){
        accessQr = true;
        lv_label_set_text(btn_label, "2499 QR: true");
    }else{
        accessQr = false;
        lv_qrcode_update(qr, "", 0);
        lv_label_set_text(btn_label, "2499 QR: false");        
    }
}

// Callback để cập nhật QR code
void update_qr_code_MQTT(const char *text) {
    if(accessQr == true){
        if (strlen(text) > 0) {
            lv_qrcode_update(qr, text, strlen(text));
        }
    }
}

// Callback để hiển thị bàn phím khi nhấn vào textarea
void text_input_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);

    if (code == LV_EVENT_FOCUSED) {
        // Hiển thị bàn phím và kết nối với textarea
        lv_keyboard_set_textarea(keyboard, ta);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    } else if (code == LV_EVENT_DEFOCUSED) {
        // Ẩn bàn phím khi textarea mất focus
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

void updateNoticeLable(const char *textLable){
    lv_label_set_text(label, textLable); 
}

// Tạo màn hình giao diện QR code
void create_qr_code_screen(void) {
    // Tạo tabview với một tab
    lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 50);
    lv_obj_t *tab = lv_tabview_add_tab(tabview, "QR Generator");

    // Tạo một container chia bố cục 2 nửa
    lv_obj_t *container = lv_obj_create(tab);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW); // Chia theo hàng ngang
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_all(container, 10, 0);

    lv_obj_set_style_align(container, LV_ALIGN_CENTER, 0); // Căn giữa container
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Panel bên trái (ô nhập và nút)
    lv_obj_t *left_panel = lv_obj_create(container);
    lv_obj_set_flex_flow(left_panel, LV_FLEX_FLOW_COLUMN); // Bố trí theo cột
    lv_obj_set_size(left_panel, lv_pct(45), lv_pct(100));
    lv_obj_set_style_pad_all(left_panel, 10, 0);
    lv_obj_set_flex_align(left_panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // Căn giữa nội dung

    // Ô hiển thị text (label)
    label = lv_label_create(left_panel);
    lv_label_set_text(label, "thanhpv2499"); 
    lv_obj_set_width(label, lv_pct(100));    
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP); 
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); 

    // // Ô nhập text (textarea)
    // textarea = lv_textarea_create(left_panel);
    // lv_textarea_set_one_line(textarea, true);
    // lv_textarea_set_placeholder_text(textarea, "Enter text for QR code");
    // lv_obj_set_width(textarea, lv_pct(100));
    // lv_obj_add_event_cb(textarea, text_input_event_cb, LV_EVENT_ALL, NULL);

    // Nút để cập nhật QR code
    lv_obj_t *btn = lv_btn_create(left_panel);
    lv_obj_set_width(btn, lv_pct(100));
    btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "2499 QR: false");
    lv_obj_center(btn_label);

    // Gắn sự kiện cho nút
    lv_obj_add_event_cb(btn, update_qr_code, LV_EVENT_CLICKED, NULL);

    // Panel bên phải (QR code)
    lv_obj_t *right_panel = lv_obj_create(container);
    lv_obj_set_size(right_panel, lv_pct(45), lv_pct(100));
    lv_obj_set_flex_flow(right_panel, LV_FLEX_FLOW_COLUMN); // Bố trí theo cột
    lv_obj_set_flex_align(right_panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // Căn giữa nội dung


    // QR code
    lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 5);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_BLUE, 4);
    qr = lv_qrcode_create(right_panel, 130, fg_color, bg_color);
    lv_obj_set_style_border_color(qr, bg_color, 0);
    lv_obj_set_style_border_width(qr, 2, 0);
    lv_obj_center(qr);
    lv_qrcode_update(qr, "", 0);

    // Tạo bàn phím nhưng ẩn đi lúc đầu
    keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
}
