#!/bin/bash

MYDIR="$(dirname $0)"
EXIT=0

[[ ! -d ./examples ]] && echo Error: No ./examples directory 1>&2 && exit 5

# Make a temp directory
DIR=$(mktemp)
rm $DIR
mkdir $DIR
[[ $? -ne 0 ]] && exit 1
echo Using temporary directory $DIR

# Generate examples in that dir
echo Attempting to generate the same examples ... && sleep 2
"$MYDIR/mkexamples.sh" $DIR

# Check Maze Images
echo Checking maze images
for file in ./examples/*.maze.ppm; do
    BASE=$(basename $file)
    diff -bB "./examples/$BASE" "$DIR/$BASE" 2>&1 >/dev/null
    [[ $? -ne 0 ]] && echo Failure in test $BASE && EXIT=1
done

# Check serialized dump for diffs after sorting
echo Checking maze dumps
for file in ./examples/*.maze.dat; do
    BASE=$(basename $file)
    diff <(sort "./examples/$BASE") <(sort "$DIR/$BASE") 2>&1 >/dev/null
    [[ $? -ne 0 ]] && echo Failure in test $BASE && EXIT=1
done

echo Checking solved dumps
for file in ./examples/*.solv.dat; do
    BASE=$(basename $file)
    diff <(sort "./examples/$BASE") <(sort "$DIR/$BASE") 2>&1 >/dev/null
    [[ $? -ne 0 ]] && echo Failure in test $BASE && EXIT=1
done

exit $EXIT
