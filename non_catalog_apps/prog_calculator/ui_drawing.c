#include "ui_drawing.h"
#include <string.h>
#include "utilities.h"
#include "calculator.h"

void drawElement(Canvas* canvas, const char* str, int x, int y, int width, int height, Font font) {
    if (str) {
        canvas_set_font(canvas, font);
        canvas_draw_str(canvas, x, y, str);
    } else {
        canvas_draw_box(canvas, x, y, width, height);
    }
}

void generate_calculator_layout(Canvas* canvas) {


    // display
    canvas_draw_frame(canvas, 0, 0, 64, 49);  // display frame 
    canvas_draw_frame(canvas, 2, 2, 60, 31);  // output  frame
    canvas_draw_frame(canvas, 2, 34, 60, 13); // input   frame

    // Horizonal and Vertical lines
    drawElement(canvas, NULL, 0, 48, 64, 2, 0);  // H line 1
    drawElement(canvas, NULL, 0, 61, 64, 1, 0);  // H line 2
    drawElement(canvas, NULL, 0, 73, 64, 2, 0);  // H line 3
    drawElement(canvas, NULL, 0, 87, 64, 1, 0);  // H line 4
    drawElement(canvas, NULL, 0, 100, 64, 1, 0); // H line 5
    drawElement(canvas, NULL, 0, 113, 64, 1, 0); // H line 6
    drawElement(canvas, NULL, 0, 126, 64, 2, 0); // H line 7
    
    drawElement(canvas, NULL, 0, 50, 2, 80, 0);  // V line 1
    drawElement(canvas, NULL, 13, 73, 1, 55, 0); // V line 2
    drawElement(canvas, NULL, 25, 73, 1, 55, 0); // V line 3
    drawElement(canvas, NULL, 37, 73, 2, 55, 0); // V line 4
    drawElement(canvas, NULL, 50, 73, 1, 41, 0); // V line 5
    drawElement(canvas, NULL, 62, 48, 2, 81, 0); // V line 6

    // MODE key and display
    // row 1 and 2
    drawElement(canvas, "MODE", 19, 59, 0, 0, FontSecondary);
    drawElement(canvas, "", 5, 38, 0, 0, FontSecondary);

    // Keys
    // row 3
    drawElement(canvas, "7", 5, 85, 0, 0, FontSecondary);
    drawElement(canvas, "8", 17, 85, 0, 0, FontSecondary);
    drawElement(canvas, "9", 29, 85, 0, 0, FontSecondary);
    drawElement(canvas, "A", 42, 85, 0, 0, FontSecondary);
    drawElement(canvas, "B", 54, 85, 0, 0, FontSecondary);
    // row 4
    drawElement(canvas, "4", 5, 98, 0, 0, FontSecondary);
    drawElement(canvas, "5", 17, 98, 0, 0, FontSecondary);
    drawElement(canvas, "6", 29, 98, 0, 0, FontSecondary);
    drawElement(canvas, "C", 42, 98, 0, 0, FontSecondary);
    drawElement(canvas, "D", 54, 98, 0, 0, FontSecondary);
    // row 5
    drawElement(canvas, "1", 6, 111, 0, 0, FontSecondary);
    drawElement(canvas, "2", 17, 111, 0, 0, FontSecondary);
    drawElement(canvas, "3", 29, 111, 0, 0, FontSecondary);
    drawElement(canvas, "E", 42, 111, 0, 0, FontSecondary);
    drawElement(canvas, "F", 54, 111, 0, 0, FontSecondary);
    // row 6
    drawElement(canvas, "<", 6, 124, 0, 0, FontSecondary);
    drawElement(canvas, "0", 17, 124, 0, 0, FontSecondary);
    drawElement(canvas, "=", 29, 124, 0, 0, FontSecondary);
    drawElement(canvas, "CLR", 42, 124, 0, 0, FontSecondary);

}

// highlighting when cursor moves
void draw_highlighted_cell(Canvas* canvas, short x, short y, short width, short height) {
    canvas_set_color(canvas, ColorBlack);  
    canvas_draw_box(canvas, x, y, width, height);
}

void displayResult(Canvas* canvas, char* result, int x, int y) {
    int length = strlen(result);
    int lineSpacing = 9; // Reduced line spacing

    if (length > 22) {
        char line1[12];
        strncpy(line1, result, 11);
        line1[11] = '\0';

        char line2[12];
        strncpy(line2, result + 11, 11);
        line2[11] = '\0';

        const char* line3 = result + 22;

        canvas_draw_str(canvas, x, y, line1);
        canvas_draw_str(canvas, x, y + lineSpacing, line2);
        canvas_draw_str(canvas, x, y + 2 * lineSpacing, line3);
    } else if (length > 11) {
        char line1[12];
        strncpy(line1, result, 11);
        line1[11] = '\0';

        const char* line2 = result + 11;

        canvas_draw_str(canvas, x, y, line1);
        canvas_draw_str(canvas, x, y + lineSpacing, line2);
    } else {
        canvas_draw_str(canvas, x, y, result); // Single line if 11 characters or less
    }
}


