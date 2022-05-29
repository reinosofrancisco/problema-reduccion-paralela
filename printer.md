```
if (ID == 1)
        {
            for (i = 0; i < (slaveSize / DIM); i++)
            {
                printf("\n\t| ");
                for (j = 0; j < DIM; j++)
                    printf("%2f ", A[i * DIM + j]);
                printf("|");
            }
            printf("\n");
        }
```