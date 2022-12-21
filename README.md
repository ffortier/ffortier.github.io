# Udemy Developing a Multithreaded Kernel From Scratch

https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

Following the course and adding few improvements

* Simplify the `Makefile` to use pattern matching and improve the change detection
* Add unit tests with custom headers ([`testsupport.h`](./src/testsupport.h)) 
* Fix random issues (memory leaks and seg faults)
* Change the path parser logic to use less memory allocations
* Use macros to standardize the error management