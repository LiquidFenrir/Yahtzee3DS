#include "game/network.h"

static u32 wlancommID = 0xF650DA89;
#define appdataLen 0xc8
#define appdataSigLen 4
#define appdataStrLen (appdataLen-appdataSigLen)
static u8 appdata[appdataLen] = {0x69, 0x8a, 0x05, 0x5c};
#define appdataStr(ad) ((char*)ad+appdataSigLen)

#define usernameLength 0x1c

static u8 data_channel = 1;

static u32 recv_buffer_size = UDS_DEFAULT_RECVBUFSIZE;

Room::Room()
{
    udsGenerateDefaultNetworkStruct(&this->network, wlancommID, 0, NetworkMaxPlayerCount);

    u8 tmpUsername[usernameLength] = {0};
    Result ret = CFGU_GetConfigInfoBlk2(usernameLength, 0x000A0000, tmpUsername);
    if(ret != 0)
        DEBUG("udsSetApplicationData() returned %.8lx\n", ret);

    ret = udsCreateNetwork(&this->network, "", 0, &this->bindctx, data_channel, recv_buffer_size);
    if(ret != 0)
        DEBUG("udsCreateNetwork() returned %.8lx\n", ret);

    utf16_to_utf8((u8*)appdataStr(appdata), (u16*)tmpUsername, usernameLength/sizeof(u16));
    DEBUG("Appdata: %s\n", appdataStr(appdata));

    ret = udsSetApplicationData(appdata, appdataLen*sizeof(u8));
    if(ret != 0)
        DEBUG("udsSetApplicationData() returned %.8lx\n", ret);

    //dont allow spectators
    ret = udsEjectSpectator();
    if(ret != 0)
        DEBUG("udsEjectSpectator() returned %.8lx\n", ret);

    for(size_t i = 0; i < this->ready.size(); i++)
        this->ready[i] = false;

    this->server = true;
    this->connected = true;
}

Room::Room(udsNetworkStruct network)
{
    this->network = network;

    for(size_t i = 0; i < this->ready.size(); i++)
        this->ready[i] = false;

    this->server = false;
    this->connected = false;
}

Room::~Room()
{
    DEBUG("Room::~Room\n");
    DEBUG("%s\n", this->server ? "server" : "client");

    if(this->server)
        udsDestroyNetwork();
    else
        udsDisconnectNetwork();

    udsUnbind(&this->bindctx);
}

void Room::join()
{
    for(int i = 0; i < 10; i++)  // 10 tries
    {
        Result ret = udsConnectNetwork(&this->network, "", 0, &this->bindctx, UDS_BROADCAST_NETWORKNODEID, UDSCONTYPE_Client, data_channel, recv_buffer_size);
        if(R_FAILED(ret))
            DEBUG("udsConnectNetwork() returned %.8lx\n", ret);
        else
        {
            DEBUG("Succesfully connected to the room!\n");
            this->connected = true;
            break;
        }
    }
}

std::string Room::getName()
{
    return std::string(appdataStr(this->network.appdata), this->network.appdata_size-appdataSigLen);
}

std::vector<std::string> Room::getPlayerNames()
{
    std::vector<std::string> names;
    for(u8 i = 0; i < this->getPlayerCount(); i++)
    {
        udsNodeInfo node;
        Result ret = udsGetNodeInformation(i+1, &node);
        if(ret != 0)
            DEBUG("udsGetNodeInformation() returned %.8lx\n", ret);

        char username[256] = {0};
        ret = udsGetNodeInfoUsername(&node, username);
        if(ret != 0)
            DEBUG("udsGetNodeInfoUsername() returned %.8lx\n", ret);

        names.push_back(std::string(username));
    }

    return names;
}

u8 Room::getPlayerCount()
{
    if(this->connected)
    {
        udsConnectionStatus constatus;
        udsGetConnectionStatus(&constatus);
        return constatus.total_nodes;
    }
    else
    {
        return this->network.total_nodes;
    }
}

