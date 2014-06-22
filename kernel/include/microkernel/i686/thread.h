#ifndef __I686_THREAD
#define __I686_THREAD

#include <microkernel/process.h>
#include <microkernel/i686/isr.h>

/* Thread ID type */
typedef unsigned tid_t;

/* Thread states */
#define THREAD_INIT			0
#define THREAD_READY		1
#define THREAD_RUNNING		2
#define THREAD_BLOCKED		3
#define THREAD_DEAD			4

/* Thread structure */
typedef struct thread
{
	/* Parent process */
	process_t *process;

	/* Thread ID */
	tid_t tid;

	/* Register context */
	struct regs *context;

	/* Kernel stack */
	vaddr_t kernel_stack;

	/* Thread state */
	int state;

	/* Scheduling policy and priority */
	int policy, priority;

	/* CPU affinity bitmap */
	uint8_t *cpu_affinity;

	/* Last CPU that the thread ran on */
	int last_cpu;

	/* Quantum, in microseconds */
	uint32_t quantum;
} thread_t;

#endif
