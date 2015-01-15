#include "EnemyState.h"

using namespace C4;

EnemyState::EnemyState()
{

}

EnemyState::EnemyState(RigidBodyController* enemyCont) : enemyController(enemyCont)
{
	
}

EnemyState::~EnemyState()
{
}

void EnemyState::Enter(unsigned_int32 prevState)
{
	EnterState(prevState);
}

void EnemyState::Leave(unsigned_int32 nextState)
{
	LeaveState(nextState);
}

void EnemyState::Update()
{
	UpdateState();
}

void EnemyState::EnterState(unsigned_int32 prevState)
{

}

void EnemyState::LeaveState(unsigned_int32 nextState)
{

}

void EnemyState::UpdateState()
{
	TheEngine->Report("update execute");
}
