/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:02 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 11:35:06 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*These lines is a preprocessor macro definitions in C++. It defines a macro named*/

#define once

#define CRLF "\r\n"

/*ERRORS*/
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
