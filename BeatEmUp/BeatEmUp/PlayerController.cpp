#include "PlayerController.h"
#include "Game.h"
#include "C4Configuration.h"

using namespace C4;


PlayerController::PlayerController() : CharacterController(kControllerPlayer)
{
	//walkForward = false;
	//walkBackward = false;

	//Set the movement default speeds
	movementSpeed = 1.5f;
	jumpPower = 3.0f;
	//rotationSpeed = 1.0;

	faceDirection = kFaceRight;
}


PlayerController::~PlayerController()
{
}

PlayerController::PlayerController(const PlayerController& playerController) : CharacterController(playerController)
{
	//walkForward = false;
	//walkBackward = false;

	movementSpeed = 1.5f;
	jumpPower = 3.0f;
	//rotationSpeed = 1.0;

	faceDirection = kFaceRight;
}

void PlayerController::Pack(Packer& data, unsigned long packFlags) const
{
	Controller::Pack(data, packFlags);
}

void PlayerController::Unpack(Unpacker& data, unsigned long unpackFlags)
{
	Controller::Unpack(data, unpackFlags);
}

Controller *PlayerController::Replicate(void) const
{
	return (new PlayerController(*this));
}

void PlayerController::SetMovementFlags(unsigned_int32 type)
{

	if (type == kMovementRight)											//if key down
	{
		//Engine::Report("Move Robbie Right Engage");
		movementVector.x = 1 * movementSpeed;
		
		//rotationVector.z = -1;

		//if currently facing left, make the controller face right
		if (faceDirection == kFaceLeft)
		{
			Matrix3D    rotator;

			rotator.SetRotationAboutZ(1.57f);	//rotate in radians

			SetRigidBodyMatrix3D(rotator);

			faceDirection = kFaceRight;
		}
		
	}
	else if (type == ~kMovementRight)			//if key down
	{
		if (movementVector.x != -1 * movementSpeed)
			movementVector.x = 0;
		
		//rotationVector.z = 0;
	}

	else if (type == kMovementLeft)
	{
		//Engine::Report("Move Robbie Left Engage");
		movementVector.x = -1 * movementSpeed;

		//rotationVector.z = 1;

		//if currently facing right, make the controller face left
		if (faceDirection == kFaceRight)
		{
			Matrix3D    rotator;

			rotator.SetRotationAboutZ(-1.57f);		//rotate in radians

			SetRigidBodyMatrix3D(rotator);

			faceDirection = kFaceLeft;
		}
	}
	else if (type == ~kMovementLeft)
	{
		if (movementVector.x != 1 * movementSpeed)
			movementVector.x = 0;

		//rotationVector.z = 0;
	}


	else if (type == kMovementUp)			//if key down
	{
		//Engine::Report("Move Robbie Up Engage");
		movementVector.y = 1 * movementSpeed;
		
		//walkForward = true;
	}
	else if (type == ~kMovementUp)			//if key down
	{
		if (movementVector.y != -1 * movementSpeed)
			movementVector.y = 0;
		
		//walkForward = false;
	}

	else if (type == kMovementDown)
	{
		//Engine::Report("Move Robbie Down Engage");
		movementVector.y = -1 * movementSpeed;
		
		//walkBackward = true;
	}
	else if (type == ~kMovementDown)
	{
		if (movementVector.y != 1 * movementSpeed)
			movementVector.y = 0;
		
		//walkBackward = false;
	}

	else if (type == kMovementJump)
	{
		Engine::Report("Robbie Jump");
		//movementVector.y = -1;
		if (GetGroundContact())
		{
			ApplyImpulse(Vector3D(0, 0, jumpPower));
		}
		
	}
	else if (type == ~kMovementJump)
	{

	}

}

void PlayerController::Preprocess(void)
{
	CharacterController::Preprocess();

	SetRigidBodyFlags(kRigidBodyKeepAwake | kRigidBodyFixedOrientation);
	SetFrictionCoefficient(0.0F);
	SetGravityMultiplier(2.0);

	//rotate the controller to face right when it spawns
	Matrix3D    rotator;

	rotator.SetRotationAboutZ(1.57f);	//rotate in radians

	SetRigidBodyMatrix3D(rotator);
}

void PlayerController::Move(void)
{

	//Apply movement to the controller
	float gravity = GetLinearVelocity().z;
	movementVector.z = gravity;

	SetLinearVelocity(movementVector);


	//forwardVector.Normalize();

	/*
	if (walkForward)
	{

		forwardVector = GetTargetNode()->GetNodeTransform()[1];
		forwardVector *= -movementSpeed;
		SetLinearVelocity(Vector3D(forwardVector.x, forwardVector.y, gravity));
	}
	else if (walkBackward)
	{

		forwardVector = GetTargetNode()->GetNodeTransform()[1];
		forwardVector *= movementSpeed;
		SetLinearVelocity(Vector3D(forwardVector.x, forwardVector.y, gravity));
	}
	else
	{
		SetLinearVelocity(Vector3D(0, 0, gravity));
	}
	*/


	//Used for updating the rotation of the character
	//SetAngularVelocity(rotationVector * rotationSpeed);



	// Update the external force for the rigid body representing the character.
	// The GetGroundContact() function is a member of the CharacterController base class.

	if (GetGroundContact())
	{
		//SetExternalLinearResistance(Vector2D(10.0F, 10.0F));
		//SetExternalForce(propel);
	}
	else
	{
		// If the soldier is not on the ground, reduce the propulsive force down to 2%.
		// This controls how well the player is able to control his movement while
		// falling through the air.

		//SetExternalLinearResistance(Zero2D);
		//SetExternalForce(propel * 0.02F);
	}
}

