#!/bin/bash

[[ $# -ne 1 ]] && exit 1

OUTPUT_DIR=$1
SEEDS=(0 282 593 740)
X=(5 10 17 30 50 100)
Y=(5 10 30 37 50 200)
STARTX=(0 15 25 50)
STARTY=(0 10 20 30)
START=()

for sx in ${STARTX[@]}; do
    for sy in ${STARTY[@]}; do
        START=(${START[@]} $sx:$sy)
    done
done

for s in ${SEEDS[@]}; do
    for x in ${X[@]}; do
        for y in ${Y[@]}; do
            for start in ${START[@]}; do
                SX=$(echo $start | cut -f1 -d:)
                SY=$(echo $start | cut -f2 -d:)
                [[ $SX -ge $x ]] && SX=$(($x / 2))
                [[ $SY -ge $y ]] && SY=$(($y / 2))
                echo -n "     Seed $(printf %3d $s)"
                echo -n "     Dimensions $(printf %3d:%3d $x $y)"
                echo    "     Start $(printf %3d:%3d $SX $SY)"
                FNAME=${OUTPUT_DIR}/${s}_${x}x${y}
                ./generator -s $s -x $x -y $y -p $FNAME.gen.ppm                 |\
                    tee $FNAME.gen.dat                                          |\
                    ./maze -p $FNAME.maze.ppm                                   |\
                    tee $FNAME.maze.dat                                         |\
                    ./solver -s $SX:$SY -p ${FNAME}_s${SX}x${SY}.solv.ppm       |\
                    tee ${FNAME}_s${SX}x${SY}.solv.dat                          >/dev/null
            done
        done
    done
done
