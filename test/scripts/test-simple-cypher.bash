#!/bin/bash

cd `dirname $0`
source ./test.bash

test_command 'echo -n "Hello." | simple-cypher -k 0x123 | simple-cypher -k 0x123' 'Hello.'
