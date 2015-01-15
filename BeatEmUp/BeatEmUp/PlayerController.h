#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "C4Character.h"

namespace C4
{
	enum
	{
		kMovementUp= 1 << 0,
		kMovementDown = 1 << 1,
		kMovementLeft = 1 << 2,
		kMovementRight = 1 << 3,
		kMovementJump = 1 << 4,
		kMovementPlanarMask = 15
	};

	// Model types are associated with a model resource using the ModelRegistration
	// class. Models are registered with the engine in the Game constructor.

	enum
	{
		kModelRobbie = 'robb'
	};

	//Register controller so World Editor can view it
	enum
	{
		kControllerPlayer = 'plco'
	};

	//Enum used for tracking the controllers facing direction
	enum
	{
		kFaceRight,
		kFaceLeft
	};

	class PlayerController : public CharacterController
	{
	private:
		PlayerController(const PlayerController& playerController);

		// The movement flags tell how the user is trying to move the player.

		unsigned_int32		movementFlags;

		Vector3D			movementVector;

		Vector3D			forwardVector;

		Antivector3D		rotationVector;

		float				movementSpeed;
		float				jumpPower;
		//float				rotationSpeed;

		Controller *Replicate(void) const override;

		//bool				walkForward;
		//bool				walkBackward;

		unsigned_int32		faceDirection;
	public:
		enum
		{
			kControllerMessageBeginMovement = kRigidBodyMessageBaseCount,
			kControllerMessageEndMovement,
			kControllerMessageState,
			kControllerMessageDestroy
		};

		PlayerController();
		~PlayerController();

		/*
		Model *GetTargetNode(void) const
		{
		return (static_cast<Model *>(Controller::GetTargetNode()));
		}
		*/

		void Pack(Packer& data, unsigned long packFlags) const;
		void Unpack(Unpacker& data, unsigned long unpackFlags);

		unsigned_int32 GetMovementFlags(void) const
		{
			return (movementFlags);
		}

		void SetMovementFlags(unsigned_int32 type);

		void Preprocess(void);

		// The function that moves the target node
		void Move(void);

		void Destroy(void);

		void BeginMovement(unsigned long flag);
		void EndMovement(unsigned long flag);

		void ReceiveMessage(const ControllerMessage *message);
		ControllerMessage *ConstructMessage(ControllerMessageType type) const;
	};


	class ControllerStateMessage : public ControllerMessage
	{
	public:

		ControllerStateMessage(long controllerIndex);
		ControllerStateMessage(long controllerIndex, Point3D p);
		~ControllerStateMessage();

		void Compress(Compressor &data) const override;
		bool Decompress(Decompressor &data) override;

		Point3D GetPosition(void) const
		{
			return position;
		}

		void SetPosition(Point3D p)
		{
			position = p;
		}

	private:

		Point3D position;
	};

	class ControllerDestroyMessage : public ControllerMessage
	{
	public:

		ControllerDestroyMessage(long controllerIndex);
		~ControllerDestroyMessage();
	};

	class CharacterStateMessage : public ControllerMessage
	{
		friend class CharacterController;

	private:

		Point3D		initialPosition;
		Vector3D	initialVelocity;

	public:

		CharacterStateMessage(ControllerMessageType type, long controllerIndex);
		CharacterStateMessage(ControllerMessageType type, long controllerIndex, const Point3D& position, const Vector3D& velocity);
		~CharacterStateMessage();

		const Point3D& GetInitialPosition(void) const
		{
			return (initialPosition);
		}

		const Vector3D& GetInitialVelocity(void) const
		{
			return (initialVelocity);
		}

		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};

	class ControllerMovementMessage : public CharacterStateMessage
	{
		friend class PlayerController;

	private:

		unsigned long	movementFlag;

		ControllerMovementMessage(ControllerMessageType type, long controllerIndex);

	public:

		ControllerMovementMessage(ControllerMessageType type, long controllerIndex, const Point3D& position, const Vector3D& velocity, unsigned long flag);
		~ControllerMovementMessage();

		long GetMovementFlag(void) const
		{
			return (movementFlag);
		}

		void Compress(Compressor& data) const;
		bool Decompress(Decompressor& data);
	};
}

#endif

