#! /bin/bash

S1="================================================================================"
S2="--------------------------------------------------------------------------------"
GREEN="\033[38;2;57;181;74m"
RED="\033[38;2;222;56;43m"
BLUE="\033[38;2;34;183;235m"
YELLOW="\033[38;2;255;176;0m"
PURPLE="\033[38;2;255;105;180m"
RESET="\033[0m"

comp() {
	printf "$RED"; diff -u $1 $2; printf "$RESET"
	if [ $? -eq 0 ]; then
		printf "$GREEN""OK !$RESET\n"
	fi
}

echo -n "hello\nhello world\nhell\n" > test1
echo -n "hello\nhello world\nhell\n" > user1

printf "$YELLOW$S1\n\n"
echo "██████╗ ██╗██████╗ ███████╗██╗  ██╗    ███╗   ███╗███████╗██████╗ ██╗ ██████╗"
echo "██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝    ████╗ ████║██╔════╝██╔══██╗██║██╔════╝"
echo "██████╔╝██║██████╔╝█████╗   ╚███╔╝     ██╔████╔██║█████╗  ██║  ██║██║██║     "
echo "██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗     ██║╚██╔╝██║██╔══╝  ██║  ██║██║██║     "
echo "██║     ██║██║     ███████╗██╔╝ ██╗    ██║ ╚═╝ ██║███████╗██████╔╝██║╚██████╗"
echo "╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝╚══════╝╚═════╝ ╚═╝ ╚═════╝"
printf "\n$S1$RESET\n"
printf "$BLUE"; make -C ../ all; printf "$RESET"

printf "$YELLOW$S1$RESET\n"
printf "$PURPLE""test: < infile grep hello | awk '{count++} END {print count}' > outfile$RESET\n"
printf "$PURPLE$S2$RESET\n"
< test1 grep hello | awk '{count++} END {print count}' > test2 2> test2
../pipex user1 "grep hello" "awk '{count++} END {print count}'" user2 2> user2
comp test2 user2
rm -rf test2 user2

printf "$YELLOW$S1$RESET\n"
printf "$PURPLE""test: < infile grep hello | awk \"{count++} END {print count}\" > outfile$RESET\n"
printf "$PURPLE$S2$RESET\n"
< test1 grep hello | awk "{count++} END {print count}" > test2 2> test2
../pipex user1 "grep hello" "awk \"{count++} END {print count}\"" user2 2> user2
comp test2 user2
rm -rf test2 user2

printf "$YELLOW$S1$RESET\n"
printf "$PURPLE""test: < infile grep hello | awk '\"{count++} END {print count}\"' > outfile$RESET\n"
printf "$PURPLE$S2$RESET\n"
< test1 grep hello | awk '"{count++} END {print count}"' > test2 2> test2
../pipex user1 "grep hello" "awk '\"{count++} END {print count}\"'" user2 2> user2
comp test2 user2
rm -rf test2 user2

printf "$YELLOW$S1$RESET\n"
printf "$PURPLE""test: < infile grep hello | awk \"'{count++} END {print count}'\" > outfile$RESET\n"
printf "$PURPLE$S2$RESET\n"
< test1 grep hello | awk "'{count++} END {print count}'" > test2 2> test2
../pipex user1 "grep hello" "awk \"'{count++} END {print count}'\"" user2 2> user2
comp test2 user2
rm -rf test2 user2

rm -rf test1 user1

printf "\033[38;2;255;176;0m\n%80s\n%80s\n%80s\033[0m\n" "Test finished." "@2021, Pipex Medic." "https://github.com/gmarcha"
