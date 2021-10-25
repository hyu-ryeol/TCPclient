#include <iostream>
#include <sstream>
#include <string>

#include <Poco/Net/Net.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>

#include <Poco/DateTime.h>
#include <Poco/Timespan.h>

const Poco::UInt16 PORT = 32452;
const std::string server_add = "192.168.1.3";

struct packet_data_joint{
    unsigned char index1;
    unsigned char index2;
    unsigned char subindex;
    //float Target[16];
};

struct packet_data_task{
    unsigned char index1;
    unsigned char index2;
    unsigned char subindex;
    //float Target[12];
};

union TCP_Packet_Joint{
    packet_data_joint info;
    unsigned char data[sizeof(packet_data_joint)];
};

union TCP_Packet_Task{
    packet_data_task info;
    unsigned char data[sizeof(packet_data_task)];
};

int main(int argc, char **argv){
    Poco::Net::StreamSocket ss;
    TCP_Packet_Task packet_task;
    TCP_Packet_Task packet_task_recv;
    TCP_Packet_Joint packet_joint;


    std::cout<<argc<<std::endl;



    try
    {
        std::cout << "Attempting to connect to server.." << std::endl;

        ss.connect(Poco::Net::SocketAddress(server_add, PORT));

        Poco::Timespan timeout(1, 0);    // 1 초

        while (!ss.poll(timeout, Poco::Net::Socket::SELECT_WRITE))
        {
            std::cout << "Connecting to server..." << std::endl;
        }
        std::cout << "Connection to server complete.." << std::endl;
        int subindex;
        while(1){
            std::cout << "SubIndex:" << std::endl;
            std::string numbers_str;
            std::cin>> subindex;
            packet_task.data[0] = 0x01;
            packet_task.data[1] = 0x00;
            packet_task.data[2] = subindex;
            std::cout <<"subindex"<<subindex<< std::endl;

            ss.sendBytes(packet_task.data, sizeof(TCP_Packet_Task));

            Poco::DateTime now1;
            std::cout << "Starting poll: " << std::dec << now1.millisecond()<< std::endl;


            while (!ss.poll(timeout, Poco::Net::Socket::SELECT_READ))
            {
                std::cout << "Ready for receiving..." << std::endl;
            }

            Poco::DateTime now2;
            std::cout << "poll complete: " << std::dec << now2.millisecond() << std::endl;

            auto len = ss.receiveBytes(packet_task_recv.data, sizeof(TCP_Packet_Task));
            std::cout << "Received Index1: " << std::hex << packet_task_recv.info.index1 << std::endl;
            std::cout << "Received Index2: " << std::hex << packet_task_recv.info.index2 << std::endl;
            std::cout << "Received SubIndex: " << std::hex << packet_task_recv.info.subindex << std::endl;


        }




        ss.close();
    }
    catch (Poco::Exception& exc)
    {
        std::cout << exc.displayText() << std::endl;
    }
    return 0;
}
