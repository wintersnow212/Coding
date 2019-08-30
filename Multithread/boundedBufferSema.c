//http://www.csl.mtu.edu/cs4411.ck/www/NOTES/threads/buffer.html
/* ----------------------------------------------------------------- */
/* PROGRAM  buffer.c                                                 */
/*    This is a solution to the bounded buffer problem.  Producers   */
/* keep sending positive integers to a buffer and at the end EOD is  */
/* sent indicating the end of data   Consumers keep reading data     */
/* from the buffer until they see EOD.  EOD is defined to be a       */
/* negative number.                                                  */
/* ----------------------------------------------------------------- */

#include  <stdio.h>
#include  <stdlib.h>
#include  <thread.h>
#include  <synch.h>

#define   MAX_SIZE      5                /* buffer size              */
#define   MAX_THREADS   5                /* # of producers/consumers */
#define   EOD          -1                /* end of data mark         */

/* ----------------------------------------------------------------- */
/*                              Semaphores                           */
/* ----------------------------------------------------------------- */

sema_t    UntilNotFull;                  /* if buffer is not full    */
sema_t    UntilNotEmpty;                 /* if buffer is not empty   */
sema_t    BufferLock;                    /* to lock the buffer       */
sema_t    ScreenLock;                    /* to lock the screen       */

/* ----------------------------------------------------------------- */
/*                              The buffer                           */
/* ----------------------------------------------------------------- */

int       Buffer[MAX_SIZE];              /* the buffer array         */
int       in, out;                       /* pointers in and out      */

int       Max_Run;                       /* max # of deposit/withdraw*/

/* ----------------------------------------------------------------- */
/* FUNCTION  Fill()                                                  */
/*    This function fills a char array with 'n' spaces.              */
/* ----------------------------------------------------------------- */

void  Fill(char x[], int n)
{
     int  i;

     for (i = 0; i < n*2; i++)
          x[i] = ' ';
     x[i] = '\0';
}

/* ----------------------------------------------------------------- */
/* FUNCTION  Producer()                                              */
/*    This is the producer thread.  It sends out i*10000_j into the  */
/* buffer, where i is the producer number and j is in the range of   */
/* 0 and Max_Run-1.  After this, it sends in EOD.                    */
/* ----------------------------------------------------------------- */

void *Producer(void *voidPTR)
{
     int   *intPTR = (int *) voidPTR;
     int   ID      = *intPTR;
     int   i, data;
     char  filler[100];

     Fill(filler, ID);
     sema_wait(&ScreenLock);
          printf("%sProducer %d started ...\n", filler, ID);
     sema_post(&ScreenLock);

     for (i = 0; i < Max_Run; i++) {     /* for each iteration       */
          thr_yield();                   /* rest for unspecified time*/
          sema_wait(&ScreenLock);        /* lock the screen          */
               printf("%sProducer %d is waiting for an empty slot\n",
                       filler, ID);      /* display a message        */
          sema_post(&ScreenLock);        /* release the screen       */

          data = ID*10000 + i;           /* generate a data item     */
          sema_wait(&UntilNotFull);      /* wait until buffer !full  */
               sema_wait(&BufferLock);   /* lock the buffer          */
                    Buffer[in] = data;   /* deposit data             */
                    in         = (in + 1) % MAX_SIZE; /* advance in  */
                    printf("%sProducer %d has added %d to the buffer\n",
                            filler, ID, data);
               sema_post(&BufferLock);   /* release buffer           */
          sema_post(&UntilNotEmpty);     /* tells consumer data is in*/
     }

     thr_yield();                        /* rest for unspecified time*/
     sema_wait(&UntilNotFull);           /* done here.  send EOD     */
          sema_wait(&BufferLock);        /* it is the same as above  */
               Buffer[in] = EOD;
               in = (in + 1) % MAX_SIZE;
               printf("%sProducer %d adds EOD and exits\n",
                      filler, ID);
          sema_post(&BufferLock);
     sema_post(&UntilNotEmpty);
     thr_exit(0);
}

/* ----------------------------------------------------------------- */
/* FUNCTION  Consumer()                                              */
/*    This is the consumer thread.  It keeps taking a data item from */
/* the buffer until the EOD mark is seen.                            */
/* ----------------------------------------------------------------- */

