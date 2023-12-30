# ⟫⟫⟫ Operating System project
⟫⟫⟫ team work :  
  
  ※ Boudaoud Hamza Zakaria  
  ※ Mokrani Mohamed Amine  
  ※ Bessire Dina  

# Q1 :  
We use a two-dimensional array to represent the matrices A, B and C. and a circular queue to represent the buffer T (BufferQueue and the items inside it as BufferItem).  

# Q2 :  
Each producer thread must process a distinct section of matrix B and C. Thus, data sharing is done by dividing the tasks. We also use semaphores to protect and synchronize access to shared data (result matrix and buffer) and lock the critical sections. And suspend the producer threads when the buffer is full and the consumer threads when the buffer is empty.  

# Q3 :  
Current implementation does not have any predictable risks, because we are using semaphores to carefully protect and synchronize access to shared data (result matrix and buffer) and lock the critical sections.  

###  
# ⟫⟫⟫ About :  
The Operating System project involves teamwork with members Boudaoud Hamza Zakaria, Mokrani Mohamed Amine, and Bessire Dina. They use a two-dimensional array for matrices A, B, and C, employing a circular queue for buffer T. Each producer thread handles a distinct section of matrices B and C, with semaphores ensuring protected and synchronized access to shared data. The implementation currently lacks predictable risks due to careful use of semaphores and critical section locks.

