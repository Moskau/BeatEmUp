#include "Camera.h"

using namespace C4;

SimpleCamera::SimpleCamera() :
FrustumCamera(2.0F, 1.0F)
{
}


SimpleCamera::~SimpleCamera()
{
}

void SimpleCamera::Preprocess(void)
{
	FrustumCamera::Preprocess();
}

void SimpleCamera::Move(void)
{
	//get local player
	GamePlayer *player = static_cast<GamePlayer*>(TheMessageMgr->GetLocalPlayer());
	if (player)
	{
		//get player's controller
		PlayerController* controller = player->GetController();
		if (controller)
		{
			//get node for controller 
			Model *model = static_cast<Model*>(controller->GetTargetNode());
			if (model)
			{
				//get model world position
				const Point3D& position = model->GetWorldPosition();
				SetNodePosition(Point3D(position.x, -12.0F, 4.5F));
				LookAtPoint(Point3D(position.x, 0.0F, 0.0F));
			}
		}
	}
}

GroupCamera::GroupCamera() :
FrustumCamera(2.0F, 1.0F)
{
}


GroupCamera::~GroupCamera()
{
}

void GroupCamera::Preprocess(void)
{
	FrustumCamera::Preprocess();
}

void GroupCamera::Move(void)
{

	GamePlayer *gp = nullptr;
	PlayerController *cont = nullptr;
	Node *node = nullptr;
	Point3D loc;
	int numPlayers = 0;
	float sumX = 0;
	float xPos = 0;
	//float sumY = 0;
	float yPos = 0;
	float min = 0;
	float max = 0;
	float offset=0;

	//find center of mass, min, and max
	Player *p = TheMessageMgr->GetFirstPlayer();
	if (p)
	{
		gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			node = cont->GetTargetNode();
			loc = node->GetWorldPosition();
			sumX = sumX + loc.x;
			yPos = loc.y;
			min = sumX;
			max = sumX;
			numPlayers++;
		}

		p = p->Next();
	}
	while (p)
	{
		gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			node = cont->GetTargetNode();
			loc = node->GetWorldPosition();
			sumX = sumX + loc.x;
			if (loc.y < yPos)
			{
				yPos = loc.y;
			}
			if (loc.x < min){
				min = loc.x;
			}
			else if (loc.x > max)
			{
				max = loc.x;
			}
			numPlayers++;
		}

		p = p->Next();
	}

	xPos = sumX / numPlayers;
	offset = max - min;
	if (offset < 8.0F){
		offset = 8.0F;
	}

	SetNodePosition(Point3D(xPos, yPos - offset, 4.5F));
	LookAtPoint(Point3D(xPos, yPos, 0.0F));
}

GroupCamera2::GroupCamera2() :
FrustumCamera(2.0F, 1.0F)
{
}


GroupCamera2::~GroupCamera2()
{
}

void GroupCamera2::Preprocess(void)
{
	FrustumCamera::Preprocess();
}

void GroupCamera2::Move(void)
{

	GamePlayer *gp = nullptr;
	PlayerController *cont = nullptr;
	Node *node = nullptr;
	Point3D loc;
	int numPlayers = 0;
	float sumX = 0;
	float xPos = 0;
	float min = 0;
	float max = 0;


	//find center of mass, min, and max
	Player *p = TheMessageMgr->GetFirstPlayer();
	if (p)
	{
		gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			node = cont->GetTargetNode();
			loc = node->GetWorldPosition();
			sumX = sumX + loc.x;
			min = sumX;
			max = sumX;
			numPlayers++;
		}

		p = p->Next();
	}
	while (p)
	{
		gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			node = cont->GetTargetNode();
			loc = node->GetWorldPosition();
			sumX = sumX + loc.x;
			if (loc.x < min){
				min = loc.x;
			}
			else if (loc.x > max)
			{
				max = loc.x;
			}
			numPlayers++;
		}

		p = p->Next();
	}

	xPos = sumX / numPlayers;

	//float offset = -5.0F;
	float yPos = 4.0F;
	float y = 0;

	//find center of mass, min, and max
	p = TheMessageMgr->GetFirstPlayer();
	while (p)
	{
		gp = static_cast<GamePlayer *>(p);
		cont = gp->GetController();
		if (cont)
		{
			node = cont->GetTargetNode();
			loc = node->GetWorldPosition();
			if (loc.x > xPos)
			{
				//use y = 2x
				y = 2.0F*(loc.x - xPos);
			}
			else
			{
				//use y = -2x
				y = 2.0F*(xPos - loc.x);
			}
			//get distance y to loc.y
			y = y - loc.y;
			//check is y is minimum value (negative)
			if (yPos < y){
				yPos = y;
			}
		}

		p = p->Next();
	}

	SetNodePosition(Point3D(xPos, -4.5F - yPos, 3.5F));
	LookAtPoint(Point3D(xPos, 0.0, 0.0F));
}
