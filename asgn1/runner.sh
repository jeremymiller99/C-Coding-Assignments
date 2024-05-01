#!/bin/bash
VERBOSE=0
EXACTERR=0
COLOR=1

for script in $(find tests -name 'test_*' ); do
    cp $script current_test.sh
    scriptname=$(basename $script .sh)
    err=$(bash current_test.sh 2> /dev/null)

    if [ $? -eq 0 ]
    then
        if [ $VERBOSE -eq 1 ] ; then
            if [ $COLOR -eq 1 ] ; then
                echo -e "\033[32mPASS\033[0m:\t$scriptname"
                if [ $EXACTERR -ne 0 ] ; then
                    echo Message:
                    echo -e "\033[33m$err\033[0m"
                fi
            else
                echo -e "PASS:\t$scriptname"
                if [ $EXACTERR -ne 0 ] ; then
                    echo Message:
                    echo $err
                fi
            fi
        fi
    else
        if [ $COLOR -eq 1 ] ; then
            echo -e "\033[31mFAIL\033[0m:\t$scriptname"
            if [ $EXACTERR -ne 0 ] ; then
                echo Message:
                echo -e "\033[33m$err\033[0m"
            fi
        else
            echo -e "FAIL\t$scriptname"
            if [ $EXACTERR -ne 0 ] ; then
                echo Message:
                echo $err
            fi
        fi
    fi
    rm current_test.sh
	
done