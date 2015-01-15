#include "EnemyManager.h"

using namespace C4;

EnemyManager *C4::TheEnemyManager = nullptr;

EnemyManager::EnemyManager() : Singleton<EnemyManager>(TheEnemyManager)
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::SpawnEnemy(void)
{
	Node *root = TheWorldMgr->GetWorld()->GetRootNode();
	Controller *enemyController = new BasicEnemyController();
	Model *ballModel = nullptr;
	ballModel = Model::Get(kModelBall);
	if (ballModel)
	{
		ballModel->SetController(enemyController);
		ballModel->SetNodePosition(Point3D(0.0F, 0.0F, 5.0F));
		root->AddNewSubnode(ballModel);
		ballModel->Update();
	}
}
