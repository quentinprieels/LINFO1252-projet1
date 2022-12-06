/* ===== VERROU PAR ATTENTE ACTIVE TEST-AND-SET ===== */
void ts_lock(int *verrou) {
    int val = 1;
    
    while(val) {
        asm(
            "xchgl %0, %1"      /* Atomic exchange of values */
            : "+r" (val)        /* Operand referred to by %0, stored into any register (and writer/reading access) */
            : "m" (*verrou)     /* Operand referred to by %1, stored into any register */
            : "memory"          /* There is a operation between memory and register */
        );
    }
}

void ts_unlock(int *verrou) {
    int val = 0;

    asm(
        "xchgl %0, %1"
        : "+r" (val)
        : "m" (*verrou)
        : "memory"
    );
}

/* ====== VERROU AVEC L'ALGORITHME TEST-AND-TEST-AND-SET =====*/
void tts_lock(int *verrou) {
    int val = 1;
    
    do {
        asm(
            "xchgl %0, %1"
            : "+r" (val)
            : "m" (*verrou)
            : "memory"
        );

        while(val == 1 && *verrou == 1) {}

    } while(val);
}

void tts_unlock(int *verrou) {
    ts_unlock(verrou);
}