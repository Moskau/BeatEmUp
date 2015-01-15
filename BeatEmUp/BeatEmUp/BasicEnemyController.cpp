#include "BasicEnemyController.h"

using namespace C4;

BasicEnemyController::BasicEnemyController(const BasicEnemyController& enemyController) : RigidBodyController(enemyController)
{

}

BasicEnemyController::BasicEnemyController() : RigidBodyController(kControllerBasicEnemy)
{

}


BasicEnemyController::~BasicEnemyController()
{
}

Controller *BasicEnemyController::Replicate(void) const
{
	return(new BasicEnemyController(*this));
}

void BasicEnemyController::Preprocess(void)
{
	RigidBodyController::Preprocess();

	SetRigidBodyFlags(kRigidBodyKeepAwake | kRigidBodyFixedOrientation);
	SetFrictionCoefficient(0.0F);
	SetGravityMultiplier(2.0);

	currentState = nullptr;

	currentStateIndex = 0;
	nextStateIndex = 0;

	currentStateIndex = kIdleState;
	EnemyStateApproach * temp = new EnemyStateApproach();
	enemyStateMap.insert(std::pair<unsigned_int32, EnemyState *>(kIdleState, temp));


	currentState = enemyStateMap[currentStateIndex];

	currentState->Enter(currentStateIndex);

	//EnemyState_Approach *name = new EnemyState_Approach(*this);
	//enemyStateMap.Insert(name);
}

void BasicEnemyController::Move(void)
{
	if (currentState != nullptr)
	{
		currentState->Update();
	}

	if (nextStateIndex != 0)
	{
		currentState->Leave(nextStateIndex);
		currentState = enemyStateMap[nextStateIndex];
		currentState->Enter(currentStateIndex);

		currentStateIndex = nextStateIndex;
		nextStateIndex = 0;
	}
}