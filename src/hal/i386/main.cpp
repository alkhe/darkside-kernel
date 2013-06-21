#include <hal/i386/gdt.h>
#include <hal/i386/idt.h>
#include <hal/i386/isrs.h>
#include <hal/i386/irq.h>
#include <hal/i386/timer.h>
#include <hal/i386/pmm.h>
#include <hal/i386/vmm.h>
#include <kernel/debug/log.h>
#include <kernel/modules/multiboot.h>

/* Initialize the HAL */
void hal_main(struct multiboot *mboot_ptr)
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install(100);

	log("Interrupts enabled");
	log("Timer and log are up");

 	init_pmm(0x100000 + (mboot_ptr->mem_upper * 1024));
 	log("PMM initialization complete");

 	init_vmm();
}