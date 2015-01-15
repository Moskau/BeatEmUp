#pragma once

#include "C4Cameras.h"
#include "C4World.h"
#include "BEUMultiplayer.h"

namespace C4
{

	class SimpleCamera : public FrustumCamera
	{
	public:
		SimpleCamera();
		~SimpleCamera();

		void Preprocess(void);

		void Move(void);
	};

	class GroupCamera : public FrustumCamera
	{
	public:
		GroupCamera();
		~GroupCamera();

		void Preprocess(void);

		void Move(void);
	};

	class GroupCamera2: public FrustumCamera
	{
	public:
		GroupCamera2();
		~GroupCamera2();

		void Preprocess(void);

		void Move(void);
	};


}

