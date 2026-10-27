#include <stdint.h>

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _estack;
int main(void);

void Reset_Handler(void) {
    uint32_t *src = &_etext;
    uint32_t *dest = &_sdata;
    
    /* Copy .data segment from Flash to SRAM */
    while (dest < &_edata) *dest++ = *src++;
    
    /* Zero out .bss segment in SRAM */
    dest = &_sbss;
    while (dest < &_ebss) *dest++ = 0;
    
    main();
    while(1);
}

/* Vector table placed at 0x08000000 */
__attribute__((section(".isr_vector")))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),  /* Initial Stack Pointer */
    Reset_Handler                /* Reset Sequence */
};