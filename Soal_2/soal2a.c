#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
 
int main() {
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int hasilKali[4][6];

    int row, coloumn, middle;
    int sum = 0;
    
    int mtxA[4][3];
    printf ("INPUT 4x3 MATRIX : \n");
    // Example
    // {{1, 2, 5},
    // {3, 4, 2},
    // {5, 6, 1},
    // {7, 8, 3}
    for (int x=0; x<4; x++) {
        for (int y=0; y<3; y++) {
            scanf ("%d", &mtxA[x][y]);
        }
    }
    
    int mtxB[3][6];
    printf ("INPUT 3x6 MATRIX : \n");
    //Example
    // {{1, 2, 2, 3, 3, 1},
    // {4, 5, 5, 6, 6, 4},
    // {7, 8, 8, 9, 9, 7}}
    for (int x=0; x<3; x++){
        for (int y=0; y<6; y++){
            scanf ("%d", &mtxB[x][y]);
        }
    }
    
    for (row = 0; row < 4; row++) {
      for (coloumn = 0; coloumn < 6; coloumn++) {
        for (middle = 0; middle < 3; middle++) {
          sum = sum + mtxA[row][middle]*mtxB[middle][coloumn];
        }
        hasilKali[row][coloumn] = sum;
        sum = 0;
      }
    }
 
    printf ("Hasil Matriks:\n");
     for (row = 0; row < 4; row++) {
      for (coloumn = 0; coloumn < 5; coloumn++){
        printf ("%d\t", hasilKali[row][coloumn]);
      }
      printf("\n");
    }

    for (row = 0; row < 4; row++) {
      for (coloumn = 0; coloumn < 5; coloumn++) {
      *value = hasilKali[row][coloumn];
      }
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
