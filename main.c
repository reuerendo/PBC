#include "inkview.h"
#include <string.h>

// Configuration structure
typedef struct {
    char ip[64];
    char port[16];
    char password[64];
    char read_status_column[64];
    char read_date_column[64];
    char favorite_column[64];
    char books_folder[256];
} AppConfig;

// Global variables
static AppConfig config;
static ifont *font_normal;
static ifont *font_title;

// Input field tracking
static int active_field = -1;
static char *field_pointers[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static const char *field_labels[] = {
    "IP Address:",
    "Port:",
    "Password:",
    "Read Status Column:",
    "Read Date Column:",
    "Favorite Column:",
    "Books Folder:"
};

// Forward declarations
static void draw_main_screen();
static void handle_keyboard_input(char *text);
static void open_keyboard_for_field(int field_index);

// Initialize configuration with default values
static void init_config() {
    strcpy(config.ip, "192.168.1.1");
    strcpy(config.port, "8080");
    strcpy(config.password, "");
    strcpy(config.read_status_column, "read_status");
    strcpy(config.read_date_column, "read_date");
    strcpy(config.favorite_column, "favorite");
    strcpy(config.books_folder, "/mnt/ext1/books");
    
    // Set up field pointers
    field_pointers[0] = config.ip;
    field_pointers[1] = config.port;
    field_pointers[2] = config.password;
    field_pointers[3] = config.read_status_column;
    field_pointers[4] = config.read_date_column;
    field_pointers[5] = config.favorite_column;
    field_pointers[6] = config.books_folder;
}

// Keyboard callback handler
static void handle_keyboard_input(char *text) {
    if (active_field >= 0 && active_field < 7 && text != NULL) {
        strncpy(field_pointers[active_field], text, 
                (active_field == 6) ? 255 : ((active_field == 0) ? 63 : 
                ((active_field == 1) ? 15 : 63)));
        field_pointers[active_field][(active_field == 6) ? 255 : 
                                     ((active_field == 0) ? 63 : 
                                     ((active_field == 1) ? 15 : 63))] = '\0';
    }
    active_field = -1;
    draw_main_screen();
}

// Open keyboard for specific field
static void open_keyboard_for_field(int field_index) {
    active_field = field_index;
    
    // Determine keyboard type based on field
    int kbd_type = (field_index == 1) ? KBD_NUMERIC : KBD_NORMAL;
    
    OpenKeyboard(field_labels[field_index], field_pointers[field_index], 
                 (field_index == 6) ? 255 : ((field_index == 0) ? 63 : 
                 ((field_index == 1) ? 15 : 63)),
                 kbd_type, handle_keyboard_input);
}

// Draw the main screen
static void draw_main_screen() {
    ClearScreen();
    
    int screen_width = ScreenWidth();
    int screen_height = ScreenHeight();
    int y_offset = 80;
    int field_height = 70;
    int label_offset = 10;
    int value_offset = 35;
    int padding = 20;
    
    // Draw title
    SetFont(font_title, BLACK);
    DrawTextRect(0, 20, screen_width, 50, "Pocketbook Companion", ALIGN_CENTER);
    
    // Draw input fields
    SetFont(font_normal, BLACK);
    
    for (int i = 0; i < 7; i++) {
        int y_pos = y_offset + (i * field_height);
        
        // Draw label
        DrawString(padding, y_pos + label_offset, field_labels[i]);
        
        // Draw field background
        DrawRect(padding, y_pos + value_offset, 
                 screen_width - (padding * 2), 40, BLACK);
        FillArea(padding + 2, y_pos + value_offset + 2, 
                 screen_width - (padding * 2) - 4, 36, WHITE);
        
        // Draw field value (mask password)
        if (i == 2 && strlen(field_pointers[i]) > 0) {
            char masked[64];
            memset(masked, '*', strlen(field_pointers[i]));
            masked[strlen(field_pointers[i])] = '\0';
            DrawString(padding + 10, y_pos + value_offset + 10, masked);
        } else {
            DrawString(padding + 10, y_pos + value_offset + 10, field_pointers[i]);
        }
    }
    
    // Draw save button
    int button_width = 200;
    int button_height = 50;
    int button_x = (screen_width - button_width) / 2;
    int button_y = screen_height - 100;
    
    FillArea(button_x, button_y, button_width, button_height, BLACK);
    SetFont(font_normal, WHITE);
    DrawTextRect(button_x, button_y + 10, button_width, button_height - 20, 
                 "Save", ALIGN_CENTER);
    
    FullUpdate();
}

// Handle pointer events (touch)
static int handle_pointer(int type, int x, int y) {
    if (type == EVT_POINTERUP) {
        int screen_width = ScreenWidth();
        int screen_height = ScreenHeight();
        int y_offset = 80;
        int field_height = 70;
        int value_offset = 35;
        int padding = 20;
        
        // Check if touch is on any input field
        for (int i = 0; i < 7; i++) {
            int y_pos = y_offset + (i * field_height) + value_offset;
            if (x >= padding && x <= screen_width - padding &&
                y >= y_pos && y <= y_pos + 40) {
                open_keyboard_for_field(i);
                return 1;
            }
        }
        
        // Check if touch is on save button
        int button_width = 200;
        int button_height = 50;
        int button_x = (screen_width - button_width) / 2;
        int button_y = screen_height - 100;
        
        if (x >= button_x && x <= button_x + button_width &&
            y >= button_y && y <= button_y + button_height) {
            Message(ICON_INFORMATION, "Success", 
                    "Configuration saved successfully!", 2000);
            return 1;
        }
    }
    return 0;
}

// Main event handler
static int main_handler(int event_type, int param_one, int param_two) {
    switch (event_type) {
        case EVT_INIT:
            font_normal = OpenFont("LiberationSans", 20, 0);
            font_title = OpenFont("LiberationSans-Bold", 28, 0);
            init_config();
            draw_main_screen();
            break;
            
        case EVT_EXIT:
            CloseFont(font_normal);
            CloseFont(font_title);
            break;
            
        case EVT_SHOW:
            draw_main_screen();
            break;
            
        case EVT_KEYPRESS:
            if (param_one == KEY_BACK) {
                CloseApp();
                return 1;
            }
            break;
            
        case EVT_POINTERUP:
        case EVT_POINTERDOWN:
        case EVT_POINTERMOVE:
            return handle_pointer(event_type, param_one, param_two);
    }
    
    return 0;
}

// Application entry point
int main(int argc, char *argv[]) {
    InkViewMain(main_handler);
    return 0;
}
