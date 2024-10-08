--------------------------- MODULE ProducersConsumers ---------------------------
EXTENDS Naturals, Sequences, FiniteSets

CONSTANTS Producers,   (* the set of producers *)
          Consumers,   (* the set of consumers *)
          BufCapacity  (* the maximum number of elements in the buffer *)

-----------------------------------------------------------------------------

(* Variables of our system that define current state *)
VARIABLES buffer, waitSet
vars == <<buffer, waitSet>>

(* All non-waiting threads *)
RunningThreads ==
    (Producers \cup Consumers) \ waitSet

Signal ==
    IF waitSet /= {}
    THEN \E x \in waitSet: waitSet' = waitSet \ {x}
    ELSE UNCHANGED waitSet

Wait(thread) ==
    /\ waitSet' = waitSet \cup {thread}
    /\ UNCHANGED <<buffer>>

-----------------------------------------------------------------------------

Produce(thread, data) ==
   \/ /\ Len(buffer) = BufCapacity
      /\ Wait(thread)
   \/ /\ Len(buffer) < BufCapacity
      /\ buffer' = Append(buffer, data)
      /\ Signal

Consume(thread) ==
   \/ /\ buffer = <<>>
      /\ Wait(thread)
   \/ /\ buffer /= <<>>
      /\ buffer' = Tail(buffer)
      /\ Signal

-----------------------------------------------------------------------------

(* Initial state of the system *)
Init ==
    /\ buffer = <<>>
    /\ waitSet = {}

(* All possible actions in our system *)
Next ==
    \E thread \in RunningThreads:
            \/ /\ thread \in Producers
               /\ Produce(thread, thread) \* Add some data to buffer
            \/ /\ thread \in Consumers
               /\ Consume(thread)

-----------------------------------------------------------------------------

(* What we check *)
NoDeadlock ==
    waitSet /= (Producers \cup Consumers)

=============================================================================
