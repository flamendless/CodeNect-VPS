u8R"====(## INFORMATION
In *CodeNect*, nodes are stored in order of their creation. So for example, if
you create the following:
  1. Node A
  2. Node B
  3. Node C
And then you *delete Node B* and then *added Node D*, the order is now:
  1. Node A
  2. Node C
  3. Node D

This order does not affect the logic of how nodes are processed but one thing it can affect is the *save* and *load* order for *connections*. An example would be is that if you connected nodes *C* and *A* into a *subtraction* operation, you will notice that if you save and reloaded the project. The *order of operation* for the subtraction will be *A* and *C*. This is because *A* is processed first before *C*.

Using the *sort* feature in *Inspector* will allow users to see the order of nodes and will help them arrange it if needed.
)===="
