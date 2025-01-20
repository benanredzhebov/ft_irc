#!/bin/bash

SERVER="localhost"
PORT=8080
PASSWORD="pass"
CHANNEL="#test"

# Lists of possible values
NICKS=("User1" "User2" "User3" "User4" "User5" "User6" "User7" "User8" "User9" "User10")
USERS=("User1" "User2" "User3" "User4" "User5" "User6" "User7" "User8" "User9" "User10")
REALNAMES=("Real1" "Real2" "Real3" "Real4" "Real5" "Real6" "Real7" "Real8" "Real9" "Real10")

# Function to simulate a client
simulate_client() {
    local NICK=$1
    local USER=$2
    local REALNAME=$3

    {
        sleep 0.5
        echo "PASS $PASSWORD"
        sleep 0.5
        echo "NICK $NICK"
        sleep 0.5
        echo "USER $USER 0 * :$REALNAME"
        sleep 0.5
        echo "JOIN $CHANNEL"
        sleep 0.5
        for i in {1..10}; do
            echo "PRIVMSG $CHANNEL :Message $i from $NICK"
            sleep 0.1
        done
        sleep 0.5
        echo "QUIT :Goodbye from $NICK"
    } | nc -C $SERVER $PORT
}

# Simulate multiple clients
for i in {0..9}; do
    simulate_client ${NICKS[$i]} ${USERS[$i]} ${REALNAMES[$i]} &
done

# Wait for all background processes to finish
wait
