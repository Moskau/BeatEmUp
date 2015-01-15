#ifndef ENEMY_STATE_APPROACH_H
#define ENEMY_STATE_APPROACH_H

#include "EnemyState.h"

namespace C4
{

	class EnemyStateApproach : public EnemyState
	{
	public:
		EnemyStateApproach();
		EnemyStateApproach(RigidBodyController* enemyCont);
		~EnemyStateApproach();
	protected:
		void EnterState(unsigned_int32 prevState) override;
		void LeaveState(unsigned_int32 nextState) override;
		void UpdateState() override;
	};

}

#endif