void *Consumer(void *voidPTR)
{
     int   *intPTR = (int *) voidPTR;
     int   ID      = *intPTR;
     int   i, data;
     char  filler[100];

     Fill(filler, ID+10);
     sema_wait(&ScreenLock);
          printf("%sConsumer %d started ...\n", filler, ID);
     sema_post(&ScreenLock);

     do {                                /* iterate until EOD        */
          thr_yield();                   /* rest for unspecified time*/
          sema_wait(&ScreenLock);        /* lock the screen          */
               printf("%sConsumer %d is waiting for an item\n",
                       filler, ID);      /* display a message        */
          sema_post(&ScreenLock);        /* release the screen       */

          sema_wait(&UntilNotEmpty);     /* wait until something in  */
               sema_wait(&BufferLock);   /* lock the semaphore       */
                    data = Buffer[out];  /* take the data item       */
                    out  = (out + 1) % MAX_SIZE;   /* advance out    */
                    if (data != EOD)     /* if it is not EOD         */
                         printf("%sConsumer %d has taken %d from the buffer\n",
                                filler, ID, data);  /* display data  */
               sema_post(&BufferLock);   /* release the buffer       */
          sema_post(&UntilNotFull);      /* tell producer slot avail */
     } while (data != EOD);              /* do until EOD is seen     */

     sema_wait(&ScreenLock);
          printf("%sConsumer %d receives EOD and exits\n", filler, ID);
     sema_post(&ScreenLock);
     thr_exit(0);
}

/* ----------------------------------------------------------------- */
/*                        The main program                           */
/* ----------------------------------------------------------------- */

void  main(int argc, char *argv[])
{
     thread_t   pID[MAX_THREADS];        /* producer ID              */
     thread_t   cID[MAX_THREADS];        /* consumer ID              */
     size_t     pStatus[MAX_THREADS];    /* producer status          */
     size_t     cStatus[MAX_THREADS];    /* consumer status          */
     int        pArg[MAX_THREADS];       /* producer argument        */
     int        cArg[MAX_THREADS];       /* consumer argument        */
     int        Threads;                 /* # of producers/consumers */
     int        i;

     if (argc != 3)  {
          printf("Use %s #-of-iterations #-of-producers/consumers\n", argv[0]);
          exit(0);
     }
     Max_Run = abs(atoi(argv[1]));
     Threads = abs(atoi(argv[2]));
     if (Threads > MAX_THREADS) {
          printf("The no. of producers/consumers is too large.  Reset to %d\n",
                 MAX_THREADS);
          Threads = MAX_THREADS;
     }

     printf("Parent started ...\n");

     /* initialize semaphores:                                       */
     /*    ScreenLock    - 1 (at most 1 thread can display)          */
     /*    BufferLock    - 1 (at most 1 thread can access buffer)    */
     /*    UntilNotFull  - 1 (at most MAX_SIZE threads can put data) */
     /*    UntilNotEmpty - 0 (no consumer can take data at beginning)*/

     sema_init(&ScreenLock, 1, USYNC_THREAD, (void *) NULL);
     sema_init(&BufferLock, 1, USYNC_THREAD, (void *) NULL);
     sema_init(&UntilNotFull, MAX_SIZE, USYNC_THREAD, (void *) NULL);
     sema_init(&UntilNotEmpty, 0, USYNC_THREAD, (void *) NULL);

     in = out = 0;                       /* in and out start with 0  */

     for (i = 0; i < Threads; i++) {     /* start consumers first    */
          cArg[i] = i + 1;
          thr_create(NULL, 0, Consumer, (void *) &(cArg[i]),
                     0, (void *) &(cID[i]));
     }
     for (i = 0; i < Threads; i++) {     /* followed by producers    */
          pArg[i] = i + 1;
          thr_create(NULL, 0, Producer, (void *) &(pArg[i]),
                     0, (void *) &(pID[i]));
     }

     for (i = 0; i < Threads; i++)       /* wait producers/consumers */
          thr_join(cID[i], 0, (void *) &(cStatus[i]));
     for (i = 0; i < Threads; i++)
          thr_join(pID[i], 0, (void *) &(pStatus[i]));

     printf("Parent exits ...\n");
}