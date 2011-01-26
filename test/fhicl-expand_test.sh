#!/bin/bash

OUTPUT_FILE=/tmp/out.txt

rm -rf ${OUTPUT_FILE}
fhicl-expand > ${OUTPUT_FILE} <<EOF
EOF
STATUS=$?
[[ ${STATUS}           ]] || exit ${STATUS}
[[ -r ${OUTPUT_FILE}   ]] || exit 11
[[ ! -s ${OUTPUT_FILE} ]] || exit 12

rm -rf ${OUTPUT_FILE}
fhicl-expand - > ${OUTPUT_FILE} <<EOF
hello
EOF
STATUS=$?
[[ ${STATUS}         ]] || exit ${STATUS}
[[ -r ${OUTPUT_FILE} ]] || exit 21
[[ -s ${OUTPUT_FILE} ]] || exit 22

rm -rf ${OUTPUT_FILE}
fhicl-expand a b c > ${OUTPUT_FILE}
STATUS=$?
[[ ${STATUS} == 3 ]] || exit ${STATUS}

rm -rf ${OUTPUT_FILE}
F1=/tmp/F1.txt
F2=/tmp/F2.txt
F3=/tmp/F3.txt
cat - > ${F1} <<EOF
hello
EOF
cat - > ${F2} <<EOF
#include "${F1}"
there
EOF
cat - > ${F3} <<EOF
hello
there
EOF
fhicl-expand ${F2} > ${OUTPUT_FILE}
STATUS=$?
[[ ${STATUS}          ]] || exit ${STATUS}
cmp ${F3} ${OUTPUT_FILE} || exit 31

rm -fr ${OUTPUT_FILE} ${F1} ${F2} ${F3}
exit 0
