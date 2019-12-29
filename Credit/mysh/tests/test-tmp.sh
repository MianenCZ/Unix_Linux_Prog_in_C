#/bin/bash

output=$( ../mysh test-022.mysh )

# We expect an error in the cd command.
(( $? == 0 )) && exit 1

# "cd x x" must have the following string in its error message
echo "$output" | grep "too many"
