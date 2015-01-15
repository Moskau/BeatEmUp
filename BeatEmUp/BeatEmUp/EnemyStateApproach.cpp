#include "EnemyStateApproach.h"

using namespace C4;

EnemyStateApproach::EnemyStateApproach()
{
}

EnemyStateApproach::EnemyStateApproach(RigidBodyController* enemyCont) : EnemyState(enemyCont)
{
}


EnemyStateApproach::~EnemyStateApproach()
{
}

void EnemyStateApproach::EnterState(unsigned_int32 prevState)
{
	TheEngine->Report("approach entered");
}

void EnemyStateApproach::LeaveState(unsigned_int32 nextState)
{

}

void EnemyStateApproach::UpdateState()
{
	//TheEngine->Report("approach execute");
}