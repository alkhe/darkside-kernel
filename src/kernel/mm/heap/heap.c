#include <lib/libc/stdint.h>
#include <lib/libc/stdbool.h>
#include <lib/libc/string.h>
#include <kernel/init/hal.h>
#include <kernel/mm/address_space.h>
#include <kernel/mm/heap/heap.h>

/* The kernel heap */
heap_t *kheap = 0;

extern uint32_t kernel_directory;

/* Allocate memory on the kernel heap */
void *kmalloc(uint32_t size)
{
	/* Allocate the memory */
	void *address = heap_malloc(kheap, size, false);

	/* Return the address */
	return address;
}

/* Free memory on the kernel heap */
void kfree(void *ptr)
{
	heap_free(kheap, ptr);
}

/* Resize memory allocated on the kernel heap */
void *krealloc(void *ptr, uint32_t size)
{
	/* Resize the memory */
	void *address = heap_realloc(kheap, ptr, size, false);

	/* Return the address */
	return address;
}

/* Create a heap */
heap_t *create_heap(uint32_t start_address, uint32_t end_address, uint32_t min_address, uint32_t max_address, bool user, bool global)
{
	/* First, place a heap structure, make sure it's 0, and fill in its data */
	heap_t *heap = (heap_t*) start_address;
	memset(heap, 0, sizeof(heap_t));

	heap->start_address = start_address;
	heap->end_address = end_address;
	heap->min_address = min_address;
	heap->max_address = max_address;

	heap->user = user;
	heap->global = global;

	/* Second, create the heap index */
	header_t *header = (header_t*) start_address + sizeof(heap_t);

	header->magic = HEAP_MAGIC;
	header->type = 0;
	header->size = (end_address - start_address) - sizeof(heap_t);

	header->left = 0;
	header->right = 0;

	footer_t *footer = (footer_t*) (start_address + sizeof(heap_t) + header->size) - 8;

	footer->magic = HEAP_MAGIC;
	footer->header = header;

	heap->index = header;

	/* Finally, return the new heap */
	return heap;
}

/* Expand or contract a heap */
void resize_heap(heap_t *heap, uint32_t new_size)
{
	/* If the heap doesn't exist, just return */
	if (!heap)
	{
		return;
	}

	/* Get the old and maximum size of the heap */
	uint32_t old_size = heap->end_address - heap->start_address;

	/* Expand the heap */
	if (new_size > old_size)
	{
		/* If the new size is greater than the maximum heap size, just return */
		if (heap->start_address + new_size > heap->max_address)
		{
			return;
		}

		/* Make sure the new end address is page aligned */
		new_size = page_align(new_size);

		int32_t i;

		/* Allocate new pages for the heap */
		for (i = heap->start_address + old_size; i < heap->start_address + new_size; i += 0x1000)
		{
			map_page(kernel_directory, i, pmm_alloc_page(), true, true, heap->user, heap->global);
		}

		/* Finally, modify the heap's end address */
		heap->end_address = heap->start_address + new_size;
	}
	/* Contract the heap */
	else if (new_size < old_size)
	{
		/* If the new size is less than the minimum heap size, just return */
		if (heap->start_address + new_size < heap->min_address)
		{
			return;
		}

		/* Make sure the new end address is page aligned */
		new_size = page_align(new_size);

		int32_t i;

		/* Free pages from the heap */
		for (i = heap->start_address + old_size; i > heap->start_address + new_size; i -= 0x1000)
		{
			unmap_page(kernel_directory, i);
		}

		/* Finally, modify the heap's end address */
		heap->end_address = heap->start_address + new_size;
	}
	/* The two sizes are equal */
	else
	{
		return;
	}
}

/* Allocate memory on a heap */
void *heap_malloc(heap_t *heap, uint32_t size, bool align)
{
	/* If the heap doesn't exist, return 0 */
	if (!heap)
	{
		return 0;
	}

	/* Get the new size of the chunk */
	uint32_t chunk_size = sizeof(header_t) + size + sizeof(footer_t);

	/* Find a chunk that can fit our size */
	//header_t *chunk = lookup_chunk(heap->index, chunk_size);
	header_t *chunk = 0;

	/* If we found a chunk, return the address of the memory we found */
	if (chunk != 0)
	{
		return (void*) ((uint32_t)chunk + sizeof(header_t));
	}
	/* Otherwise, if we somehow reached here, return 0 */
	else
	{
		return 0;
	}
}

/* Free memory on a heap */
void heap_free(heap_t *heap, void *ptr)
{
}

/* Resize memory allocated on a heap */
void *heap_realloc(heap_t *heap, void *ptr, uint32_t size, bool align)
{
	/* If the heap doesn't exist, return 0 */
	if (!heap)
	{
		return 0;
	}

	/* Make sure the pointer and size aren't 0 */
	if (ptr != 0 && size != 0)
	{
		/* Find the header and footer of the memory and get its old size, not counting the size of the header or footer */
		header_t *header = (header_t*) (ptr - sizeof(header_t));
		footer_t *footer = (footer_t*) (header + header->size);

		uint32_t old_size = header->size - sizeof(header_t) - sizeof(header_t);

		/* See if we have enough space to our left, and if we do, use that */

		/* See if we have enough space to our right, and if we do, use that */

		/* Otherwise, we need to allocate a completely new memory block, and copy the data there */
		void *new_address = heap_malloc(heap, size, align);

		/* Expand the memory block */
		if (old_size < size)
		{
			memcpy(new_address, ptr, old_size);
			heap_free(heap, ptr);
		}
		/* Shrink the memory block */
		else if (old_size > size)
		{
			memcpy(new_address, ptr, size);
			heap_free(heap, ptr);
		}
		/* Same size memory block */
		else
		{
			return;
		}

		return new_address;
	}
	/* Null pointer */
	else if (ptr == 0)
	{
		return heap_malloc(heap, size, align);
	}

	/* If the reallocation failed, return 0 */
	return 0;
}

/* Initialize the kernel heap */
void init_kheap()
{
	/* Create the kernel heap */
	kheap = create_heap(KHEAP_VIRTUAL_START, KHEAP_VIRTUAL_START + KHEAP_INITIAL_SIZE, KHEAP_VIRTUAL_START + KHEAP_MIN_SIZE, KHEAP_VIRTUAL_START + KHEAP_MAX_SIZE, false, true);
}