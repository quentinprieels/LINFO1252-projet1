/* ===== VERROU PAR ATTENTE ACTIVE ===== */
void lock(int *verrou) {
    int val = 1;
    
    while(val) {
        asm volatile(
            "xchgl %0, %1"      /* Atomic exchange of values */
            : "+r" (val)        /* Operand referred to by %0, stored into any register (and writer/reading access) */
            : "m" (*verrou)     /* Operand referred to by %1, stored into any register */
            : "memory"          /* xchgl might have change the variabe at the memory location */
        );
    }
}

void unlock(int *verrou) {
    int val = 0;

    asm volatile(
        "xchgl %0, %1"      /* Atomic exchange of values, set verrou to 0*/
        : "+r" (val)
        : "m" (*verrou)
        : "memory"
    );
}
