#include "types.h"

void set_cpsr(ctx_t * ctx, uint32_t cpsr) {
    ctx->cpsr = cpsr;
}

void set_pc (ctx_t * ctx, uint32_t pc) {
    ctx->pc = pc;
}

void set_sp(ctx_t * ctx, uint32_t sp) {
    ctx->sp = sp;
}

void INIT_CTX(ctx_t * ctx, uint32_t cpsr, uint32_t pc, uint32_t sp) {
    set_cpsr(ctx, cpsr);
    set_pc(ctx, pc);
    set_sp(ctx, sp);
    return;
}

void update_ctx(ctx_t * new, ctx_t * source) {
    memcpy(new, source, sizeof(ctx_t));
    return;
}


