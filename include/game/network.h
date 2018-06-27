#pragma once

#include "common.h"

static constexpr u8 NetworkMaxPlayerCount = diceMaxValue;
static constexpr u8 NetworkMinPlayerCount = diceMinValue;

enum NetworkPacketType
{
    NETWORK_PACKET_READY,
    NETWORK_PACKET_KEYS,
};

struct NetworkPacket
{
    NetworkPacketType type;
    bool ready;
    u32 kDown, kHeld, kUp;
    touchPosition touch;
};

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

        Result receivePacket(NetworkPacket* packet, size_t* actualSize, u16* sender);
        Result sendPacket(NetworkPacket packet);

        void receiveReadyPacket();
        void sendReadyPacket(size_t playerID, bool ready);

        void getRemotePlayerKeys();
        void sendLocalKeys();

    private:
        udsNetworkStruct network;
        udsBindContext bindctx;

        bool server;
        bool connected;
        std::array<bool, NetworkMaxPlayerCount> ready;
};

std::vector<std::shared_ptr<Room>> getRoomList();
