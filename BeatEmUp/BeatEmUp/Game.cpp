#include "Game.h"

using namespace C4;

Game *C4::TheGame = nullptr;

Application *ConstructApplication(void)
{
	return (new Game);
}

Game::Game() : Singleton<Game>(TheGame),
hostObserver(this, &Game::HostCommand),
joinObserver(this, &Game::JoinCommand),
quitObserver(this, &Game::QuitCommand),
hostCommand("host", &hostObserver),
joinCommand("join", &joinObserver),
quitCommand("leave", &quitObserver),
robbieModelReg(kModelRobbie, "Robbie", "models/robbie/Robbie", kModelPrecache, kControllerPlayer),
ballModelReg(C4::kModelBall, nullptr, "model/Ball", C4::kModelPrecache, C4::kControllerkBall)
{
	// Register both console commands with the engine.
	TheEngine->AddCommand(&hostCommand);
	TheEngine->AddCommand(&joinCommand);
	TheEngine->AddCommand(&quitCommand);

	joinPort = 0;

	TheMessageMgr->SetPlayerConstructor(&ConstructPlayer, this);

	// These create the movement actions that are used to
	// move the player around and interact with objects.

	upAction = new NavAction(kActionUp, kMovementUp);
	downAction = new NavAction(kActionDown, kMovementDown);
	leftAction = new NavAction(kActionLeft, kMovementLeft);
	rightAction = new NavAction(kActionRight, kMovementRight);
	jumpAction = new NavAction(kActionJump, kMovementJump);

	// These register our new actions with the Input Manager.

	TheInputMgr->AddAction(upAction);
	TheInputMgr->AddAction(downAction);
	TheInputMgr->AddAction(leftAction);
	TheInputMgr->AddAction(rightAction);
	TheInputMgr->AddAction(jumpAction);


	prevEscapeProc = TheInputMgr->GetEscapeProc();
	prevEscapeCookie = TheInputMgr->GetEscapeCookie();
	TheInputMgr->SetEscapeProc(&EscapeProc, this);

	// Let the Interface Manager determine when to change input devices to gameplay mode.

	TheInterfaceMgr->SetInputManagementMode(kInputManagementAutomatic);

	// Create the start window and tell the Interface Manager to display it.

	StartWindow::Open();
}

Game::~Game()
{
	if (upAction)
		delete upAction;

	if (downAction)
		delete downAction;

	if (leftAction)
		delete leftAction;

	if (rightAction)
		delete rightAction;

	if (jumpAction)
		delete jumpAction;

	this->UnloadWorld();

	// If the start window exists, delete it. We need to do this here, instead of letting the
	// Interface Manager clean it up, because the destructor code for the window is in this DLL.

	if (TheStartWindow)
		delete TheStartWindow;

	TheInputMgr->SetEscapeProc(prevEscapeProc, prevEscapeCookie);

}

void Game::HostCommand(Command *command, const char *text)
{
	HostGame();
}

void Game::JoinCommand(Command *command, const char *text)
{
	JoinGame(text);
}


// functions used for handling connection widgets

//Handles the event when ESC is pressed
void Game::EscapeProc(void *data)
{
	StartWindow::Open();
}

void Game::QuitCommand(Command *command, const char *text)
{
	QuitGame();
}

void Game::HostGame()
{
	// Set some settings in the network manager.
	TheNetworkMgr->SetProtocol(kGameProtocol);
	TheNetworkMgr->SetPortNumber(kGamePort);
	TheNetworkMgr->SetBroadcastPortNumber(kGamePort);
	
	// Just start the server. The 'true' parameter 
	// is to indicate that this machine is the server.
	TheMessageMgr->SetMaxPlayerCount(4);
	TheMessageMgr->BeginMultiplayerGame(true);
	TheEngine->Report(String<>("Initialized. Hosting on: ") + MessageMgr::AddressToString(TheNetworkMgr->GetLocalAddress(), true));

	//Spawn World
	WorldResult result = TheGame->LoadWorld("Base-01");				//HARDCODED

	//Spawn controller
	TheMessageMgr->SendMessageAll(RequestMessage());
}

