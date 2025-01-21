## ft_irc

                
## Table of Contents
- [Introduction](#introduction)
- [Project Overview](#projectoverview)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Usage](#usage)
- [Commands](#commands)
- [Technical Details](#technical-details)
- [Testing](#testing)
- [Acknowledgments](#acknowledgments)

This project is a very small 2D Game. It is built to make you work with textures, sprites. And some very basic gameplay elements.

## STATUS:

Result : 
ft_irc - 100%✅


## Introduction

ft_irc is a 42 project aimed at implementing a simple Internet Relay Chat (IRC) server compliant with the
RFC 2812 standard. This project helps students learn about network programming, sockets,
multithreading, and protocol implementation in C++.


## Project Overview

The goal of this project is to create a working IRC server that allows multiple clients to connect,
communicate, and manage channels. The server must handle client connections, parse commands, and
manage users and channels according to the IRC protocol specifications.


## Features

- # Client-Server Communication:

  - Handles multiple simultaneous client connections.

  - processes client messages in real-time.

- # IRC Protocol Compliance:

  - Implements essential commands defined in RFC 2812.

- # Channel Management:

  - Create, join, and leave channels.

  - Set channel operators and manage permissions.

- # User Authentication:

  - Supports user registration and nickname management.

- # Error Handling:

  - Gracefully handles errors such as invalid commands or nickname conflicts.


## Getting Started

# Installation
1. Clone the repository:

  - `git@github.com:benanredzhebov/ft_irc.git`
  
  - `cd ft_irc`

3. Build the project:

  - `make`


# Usage
1. Start the server:

  - `./ircserv <port> <password>`.
  
    - `<port>`: The port number the server will listen on.
    - `<password>`: The password clients must use to connect to the server.

3. Connect to the server using an IRC client (e.g., HexChat, irssi, or nc for testing):
   
 - `/server <host> <port> -p <password>`


# Commands
Below are some of the basic IRC commands supported by the server:
| Command                   | Description                                      |
|---------------------------|--------------------------------------------------|
| `/NICK <name>`            | Set or change your nickname.                     |
| `/USER <info>`            | Register your user details.                      |
| `/JOIN <channel>`         | Join a channel or create one if it doesn't exist.|
| `/PRIVMSG <target>`       | Send a private message to a user or channel.     |
| `/QUIT`                   | Disconnect from the server.                      |
| `/TOPIC <channel> <topic>`| Set or view the channel's topic.                 |
| `/KICK <channel> <user>`  | Remove a user from a channel.                    |

For a full list of commands and their syntax, refer to the [IRC RFC documentation](https://datatracker.ietf.org/doc/html/rfc2812#section-1.2.1).


## Technical Details

- # Sockets:

  - The server uses epoll() to handle multiple clients.

- # Multithreading:

  - Threads are used to manage incoming connections and message parsing.

- # Data Structures:

  - Users and channels are managed using STL containers like std::map and std::vector.

- # Protocol Implementation:

 - The server strictly follows the syntax and semantics of the IRC protocol.

## Acknowledgments

This project is part of the 42 curriculum and draws inspiration from the official [IRC RFC documentation](https://datatracker.ietf.org/doc/html/rfc2812#section-1.2.1).
Special thanks to peers and staff for guidance and support during development.

## Collaborators
<p align="center">
  <a href="https://github.com/benanredzhebov" title="Benan Redzhebov">
    <img src="https://github.com/benanredzhebov.png?size=50" alt="benanredzhebov" style="border-radius: 50%; width: 50px;">
  </a>
  <a href="https://github.com/daniek1010" title="Daniel Evans">
    <img src="https://github.com/StefanPenev.png?size=50" alt="StefanPenev" style="border-radius: 50%; width: 50px;">
  </a>
</p>
