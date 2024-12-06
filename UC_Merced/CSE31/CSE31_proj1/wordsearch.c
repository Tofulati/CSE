#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char **arr);
void searchPuzzle(char **arr, char *word);
int bSize;

typedef struct {
  int row;
  int col;
} Position;

// Main function, DO NOT MODIFY
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
    return 2;
  }
  int i, j;
  FILE *fptr;

  // Open file for reading puzzle
  fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    printf("Cannot Open Puzzle File!\n");
    return 0;
  }

  // Read the size of the puzzle block
  fscanf(fptr, "%d\n", &bSize);

  // Allocate space for the puzzle block and the word to be searched
  char **block = (char **)malloc(bSize * sizeof(char *));
  char *word = (char *)malloc(20 * sizeof(char));

  // Read puzzle block into 2D arrays
  for (i = 0; i < bSize; i++) {
    *(block + i) = (char *)malloc(bSize * sizeof(char));
    for (j = 0; j < bSize - 1; ++j) {
      fscanf(fptr, "%c ", *(block + i) + j);
    }
    fscanf(fptr, "%c \n", *(block + i) + j);
  }
  fclose(fptr);

  printf("Enter the word to search: ");
  scanf("%s", word);

  // Print out original puzzle grid
  printf("\nPrinting puzzle before search:\n");
  printPuzzle(block);

  // Call searchPuzzle to the word in the puzzle
  searchPuzzle(block, word);

  return 0;
}

void printPuzzle(char **arr) {
  // This function will print out the complete puzzle grid (arr).
  // It must produce the output in the SAME format as the samples
  // in the instructions.
  // Your implementation here...
  for (int row = 0; row < bSize; row++) {
    for (int col = 0; col < bSize; col++) {
      printf("%c ", *(*(arr + row) + col));
    }
    printf("\n");
  }
  printf("\n");
}

char *toUpper(char *word) {
  for (int i = 0; i < strlen(word); i++) {
    if (*(word + i) >= 'a' && *(word + i) <= 'z') {
      *(word + i) = *(word + i) - 32;
    }
  }
  return word;
}

void findPath(char **arr, Position **result, Position pos, char *word,
              int wordIndex, int resultIndex) {
  if (wordIndex < strlen(word)) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if (i == 0 && j == 0) {
          continue;
        } else if (pos.row + i < 0 || pos.row + i >= bSize || pos.col + j < 0 ||
                   pos.col + j >= bSize) {
          continue;
        } else if (*(*(arr + pos.row + i) + pos.col + j) !=
                   *(word + wordIndex)) {
          continue;
        }

        Position found = (Position){pos.row + i, pos.col + j};
        *(*(result + resultIndex) + wordIndex) = found;
        findPath(arr, result, found, word, wordIndex + 1, resultIndex);
      }
    }
  }
}

void searchPuzzle(char **arr, char *word) {
  // This function checks if arr contains the search word. If the
  // word appears in arr, it will print out a message and the path
  // as shown in the sample runs. If not found, it will print a
  // different message as shown in the sample runs.
  // Your implementation here...
  int **final = (int **)malloc(bSize * sizeof(int *));
  Position *first = (Position *)malloc((bSize * bSize) * sizeof(Position));
  Position **result = (Position **)malloc((bSize * bSize) * sizeof(Position *));
  int occurances = 0;
  word = toUpper(word);

  for (int i = 0; i < bSize; i++) {
    for (int j = 0; j < bSize; j++) {
      if (*(*(arr + i) + j) == *word) {
        *(first + occurances) = (Position){i, j};
        occurances++;
      }
    }
  }

  // initialize results array with invalid
  for (int i = 0; i < occurances; i++) {
    *(result + i) = (Position *)malloc((strlen(word)) * (sizeof(Position)));
    for (int j = 1; j < strlen(word); j++) {
      *(*(result + i) + j) = (Position){-1, -1};
    }
    *(*(result + i)) = *(first + i);
  }

  // initialize final array with 0s
  for (int i = 0; i < bSize; i++) {
    *(final + i) = (int *)malloc(bSize * sizeof(int));
    for (int j = 0; j < bSize; j++) {
      *(*(final + i) + j) = 0;
    }
  }

  // look at first occurances and search
  for (int i = 0; i < occurances; i++) {
    findPath(arr, result, *(first + i), word, 1, i);
  }

  int found = 0;
  for (int i = 0; i < occurances; i++) {
    if ((*(result + i) + strlen(word) - 1)->col == -1) {
      continue;
    }

    for (int j = 0; j < strlen(word); j++) {
      int *curr =
          *(final + (*(result + i) + j)->row) + (*(result + i) + j)->col;
      if (*curr != 0) {
        *curr = (j + 1) * pow(10, (int)log10(*curr) + 1) + *curr;
      } else {
        *curr = j + 1;
      }
    }
    found++;
  }

  // print if found
  if (found != 0) {
    printf("Word found!\n");
    printf("Printing the search path:\n");

    for (int i = 0; i < bSize; i++) {
      for (int j = 0; j < bSize; j++) {
        printf("%-8d", *(*(final + i) + j));
      }
      printf("\n");
    }
    printf("\n");

  } else {
    printf("Word not found!\n");
  }

  for (int i = 0; i < bSize; i++) {
    free(*(final + i));
  }
  free(final);
  free(first);
}
