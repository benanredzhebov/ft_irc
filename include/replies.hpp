/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:02 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/27 15:19:44 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*These lines is a preprocessor macro definitions in C++. It defines a macro named*/

#define once

#define CRLF "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)

/*ERRORS*/
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF) //a nickname with errors in it
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)



#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN #" + channelname + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " #" +channelname + " :" + topic + "\r\n")
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)


#define ERR_NOSUCHNICK(sender_nick, target_nick)("\t :401 sender:" + sender_nick + " " + target_nick + ":No such nick/channel" + CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (":	461 \n\t\t" + nickname + " :Not enough parameters." + CRLF)
#define ERR_INCORPASS(password) (":	464 \n\t\t" + password + " :Password incorrect !" + CRLF )
#define ERR_NEEDMOREPARAMS(command) (":	461 \n\t\t" + command + " :Not enough parameters" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (":	462 \n\t\t" + nickname + " :You may not reregister !" + CRLF )
#define G_PASSWORD "	✅✅ PASSWORD CORRECT ✅✅\n ...continue with nick and username\n"
#define PASSWORD_AUTH_FAILED "	PASSWORD FAILED BYE 👋\n"
#define ERR_NICKNAME_NEEDED(command) (":	m420 \n\t\t" + command + ":pass NICK first" + CRLF)
#define RPL_NICKCHANGE(oldnickname, nickname) (":	\t\t " + oldnickname + " NICK " + nickname + CRLF)



#define USER_SET "	😎😎 CLIENT USERNAME SET 😎😎\n"
#define NICK_SET "	😎😎 CLIENT NICKNAME SET 😎😎\n"