u8R"====(## EXPLANATION
It can be confusing as to why we can not use *Variable Node* and assign it with an array from *Array Node* since they are using the same data type. The reason for this is that *Array Node* can contain *multiple* elements of that data type while a *Variable Node* can only store and reference a *single* value. One way that a *Variable Node* can store from an *Array Node* is by a *single element* only using *array indexing*.
## SAMPLE CODE
Here is a *wrong* C code example:
int array[5] = { 1, 2, 3, 4, 5 };
int array2 = array; //ERROR 'int' is a single storage unit while 'int []' is an array
***
Here is a *correct* C code example:
int array[5] = { 1, 2, 3, 4, 5 };
int first_element = array[0];
int second_element = array[1];
int array2[5] = {array[0], array[1], array[2], array[3], array[4]};

//or better:
int array3[5];
for (int i = 0; i < 5; i++)
{
  array3[i] = array[i];
}
***
## KEY POINTS
  * A variable can only store a single value
  * An array is used as a container for multiple values, the values are called *elements*
  * An array element is accessed using an *index* which is an integer
)===="
