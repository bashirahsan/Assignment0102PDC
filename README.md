# Assignment0102PDC
This was an assignment of Parallel and distributed computing for spring 2020

# Problem # 1

The consensus problem in distributed systems is the problem of getting a set of nodes to agree on something – it might be a value, a course of action or a decision. This problem has been at the core of distributed systems research for over twenty years. A Simple solution, which does work under some constraints, is called two-phase commit, or 2PC.
Think about how you would solve a real world consensus problem – perhaps trying to arrange a graduate party! You’d call all your friends and suggest a date and a time. If that date/time is good for everybody you have to ring again and confirm, but if someone can’t make it you need to call everybody again and tell all your friends that the party is off. You might at the same time suggest a new date and time, which then kicks off another round of consensus. More specifically, we can identify two steps in the process:
1. The first proposal phase involves proposing a value to every participant in the system and gathering responses.
2. If everyone agrees, contact every participant again to let him or her know. Otherwise, contact every participant to abort the consensus.

#Problem # 2

Master distributes the list and waits for anyone to signal successful search. Once a slave process finds the key, it stops execution, sends to master, it notifies other slaves to stop and the program terminates.
