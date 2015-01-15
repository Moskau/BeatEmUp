#ifndef MOVEMENT_ACTION_H
#define MOVEMENT_ACTION_H

#include "C4Engine.h"
#include "C4Input.h"

namespace C4
{
	enum
	{
		kActionUp = 'frwd',
		kActionDown = 'bkwd',
		kActionLeft = 'left',
		kActionRight = 'rght',
		kActionJump = 'jump'
	};

	class NavAction : public Action
	{
	private:

		unsigned long	movementFlag;

	public:

		NavAction(unsigned long type, unsigned long flag);
		~NavAction();

		void Begin(void);
		void End(void);
	};
}

#endif