void calculator_draw_callback(Canvas* canvas, void* ctx) {
    furi_assert(ctx);
    const Calculator* calculator_state = ctx;
    furi_mutex_acquire(calculator_state->mutex, FuriWaitForever);

    // Clear the canvas for fresh drawing
    canvas_clear(canvas);

    // Draw the static UI layout
    generate_calculator_layout(canvas);

    char resultLabel[2 * MAX_TEXT_LENGTH]; // Buffer to hold the result label

    // Check which mode is selected and prepare the label accordingly
    if(calculator_state->mode == ModeHexToDec) {
        snprintf(resultLabel, sizeof(resultLabel), "Dec: %s", calculator_state->decResult);
    } else if(calculator_state->mode == ModeDecToBin) {
        snprintf(resultLabel, sizeof(resultLabel), "Bin: %s", calculator_state->binaryResult);
    } else if(calculator_state->mode == ModeDecToChar) {
        snprintf(resultLabel, sizeof(resultLabel), "Charachter %s", calculator_state->charResult);
    } else if(calculator_state->mode == ModeHexToBin) {
        snprintf(resultLabel, sizeof(resultLabel), "Bin: %s", calculator_state->binaryResult);
    } else if(calculator_state->mode == ModeDecToHex) {
        snprintf(resultLabel, sizeof(resultLabel), "Hex: %s", calculator_state->hexResult);
    } else if(calculator_state->mode == ModeBinToDec) {
        snprintf(resultLabel, sizeof(resultLabel), "Dec: %s", calculator_state->decResult);
    } else if(calculator_state->mode == ModeBinToHex) {
        snprintf(resultLabel, sizeof(resultLabel), "Hex: %s", calculator_state->hexResult);
    } else {
        // If no mode is selected, you can display a default message or leave it empty
        strncpy(resultLabel, "Click MODE Programmer Calc v0.8", sizeof(resultLabel));
    }

    // Display the result, splitting into two lines if necessary
    displayResult(canvas, resultLabel, 5, 12);

    // Draw new input with ">" label or mode selection prompt
    char inputLabel[MAX_TEXT_LENGTH + 3]; // Adjusted size for "> "
    
    snprintf(inputLabel, sizeof(inputLabel), "$> %s", calculator_state->text);
    canvas_draw_str(canvas, 5, 44, inputLabel);
    // Define the cell dimensions for each row and column
    const short cellDimensions[6][5][2] = {
        // {Width, Height} for each cell
        {{12, 13}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, // Row 1 (One column)
        {{9, 13}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, // Row 2 (One column)
        {{12, 13}, {12, 13}, {12, 13}, {12, 13}, {12, 13}}, // Row 3
        {{12, 13}, {12, 13}, {12, 13}, {12, 13}, {12, 13}}, // Row 4
        {{12, 13}, {12, 13}, {12, 13}, {12, 13}, {12, 13}}, // Row 5
        {{12, 13}, {12, 13}, {12, 13}, {24, 13}, {0, 0}}, // Row 6, with different width for column 4
    };

    const char* modeStr = "";
    switch(calculator_state->mode) {
        case ModeDecToBin:
            modeStr = " 1: Dec >> Bin";
            break;
        case ModeDecToHex:
            modeStr = "2: Dec >> Hex";
            break;
        case ModeDecToChar:
            modeStr = "3: Dec >> Char";
            break;
        case ModeHexToBin:
            modeStr = "4: Hex >> Bin";
            break;
        case ModeHexToDec:
            modeStr = "5: Hex >> Dec";
            break;
        case ModeBinToDec:
            modeStr = "6: Bin >> Dec";
            break;
        case ModeBinToHex:
            modeStr = "7: Bin >> Hex";
            break;
        default:
            modeStr = "      waiting ...";
            break;
    }
    canvas_draw_str(canvas, 4, 71, modeStr);
    short cursorX = 2;
    short cursorY = 48; // Starting Y position

    for (int i = 0; i < calculator_state->position.y; i++) {
        cursorY += cellDimensions[i][0][1]; // Add the height of each previous row
    }

    for (int i = 0; i < calculator_state->position.x; i++) {
        cursorX += cellDimensions[calculator_state->position.y][i][0]; // Add the width of each previous column
    }

    short cellWidth = cellDimensions[calculator_state->position.y][calculator_state->position.x][0];
    short cellHeight = cellDimensions[calculator_state->position.y][calculator_state->position.x][1];

    // Draw the highlighted cell
    draw_highlighted_cell(canvas, cursorX, cursorY, cellWidth, cellHeight);

    // Determine the content to display in the cell
    char cellContent[2]; // Buffer to hold the cell content
    snprintf(cellContent, sizeof(cellContent), "%c", getKeyAtPosition(calculator_state->position.x, calculator_state->position.y));

    // Calculate center position for the text
    short textX = cursorX + (cellWidth - calculateStringWidth(cellContent, 2)) / 1;
    short textY = cursorY + (cellHeight - 2) / 1;

    // Set the color for the text and draw it
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_str(canvas, textX, textY, cellContent);

    furi_mutex_release(calculator_state->mutex);
}
