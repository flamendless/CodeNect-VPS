u8R"====(# ARRAY OUT OF BOUNDS
## EXPLANATION
An array out of bounds error occur when the program is *accessing* a memory that
is *out of the scope* of an array. An array is a data structure that contains
multiple values *contiguously* that is, each element is next to each other in
the memory layout. Array elements can be accessed using array index. For most
programming languages, the *index starts at 0* but there are some programming
languages where index starts at 1. Using an index that is less than 0 or
exceeds the size of array leads into accessing other value in memory, and that
is unwanted because of security issues and corruption.
## SAMPLE CODE
Here is a C code example:
```
//here we declare an array of integers that can store up to 5 elements
int array[5];

//here we are going to set values for each element
array[0] = 0; //first element
array[1] = 1; //second element
array[2] = 2; //third element
array[3] = 3; //fourth element
array[4] = 4; //fifth element
array[5] = 5; //index 6 exceeds the size of array which is 5, this results in error
```
## ILLUSTRATION:
Think of array as a container for your lunch, even better think of it as
Japanese bento. Each of the food in your lunch is arranged in a row and column.
Rice, pork, vegetable, and shrimp in order. Each of the food there is an
element of an array. You can access the rice by accessing the first element
(which is the left-most in our illustration), and so on up to the shrimp (which
is the right-most). Now, imagine if you use your fork and stick it past the
shrimps, what will you get? Nothing! At worse, you may hit your seatmate's
hand!
)===="
