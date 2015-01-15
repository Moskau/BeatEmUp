#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

#include "C4Character.h"
#include "C4Map.h"
#include "C4Engine.h"

namespace C4
{

	enum EnemyStateEnum
	{
		kIdleState = 1,
		kApproachState,
		kAttackState
	};

	class EnemyState //: public MapElement<EnemyState>
	{
	public:
		//typedef EnemyStateEnum KeyType;
		
		//EnemyStateEnum theKey;
		EnemyState();
		EnemyState(RigidBodyController* enemyCont);
		~EnemyState();

		void Enter(unsigned_int32 prevState);
		void Leave(unsigned_int32 nextState);
		void Update();

		/*
		KeyType GetKey(void) const
		{
			return (theKey);
		}
		*/

	protected:
		virtual void EnterState(unsigned_int32 prevState) = 0;
		virtual void LeaveState(unsigned_int32 nextState) = 0;
		virtual void UpdateState() = 0;

		RigidBodyController* enemyController;
	};
}

#endif

