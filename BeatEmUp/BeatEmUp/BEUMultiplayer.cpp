#include "BEUMultiplayer.h"

RequestMessage::RequestMessage() : Message(C4::kMessageRequest)
{
}

RequestMessage::~RequestMessage()
{
}

void RequestMessage::Compress(Compressor &data) const
{
}

bool RequestMessage::Decompress(Decompressor &data)
{
	return true;
}

SpawnMessage::SpawnMessage() : Message(C4::kMessageSpawn)
{
	this->contIndex = 0;
	this->key = -1;
}

SpawnMessage::SpawnMessage(PlayerKey playerkey, int64 controllerIndex, Point3D location) : Message(C4::kMessageSpawn)
{
	this->contIndex = controllerIndex;
	this->key = playerkey;
	this->loc = location;
}

SpawnMessage::~SpawnMessage()
{
}

bool SpawnMessage::HandleMessage(Player *sender) const
{
	Player *player = TheMessageMgr->GetPlayer(this->GetPlayerKey());
	TheGame->SpawnController(player, this->GetLocation(), this->GetControllerIndex());
	return (true);
}

void SpawnMessage::Compress(Compressor &data) const
{
	data << loc;
	data << contIndex;
	data << key;
}

bool SpawnMessage::Decompress(Decompressor &data)
{
	data >> loc;
	data >> contIndex;
	data >> key;
	return (true);
}

GamePlayer::GamePlayer(PlayerKey key) : Player(key)
{
	cont = nullptr;
}

GamePlayer::~GamePlayer()
{
}


ClientMovementMessage::ClientMovementMessage(MessageType type) : Message(type)
{
}

ClientMovementMessage::ClientMovementMessage(MessageType type, unsigned long flag) : Message(type)
{
	movementFlag = flag;
}

ClientMovementMessage::~ClientMovementMessage()
{
}

void ClientMovementMessage::Compress(Compressor &data) const
{
	data << (unsigned char)movementFlag;
}

bool ClientMovementMessage::Decompress(Decompressor &data)
{
	unsigned char	flag;

	data >> flag;
	movementFlag = flag;

	return (true);
}

bool ClientMovementMessage::HandleMessage(Player *sender) const
{
	PlayerController *controller = static_cast<GamePlayer *>(sender)->GetController();
	if (controller)
	{
		switch (GetMessageType())
		{
		case kMessageMovementBegin:

			controller->BeginMovement(movementFlag);
			break;

		case kMessageMovementEnd:

			controller->EndMovement(movementFlag);
			break;
		}
	}

	return (true);
}



ClientOrientationMessage::ClientOrientationMessage() : Message(C4::kMessageOrientation)
{
}

ClientOrientationMessage::~ClientOrientationMessage()
{
}

void ClientOrientationMessage::Compress(Compressor &data) const
{
}

bool ClientOrientationMessage::Decompress(Decompressor &data)
{
	return true;
}