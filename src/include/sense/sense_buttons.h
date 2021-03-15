/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_keyboard_keys_h
#define sense_keyboard_keys_h

typedef struct SenseNamedButtons {
    int down;
    int up;
    int left;
    int right;
    int a;
    int b;
    int x;
    int y;
    int menu;
} SenseNamedButtons;

typedef enum SenseButtonNames {
    SENSE_KEY_DOWN,
    SENSE_KEY_UP,
    SENSE_KEY_LEFT,
    SENSE_KEY_RIGHT,
    SENSE_KEY_A,
    SENSE_KEY_B,
    SENSE_KEY_X,
    SENSE_KEY_Y,
    SENSE_KEY_MENU,
} SenseButtonNames;

typedef union SenseButtons {
    SenseNamedButtons named;
    int values[9];
} SenseButtons;

#endif
