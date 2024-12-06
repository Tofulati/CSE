#include "vm.h"
#include "vmlib.h"

void *vmalloc(size_t size) {
	if (size == 0) {
		return NULL;
	}
	
	size_t total_size = ROUND_UP(size + sizeof(struct block_header), BLKALIGN);
	struct block_header *best_fit_block = NULL;
	size_t best_fit_size = 0;

	struct block_header *current = heapstart;
	// Search for best_fit_block
	while (current->size_status != VM_ENDMARK) {
		size_t curr_size = current->size_status & VM_BLKSZMASK;
		
		if ((current->size_status & VM_BUSY) == 0 && (curr_size >= size)) {
			if (best_fit_block == NULL || curr_size < best_fit_size) {
				best_fit_block = current;
				best_fit_size = curr_size;
			}
		}
		current = (struct block_header *)((char *)current + curr_size);	
	}

	if (!best_fit_block) {
		return NULL;
	}

	// Coalesce if previous block is free
	struct block_footer *prev_block_footer = (struct block_footer *)((char *)best_fit_block - sizeof(struct block_footer));	
	if ((best_fit_block->size_status & VM_PREVBUSY) == 0 && prev_block_footer) {
		size_t prev_block_size = prev_block_footer->size;
		size_t new_block_size = prev_block_size + best_fit_size;

		struct block_header *prev_block_header = (struct block_header *)((char *)best_fit_block - prev_block_size);

		prev_block_header->size_status = new_block_size | VM_BUSY | (prev_block_header->size_status & VM_PREVBUSY);
 
		best_fit_block = prev_block_header;
	}

	size_t block_size = best_fit_size;

	// Allocate
	if (block_size > total_size) {
		// Split block if too large
		struct block_header *next_block = (struct block_header *)((char *)best_fit_block + total_size);
		next_block->size_status = (block_size - total_size) | VM_PREVBUSY;
		best_fit_block->size_status = total_size | VM_BUSY | (best_fit_block->size_status & VM_PREVBUSY);
		block_size = total_size;
	} else {
		best_fit_block->size_status = best_fit_size | VM_BUSY | (best_fit_block->size_status & VM_PREVBUSY);
	}

	struct block_header *next_block = (struct block_header *)((char *)best_fit_block + block_size);
	if (next_block->size_status != VM_ENDMARK) {
		next_block->size_status |= VM_PREVBUSY;
	}

	return (void *)((char *)best_fit_block + sizeof(struct block_header));
}
