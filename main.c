#include <inkview.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 1. Структура для хранения настроек
typedef struct {
    char ip[32];
    char port[8];
    char password[64];
    char read_col[64];
    char date_col[64];
    char fav_col[64];
    char folder[128];
} AppConfig;

// Значения по умолчанию
AppConfig config = {
    "192.168.1.10",
    "9090",
    "",
    "#read",
    "#read_date",
    "#favorite",
    "/mnt/ext1/Books/Calibre"
};

// Названия полей для отображения
const char *labels[] = {
    "IP Address",
    "Port",
    "Password",
    "Read Status Column",
    "Read Date Column",
    "Favorite Column",
    "Books Folder",
    "[ CONNECT ]", // Кнопка действия
    NULL
};

// Глобальный шрифт
ifont *font = NULL;
int selected_item = 0;
#define ITEM_COUNT 8

// Обработчик перерисовки экрана
void draw_screen() {
    int y = 50;
    int h = 40; // высота строки
    int w = ScreenWidth();

    FillArea(0, 0, w, ScreenHeight(), 0xFFFFFF); // Очистка белым
    SetFont(font, 0x000000);

    // Заголовок
    DrawString(w/2, 10, "PocketBook Companion");

    for (int i = 0; i < ITEM_COUNT; i++) {
        // Подсветка выбранного элемента
        if (i == selected_item) {
            FillArea(10, y - 5, w - 20, h, 0x000000); // Черный фон
            SetFontColor(0xFFFFFF); // Белый текст
        } else {
            SetFontColor(0x000000); // Черный текст
        }

        // Отображение названия поля
        DrawTextRect(20, y, w/2 - 20, h, labels[i], ALIGN_LEFT | VALIGN_MIDDLE);

        // Отображение текущего значения (кроме кнопки Connect)
        if (i < 7) {
            char *val = "";
            switch(i) {
                case 0: val = config.ip; break;
                case 1: val = config.port; break;
                case 2: val = (strlen(config.password) > 0) ? "***" : "<empty>"; break;
                case 3: val = config.read_col; break;
                case 4: val = config.date_col; break;
                case 5: val = config.fav_col; break;
                case 6: val = config.folder; break;
            }
            DrawTextRect(w/2, y, w/2 - 20, h, val, ALIGN_RIGHT | VALIGN_MIDDLE);
        }

        y += h + 10;
    }
    
    // Принудительное обновление E-Ink
    FullUpdate(); 
}

// Обработчик ввода с клавиатуры
void on_keyboard_input(char *text) {
    if (text) {
        switch(selected_item) {
            case 0: snprintf(config.ip, sizeof(config.ip), "%s", text); break;
            case 1: snprintf(config.port, sizeof(config.port), "%s", text); break;
            case 2: snprintf(config.password, sizeof(config.password), "%s", text); break;
            case 3: snprintf(config.read_col, sizeof(config.read_col), "%s", text); break;
            case 4: snprintf(config.date_col, sizeof(config.date_col), "%s", text); break;
            case 5: snprintf(config.fav_col, sizeof(config.fav_col), "%s", text); break;
            case 6: snprintf(config.folder, sizeof(config.folder), "%s", text); break;
        }
    }
    draw_screen();
}

// Обработчик событий
void handler(int type, int par1, int par2) {
    switch (type) {
        case EVT_INIT:
            font = OpenFont("LiberationSans", 24, 1);
            break;
        case EVT_SHOW:
            draw_screen();
            break;
        case EVT_KEYPRESS:
            if (par1 == KEY_DOWN || par1 == KEY_NEXT) {
                selected_item = (selected_item + 1) % ITEM_COUNT;
                draw_screen();
            } else if (par1 == KEY_UP || par1 == KEY_PREV) {
                selected_item = (selected_item - 1 + ITEM_COUNT) % ITEM_COUNT;
                draw_screen();
            } else if (par1 == KEY_OK || par1 == KEY_ENTER) {
                if (selected_item == 7) {
                    // Логика подключения
                    Message(ICON_INFORMATION, "Connecting...", "Trying to connect to Calibre...", 2000);
                } else {
                    // Открываем клавиатуру
                    char *current_val = "";
                    // ... (выбор текущего значения для предзаполнения, упрощено)
                    OpenKeyboard("Enter Value", current_val, 1024, 0, on_keyboard_input);
                }
            } else if (par1 == KEY_BACK || par1 == KEY_EXIT) {
                CloseApp();
            }
            break;
    }
}

int main(int argc, char **argv) {
    InkViewMain(handler);
    return 0;
}