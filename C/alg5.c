/*
  Name: Alg 5
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

// A1
#define R1 (i < m)
// A3
#define R2 (0 <= i + j - n - m)
#define R3 (i < n + m)
#define R4 (j < m - 1)

// A4
#define R5 (i == n + m)

// B1
#define R6 (m <= i)
#define R7 (i < 2*m - j)

// B2
#define R8 (2*m - j <= i)
#define R9 (i < n + m - j)

// B3
#define RA (n + m < i)

// B4
// r2
#define RB (j == m - 1)

#define CORES 8

int k = 0;

/* ---------------------------------------------------------------- */
int max(int x, int y, int z) {
	return (x > y? (x > z? x: z): (y > z? y: z));
}
/* ---------------------------------------------------------------- */
int match(char a, char b) {
	return (a == b? MATCH: MISMATCH);
}
/* ---------------------------------------------------------------- */
int map_x(int x, int y, int w, int h) {
	if (x < h) {
		return h - y - 1;
	}
	if (x >= w+h) {
		return x + y + 1 - 2*h; // 2*w + h - x - 1;
	}
	return -1;
}

int map_y(int x, int y, int w, int h) {
	if (x < h) {
		return x + y - h + 1;
	}
	if (x >= w+h) {
		return w + 2*h - x - 1; // h - y - 1;
	}
	return -1;
}

void alg5_new_thread(char* s1, char* s2, int n, int m, int t[m][n]) {
	int i;
	int j = 0;
	int x;
	int y;
	
	int w = 2*m + n - 1;
	int h = m;
	
	int l = m*(n + m);
	
	while (k < l) {
    #pragma omp critical(inc_k)
		i = k++; // l =
    
    j = i%m;
    i = (i-j)/m;

		i = i - j + h - 1; // i + h - 1 - j;
		
    
		if (R1) {
			// A1
			x = map_x(i, j, n, m);
			y = map_y(i, j, n, m);
			if (x != 0 && y != 0) {
				t[y][x] = match(s1[x-1], s2[y-1]);
			}
		}
		if (R2) {
			if (RB) {
				// B4
				x = i - m;
				y = j;
				
				t[y][x] = max(t[y-1][x-1] + t[y][x], t[y-1][x] + GAP_PENALTY, t[y][x-1] + GAP_PENALTY); // l;
			}
			if (R3 && R4) {
				// A3
				x = i - m;
				y = j;
				
				t[y][x] = match(s1[x-1], s2[y-1]);
			}
		}
		if (R5) {
			// A4
			x = map_x(i, j, n, m);
			y = map_y(i, j, n, m);
			
			t[y][x] = match(s1[x-1], s2[y-1]);
		}
		if (RA) {
			// B3
			x = map_x(i, j, n, m);
			y = map_y(i, j, n, m);
			
			t[y][x] = max(t[y-1][x-1] + t[y][x], t[y-1][x] + GAP_PENALTY, t[y][x-1] + GAP_PENALTY); // l;
		}
		if (R6 && R7) {
			// B1
			x = i - m;
			y = j;
			if (x != 0 && y != 0) {
				t[y][x] = max(t[y-1][x-1] + t[y][x], t[y-1][x] + GAP_PENALTY, t[y][x-1] + GAP_PENALTY); // l;
			} else if (x == 0) {
        t[y][x] = y*GAP_PENALTY;
			} else {
        t[y][x] = x*GAP_PENALTY;
			}
		}
		if (R8 && R9) {
			// B2
			x = i - m;
			y = j;
			if (x != 0 && y != 0) {
				t[y][x] = max(t[y-1][x-1] + match(s1[x-1], s2[y-1]), t[y-1][x] + GAP_PENALTY, t[y][x-1] + GAP_PENALTY); // l;
			} else if (x == 0) {
        t[y][x] = y*GAP_PENALTY;
			} else {
        t[y][x] = x*GAP_PENALTY;
			}
		}
		
		#pragma omp barrier
		// restriccion: h >= P (h cuadrantes)
  }
}

void alg5(char* s1, char* s2) {
  int n = strlen(s1) + 1;
	int m = strlen(s2) + 1;

	int x;
	int y;

  int t[m][n];

	int i;

	struct timeval stop, start;
	gettimeofday(&start, NULL);

	#pragma omp parallel for shared(k, t)
	for (i=0; i<CORES; i++) {
		alg5_new_thread(s1, s2, n, m, t);
	}

	gettimeofday(&stop, NULL);
  printf("Alg5 took %ld.%06ld\n", stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
  /*
	for (y=0; y<m; y++) {
		for (x=0; x<n; x++) {
			printf("%i,", t[y][x]);
		}
		printf("\n");
  }
	*/
}
/* ---------------------------------------------------------------- */
main() {
  printf("INT_MAX: %i\n\n", INT_MAX);
  
	char* s1 = "acgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacg"; // "acgtacg";
	char* s2 = "acgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacgtacg"; // "acgt";

  alg5(s1, s2);
  
	system("pause");

	return 0;
}
