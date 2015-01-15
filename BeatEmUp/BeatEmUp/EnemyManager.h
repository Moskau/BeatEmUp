#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "C4Engine.h"
#include "Game.h"
#include "BasicEnemyController.h"

namespace C4
{
	class EnemyManager : public Singleton<EnemyManager>
	{
	public:
		EnemyManager();
		~EnemyManager();

		void SpawnEnemy();
	};

	extern EnemyManager * TheEnemyManager;
}

#endif

