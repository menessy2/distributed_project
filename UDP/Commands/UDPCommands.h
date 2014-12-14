#ifndef UDPCOMMANDS_H
#define	UDPCOMMANDS_H

// UP TO 256 Commands

enum class UPD_ENUM_COMMANDS  {
    NA,                     // UNINITIALIZED
    IDLE,                   // FOR RECEPIENTS
    
    TRANSMIT_DATA,
    RETRANSMIT,
    ACK,
    ACK_SUCCESS,
    RECEIVE_SUCCESS,        // FULL MESSAGE RECEIVE
    
    PING,
    STOP,
    
    AUTH,
    AUTH_SUCCESS,
    AUTH_FAILURE,
    ACCESS_DENIED,
    
    CREATE_FILE,
    CREATE_FILE_SUCCESS,
    CREATE_FILE_FAILURE,
    
    REQUEST_FILE,
    REQUEST_FILE_FAILURE
};


#endif	/* UDPCOMMANDS_H */

