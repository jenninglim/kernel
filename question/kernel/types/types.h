#ifndef _TYPES_H
#define _TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


/* The kernel source code is made simpler via three type definitions:
 *
 * - a type that captures a Process IDentifier (PID), which is really
 *   just an integer,
 * - a type that captures each component of an execution context (i.e.,
 *   processor state) in a compatible order wrt. the low-level handler
 *   preservation and restoration prologue and epilogue, and
 * - a type that captures a process PCB.
 */

// PID type.
typedef int pid_t;

// Context type.
typedef struct {
        uint32_t cpsr, pc, gpr[ 13 ], sp, lr;
} ctx_t;

// Initialise new context.
void INIT_CTX(ctx_t * ctx, uint32_t cpsr, uint32_t pc, uint32_t sp);

// Update a context.
void update_ctx(ctx_t * new, ctx_t * source);

#endif

