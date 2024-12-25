/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:02 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/25 13:33:21 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*These lines is a preprocessor macro definitions in C++. It defines a macro named*/

#define once

#define CRLF "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)

/*ERRORS*/
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF) //a nickname with errors in it
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )


#define USER_SET " 😎😎 CLIENT USERNAME SET 😎😎\n"
#define NICK_SET " 😎😎 CLIENT NICKNAME SET 😎😎\n"
#define PASSWORD_AUTH_FAILED " PASSWORD FAILED BYE 👋\n"

#define E_PASSWORD "🚨🚨 ENTER PASSWORD OR GET KICKED 🚨🚨\n"
#define G_PASSWORD "✅✅ PASSWORD CORRECT ✅✅\n"
#define USERNAME "🚨🚨 ENTER USERNAME 🚨🚨\n"
#define NICKNAME "🚨🚨 ENTER NICKNAME 🚨🚨\n"