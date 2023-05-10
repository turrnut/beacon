/**
 * Author: turrnut
 * Copyrighted © turrnut under the Apache 2.0 license
 *
 * We hoped that you will use this piece of open source
 * software fairly. read the LICENSE for details for
 * more details about how you can use it, you have freedom
 * to distribute and use this file in your project. However,
 * you will have to state changes you made and include the
 * orginal author of this file.
 *
 * os.c
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "console.h"

typedef enum
{
	False,
	True
} boolean;

void start()
{
	clear_screen();
	set_color(WHITE_COLOR, BLACK_COLOR);
	println("Welcome to my operating system!");
	while (1)
	{
	}
}
