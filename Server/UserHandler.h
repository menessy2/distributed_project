/* 
 * File:   UserHandler.h
 * Author: root
 *
 * Created on November 20, 2014, 6:58 PM
 */

#ifndef USERHANDLER_H
#define	USERHANDLER_H

#include <thread>

class UserHandler : public std::thread {
public:
    UserHandler();
    UserHandler(const UserHandler& orig);
    virtual ~UserHandler();
private:

};

#endif	/* USERHANDLER_H */

