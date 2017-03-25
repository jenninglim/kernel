#ifndef __P6_H
#define __P6_H

#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "libc.h"

#define NO_OF_PHILOSOPHERS 16

#define EATING 1
#define THINKING 0
#define HUNGER 2

typedef enum {T, H, E} state_e;

#endif
