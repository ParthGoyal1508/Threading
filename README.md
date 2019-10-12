# MULTITHREADING ASSIGNMENT

# Introduction:
	Simulate real life problems using multithreading.

## Author:
	PARTH GOYAL

## Problems:
	
 - Concurrent Quick Sort:
	Problem:
	> Implement concurrent quick sort using forking and multithreading.

	Conclusion:
	> The quick sort which was implemented using multithreading is taking lesser time when compared to the quick sort which was inplemented using forking.
	
	How To Run:
	> Forking:
		gcc 1a.c -o 1a.out
		./1a.out
	 
	> Threading:
		gcc 1b.c -pthread -o 1b.out
		./1b.out
  
	> Normal:
	  gcc 1c.c - o 1c.out
	  ./1c.out

- Automatic Biryani Serving:
	Problem:
	> Simulate an automate biryani serving machine to automate the entire pipeline of serving Biryani.
	
	How To Run:
	> gcc 2.c -pthread -o 2.out
		./2.out

- Ober Cab Services:
	Problem:
	> Simulate a Ober Cab Service which accepts three inputs (N cabs, M riders and K Payment Server). Ober provides two types of cab service namely pool and premier ride. A rider can chose among the two. Riders completes the ride and then payment to be made. If there are no Payment server remaining then rider waits until a payment server is free.
	
	How To Run:
	> gcc 3.c -pthread -o 3.out
		./3.out

