#if OS_CPU_HOOKS_EN > 0u && OS_VERSION > 290u
void OSTaskReturnHook(OS_TCB *ptcb) {
    (void)ptcb;
}
#endif
