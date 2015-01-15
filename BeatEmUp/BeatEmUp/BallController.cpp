#include "BallController.h"

using namespace C4;

BallController::BallController(const BallController& ballController) : RigidBodyController(ballController)
{
}

BallController::BallController() : RigidBodyController(kControllerkBall)
{
}

BallController::~BallController()
{
}

Controller *BallController::Replicate(void) const
{
	return(new BallController(*this));
}

void BallController::Preprocess(void)
{
	RigidBodyController::Preprocess();

	SetRestitutionCoefficient(0.95F);
}

BallMessage::BallMessage(long controllerIndex) : ControllerMessage(kMessageBallMessage, controllerIndex)
{
	value = 0;
}

BallMessage::BallMessage(long controllerIndex, int v) : ControllerMessage(kMessageBallMessage, controllerIndex)
{
	value = v;
}

BallMessage::~BallMessage()
{
}

void BallMessage::Compress(Compressor &data) const
{
	ControllerMessage::Compress(data);
	data << value;
}
bool BallMessage::Decompress(Decompressor &data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> value;
		return(true);
	}
	return(false);
}