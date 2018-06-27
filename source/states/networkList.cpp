#include "states/networkList.h"
#include "game/drawing.h"

static constexpr float NetworkListButtonsY = 172;
static constexpr float NetworkListButtonsTextureWidth = 120;
static constexpr float NetworkListButtonsZ = 0.5f;

static constexpr float NetworkListRoomSelectButtonX = 150;
static constexpr float NetworkListRoomSelectButtonYStart = 42;
static constexpr float NetworkListRoomSelectButtonTextureWidth = 140;
static constexpr float NetworkListRoomSelectButtonTextOffset = 60;

NetworkListState::NetworkListState()
{
    DEBUG("NetworkListState::NetworkListState\n");
    this->buttons[NETWORK_LIST_REFRESH] = Button(168, NetworkListButtonsY, NetworkListButtonsZ, NetworkListButtonsTextureWidth, 68, "Refresh", std::bind(&NetworkListState::refreshList, this));
    this->buttons[NETWORK_LIST_BACK] = Button(32, NetworkListButtonsY, NetworkListButtonsZ, NetworkListButtonsTextureWidth, -66, "Back", std::bind(&NetworkListState::goBack, this));
    for(int i = 0; i < NetworkListRoomsPerScreen; i++)
    {
        this->joinButtons[i] = Button(NetworkListRoomSelectButtonX, NetworkListRoomSelectButtonYStart+i*40, NetworkListButtonsZ, NetworkListRoomSelectButtonTextureWidth, NetworkListRoomSelectButtonTextOffset, "Join", std::bind(&NetworkListState::joinRoom, this, i));
    }

    this->selectedButton = static_cast<int>(NETWORK_LIST_BACK);
    this->refreshList();
}

NetworkListState::~NetworkListState()
{
    DEBUG("NetworkListState::~NetworkListState\n");
}

void NetworkListState::update()
{
    int backButtonID = static_cast<int>(NETWORK_LIST_BACK);
    auto& backButton = this->buttons[backButtonID];
    if(backButton.isPressed())
    {
        if(this->selectedButton == backButtonID)
        {
            backButton.action();
        }
        else
        {
            this->selectedButton = backButtonID;
        }
    }

    int refreshButtonID = static_cast<int>(NETWORK_LIST_REFRESH);
    auto& refreshButton = this->buttons[refreshButtonID];
    if(refreshButton.isPressed())
    {
        if(this->selectedButton == refreshButtonID)
        {
            refreshButton.action();
        }
        else
        {
            this->selectedButton = refreshButtonID;
        }
    }

    for(auto& joinButton : this->joinButtons)
    {
        if(joinButton.isPressed())
            joinButton.action();
    }

    if(this->mode == NETWORK_LIST_SELECT_BUTTON)
    {
        if(kDown & KEY_A)
        {
            this->buttons[this->selectedButton].action();
        }
        else if(kDown & KEY_LEFT)
        {
            this->selectedButton = backButtonID;
        }
        else if(kDown & KEY_RIGHT)
        {
            this->selectedButton = refreshButtonID;
        }
        else if(kDown & KEY_UP && this->rooms.size() != 0)
        {
            this->mode = NETWORK_LIST_SELECT_ROOM;
        }
    }
    else if(this->mode == NETWORK_LIST_SELECT_ROOM)
    {
        if(kDown & KEY_LEFT)
        {
            this->selectedButton = backButtonID;
            this->mode = NETWORK_LIST_SELECT_BUTTON;
        }
        else if(kDown & KEY_RIGHT)
        {
            this->selectedButton = refreshButtonID;
            this->mode = NETWORK_LIST_SELECT_BUTTON;
        }
        else if(this->rooms.size() != 0)
        {
            int MaxRoom = static_cast<int>(this->rooms.size()-1);
            int MinRoom = 0;
            int MaxScroll = this->rooms.size()-NetworkListRoomsPerScreen;
            int MinScroll = 0;
            if(kDown & KEY_A)
            {
                this->joinSelectedRoom();
            }
            else if(kDown & KEY_DOWN)
            {

                this->selectedRoom++;
                if(this->selectedRoom > MaxRoom)
                    this->selectedRoom = MaxRoom;

                if(this->roomsScroll+this->selectedRoom >= NetworkListRoomsPerScreen && this->selectedRoom == this->roomsScroll+NetworkListRoomsPerScreen)
                {
                    this->roomsScroll++;
                    if(this->roomsScroll > MaxScroll)
                        this->roomsScroll = MaxScroll;
                }
            }
            else if(kDown & KEY_UP)
            {
                this->selectedRoom--;
                if(this->selectedRoom < MinRoom)
                    this->selectedRoom = MinRoom;

                if(this->roomsScroll > MinScroll && this->selectedRoom == this->roomsScroll-1)
                {
                    this->roomsScroll--;
                    if(this->roomsScroll < MinScroll)
                        this->roomsScroll = MinScroll;
                }
            }
        }
    }

    if(kDown & KEY_B)
    {
        this->buttons[backButtonID].action();
    }
}

