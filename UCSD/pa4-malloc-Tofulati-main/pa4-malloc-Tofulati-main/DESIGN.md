1. An example of this would be multiple allocations of large and small allocations (alternating one after another), then freeing up all the small allocations, and finally calling an allocation of a large byte. This would be a fragmentation error as none of the blocks can coalesce, even with best fit. Such as follows:

```c
	#include <stdio.h>
	#include <stdlib.h>

	void* vmalloc(size_t size);
	void vmfree(void* ptr);
	
	int main() {
		init_heap(214);

		void *ptrs[7];

    		ptrs[0] = vmalloc(50); // 56
    		ptrs[1] = vmalloc(30); // 32
    		ptrs[2] = vmalloc(40); // 48
   		ptrs[3] = vmalloc(60); // 60

		/* Heap after allocation

		[[Hd][56 used][Hd][32 used][Hd][48 used][Hd][60 used][Ft (2 free)]]

		*/

		vmfree(ptrs[1]);
		vmfree(ptrs[3]);
	
		/* Heap after free

		[[Hd][56 used][Hd][Ft (32 free)][Hd][48 used][Hd][Ft (62 free)]]

		*/

		ptrs[4] = vmalloc(40); // 48
		ptrs[5] = vmalloc(10); // 16


		/* Heap after allocation

		[[Hd][56 used][Hd][16 used][Hd][16 free][Hd][48 used][Hd][48 used][Ft (14 free)]]

		*/

		ptrs[6] = vmalloc(20); // 32
		
		// Fragmentation error
	
		return 0;
	}
	
```

2. An example of this would be a calling allocations and then freeing in a specific order. In this case, best case would fill in the last free allocation spot, while the first fit would fit into the large block near the beginning. However, once we call free again, but for the very first pointer, the two blocks in best fit coalesce and form a larger block. Finally, when we call allocate for a large byte size, the best fit heap is the only one that can store the value.

```c
	#include <stdio.h>
	#include <stdlib.h>

	void* vmalloc(size_t size);
	void vmfree(void* ptr);
	
	int main() {
		init_heap(212);

		void *ptrs[8];

    		ptrs[0] = vmalloc(50); // 56
    		ptrs[1] = vmalloc(60); // 60
    		ptrs[2] = vmalloc(40); // 48
   		ptrs[3] = vmalloc(40); // 48

		/* Heap after allocation

		[[Hd][56 used][Hd][60 used][Hd][48 used][Hd][48 used]]

		*/

		vmfree(ptrs[1]);
		vmfree(ptrs[3]);
	
		/* Heap after free

		[[Hd][56 used][Hd][Ft (60 free)][Hd][48 used][Hd][Ft (48 free)]]

		*/

		ptrs[4] = vmalloc(30); // 32
		ptrs[5] = vmalloc(10); // 16


		/* 
		   Best fit Heap after allocation

		[[Hd][56 used][Hd][60 free][Hd][48 used][Hd][32 used][Hd][16 used]]
		   
		   First fit Heap after allocation

		[[Hd][56 used][Hd][32 used][Hd][16 used][Hd][12 free][Hd][48 used][Hd][16 used][Ft (48 free)]]
		*/

		vmfree(ptrs[0]);

		/* 
		   Best fit Heap after allocation

		[[Hd][Ft (116 free)][Hd][48 used][Hd][32 used][Hd][16 used]]
		   
		   First fit Heap after allocation

		[[Hd][Ft (56 free)][Hd][32 used][Hd][16 used][Hd][12 free][Hd][48 used][Hd][16 used][Ft (48 free)]]
		*/

		ptrs[7] = vmalloc(70); // 80

		// Fragmentation only for First fit heap

		return 0;
	}
	
```

RESUBMISSION:
1. Using the example from above (corrected allocation and without mathematical errors [sry]), we get the following example which implements the updated freeing strategy (exactly one next block). We are still comparing best-fit and first-fit as there will be similar issues with choosing the best spot after coalescing. 

```c
        #include <stdio.h>
	#include <stdlib.h>

        void* vmalloc(size_t size);
	void vmfree(void* ptr);

        int main() {                                                                                 
		init_heap(230);
	
		void *ptrs[8];
	
		ptrs[0] = vmalloc(50); // 64
		ptrs[1] = vmalloc(60); // 64 
		ptrs[2] = vmalloc(40); // 48
		ptrs[3] = vmalloc(40); // 48                                                         
		
		/* Heap after allocation                                                               
		
		[[Hd][64 used][Hd][64 used][Hd][48 used][Hd][48 used][Ft (6 free)]]                                                                                                                         
		*/                                                                                                                                                                                            
		vmfree(ptrs[1]);                                                                            
		vmfree(ptrs[3]);                                                                                                                                                                                 
		/* Heap after free                                                                          
		                                                                                           
		[[Hd][64 used][Ft (64 free)][Hd][48 used][Ft (48 free)][Ft (6 free)]]                            
		                                                                                         
		*/                                                                                                                                                                                              
		ptrs[4] = vmalloc(30); // 48 (Hd/Ft + Bytes)                                                               
		ptrs[5] = vmalloc(10); // 32 (Hd/Ft + Bytes)                                                                                                                                                                                                                                           
		/*                                                                                          
			Best fit Heap after allocation 
	
			[[Hd][64 used][Hd][32 used][Ft (32 free)][Hd][48 used][Hd][48 used][Ft (6 free)]]    
			
			First fit Heap after allocation  
	
			[[Hd][64 used][Hd][48 used][Ft (16 free)][Hd][48 used][Hd][32 used][Ft (16 free)][Ft (6 free)]]                                                                                               
		*/                                                                                                                                                                                               
		vmfree(ptrs[2]);                                                                                                                                                                              
		/*                                                                                          
			Best fit Heap after allocation                                           
	
			[[Hd][64 used][Hd][32 used][Ft (80 free)][Hd][48 used][Ft (6 free)]]
	
			First fit Heap after allocation
	
			[[Hd][64 used][Hd][48 used][Ft (64 free)][Hd][32 used][Ft (16 free)][Ft (6 free)]]                                                                                                                                                  
		*/                                                                                                                                                                                              
		ptrs[7] = vmalloc(64); // 80 (Hd/Ft + Bytes)                                                                                                                                                
		// Fragmentation only for First fit heap                                                                                                                                                       
		return 0;
	}
``` 
