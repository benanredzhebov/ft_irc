/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:02 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/14 08:28:54 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*These lines is a preprocessor macro definitions in C++. It defines a macro named*/

#define once

#define CRLF "\r\n"

// Responses
#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)
#define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + channelname + " " + modes + CRLF
#define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + channelname + " " + creationtime + CRLF
#define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE " + channelname + " " + mode + " " + arguments + CRLF)
#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + CRLF)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + CRLF)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)
#define CHANTOPIC(client_nick, channel, topic) (": " + client_nick + "!" + client_nick + "@hostname TOPIC " + channel + " " + topic + CRLF)
#define RPL_TOPIC(nick, channel, topic) (":" + std::string("server") + " 332 " + nick + " " + channel + " " + topic + "\r\n")
#define RPL_NOTOPIC(nick, channel) (":" + std::string("server") + " 331 " + nick + " " + channel + " :No topic is set\r\n")

#define ADCHANGE(admin_nick, channel, new_admin) \
    (std::string(RED) + admin_nick + ": WE HAVE JUST ONE PARACHUTE LEFT ON BOARD, sorry guys\nJUMPS OFF 🤸 \n" + \
    RESET + "ADMIN: " + admin_nick + " ABANDONED YOU GUYS\nWE NEED A NEW CAPTAIN\n" + \
    YEL + new_admin + "@hostname has accepted the position and is here to serve CHANNEL: " + channel + CRLF RESET)

	
#define CLIOUT(cli_nick, channel) (RED ": " + cli_nick + "! @hostname LEFT CHANNEL: " + channel + CRLF RESET)
#define CLIKICKOUT(hostname, cli_nick, channel, reason)(RED ": " + hostname + "@localhost KICKED " + cli_nick + " OUT OF CHANNEL: " + channel + "\n REASON:" + reason + CRLF RESET)

// Errors
#define ERR_NOSUCHCHANNEL(client_nick, channel) (":403 " + client_nick + " " + channel + " :No such channel" + CRLF)
#define ERR_CHANNELNOTFOUND(nickname, channelname) (":403 " + nickname + " " + channelname + " :No such channel" + CRLF)

#define ERR_NORECIPIENT(nick) (":" + nick + " 411 :No recipient given (PRIVMSG)" + CRLF)
#define ERR_NOTEXTTOSEND(nick) (":" + nick + " 412 :No text to send" + CRLF)
#define ERR_TOOMANYTARGETS(nick) (":" + nick + " 407 :Too many recipients" + CRLF)
#define ERR_ERRONEUSNICK(nickname) (":432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NICKINUSE(nickname) (":433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_NOTREGISTERED(nickname) (":451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CANNOTSENDTOCHAN(nick, channel) (":" + nick + " 404 " + channel + " :Cannot send to channel" + CRLF)
#define ERR_NOTOPERATOR(channelname) (":482 " + channelname + " :You're not a channel operator" + CRLF)
#define ERR_KEYSET(channelname) (":467 " + channelname + " Channel key already set." + CRLF)
#define ERR_NEEDMODEPARM(channelname, mode) (":696 " + channelname + " * You must specify a parameter for the key mode." + mode + CRLF)
#define ERR_NOSUCHNICK(sender_nick, target_nick) (":401 " + sender_nick + " " + target_nick + " :No such nick/channel" + CRLF)
#define ERR_UNKNOWNMODE(nickname, channelname, mode) (":472 " + nickname + channelname + " " + mode + " :is not a recognized channel mode" + CRLF)
#define ERR_INVALIDMODEPARM(channelname, mode) (":696 " + channelname + " Invalid mode parameter." + mode + CRLF)
#define ERR_BADCHANNELKEY(client_nick, channel) (":475 " + client_nick + " " + channel + " :Cannot join channel (+k) - bad key" + CRLF)
#define ERR_TOOMANYCHANNELS(client_nick) (":405 " + client_nick + " :You have joined too many channels" + CRLF)
#define ERR_INVITEONLYCHAN(client_nick, channel) (":473 " + client_nick + " " + channel + " :Cannot join channel (+i) - invite only" + CRLF)
#define ERR_CHANNELISFULL(client_nick, channel) (":471 " + client_nick + " " + channel + " :Cannot join channel (+l) - channel is full" + CRLF)
#define ERR_BADCHANMASK(client_nick, channel) (":476 " + client_nick + " " + channel + " :Invalid channel name" + CRLF)
#define ERR_NOTONCHANNEL(client_nick, channel) (":442 " + client_nick + " " + channel + " :You're not on that channel" + CRLF)
#define ERR_USERNOTINCHANNEL(client_nick, channel) (":441 " + client_nick + " " + channel + " :They aren't on that channel" + CRLF)
#define ERR_CHANOPRIVSNEEDED(client_nick, channel) (":482 " + client_nick + " " + channel + " :You're not channel operator" + CRLF)
#define ERR_USERONCHANNEL(client_nick, channel, inviter) (":443 " + inviter + " " + client_nick + " " + channel + " :is already on channel" + CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (":461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_NEEDMOREPARAMS(command) (":461 " + command + " :Not enough parameters" + CRLF)
#define ERR_INCORPASS(password) (":464 " + password + " :Password incorrect!" + CRLF)
#define ERR_ALREADYREGISTERED(nickname) (":462 " + nickname + " :You may not reregister!" + CRLF)
#define ERR_NICKNAME_NEEDED(command) (":420 " + command + ":pass NICK first" + CRLF)
#define ERR_UNKNOWNCOMMAND(command) (":" + std::string("server") + " 421 " + command + " :Unknown command.Benan\r\n")

// Status Messages
#define G_PASSWORD "✅✅ PASSWORD CORRECT ✅✅\n ...continue with nick and username\n"
#define PASSWORD_AUTH_FAILED "PASSWORD FAILED BYE 👋\n"
#define USER_SET "😎😎 CLIENT USERNAME SET 😎😎\n"
#define NICK_SET "😎😎 CLIENT NICKNAME SET 😎😎\n"
