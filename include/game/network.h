#pragma once

#include "common.h"

static constexpr u8 NetworkMaxPlayerCount = diceMaxValue;
static constexpr u8 NetworkMinPlayerCount = diceMinValue;

class Room
{
    public:
        Room();
        Room(udsNetworkStruct network);
        ~Room();

        void join();
        std::string getName();
        std::vector<std::string> getPlayerNames();
        u8 getPlayerCount();
        bool everyoneReady();
        bool isPlayerReady(size_t playerID);

        udsNetworkStruct network;
        udsBindContext bindctx;
    private:
        bool server;
        std::array<bool, NetworkMaxPlayerCount> ready;
};

std::vector<std::shared_ptr<Room>> getRoomList();
void closeNetworkConnection();

void getRemotePlayerKeys();
void sendLocalKeys();
