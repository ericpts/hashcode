#!/bin/bash

rm -rf out
mkdir out

for i in a_example.in  b_should_be_easy.in  c_no_hurry.in  d_metropolis.in  e_high_bonus.in; do
    echo -n "$i - "
    ./main < ../$i > out/$i
done

