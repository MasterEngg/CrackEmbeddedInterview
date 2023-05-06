#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIFO_SIZE   8

#define E_OK         0
#define E_FULL      -1
#define E_EMPTY     -2
#define E_INIT_FAIL -3 // Added this error code 

struct fifo {
    uint8_t data[FIFO_SIZE];
    uint8_t tail;
    uint8_t head;
};

/* I am enqueuing to the circular buffer in the fifo_write() and 
    dequeuing from the circular buffer in the fifo_read() */
    
int fifo_init(struct fifo* f) {
    memset(f->data, 0, FIFO_SIZE); //setting the buffer to zeros
    f->head = E_EMPTY;
    f->tail = E_EMPTY;
    
    return E_OK;
}


int fifo_write(struct fifo *f, uint8_t data) {
    
    if ((f->head == E_EMPTY && f->tail == E_EMPTY)) {
        f->head = 0;
        f->tail = 0;
    } else if ( (f->tail+1)%FIFO_SIZE == f->head) {
        printf("fifo_write: Fifo is full. Write operation failed.\n");
        return E_FULL;
    } else {
        f->tail = (f->tail+1)%FIFO_SIZE;
    }

    f->data[f->tail] = data;
    // printf("fifo_write: Wrote %d to the Fifo.\n", f->data[f->tail]);    
    
    return E_OK; 
}

int fifo_read(struct fifo* f, uint8_t *data) 
{ 
    if (f->head == E_EMPTY) {
        printf("fifo_read: Fifo is empty. Read operation failed.\n");
        return E_EMPTY;
    }  

    *data = f->data[f->head];
    // printf("fifo_read: Read %d from the Fifo.\n", *data);
    
    if (f->head == f->tail) {
        f->head = E_EMPTY;
        f->tail = E_EMPTY;
    } else {
        f->head = (f->head+1)%FIFO_SIZE;
    }
    
    return E_OK;
}


int main() {
    int retCode = E_OK;
    uint8_t data, i;
    struct fifo f;
    
    retCode = fifo_init(&f);
    if(retCode == E_OK)
        printf("main: Fifo inialized successfully \n", data);
    else {
        printf("main: Fifo init failed\n");
        return E_INIT_FAIL;
    }
    
    printf("\n\n");
        
    // Reading Empty buffer and printing error
    retCode = fifo_read(&f, &data);
    if(retCode == E_OK)
        printf("main: Read %d from the Fifo.\n", data);
    else if (retCode == E_EMPTY) {
        printf("main: Fifo is empty. Read operation failed.\n");
    }
    
    printf("\n\n");
    
    // writing more than the buffer size intentionally so that the last write fails
    for(i = 0; i <= FIFO_SIZE; i++) {
        retCode = fifo_write(&f, i);
        if(retCode == E_OK)
            printf("main: Wrote %d to the Fifo.\n", i); 
        else if (retCode == E_FULL) {
            printf("main: Fifo is full\n");
        }
    }

    printf("\n\n");
    
    // Reading Two values to allow more writes
    for(i = 0; i < 2; i++) {
        retCode = fifo_read(&f, &data);
        if(retCode == E_OK)
            printf("main: Read %d from the Fifo.\n", data);
        else if (retCode == E_EMPTY) {
            printf("main: Fifo is empty\n");
        }    
    }   
    
    printf("\n\n");
        
    // Writing one value
    for(i = 0; i < 1; i++) {
        retCode = fifo_write(&f, i);
        if(retCode == E_OK)
            printf("main: Wrote %d to the Fifo.\n", i); 
        else if (retCode == E_FULL) {
            printf("main: Fifo is full\n");
        }
    }
    
    printf("\n\n");
     
    // Reading more values than the buffer size to print error on the last two reads
    for(i = 0; i < FIFO_SIZE; i++) {
        retCode = fifo_read(&f, &data);
        if(retCode == E_OK)
            printf("main: Read %d from the Fifo.\n", data);
        else if (retCode == E_EMPTY) {
            printf("main: Fifo is empty\n");
        }    
    }    
    return E_OK;
}
