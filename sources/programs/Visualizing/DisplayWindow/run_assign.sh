#!/bin/bash

mkfifo foobar
../../Generators/Random/gen_random 15 30 | ../AssignCoordinates/assign_coordinates 2> foobar &
cat foobar | ./display
rm -f foobar