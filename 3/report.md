## Ober Cab Services


**Simulate a Ober Cab Service which accepts three inputs (N cabs, M riders and K Payment Server). Ober provides two types of cab service namely pool and premier ride and a rider can chose among the two. Rider completes the ride and then payment is to be done. If there are no Payment server remaining then rider waits until a payment server is free.**

**How to Run** 

> gcc 3.c -pthread -o 3.out
   ./3.out

 
 **Design**

- Cab States
> There are four states for a cab namely **waitState** (no rider in cab), **onRidePremier**, **onRidePoolFull** (pool ride with two riders), **onRidePoolOne** (pool ride with only one rider)
- Rider functionality
> **BookCab**: This function takes three inputs namely ​ **cabType, maxWaitTime, ​RideTime​**. If the maxWaitTime exceeds the current time then the user exits by displaying a message **TimeOut.**
> **Make Payment**: A payment to be made after a rider completes his ride. If all the payment servers are busy then the rider will wait until payment sever get free.

- Driver Functionality
> **Accept Ride**: Driver can accept a ride depending on the state of his cab. If ride is premier cab in ​waitState ​should accept ride and change its state to onRidePremier. ​ If the ride is pool and there is a cab with state ​ onRidePoolOne ​then that cab should accept the ride and changes its state to onRidePoolFull. If the ride is pool and there is no cab with the state ​ onRidePoolOne ​then cab in wait state should accept
ride and change its state to ​onRidePoolOne. ​
> **EndRide**:  The driver ends the ride after completion and waits until the payment start. As soon as payment started, the driver is eligible for taking another ride by changing state of his cab.
- Payment Server

> **Payment Server** : If a payment server is free, payment server accepts a payment and payment time is 2 sec.  

**Assumptions**

- A particular cab can accept both type of riders depending on the cab state.
-  A rider can not ride twice.
- If a rider is unable to get a cab until its MaxWaitTime then he is not again allowed to book the cab.

**Implementation** 

> The code is implemented with the help of two global arrays of struct which is shared between the threads, one for the riders and other for the cabs. Rider struct stores the intime, maxwaittime, ridetime, outtime, cabtype, cabstatus and cabId. whereas, Driver struct stores only CabState of his cab. A global array which PaymentServerStatus tracks which payment server is being used for which cab's payment. Another global variable no_of_riders_left, stores the value of number of riders left, as soon as this variable become zero the program terminates.

>There are two threads one for rider and another for payment. In rider thread, each rider is assigned a random intime, maxwaittime, ridetime and cabtype. If a cab is free and can give a ride to this rider, then the cabstate and riderstate changes. While in payment thread, it waits for a rider who wants to make payment and if the rider has started payment it takes two second to complete the payment and during this time no other rider is able to use this payment server.  