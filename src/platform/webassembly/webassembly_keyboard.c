/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "webassembly_keyboard.h"
#include <emscripten/html5.h>

#include <clog/clog.h>
#include <sense/SenseInput.h>
#include <sense/sense_input_manager.h>
#include <tiny-libc/tiny_libc.h>
#include <tyran/tyran_types.h>

static int string_is_equal(const char *a, const char *b)
{
	return strcmp(a, b) == 0;
}

static void handle_key(sense_keyboard_keys *k, int key_down, tyran_boolean state)
{
	int *ref = 0;

	switch (key_down)
	{
	case 0:
		ref = &k->up;
		break;
	case 1:
		ref = &k->down;
		break;
	case 2:
		ref = &k->left;
		break;
	case 3:
		ref = &k->right;
		break;
	case 4:
		ref = &k->a;
		break;
	case 5:
		ref = &k->b;
		break;
	default:
		return;
	}

	*ref = state ? 1 : 0;
}

static size_t handle_known_keys(const char *keyname)
{
	char *keynames[] = {"w", "s", "a", "d", "q", "e", "r", "f", "esc",
						"i", "k", "j", "l", "o", "u", "y", "h", "n"};

	for (size_t i = 0; i < sizeof(keynames) / sizeof(char *); ++i)
	{
		if (string_is_equal(keynames[i], keyname))
		{
			return i + 1;
		}
	}
	return 0;
}

static EM_BOOL on_key(sense_webassembly_keys *self, const EmscriptenKeyboardEvent *keyEvent, tyran_boolean state)
{
	if (keyEvent->repeat)
	{
		return EM_FALSE;
	}
	size_t detected_key = handle_known_keys(keyEvent->key);

	if (detected_key)
	{
		detected_key--;
		size_t player_index = detected_key / 9;
		size_t local_key = detected_key % 9;
		self->device_in_focus = TYRAN_TRUE;
		handle_key(&self->keys[player_index], local_key, state);
		return EM_TRUE;
	}
	return EM_FALSE;
}

static EM_BOOL on_key_down(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *user_data)
{
	(void)eventType;

	return on_key((sense_webassembly_keys *)user_data, keyEvent, TYRAN_TRUE);
}

static EM_BOOL on_key_up(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *user_data)
{
	(void)eventType;
	return on_key((sense_webassembly_keys *)user_data, keyEvent, TYRAN_FALSE);
}

void sense_webassembly_keys_init(sense_webassembly_keys *self, const char *node)
{
	CLOG_OUTPUT("Setting callbacks!");
	tc_mem_clear_type(self);
	emscripten_set_keydown_callback(node, self, 1, on_key_down);
	emscripten_set_keyup_callback(node, self, 1, on_key_up);
}
