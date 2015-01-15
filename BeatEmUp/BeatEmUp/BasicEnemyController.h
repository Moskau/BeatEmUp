#ifndef BASIC_ENEMY_CONTROLLER_H
#define BASIC_ENEMY_CONTROLLER_H

/*
#include "C4Physics.h"
#include "BEUMultiplayer.h"

#include "EnemyStateApproach.h"
*/
#include "C4Physics.h"
#include <map>
#include "EnemyState.h"
#include "EnemyStateApproach.h"

namespace C4
{

	enum
	{
		kModelEnemy = 'bnme',
	};

	enum
	{
		kControllerBasicEnemy = 'bnme'
	};

	class BasicEnemyController : public RigidBodyController
	{
	private:
		BasicEnemyController(const BasicEnemyController& enemyController);
		Controller *Replicate(void) const override;

		std::map<unsigned_int32, EnemyState *> enemyStateMap;

		EnemyState *currentState;

		unsigned_int32 currentStateIndex;
		unsigned_int32 nextStateIndex;

	public:
		
		BasicEnemyController();
		~BasicEnemyController();

		void Preprocess(void);
		void Move(void);

		unsigned_int32 GetCurrentStateIndex(void)
		{
			return currentStateIndex;
		}
		
		const EnemyState& GetCurrentState(void) const
			{
				return *currentState;
			}
			
	};

}

#endif