void PlayerController::ReceiveMessage(const ControllerMessage *message)
{
	switch (message->GetControllerMessageType())
	{
	case kControllerMessageState:
	{
		const ControllerStateMessage *msg = static_cast<const ControllerStateMessage *>(message);
		GetTargetNode()->SetNodePosition(msg->GetPosition());
		break;
	}
	case kControllerMessageDestroy:
	{
		Destroy();
		break;
	}
	case kControllerMessageBeginMovement:
	{
		const ControllerMovementMessage *m = static_cast<const ControllerMovementMessage *>(message);
		unsigned long flag = m->GetMovementFlag();
		//movementFlags |= flag;
		SetMovementFlags(flag);
		break;
	}
	case kControllerMessageEndMovement:
	{
		const ControllerMovementMessage *m = static_cast<const ControllerMovementMessage *>(message);
		//movementFlags &= ~m->GetMovementFlag();
		unsigned long flag = m->GetMovementFlag();
		SetMovementFlags(~flag);
		break;
	}
	}

	CharacterController::ReceiveMessage(message);
}

ControllerMessage *PlayerController::ConstructMessage(ControllerMessageType type) const
{
	switch (type)
	{
	case kControllerMessageState:

		return (new ControllerStateMessage(GetControllerIndex()));

	case kControllerMessageBeginMovement:
	case kControllerMessageEndMovement:

		return (new ControllerMovementMessage(type, GetControllerIndex()));
	}
	return (CharacterController::ConstructMessage(type));
}

void PlayerController::Destroy()
{
	Node *node = this->GetTargetNode();
	if (node)
	{
		delete node;
	}
}

void PlayerController::BeginMovement(unsigned long flag)
{
	const Point3D& position = GetTargetNode()->GetWorldPosition();
	Vector3D velocity = GetLinearVelocity();

	ControllerMovementMessage message(kControllerMessageBeginMovement, GetControllerIndex(), position, velocity, flag);
	TheMessageMgr->SendMessageAll(message);
}

void PlayerController::EndMovement(unsigned long flag)
{
	const Point3D& position = GetTargetNode()->GetWorldPosition();
	Vector3D velocity = GetLinearVelocity();

	ControllerMovementMessage message(kControllerMessageEndMovement, GetControllerIndex(), position, velocity, flag);
	TheMessageMgr->SendMessageAll(message);
}





ControllerStateMessage::ControllerStateMessage(long contIndex) : ControllerMessage(PlayerController::kControllerMessageState, contIndex)
{
	position.Set(0.0F, 0.0F, 0.0F);
}

ControllerStateMessage::ControllerStateMessage(long contIndex, Point3D p) : ControllerMessage(PlayerController::kControllerMessageState, contIndex)
{
	position = p;
}

ControllerStateMessage::~ControllerStateMessage()
{
}

void ControllerStateMessage::Compress(Compressor &data) const
{
	ControllerMessage::Compress(data);
	data << position;
}

bool ControllerStateMessage::Decompress(Decompressor &data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> position;
		return true;
	}
	return false;
}





ControllerDestroyMessage::ControllerDestroyMessage(long contIndex) : ControllerMessage(PlayerController::kControllerMessageDestroy, contIndex)
{
}

ControllerDestroyMessage::~ControllerDestroyMessage()
{
}






ControllerMovementMessage::ControllerMovementMessage(ControllerMessageType type, long controllerIndex) : CharacterStateMessage(type, controllerIndex)
{
}

ControllerMovementMessage::ControllerMovementMessage(ControllerMessageType type, long controllerIndex, const Point3D& position, const Vector3D& velocity, unsigned long flag) : CharacterStateMessage(type, controllerIndex, position, velocity)
{
	movementFlag = flag;
}

ControllerMovementMessage::~ControllerMovementMessage()
{
}

void ControllerMovementMessage::Compress(Compressor& data) const
{
	CharacterStateMessage::Compress(data);

	data << (unsigned char)movementFlag;
}

bool ControllerMovementMessage::Decompress(Decompressor& data)
{
	if (CharacterStateMessage::Decompress(data))
	{
		unsigned char	flag;

		data >> flag;
		movementFlag = flag;

		return (true);
	}

	return (false);
}









CharacterStateMessage::CharacterStateMessage(ControllerMessageType type, long controllerIndex) : ControllerMessage(type, controllerIndex)
{
}

CharacterStateMessage::CharacterStateMessage(ControllerMessageType type, long controllerIndex, const Point3D& position, const Vector3D& velocity) : ControllerMessage(type, controllerIndex)
{
	initialPosition = position;
	initialVelocity = velocity;
}

CharacterStateMessage::~CharacterStateMessage()
{
}

void CharacterStateMessage::Compress(Compressor& data) const
{
	ControllerMessage::Compress(data);

	data << initialPosition;
	data << initialVelocity;
}

bool CharacterStateMessage::Decompress(Decompressor& data)
{
	if (ControllerMessage::Decompress(data))
	{
		data >> initialPosition;
		data >> initialVelocity;
		return (true);
	}

	return (false);
}