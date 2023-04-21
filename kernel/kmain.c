/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
      
      Initial Kernel -- by Forrest Desjardin, 2013, 
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
			Ben Smith 2018, and Alex Wilson 2019, Jakob Kaivo 2022
*/

#include <mpx/gdt.h>
#include <mpx/interrupts.h>
#include <mpx/serial.h>
#include <mpx/vm.h>
#include <mpx/pcb.h>
#include <mpx/call.h>
#include <processes.h>
#include <commhand.h>
#include <sys_req.h>
#include <string.h>
#include <heap.h>
#include <memory.h>
#include <stdint.h>

static void klogv(device dev, const char *msg)
{
	char prefix[] = "klogv: ";
	serial_out(dev, prefix, sizeof(prefix));
	serial_out(dev, msg, strlen(msg));
	serial_out(dev, "\r\n", 2);
}

void kmain(void)
{ 
	// 0) Serial I/O -- mpx/serial.h
	// Note that here, you should call the function *before* the output via klogv(),
	// or the message won't print. In all other cases, the output should come first
	// as it describes what is about to happen.
	serial_init(COM1);
	klogv(COM1, "Initialized serial I/O on COM1 device...");

	// 1) Global Descriptor Table -- mpx/gdt.h
	// Keeps track of the various memory segments (Code, Data, Stack, etc.) required by the
	// x86 architecture.
	klogv(COM1, "Initializing Global Descriptor Table...");
	gdt_init();

	// 2) Interrupt Descriptor Table -- mpx/interrupts.h
	// Keeps track of where the various Interrupt Vectors are stored.
	klogv(COM1, "Initializing Interrupt Descriptor Table...");
	idt_init();

	// 3) Disable Interrupts -- mpx/interrupts.h
	// You'll be modifying how interrupts work, so disable them to avoid crashing.
	klogv(COM1, "Disabling interrupts...");
	cli();

	// 4) Interrupt Vector -- mpx/interrupts.h
	// Initialize and install Interrupt Service Routines for the first 32 IRQs
	// (bare minimum for a functioning x86 system).
	klogv(COM1, "Initializing Interrupt Vectors...");
	irq_init();

	// 5) Programmable Interrupt Controller -- mpx/interrupts.h
	// Initialize the PIC so that the ISRs installed in the previous step are connected
	// correctly.
	klogv(COM1, "Initializing Programmble Interrupt Controller...");
	pic_init();

	// 6) Reenable interrupts -- mpx/interrupts.h
	// Now that interrupt routines are set up, allow interrupts to happen again.
	klogv(COM1, "Enabling Interrupts...");
	sti();

	// 7) Virtual Memory -- mpx/vm.h
	// Initialize the Memory Management Unit's Page Tables and enable virtual memory. This
	// will also enable the kernel's (basic) heap manager, allowing the use of sys_alloc_mem()
	// (which has a maximum of 64kiB until you implement a full memory manager).
	klogv(COM1, "Initializing virtual memory...");
	vm_init();

	// 8) MPX Modules -- *headers vary*
	// Module specific initialization -- not all modules require this
	klogv(COM1, "Initializing MPX modules...");
	initialize_heap(50000);
	sys_set_heap_functions(allocate_memory, free_memory);

	//R6
	serial_open(COM1,9600);
	// R4: create commhand and idle processes

	// 9) YOUR command handler -- *create and #include an appropriate .h file*
	// Pass execution to your command handler so the user can interact with the system.
	serial_open(COM1, 9600);
	klogv(COM1, "Transferring control to commhand...");

	pcb *commhand_pcb = pcb_setup("commhand", 0, 0);
	context commhand_cxt = {
		.ds = 0x10, .es = 0x10, .fs = 0x10, .gs = 0x10, .ss = 0x10, 
		.eax = 0, .ebx = 0, .ecx = 0, .edx = 0, .esi = 0, .edi = 0, .ebp = (uint32_t) (commhand_pcb->stack + STACK_SIZE - 1 - sizeof(void *)),
		.eip = (uint32_t) commhand, .cs = 0x8, .eflags = 0x202
	};
	commhand_pcb->stack_ptr += sizeof(void *);
    commhand_pcb->stack_ptr -= sizeof(commhand_cxt);
    memcpy(commhand_pcb->stack_ptr, &commhand_cxt, sizeof(commhand_cxt));
    pcb_insert(commhand_pcb);
	
	pcb *idle_pcb = pcb_setup("idle", 0, 9);
	context idle_cxt = {
		.ds = 0x10, .es = 0x10, .fs = 0x10, .gs = 0x10, .ss = 0x10,
		.eax = 0, .ebx = 0, .ecx = 0, .edx = 0, .esi = 0, .edi = 0, .ebp = (uint32_t) (idle_pcb->stack + STACK_SIZE - 1 - sizeof(void *)),
		.eip = (uint32_t) sys_idle_process, .cs = 0x8, .eflags = 0x202
	};
	idle_pcb->stack_ptr += sizeof(void *);
	idle_pcb->stack_ptr -= sizeof(idle_cxt);
	memcpy(idle_pcb->stack_ptr, &idle_cxt, sizeof(idle_cxt));
	pcb_insert(idle_pcb);

	__asm__ volatile ("int $0x60" :: "a"(IDLE));

	// 10) System Shutdown -- *headers to be determined by your design*
	// After your command handler returns, take care of any clean up that is necessary.
	klogv(COM1, "Starting system shutdown procedure...");
	
	// System shutdown -- Nothing remains to change below here.
	klogv(COM1, "Shutdown complete.");
}
