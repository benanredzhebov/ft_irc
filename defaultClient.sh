#!/bin/bash

#ctrl + z -suspend
#resume fg
#for i in {1..1000}; do
#    echo -e "PRIVMSG #test :Flood message $i\r\n" | nc localhost 6666
#done



# Configuration
SERVER="localhost"
PORT=6666
PASSWORD="pass"
# Lists of possible values
NICKS=("Dani" "Beni" "Dori" "Defi")
USERS=("Daniel" "Benan" "Dorukhan" "Defne")
REALNAMES=("DanielD" "BenanR" "DorukhanR" "DefneR")

# Select random values
index=$((RANDOM % ${#NICKS[@]}))
NICK=${NICKS[$index]}
USER=${USERS[$index]}
REALNAME=${REALNAMES[$index]}
CHANNEL="#test"
MESSAGE="Hello, IRC!"

is_server_running()
{
    pgrep ircserv
    return $?
}

# Set the trap to catch SIGINT (Ctrl+C)
trap cleanup SIGINT

cleanup()
{
    echo "Signal received. Cleaning up..."
    pkill -P $$ nc
    exit 1
}

# Connect to the IRC server
(
    sleep 0.5
    echo "PASS $PASSWORD"
    sleep 0.5
    echo "NICK $NICK"
    sleep 0.5
    echo "USER $USER 0 * :$REALNAME"
    sleep 0.5
    echo "JOIN $CHANNEL"
    sleep 0.5
    echo "PRIVMSG $CHANNEL :$MESSAGE"
    
    while true; do
        if [ -t 0 ]; then
            read -t 1 -r input <&0
            if [ $? -eq 0 ]; then
                echo $input
                if [ "$input" == "QUIT" ]; then
                    break
                fi
            fi
        fi
        if ! is_server_running 1>/dev/null ; then
            pkill -P $$ nc 1>&2
            break
        fi
    done
    exec &1>-
) | nc $SERVER $PORT 2>/dev/null