void Game::JoinGame(String<> ipAddress)
{
	// Set some settings in the network manager.
	TheNetworkMgr->SetProtocol(kGameProtocol);
	TheNetworkMgr->SetPortNumber(joinPort);
	TheNetworkMgr->SetBroadcastPortNumber(kGamePort);

	// We'll first want to provide the user with some feedback - so he'll know what he's doing.
	String<128> str("Attempting to join ");
	str += ipAddress;
	TheEngine->Report(str, kReportError);

	// Next, we convert the entered parameters into a C4 NetworkAddress.
	// This format is used internally. It has both an IP address and a port number.
	NetworkAddress address = MessageMgr::StringToAddress(ipAddress);

	// We explicitly set a port in this example - it defaults to 0.
	address.SetPort(kGamePort);

	// Now we're just going to (try to) connect to the entered address.
	TheMessageMgr->Connect(address);
}

void Game::QuitGame()
{
	TheMessageMgr->DisconnectAll();
	TheEngine->Quit();
}

void Game::SetPort(String<> port){
	if (port != ""){
		String<128> ipAddress("0.0.0.0:");
		ipAddress += port;
		NetworkAddress address = MessageMgr::StringToAddress(ipAddress);
		joinPort = address.GetPort();
		TheEngine->Report("blahh");
	}
	TheEngine->Report("port not changed!");
}


EngineResult Game::LoadWorld(const char *name)
{
	WorldResult result = TheWorldMgr->LoadWorld(name);

	if (result == C4::kWorldOkay)
	{
		World *world = TheWorldMgr->GetWorld();
		if (world)
		{
			/*
			//Check if the game is single player or multiplayer
			if (TheMessageMgr->Multiplayer)
			{

			}
			else
			{
			TheMessageMgr->BeginSinglePlayerGame
			}
			*/

			//add camera
			GroupCamera2 *camera = new GroupCamera2();
			//camera->SetNodePosition(Point3D(-10.0F, -20.0F, 8.0F));
			//camera->LookAtPoint(Point3D(0.0F, 0.0F, 0.0F));
			//camera->Invalidate();
			world->SetCamera(camera);

			new EnemyManager();

			C4::TheEnemyManager->SpawnEnemy();

			/*
			//add ball
			Node *root = TheWorldMgr->GetWorld()->GetRootNode();
			
			Controller *ballController = new BallController();
			Model *ballModel = nullptr;
			ballModel = Model::Get(kModelBall);
			if (ballModel)
			{
				ballModel->SetController(ballController);
				ballModel->SetNodePosition(Point3D(1.0F, 1.0F, 5.0F));
				root->AddNewSubnode(ballModel);
				ballModel->Update();
			}
			
			Controller *ballController2 = new BallController();
			Model *ballModel2 = nullptr;
			ballModel2 = Model::Get(kModelBall);
			if (ballModel2)
			{
				ballModel2->SetController(ballController2);
				ballModel2->SetNodePosition(Point3D(-2.0F, -2.0F, 2.0F));
				root->AddNewSubnode(ballModel2);
				ballModel2->Update();
			}
			*/
		}

		TheEngine->Report("Game has begun!");
	}

	return result;
}

void Game::UnloadWorld()
{
	TheWorldMgr->UnloadWorld();
	TheMessageMgr->EndGame();
}


void Game::ReceiveMessage(Player *sender, const NetworkAddress &address, const Message *message)
{
	switch (message->GetMessageType())
	{
	case kMessageSpawn: break;

	case kMessageRequest:
	{
		//Only server will read message
		if (TheMessageMgr->Server())
		{
			//Generate a random spawn location
			Point3D loc;
			loc.x = -5.0F + Math::Random(10.0F);
			loc.y = -5.0F + Math::Random(10.0F);
			loc.z = 0.0F;

			long contIndex = TheWorldMgr->GetWorld()->NewControllerIndex();
			TheMessageMgr->SendMessageAll(SpawnMessage(sender->GetPlayerKey(), contIndex, loc));
		}
		break;
	}
	}
}

