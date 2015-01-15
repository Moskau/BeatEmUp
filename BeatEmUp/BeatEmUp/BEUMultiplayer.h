#pragma once

#include <C4Messages.h>
#include "Game.h"
#include "PlayerController.h"

using namespace C4;

namespace C4
{

	enum MESSAGES
	{
		kMessageServerInfo = kMessageBaseCount,
		kMessageSpawn,
		kMessageRequest,
		kMessageMovementBegin,
		kMessageMovementEnd,
		kMessageOrientation,
		kMessageBallMessage
	};

	//Messgae sent to server to request a controller

	class RequestMessage : public Message
	{
	public:
		RequestMessage();
		~RequestMessage();

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);
	};

	//Message sent from server to spawn a controller

	class SpawnMessage : public Message
	{
	public:
		SpawnMessage();
		SpawnMessage(PlayerKey playerkey, int64 controllerIndex, Point3D location);
		~SpawnMessage();

		bool HandleMessage(Player *sender) const;

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);

		Point3D GetLocation() const
		{
			return loc;
		}

		PlayerKey GetPlayerKey() const
		{
			return key;
		}

		long GetControllerIndex() const
		{
			return contIndex;
		}
	private:
		Point3D loc;
		int64 contIndex;
		PlayerKey key;
	};

	//Class used for getting and setting a player's controller

	class GamePlayer : public Player
	{
	public:
		GamePlayer(PlayerKey key);
		~GamePlayer();

		PlayerController *GetController() const
		{
			return cont;
		}
		void SetController(PlayerController *controller)
		{
			cont = controller;
		}
	private:
		PlayerController *cont;
	};


	//Message sent from client to set movement flags of their controller

	class ClientMovementMessage : public Message
	{
	private:
		unsigned long	movementFlag;

	public:
		ClientMovementMessage(MessageType type);
		ClientMovementMessage(MessageType type, unsigned long flag);
		~ClientMovementMessage();

		long GetMovementFlag(void) const
		{
			return (movementFlag);
		}

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);

		bool HandleMessage(Player *sender) const;
	};

	//Message sent from client to change orientation of player controller

	class ClientOrientationMessage : public Message
	{
	public:
		ClientOrientationMessage();
		~ClientOrientationMessage();

		void Compress(Compressor &data) const;
		bool Decompress(Decompressor &data);
	};
}

