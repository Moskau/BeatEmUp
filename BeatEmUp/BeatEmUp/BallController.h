#pragma once

#include "C4Physics.h"
#include "BEUMultiplayer.h"

namespace C4
{

	enum
	{
		kModelBall = 'ball',
	};

	enum
	{
		kControllerkBall = 'ball'
	};


	//a networked rigid body ball
	class BallController : public RigidBodyController
	{
	private:
		BallController(const BallController& ballController);
		Controller *Replicate(void) const override;
	public:
		BallController();
		~BallController();

		void Preprocess(void);
	};

	//some message for the controller
	class BallMessage : public ControllerMessage
	{
	private:
		int value;
	public:
		BallMessage(long controllerIndex);
		BallMessage(long controllerIndex, int value);
		~BallMessage();

		void Compress(Compressor &data) const override;
		bool Decompress(Decompressor &data) override;

		int GetValue() const
		{
			return value;
		}
	};
}
