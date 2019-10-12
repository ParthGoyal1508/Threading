## Automating Biryani Serving


**Simulate an automate biryani serving machine to automate the entire pipeline of serving Biryani.**

**How to Run** 

> gcc 2.c -pthread -o 2.out
   ./2.out

 
 **Design**

- Robot Chefs
> There will be​ **M** Robot Chefs ​ present in the kitchen preparing vessels of Biryani.
> Each Robot Chef can prepare a random number (greater than or equal to 1)
of Biryani vessels at a time.
> Each Biryani vessel has a capacity to serve ​ **P** Students​.
- Serving Tables
> There will be​ **N Serving Tables**​ present in the mess.
> Each Serving Table has a ​ **Serving Container​** which loads a Biryani vessel
prepared by any of our Robot Chefs. Only one vessel of Biryani can be loaded
at a time into the Serving Container. The Serving Container can be refilled
only when it is empty.
- Student
> **​K Students** ​ have registered for the Biryani and they can come in any order and any random time.
 

**Assumptions**

- Each particular table has a fixed capacity.

**Implementation** 

> The code is implemented with the help of three global arrays of struct which is shared between the threads, one for the robots, second for the serving tables and another for the students. Robot struct stores the waiTime, r(no. of vessel that particular robot can cook), p(no. of students that can be served by a vessel), mode(state of the robot).  whereas, Serving table struct stores mode, portions, max_no_of_student, no_of_student_serving. Student struct stores whether a student is arrived, tableno, iscomplete.

>The program takes tables, robots and students as individual threads.
The robots prepare r biriyanis in w time. Now, it calls the biryani ready function where the number of portions is decided.
The tables wait for a signal from the robots to get assigned.
Whenever a robot finds a table free, it allocates a vessel to the table and the table is marked as busy.
A student waits for slots. When ready_to_serve_table() function is called, the student moves to their slot and gets served.
Once a student is served, he is marked as served on an external array.
When all students are marked as served, the simulation ends.
