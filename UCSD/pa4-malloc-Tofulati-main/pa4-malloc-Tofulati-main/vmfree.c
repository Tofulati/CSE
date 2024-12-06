#include "vm.h"
#include "vmlib.h"

/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmmalloc().
 * Otherwise, or if free(ptr) has already been called before,
 * undefined behavior occurs.
 * If ptr is NULL, no operation is performed.
 */

void vmfree(void *ptr) {
	if (ptr == NULL) {
		return;
	}

	struct block_header *block = (struct block_header *)((char *)ptr - sizeof(struct block_header));
	size_t block_size = block->size_status & VM_BLKSZMASK;
	int prev_busy = block->size_status & VM_PREVBUSY;
	block->size_status &= ~VM_BUSY;

	if ((block->size_status & VM_PREVBUSY) == 0) {
		struct block_footer *prev_block_footer = (struct block_footer *)((char *)block - sizeof(struct block_footer));
		size_t prev_block_size = prev_block_footer->size & VM_BLKSZMASK;
		struct block_header *prev_block = (struct block_header *)((char *)block - prev_block_size);
			
		block_size += prev_block_size;
		prev_block->size_status = block_size | (prev_block->size_status & VM_PREVBUSY);
		block = prev_block;
	}

	struct block_header *next_block = (struct block_header *)((char *)block + block_size);
	if (next_block->size_status != VM_ENDMARK && (next_block->size_status & VM_BUSY) == 0) {
		size_t next_block_size = next_block->size_status & VM_BLKSZMASK;
		block_size += next_block_size;

		struct block_footer *next_block_footer = (struct block_footer *)((char *)next_block + next_block_size - sizeof(struct block_footer));
		next_block_footer->size = block_size;
	}

	block->size_status = block_size | (prev_busy & VM_PREVBUSY);
	struct block_footer *footer = (struct block_footer *)((char *)block + block_size - sizeof(struct block_footer));
	footer->size = block_size;

	if (next_block->size_status != VM_ENDMARK) {
		next_block->size_status &= ~VM_PREVBUSY;
	}
}
