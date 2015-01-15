#ifndef GAME_H
#define GAME_H

#include "C4Application.h"
#include "C4Interface.h"
#include "C4Engine.h"
#include "C4World.h"
#include "PlayerController.h"
#include "BEUMultiplayer.h"
#include "NavAction.h"
#include "StartWindow.h"
#include "BallController.h"
#include "Camera.h"
#include "EnemyManager.h"

// Every application/game module needs to declare a function called ConstructApplication()
// exactly as follows. (It must be declared extern "C", and it must include the tag "C4MODULEEXPORT".)
// The engine looks for this function in the DLL and calls it to construct an instance of 
// the subclass of the Application class that the application/game module defines.
extern "C"
{
	C4MODULEEXPORT C4::Application *ConstructApplication(void);
}

namespace C4
{
	// These values define what protocol and what port to use - feel free to edit the game port if required.
	enum
	{
		kGameProtocol = 0x0000000B,
		kGamePort = 28327
	};

	class Game : public Singleton<Game>, public Application
	{

	private:
		// We first define two console commands.

		// The server command will start a new server
		CommandObserver<Game> hostObserver;
		Command hostCommand;
		// The join command will join an existing game.
		CommandObserver<Game> joinObserver;
		Command joinCommand;

		CommandObserver<Game> quitObserver;
		Command quitCommand;

		int joinPort;

		ModelRegistration	robbieModelReg;
		//ball model registration
		ModelRegistration	ballModelReg;

		NavAction					*upAction;
		NavAction					*downAction;
		NavAction					*leftAction;
		NavAction					*rightAction;
		NavAction					*jumpAction;

		//Used for detecting an ESC key press
		InputMgr::KeyProc	*prevEscapeProc;
		void			*prevEscapeCookie;

	public:
		// Obligatory constructor / destructors.
		Game();
		~Game();

		EngineResult LoadWorld(const char *name);
		void UnloadWorld();

		// This method will be executed whenever the user uses the server command.
		void HostCommand(Command *command, const char *text);

		// This method will be executed whenever the user uses the join command.
		void JoinCommand(Command *command, const char *text);

		// This method will be executed whenever the user uses the join command.
		void QuitCommand(Command *command, const char *text);

		//Commands used by the GUI
		void HostGame();
		void JoinGame(String<> ipAddress);
		void QuitGame();
		void SetPort(String<> port);

		static void EscapeProc(void *cookie);

		// This method will be called by the engine whenever a chat is received.
		// It's used for a lot of other stuff, but that's outside the scope of this tutorial.
		void HandlePlayerEvent(PlayerEvent event, Player *player, const void *param);
		void HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param);

		Message *ConstructMessage(MessageType type, Decompressor &data) const;
		void ReceiveMessage(Player *sender, const NetworkAddress &address, const Message *message);

		void SpawnController(Player *player, Point3D location, long controllerIndex);

		static Player *ConstructPlayer(PlayerKey key, void *data);

	};

	// This is a pointer to the one instance of the Chat class through which
	// any other part of the application/game module can access it.
	extern Game * TheGame;

	
}

#endif // GAME_H

