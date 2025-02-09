#define STD_INPUT  0 /* File descriptor for standard input */
#define STD_OUTPUT 1 /* File descriptor for standard ouput */

pipeline(process1,  process2,  process3) /* Pipes data between three programs */
char    *process1, *process2, *process3; /* Points to program names           */
{
   int fd1[2];      /* Pipe 1 file descriptors */
   int fd2[2];      /* Pipe 2 file descriptors */

   pipe(&fd1[0]);   /* Create the first pipe  */
   pipe(&fd2[0]);   /* Create the second pipe */

   if (fork() != 0) /* Create first child; check to see who is returning from the fork() */
   {
      /* Parent process executes these statements */
      /* ======================================== */
      close(fd1[0]);                /* Process 1 does not need to read from pipe */
      close(STD_OUTPUT);            /* Prepare for new standard output           */
      dup  (fd1[1]);                /* Set standard output to write to the pipe  */
      close(fd1[1]);                /* Write pipe1 no longer needed              */
      close(fd2[0]);                /* Read pipe2 not needed                     */
      close(fd2[1]);                /* Write pipe2 not needed                    */
      execl(process1, process1, 0); /* Turn process1 into user's first command   */
   } 
   if(fork() != 0) /* Create second child; check to see who is returning */
   {
      /* First child process executes these statements */
      /* ============================================= */
      close(fd1[1]);                /* Write pipe1 not needed                    */
      close(fd2[0]);                /* Read pipe2 not needed                     */
      close(STD_INPUT);             /* Prepare for new standard input            */
      dup  (fd1[0]);                /* Set standard output to read from pipe     */
      close(fd1[0]);                /* Read pipe1 no longer needed               */
      close(STD_OUTPUT);            /* Prepare for new standard output           */
      dup  (fd2[1]);                /* Set standard output to write to the pipe  */
      close(fd2[1]);                /* Write pipe2 no longer needed              */
      execl(process2, process2, 0); /* Turn process2 into user's second command  */
   }
   else
   {
      /* Second child process executes these statements */
      /* ============================================== */
      close(fd1[0]);                /* Read pipe1 not needed                        */
      close(fd1[1]);                /* Write pipe1 not needed                       */
      close(fd2[1]);                /* Process 3 does not need to write to the pipe */
      close(STD_INPUT);             /* Prepare for new standard input               */
      dup  (fd2[0]);                /* Set standard input to read from the pipe     */
      close(fd2[0]);                /* This file descriptor not needed anymore      */
      execl(process3, process3, 0); /* Turn process2 into user's second command     */
   }
}