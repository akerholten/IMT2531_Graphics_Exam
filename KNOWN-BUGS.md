# Severity explained:

level 1: Balance issues or minor visual issues.

level 2: A nuisance for the user, either from a technical perspective or mechanical one.

level 3: Program breaking in nature. Will probably have the same origin as a level 2 bug.


###### Bug template:
- Description: *Mandatory
- Possible solution: *Optional
- Cause: *Optional *guessing is allowed


## LEVEL 3:



## LEVEL 2:



## LEVEL 1:

###### Last pixel when loading heightmap has incorrect value
- Description: When loading the heightmap, the last pixel loaded is of a slightly to high value (should be >= 1, but is ~1.17)
- Possible solution: Change for-loop for indexing?
- Cause: Guess: Might be indexing pixels incorrect
