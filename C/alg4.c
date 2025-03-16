/*
  Name: Algs 1 - 4
  Author: Wil C
  Date: 18/09/16 00:18
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#define MATCH 1
#define MISMATCH -1
#define GAP_PENALTY -2

#define CORES 8
/* ---------------------------------------------------------------- */
int max(int x, int y, int z) {
	return (x > y? (x > z? x: z): (y > z? y: z));
}
/* ---------------------------------------------------------------- */
int match(char a, char b) {
	return (a == b? MATCH: MISMATCH);
}
/* ---------------------------------------------------------------- */
void alg1(char* str1, char* str2) {
	int cols = strlen(str1);
	int rows = strlen(str2);
  
  int tabl[rows][cols];
	int x;
	int y;
	
	for (y=0; y<rows; y++) {
		tabl[y][0] = y*GAP_PENALTY;
	}
	
	for (x=1; x<cols; x++) {
		tabl[0][x] = x*GAP_PENALTY;
	}
	
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	
	for (y=1; y<rows; y++) {
		for (x=1; x<cols; x++) {
			tabl[y][x] = max(tabl[y-1][x-1] + match(str1[x], str2[y]), tabl[y-1][x] + GAP_PENALTY, tabl[y][x-1] + GAP_PENALTY);
		}
  }
  
  gettimeofday(&stop, NULL);
  printf("Alg1 took %ld.%06ld\n", stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
	/*
  for (y=0; y<rows; y++) {
		for (x=0; x<cols; x++) {
			printf("%i ", tabl[y][x]);
		}
		printf("\n");
  }
	*/
}
/* ---------------------------------------------------------------- */
void alg2(char* str1, char* str2) {
	int cols = strlen(str1);
	int rows = strlen(str2);
	int bans = 2*cols + rows - 1;
	
	int i;
	int j;
	int x;
	int y;
  
  int tab1[rows][cols];
  int tab2[rows][cols];
  
	for (y=0; y<rows; y++) {
		tab1[y][0] = y*GAP_PENALTY;
	}

	for (x=1; x<cols; x++) {
		tab1[0][x] = x*GAP_PENALTY;
	}

	struct timeval stop, start;
	gettimeofday(&start, NULL);

	for (i=rows-1; i<bans; i++) {
		for (j=0; j<rows; j++) {
			x = i - j;
			y = j;

			// printf("[%i, %i]", y, x);

			if (x < rows) {
				
			} else if (x >= cols+rows) {

			} else {
				x -= rows;

				if (x > 0 && y > 0) {
					tab1[y][x] = max(tab1[y-1][x-1] + match(str1[x], str2[y]), tab1[y-1][x] + GAP_PENALTY, tab1[y][x-1] + GAP_PENALTY);

					// printf("[%i, %i]", y, x);
					// printf("[%i, %i]: %i, [%i, %i]: %i, [%i, %i]: %i\n", y-1, x-1, tab1[y-1][x-1], y-1, x, tab1[y-1][x], y, x-1, tab1[y][x-1]);
        }
			}
		}
		// printf("\n");
  }
  
  gettimeofday(&stop, NULL);
  printf("Alg2 took %ld.%06ld\n", stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
	/*
	for (y=0; y<rows; y++) {
		for (x=0; x<cols; x++) {
			printf("%i ", tab1[y][x]);
		}
		printf("\n");
  }
  */
}
/* ---------------------------------------------------------------- */
void alg3_new_thread(int id, char* str1, char* str2, int cols, int rows, int tab1[rows][cols], int tab2[rows][cols]) {
	int bans = 2*cols + rows - 1;
	
	int i;
	int j;
	int x;
	int y;

	for (i=rows+id-2; i<bans; i+=CORES) {
		for (j=0; j<rows; j++) {
			x = i - j;
			y = j;

			if (x < rows) {
				// tab2[y][x] =
			} else if (x >= cols+rows) {

			} else {
				x -= rows;

				if (x > 0 && y > 0) {
					while (tab1[y-1][x-1] == INT_MAX || tab1[y-1][x] == INT_MAX || tab1[y][x-1] == INT_MAX);
					
					tab1[y][x] = max(tab1[y-1][x-1] + match(str1[x], str2[y]), tab1[y-1][x] + GAP_PENALTY, tab1[y][x-1] + GAP_PENALTY);

					// printf("[%i, %i]", y, x);
        }
			}
		}
		// printf("\n");
  }
}

void alg3(char* str1, char* str2) {
  int cols = strlen(str1);
	int rows = strlen(str2);
	
	int x;
	int y;

  int tab1[rows][cols];
  int tab2[rows][cols];

	for (y=0; y<rows; y++) {
		tab1[y][0] = y*GAP_PENALTY;
	}

	for (x=1; x<cols; x++) {
		tab1[0][x] = x*GAP_PENALTY;
	}
	
	for (y=1; y<rows; y++) {
		for (x=1; x<cols; x++) {
			tab1[y][x] = INT_MAX;
		}
  }
	
	int id;
	
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	
	#pragma omp parallel for
	for (id=0; id<CORES; id++) {
		alg3_new_thread(id, str1, str2, cols, rows, tab1, tab2);
	}
	
	gettimeofday(&stop, NULL);
  printf("Alg3 took %ld.%06ld\n", stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
	/*
	for (y=0; y<rows; y++) {
		for (x=0; x<cols; x++) {
			printf("%i ", tab1[y][x]);
		}
		printf("\n");
  }
  */
}
/* ---------------------------------------------------------------- */
int map_x(int x, int y, int w, int h) {
	if (x < h) {
		return h - y - 1; // 2*h - y - 1;
	}
	if (x >= w+h) {
		return 2*w + h - x - 1; // 2*w + 2*h - x - 1;
	}
	return -1;
}

int map_y(int x, int y, int w, int h) {
	if (x < h) {
		return x + y - h + 1;
	}
	if (x >= w+h) {
		return h - y - 1;
	}
	return -1;
}

void alg4_new_thread(int id, char* str1, char* str2, int cols, int rows, int tab1[rows][cols], int tab2[rows][cols]) {
	int i;
	int j;
	int x;
	int y;
	
	int bans = 2*cols + rows - 1;
	
	for (j=id; j<rows; j+=CORES) {
		for (i=rows-j-1; i<2*rows+cols-j-1; i++) {
			x = i - rows;
			y = j;
      
			if (x < 0) {
				x = map_x(i, j, cols, rows);
				y = map_y(i, j, cols, rows);
  			// printf("[i: %i, j: %i, x: %i, y: %i]\n", i, j, x, y);
				tab1[y][x] = match(str1[x], str2[y]);
			} else if (x >= cols) {
				x = map_x(i, j, cols, rows);
				y = map_y(i, j, cols, rows);
        
				if (tab1[y][x] == INT_MAX) {
          // printf("[i: %i, j: %i, x: %i, y: %i]\n", i, j, x, y);
					tab1[y][x] = match(str1[x], str2[y]); // tab1[j%cols][i]
				}
			} else {
        if (x == 0 && y == 0) {
          printf("[i: %i, j: %i, x: %i, y: %i]\n", i, j, x, y);
          tab1[y][x] = max(tab1[y][x], GAP_PENALTY, GAP_PENALTY);
				} else if (x == 0) {
          printf("[i: %i, j: %i, x: %i, y: %i]\n", i, j, x, y);
          tab1[y][0] = max(y*GAP_PENALTY + tab1[y][x], tab1[y-1][x] + GAP_PENALTY, (y+1)*GAP_PENALTY);
				} else if (y == 0) {
          printf("[i: %i, j: %i, x: %i, y: %i]\n", i, j, x, y);
          tab1[0][x] = max(x*GAP_PENALTY + tab1[y][x], (x+1)*GAP_PENALTY, tab1[y][x-1] + GAP_PENALTY);
				} else { // (x > 0 && y > 0)
          printf("[i: %i, j: %i, x: %i, y: %i]\n", i, j, x, y);
					if (tab1[y][x] == INT_MAX) {
						tab1[y][x] = match(str1[x], str2[y]);
					}
					
					tab1[y][x] = max(tab1[y-1][x-1] + tab1[y][x], tab1[y-1][x] + GAP_PENALTY, tab1[y][x-1] + GAP_PENALTY);
					
					// printf("[%i, %i]", y, x);
        }
			}
			#pragma omp barrier
		}
		// printf("\n");
  }
}

void alg4(char* str1, char* str2) {
  int cols = strlen(str1);
	int rows = strlen(str2);

	int x;
	int y;

  int tab1[rows][cols];
  int tab2[rows][cols];
	/*
	for (y=0; y<rows; y++) {
		tab1[y][0] = y*GAP_PENALTY;
	}

	for (x=1; x<cols; x++) {
		tab1[0][x] = x*GAP_PENALTY;
	}
	*/
	for (y=0; y<rows; y++) {
		for (x=0; x<cols; x++) {
			tab1[y][x] = INT_MAX;
		}
  }

	int id;

	struct timeval stop, start;
	gettimeofday(&start, NULL);

	#pragma omp parallel for
	for (id=0; id<CORES; id++) {
		alg4_new_thread(id, str1, str2, cols, rows, tab1, tab2);
	}

	gettimeofday(&stop, NULL);
  printf("Alg4 took %ld.%06ld\n", stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
  
	for (y=0; y<rows; y++) {
		for (x=0; x<cols; x++) {
			printf("%i ", tab1[y][x]);
		}
		printf("\n");
  }
}
/* ---------------------------------------------------------------- */
main() {
  printf("INT_MAX: %i\n\n", INT_MAX);
  
	char* str1 = "acgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgt"; // "acgtacg";
	char* str2 = "acgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgt"; // "acgt";

	alg1(str1, str2);
	// alg2(str1, str2);
  // alg3(str1, str2);
  // alg4(str1, str2);
  
	system("pause");

	return 0;
}