Message *Game::ConstructMessage(MessageType type, Decompressor &data) const
{
	switch (type)
	{
	case kMessageSpawn: return (new SpawnMessage);

	case kMessageRequest: return (new RequestMessage);

	case kMessageMovementBegin:
	case kMessageMovementEnd:

		return (new ClientMovementMessage(type));

	case kMessageOrientation:

		return (new ClientOrientationMessage);
	}

	return nullptr;
}

void Game::HandlePlayerEvent(PlayerEvent event, Player *player, const void *param)
{
	switch (event)
	{
	case kPlayerInitialized:
	{
		TheEngine->Report("Player initialized.");

		//Spawn controller for all players joining
		GamePlayer *gp = nullptr;
		PlayerController *cont = nullptr;
		Node *node = nullptr;
		PlayerKey key = -1;
		long id = -1;
		Point3D loc;

		// This player just joined, so send him 1 message per
		// player to spawn a character
		Player *p = TheMessageMgr->GetFirstPlayer();
		while (p)
		{
			gp = static_cast<GamePlayer *>(p);

			cont = gp->GetController();
			if (cont)
			{
				node = cont->GetTargetNode();

				key = gp->GetPlayerKey();
				id = cont->GetControllerIndex();
				loc = node->GetWorldPosition();

				TheMessageMgr->SendMessage(player->GetPlayerKey(), SpawnMessage(key, id, loc));
			}

			p = p->Next();
		}
		break;
	}

	case kPlayerDisconnected:
	{
		break;
	}

		// We've received a chat. 
	case kPlayerChatReceived:
	{
		// We'll want to display the player's name in front of the chat message,
		// so we'll first paste the player's name and his message together in a String object.
		// We limit the size of the displayed text using the String class, which automatically
		// cuts off text that exceeds the boundary set in the template parameter.

		String<kMaxChatMessageLength + kMaxPlayerNameLength + 2> text(player->GetPlayerName());
		text += ": ";
		text += static_cast<const char *>(param);

		// Next, we'll make the completed message appear in the console.
		// The kReportError parameter tells the engine to put the message in the console. 
		// It doesn't actually mean there's an error.

		TheEngine->Report(text, kReportError);
		break;
	}
	}

	// Finally, we pass the player event to the parent Application's HandlePlayerEvent method,
	// so it can display errors if needed. The method does nothing at the moment, but we'll
	// add it just in case it will somewhere in the future.
	Application::HandlePlayerEvent(event, player, param);
}

void Game::HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param)
{
	switch (event)
	{
	case kConnectionClientOpened:
	{
		Engine::Report("Client Connected.");
		break;
	}
	case kConnectionClientClosed:
	case kConnectionClientTimedOut:
	{
		Engine::Report("Client Connection Closed.");
		break;
	}
	case kConnectionServerAccepted:
	{
		Engine::Report("We are connected.");
		WorldResult result = TheGame->LoadWorld("Base-01");				//HARDCODED <-Send request for current world and load it

		//SEND CONTROLLER REQUEST
		if (result == kWorldOkay)
		{
			Engine::Report("Requesting Controller.");
			TheMessageMgr->SendMessageAll(RequestMessage());
		}
		break;
	}
	case kConnectionServerClosed:
	case kConnectionServerTimedOut:
	{
		Engine::Report("Server Connection Closed.");
		UnloadWorld();
		break;
	}
	}

	Application::HandleConnectionEvent(event, address, param);
}

void Game::SpawnController(Player *player, Point3D location, long controllerIndex)
{
	World *world = TheWorldMgr->GetWorld();
	if (world)
	{
		Engine::Report("Spawn Controller.");

		GamePlayer *gPlayer = static_cast<GamePlayer *>(player);
		PlayerController *cont = gPlayer->GetController();
		if (!cont)
		{
			cont = new PlayerController();
			gPlayer->SetController(cont);
			cont->SetControllerIndex(controllerIndex);

			Model *model = Model::Get(kModelRobbie);
			model->SetController(cont);

			model->SetNodePosition(location);
			world->AddNewNode(model);
		}
	}
}

Player *Game::ConstructPlayer(PlayerKey key, void *data)
{
	return (new GamePlayer(key));
}