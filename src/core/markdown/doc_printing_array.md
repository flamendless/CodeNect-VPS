u8R"====(## EXPLANATION
It can be confusing as to why we can not connect an *Array Node* with a *Print
Node* even though they have the same data type. The reason for this is that
*Array Node* can contain multiple elements of that data type. The way to deal
with this is to *iterate* through the array and print each element manually.
This can be done either using *for-loop* or *while-loop* statements.
## SAMPLE CODE
Here is a *wrong* C code example:
int array[5] = { 1, 2, 3, 4, 5 };
printf("%d\n", array); //error because %d is for a single integer only, but the
array contains 5 integers.
***
Here is a *correct* C code example:
int array[5] = { 1, 2, 3, 4, 5 };

//for-loop method
for (int i = 0 i < 5; i++)
{
  printf("%d\n", array[i]);
}

//while-loop method
int i = 0;
while (i != 5)
{
  printf("%d\n", array[i]);
  i++;
}
***
## KEY POINTS
  * An array is used as a container for multiple values, the values are called *elements*
  * An array element is accessed using an *index* which is an integer
  * Printing in C can only print directly basic data types like:
    * boolean - using the *%d* specifier e.g. *printf("%d", true)*
    * char - using the *%c* specifier e.g. *printf("%c", 'a')*
    * integer - using the *%d* specifier e.g. *printf("%d", 32)*
    * float - using the *%lf* specifier e.g. *printf("%lf", 32.32f)*
    * double - using the *%llf* specifier e.g. *printf("%llf", 323.323232323)*
    * string - using the *%s* specifier e.g. *printf("%s", "Hello")*
)===="
