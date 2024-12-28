/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:02 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/28 08:00:26 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*These lines is a preprocessor macro definitions in C++. It defines a macro named*/

#define once

#define CRLF "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)

/*ERRORS*/
#define ERR_ERRONEUSNICK(nickname) (":432\t" + nickname + " :Erroneus nickname" + CRLF) //a nickname with errors in it
#define ERR_NICKINUSE(nickname) (":433\t" + nickname + " :Nickname is already in use" + CRLF)
#define ERR_NOTREGISTERED(nickname) (":451\t" + nickname + " :You have not registered!" + CRLF)






#define ERR_BADCHANNELKEY(client_nick, channel)(":475\t" + client_nick + " " + channel + " :Cannot join channel (+k) - bad key" + CRLF)
#define ERR_TOOMANYCHANNELS(client_nick) (":405\t" +  client_nick + " :You have joined too many channels" + CRLF)
#define ERR_INVITEONLYCHAN(client_nick, channel)(":473\t" + client_nick + " " + channel + " :Cannot join channel (+i) - invite only" + CRLF)
#define ERR_CHANNELISFULL(client_nick, channel)(":471\t" + client_nick + " " + channel + " :Cannot join channel (+l) - channel is full" + CRLF)

#define ERR_BADCHANMASK(client_nick, channel)(":476\t" + client_nick + " " + channel + ":Invalid channel name" + CRLF)
#define ERR_NOTONCHANNEL(client_nick, channel)(":442\t" + client_nick + " " + channel + " :You're not on that channel" + CRLF)
#define ERR_USERNOTINCHANNEL(client_nick, channel)(":441\t" + client_nick + " " + channel + " :They aren't on that channel" + CRLF)
#define ERR_CHANOPRIVSNEEDED(client_nick, channel)(":482\t" + client_nick + " " + channel + " :You're not channel operator" + CRLF)
#define ERR_NOSUCHCHANNEL(client_nick, channel)(":403\t" + client_nick + " " + channel + " :No such channel" + CRLF)











#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + CRLF)
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " " +channelname + " :" + topic + "\r\n")
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + CRLF)


#define ERR_NOSUCHNICK(sender_nick, target_nick)("\t :401 sender:" + sender_nick + " " + target_nick + ":No such nick/channel" + CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (":461\t" + nickname + " :Not enough parameters." + CRLF)
#define ERR_INCORPASS(password) (":	464 \n\t\t" + password + " :Password incorrect !" + CRLF )
#define ERR_NEEDMOREPARAMS(command) (":	461 \n\t\t" + command + " :Not enough parameters" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (":	462 \n\t\t" + nickname + " :You may not reregister !" + CRLF )
#define G_PASSWORD "	✅✅ PASSWORD CORRECT ✅✅\n ...continue with nick and username\n"
#define PASSWORD_AUTH_FAILED "	PASSWORD FAILED BYE 👋\n"
#define ERR_NICKNAME_NEEDED(command) (":	m420 \n\t\t" + command + ":pass NICK first" + CRLF)
#define RPL_NICKCHANGE(oldnickname, nickname) (":	\t\t " + oldnickname + " NICK " + nickname + CRLF)



#define USER_SET "	😎😎 CLIENT USERNAME SET 😎😎\n"
#define NICK_SET "	😎😎 CLIENT NICKNAME SET 😎😎\n"