bool Room::everyoneReady()
{
    size_t playerCount = this->getPlayerCount();
    if(playerCount == 0)
        return false;

    for(size_t i = 0; i < playerCount; i++)
    {
        if(!this->ready[i])
            return false;
    }

    return true;
}

bool Room::isPlayerReady(size_t playerID)
{
    return this->ready[playerID];
}

Result Room::receivePacket(NetworkPacket* packet, size_t* actualSize, u16* sender)
{
    return udsPullPacket(&this->bindctx, packet, sizeof(NetworkPacket), actualSize, sender);
}
Result Room::sendPacket(NetworkPacket packet)
{
    return udsSendTo(UDS_BROADCAST_NETWORKNODEID, data_channel, UDS_SENDFLAG_Default, (u8*)&packet, sizeof(NetworkPacket));
}

void Room::receiveReadyPacket()
{
    NetworkPacket packet;
    size_t actualSize = 0;
    u16 sender;

    Result ret = this->receivePacket(&packet, &actualSize, &sender);
    if(R_FAILED(ret))
        DEBUG("udsPullPacket() returned %.8lx\n", ret);

    if(actualSize == sizeof(NetworkPacket) && packet.type == NETWORK_PACKET_READY)
    {
        this->ready[sender-1] = packet.ready;
    }
}
void Room::sendReadyPacket(size_t playerID, bool ready)
{
    this->ready[playerID] = ready;

    NetworkPacket packet;
    packet.type = NETWORK_PACKET_READY;
    packet.ready = ready;

    Result ret = this->sendPacket(packet);
    if(R_FAILED(ret))
        DEBUG("udsSendTo() returned %.8lx\n", ret);
}

void Room::getRemotePlayerKeys()
{
    NetworkPacket packet;
    size_t actualSize = 0;
    u16 sender;

    Result ret = this->receivePacket(&packet, &actualSize, &sender);
    if(R_FAILED(ret))
        DEBUG("udsPullPacket() returned %.8lx\n", ret);

    if(actualSize == sizeof(NetworkPacket) && packet.type == NETWORK_PACKET_KEYS)
    {
        kDown = packet.kDown;
        kHeld = packet.kHeld;
        kUp = packet.kUp;
        touch = packet.touch;
    }
}

void Room::sendLocalKeys()
{
    NetworkPacket packet;
    packet.type = NETWORK_PACKET_KEYS;
    packet.kDown = kDown;
    packet.kHeld = kHeld;
    packet.kUp = kUp;
    packet.touch = touch;

    Result ret = this->sendPacket(packet);
    if(R_FAILED(ret))
        DEBUG("udsSendTo() returned %.8lx\n", ret);
}

std::vector<std::shared_ptr<Room>> getRoomList()
{
    u32 tmpbuf_size = 0x4000;
    u32* tmpbuf = (u32*)malloc(tmpbuf_size);
    memset(tmpbuf, 0, tmpbuf_size);

    size_t total_networks = 0;
    udsNetworkScanInfo * networks = NULL;

    Result ret = udsScanBeacons(tmpbuf, tmpbuf_size, &networks, &total_networks, wlancommID, 0, NULL, false);
    if(R_FAILED(ret))
        DEBUG("udsScanBeacons() returned %.8lx\n", ret);

    DEBUG("Servers found: %u\n", total_networks);
    free(tmpbuf);

    std::vector<std::shared_ptr<Room>> rooms;
    for(size_t i = 0; i < total_networks; i++)
    {
        udsNetworkStruct network = networks[i].network;
        if(!memcmp(network.appdata, appdata, appdataSigLen) && network.max_nodes == NetworkMaxPlayerCount && network.total_nodes < NetworkMaxPlayerCount)
        {
            std::shared_ptr<Room> room(new Room(network));
            rooms.push_back(room);
        }
    }

    free(networks);
    return rooms;
}

void closeNetworkConnection()
{

}

void getRemotePlayerKeys()
{

}

void sendLocalKeys()
{

}