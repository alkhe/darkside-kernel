#include <lib/libgcc/stdbool.h>
#include <lib/libc/string.h>
#include <kernel/init/hal.h>
#include <kernel/task/process.h>
#include <kernel/task/thread.h>
#include <kernel/task/task.h>

/* Current thread ID and the number of running threads */
volatile unsigned int current_tid = 0;

/* Create a new blank thread */
thread_t *create_thread(process_t *parent_process, void (*function)(), char **argv, unsigned int user_stack_size)
{
	/* Create a new thread structure */
	thread_t *new_thread = (thread_t*) kmalloc(sizeof(thread_t));
	memset(new_thread, 0, sizeof(thread_t));
	
	/* Expand the process's thread list and find the first availible TID in it */
	parent_process->threads = (thread_t**) krealloc(parent_process->threads, sizeof(thread_t*) * (parent_process->num_threads + 1));
	parent_process->threads[parent_process->num_threads] = 0;
	parent_process->num_threads++;

	unsigned int tid;
	for (tid = 0; tid < parent_process->num_threads; tid++)
	{
		if (parent_process->threads[tid] == 0)
		{
			break;
		}
	}

	new_thread->tid = tid;
    
	/* Create and fill out the thread context */
	new_thread->context = create_registers(function, (bool) get_mode_flags() & MODE_FLAGS_USER);
	
	/* Create the thread's user and kernel stacks */
	new_thread->kernel_stack = kmalloc_a(DEFAULT_KERNEL_STACK_SIZE) + DEFAULT_KERNEL_STACK_SIZE;

	/* Set the thread's parent process to the new process */
	new_thread->parent_process = parent_process;

	/* Add the thread to the new process's thread list */
	parent_process->threads[tid] = new_thread;

	/* Return the new thread */
	return new_thread;
}

/* Return the current TID */
unsigned int gettid()
{
	return current_tid;
}

/* Return a pointer to the current thread */
thread_t *getthread()
{
	return getprocess()->threads[current_tid];
}

/* Set the current TID */
void settid(unsigned int tid)
{
	current_tid = tid;
}
