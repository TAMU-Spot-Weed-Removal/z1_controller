#ifndef _UDP_H
#define _UDP_H

#include <arpa/inet.h>
#include <termios.h>
#include <string>
#include <string.h>
#include <vector>
#include "common/utilities/timer.h"
#include "message/arm_motor_common.h"

enum class BlockYN{
    YES,
    NO
};


class IOPort{
public:
    IOPort(std::string name, BlockYN blockYN, size_t recvLength, size_t timeOutUs)
        :_name(name){
        resetIO(blockYN, recvLength, timeOutUs);
    }
    virtual ~IOPort(){}
    virtual size_t send(uint8_t *sendMsg, size_t sendLength) = 0;
    virtual size_t recv(uint8_t *recvMsg, size_t recvLength) = 0;
    virtual size_t recv(uint8_t *recvMsg) = 0;
    virtual bool sendRecv(std::vector<MOTOR_send> &sendVec, std::vector<MOTOR_recv> &recvVec) = 0;
    void resetIO(BlockYN blockYN, size_t recvLength, size_t timeOutUs);
    bool isDisConnect = false;
protected:
    std::string _name;
    BlockYN _blockYN = BlockYN::NO;
    size_t _recvLength;
    timeval _timeout;
    timeval _timeoutSaved;
    uint16_t _isDisConnectCnt;
};


class UDPPort : public IOPort{
public:
    UDPPort(std::string name, std::string toIP, uint toPort, uint ownPort, 
            size_t recvLength = 0,
            BlockYN blockYN = BlockYN::NO,
            size_t timeOutUs = 20000);
    ~UDPPort();
    size_t send(uint8_t *sendMsg, size_t sendMsgLength);
    size_t recv(uint8_t *recvMsg, size_t recvLength);
    size_t recv(uint8_t *recvMsg);
    bool sendRecv(std::vector<MOTOR_send> &sendVec, std::vector<MOTOR_recv> &recvVec);
private:
    size_t _recvBlock(uint8_t *recvMsg, size_t recvLength);
    size_t _recvUnBlock(uint8_t *recvMsg, size_t recvLength);
    sockaddr_in _ownAddr, _toAddr, _fromAddr;
    socklen_t _sockaddrSize;
    int _sockfd;
    int _on = 1;
    size_t _sentLength;

    uint8_t _sendBytes[238];    // 7 motors
    uint8_t _recvBytes[546];   // 7 motors

    fd_set _rSet;
};



#endif