void NetworkListState::draw()
{
    static float textScale = 0.6f;
    C2D_Text availableRooms;
    C2D_TextParse(&availableRooms, dynamicBuf, "Available rooms:");
    C2D_TextOptimize(&availableRooms);
    C2D_DrawText(&availableRooms, C2D_WithColor, 24, 24, 0.4f, textScale, textScale, textColor);

    for(int i = this->roomsScroll; i < this->roomsScroll+NetworkListRoomsPerScreen; i++)
    {
        if(static_cast<size_t>(i) >= this->rooms.size())
            break;

        int cleanI = i-this->roomsScroll;
        float y = 52 + cleanI*40;

        std::string roomName = this->rooms[i]->getName();
        u8 roomPlayers = this->rooms[i]->getPlayerCount();
        C2D_Text text;
        char roomNameAndPlayers[128] = {0};
        snprintf(roomNameAndPlayers, 127, "%s %d/%d", roomName.c_str(), roomPlayers, NetworkMaxPlayerCount);
        C2D_TextParse(&text, dynamicBuf, roomNameAndPlayers);
        C2D_TextOptimize(&text);
        C2D_DrawText(&text, C2D_WithColor, 24, y, 0.4f, textScale, textScale, textColor);

        this->joinButtons[cleanI].draw();
        if(this->mode == NETWORK_LIST_SELECT_ROOM && i == this->selectedRoom)
            this->joinButtons[cleanI].drawOverlay();
    }

    int backButtonID = static_cast<int>(NETWORK_LIST_BACK);
    auto& backButton = this->buttons[backButtonID];
    backButton.draw();
    if(this->mode == NETWORK_LIST_SELECT_BUTTON && this->selectedButton == backButtonID)
        backButton.drawOverlay();

    int refreshButtonID = static_cast<int>(NETWORK_LIST_REFRESH);
    auto& refreshButton = this->buttons[refreshButtonID];
    refreshButton.draw();
    if(this->mode == NETWORK_LIST_SELECT_BUTTON && this->selectedButton == refreshButtonID)
        refreshButton.drawOverlay();
}

void NetworkListState::joinRoom(int i)
{
    if(static_cast<size_t>(i) >= this->rooms.size())
        return;

    int selected = i+this->roomsScroll;
    if(this->selectedRoom == selected)
        this->joinSelectedRoom();
    else
        this->selectedRoom = selected;
}

void NetworkListState::refreshList()
{
    this->rooms.clear();
    this->rooms = getRoomList();
    this->selectedRoom = 0;
    this->roomsScroll = 0;

    if(this->rooms.size() == 0)
        this->mode = NETWORK_LIST_SELECT_BUTTON;
    else
        this->mode = NETWORK_LIST_SELECT_ROOM;
}

void NetworkListState::goBack()
{
    this->nextState = new NetworkMultiplayerState;
}

void NetworkListState::joinSelectedRoom()
{
    this->nextState = new WaitingForPlayersState(this->rooms[this->selectedRoom]->getPlayerCount(), this->rooms[this->selectedRoom]);
    this->rooms[this->selectedRoom]->join();
}
