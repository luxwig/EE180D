#!/bin/bash
rm -rf output.txt
while IFS='' read -r line || [[ -n "${line}" ]]; do
    echo -n "RUNNING ${line}"
    echo "--- ${line} ---" 1>>output.txt
    echo -e "\t EC: $?"
    ./cf_main "${line}" 2>/dev/null 1>> output.txt
done < "testfile.list"
