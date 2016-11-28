#ifndef FOUNTAIN_PARTICLE_EMITTER
#define FOUNTAIN_PARTICLE_EMITTER
#include "ParticleSystem.h"
class FountainParticleEmitter : public ParticleEmitter, public Component<FountainParticleEmitter>
{
public:

	void setFountainPull			(const float& pull)		{ fountainPull				= pull;		}
	void setFountainVerticalForce	(const float& force)	{ particleVerticalForce		= force;	}
	void setFountainSpread			(const float& spread)	{ particleHorizontalSpread	= spread;	}
	void setDefaultEndScale			(const float& scale)	{ endScale					= scale;	}
private:

	void setParticleToDefault			(Particle& p) override;
	void performParticleCalculations	(Particle& p) override;

	float endScale					= 1;
	float fountainPull				= 20;
	float particleHorizontalSpread	= 5;
	float particleVerticalForce		= 5;

};


#endif