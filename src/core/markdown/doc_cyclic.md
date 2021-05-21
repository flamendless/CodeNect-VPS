u8R"====(## EXPLANATION
A *cyclic nodes error* happen when user makes a new *connection* to a node that is already a dependency or input of the node. *Cyclic nodes* are not allowed in CodeNect because it will not make sense due the order of nodes to evaluate and process for its internal logic. CodeNect's internal processing system will be stuck in a loop because it will repeat infinitely due to the cycle.

A solution to this is to use *Set Node* to *set* a *Variable Node* value instead of making a connection.
## KEY POINTS
  * A *cyclic nodes error* will only confuse the user as to what is happening
  	with the flow of logic
  * Use *Set Node* to set a *Variable Node*'s value instead of using a connection
)===="
