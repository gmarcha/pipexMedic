#! /bin/bash

S1="================================================================================"
S2="--------------------------------------------------------------------------------"
GREEN="\033[38;2;57;181;74m"
RED="\033[38;2;222;56;43m"
BLUE="\033[38;2;34;183;235m"
YELLOW="\033[38;2;255;176;0m"
PURPLE="\033[38;2;255;105;180m"
RESET="\033[0m"

INPUT=(basic error concurrency multiple_command mandatory m bonus b all a)
ARG1=""
ARG2="0"

mkdir -p tmp/

printf "$PURPLE$S1\n\n"
echo "██████╗ ██╗██████╗ ███████╗██╗  ██╗    ███╗   ███╗███████╗██████╗ ██╗ ██████╗"
echo "██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝    ████╗ ████║██╔════╝██╔══██╗██║██╔════╝"
echo "██████╔╝██║██████╔╝█████╗   ╚███╔╝     ██╔████╔██║█████╗  ██║  ██║██║██║     "
echo "██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗     ██║╚██╔╝██║██╔══╝  ██║  ██║██║██║     "
echo "██║     ██║██║     ███████╗██╔╝ ██╗    ██║ ╚═╝ ██║███████╗██████╔╝██║╚██████╗"
echo "╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝╚══════╝╚═════╝ ╚═╝ ╚═════╝"
printf "\n$S1$RESET\n"

if [[ -z $1 ]]; then
    echo "pipexMedic: launch with basic, error, concurrency, or multiple_command,"
    echo "            like this bash test.sh basics"
    echo "            there are other aliases, as mandatory/m, bonus/b, all/a"
	echo "            you can launch a specific test with a second argument"
    exit 1
fi

if ! [[ -z $1 ]]; then
    for IN in ${INPUT[@]}; do
        if [[ $1 == $IN ]]; then
            ARG1=$1
        fi
    done
    if [[ -z $ARG1 ]]; then
        echo "Invalid first argument, launch script to know usage."; rm -rf tmp/; exit 1
    fi
fi

if ! [[ -z $2 ]]; then
    re='^[0-9]+$'
    if ! [[ $2 =~ $re ]] ; then
        echo "Invalid second argument, launch script to know usage."; rm -rf tmp/; exit 1
    fi
    ARG2=$2
fi

printf "$BLUE"
make -C ../ all
if [[ $? -ne 0 ]]; then
	rm -rf tmp/; exit 1
fi
printf "$RESET"
printf "$PURPLE$S1$RESET\n"

clang -Wall -Wextra -Werror -fsanitize=address -I ./inc src/{main,test,runTest,utils}.c -o tester
if [[ $? -ne 0 ]]; then
	rm -rf tmp/; exit 1
else
	printf "$YELLOW""Tester ready!$RESET\n"
fi
printf "$PURPLE$S1$RESET\n"

./tester $ARG1 $ARG2

rm -rf tmp/ tester
