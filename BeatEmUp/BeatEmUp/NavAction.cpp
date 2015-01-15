#include "NavAction.h"
#include "C4Engine.h"
#include "Game.h"

using namespace C4;

NavAction::NavAction(unsigned long type, unsigned long flag) : Action(type)
{
	movementFlag = flag;
}

NavAction::~NavAction()
{
}

void NavAction::Begin(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		const PlayerController *controller = static_cast<const GamePlayer *>(player)->GetController();
		if (controller)
		{
			ClientMovementMessage message(kMessageMovementBegin, movementFlag);
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}

}

void NavAction::End(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		const PlayerController *controller = static_cast<const GamePlayer *>(player)->GetController();
		if (controller)
		{
			ClientMovementMessage message(kMessageMovementEnd, movementFlag);
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}

}
