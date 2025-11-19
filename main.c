#include "inkview.h"

// Application name and version
#define APP_NAME "PocketBook Companion"
#define APP_VERSION "1.0"

// Global variables
ifont *font_main;
ifont *font_button;

// Forward declarations
int main_handler(int type, int par1, int par2);
void show_settings_dialog();
void exit_application();
void draw_main_dialog();

// Main dialog handler
int main_handler(int type, int par1, int par2) {
    switch (type) {
        case EVT_INIT:
            // Initialize fonts
            font_main = OpenFont("LiberationSans", 28, 1);
            font_button = OpenFont("LiberationSans", 24, 0);
            
            // Draw the main dialog
            draw_main_dialog();
            break;
            
        case EVT_SHOW:
            // Redraw when shown
            draw_main_dialog();
            break;
            
        case EVT_KEYPRESS:
            // Handle key presses
            if (par1 == KEY_BACK || par1 == KEY_HOME) {
                exit_application();
                return 1;
            }
            break;
            
        case EVT_EXIT:
            // Cleanup
            if (font_main) CloseFont(font_main);
            if (font_button) CloseFont(font_button);
            break;
    }
    
    return 0;
}

// Draw the main dialog window
void draw_main_dialog() {
    ClearScreen();
    
    // Get screen dimensions
    int screen_width = ScreenWidth();
    int screen_height = ScreenHeight();
    
    // Calculate dialog dimensions (centered, 60% of screen)
    int dialog_width = screen_width * 6 / 10;
    int dialog_height = screen_height * 4 / 10;
    int dialog_x = (screen_width - dialog_width) / 2;
    int dialog_y = (screen_height - dialog_height) / 2;
    
    // Draw dialog background
    SetFont(font_main, BLACK);
    DrawRect(dialog_x, dialog_y, dialog_width, dialog_height, BLACK);
    FillArea(dialog_x + 2, dialog_y + 2, dialog_width - 4, dialog_height - 4, WHITE);
    
    // Draw title
    int title_y = dialog_y + 40;
    DrawTextRect(dialog_x, title_y, dialog_width, 40, APP_NAME, ALIGN_CENTER);
    
    // Draw separator line under title
    DrawLine(dialog_x + 20, title_y + 50, dialog_x + dialog_width - 20, title_y + 50, BLACK);
    
    // Button dimensions
    int button_width = 200;
    int button_height = 60;
    int button_spacing = 30;
    int buttons_y = dialog_y + dialog_height - button_height - 60;
    
    // Calculate button positions (centered horizontally)
    int total_buttons_width = button_width * 2 + button_spacing;
    int first_button_x = dialog_x + (dialog_width - total_buttons_width) / 2;
    int second_button_x = first_button_x + button_width + button_spacing;
    
    // Draw "Settings" button
    DrawRect(first_button_x, buttons_y, button_width, button_height, BLACK);
    FillArea(first_button_x + 2, buttons_y + 2, button_width - 4, button_height - 4, WHITE);
    SetFont(font_button, BLACK);
    DrawTextRect(first_button_x, buttons_y + 15, button_width, button_height, "Настройки", ALIGN_CENTER);
    
    // Draw "Exit" button
    DrawRect(second_button_x, buttons_y, button_width, button_height, BLACK);
    FillArea(second_button_x + 2, buttons_y + 2, button_width - 4, button_height - 4, WHITE);
    DrawTextRect(second_button_x, buttons_y + 15, button_width, button_height, "Выход", ALIGN_CENTER);
    
    // Update the screen
    FullUpdate();
    
    // Set up touch areas for buttons
    SetPanelType(PANEL_ENABLED);
    
    // Settings button touch area
    AddTouchArea(first_button_x, buttons_y, button_width, button_height, 
                 (iv_handler)show_settings_dialog);
    
    // Exit button touch area
    AddTouchArea(second_button_x, buttons_y, button_width, button_height,
                 (iv_handler)exit_application);
}

// Show settings dialog
void show_settings_dialog() {
    Message(ICON_INFORMATION, "Настройки", 
            "Здесь будут настройки приложения", 2000);
    draw_main_dialog();
}

// Exit the application
void exit_application() {
    CloseApp();
}

// Application entry point
int main(int argc, char *argv[]) {
    // Initialize and start the main event loop
    InkViewMain(main_handler);
    return 0;
}
