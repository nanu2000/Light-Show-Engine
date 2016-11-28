#ifndef SETTINGS_H
#define SETTINGS_H


class Settings
{

public:

	const unsigned short	MIN_AMOUNT_LIGHTS_PER_ENTITY	= 0;
	const unsigned short	MAX_AMOUNT_LIGHTS_PER_ENTITY	= 50;
	const unsigned short	MIN_FILTER_AMOUNT				= 0;
	const unsigned short	MAX_FILTER_AMOUNT				= 20;

	const float				MIN_SHADOW_INTENSITY			= 0.5f;
	const float				MAX_SHADOW_INTENSITY			= 7;
	const float				MIN_FILTER_DISTANCE				= 0.1f;
	const float				MAX_FILTER_DISTANCE				= 10;



	void setAmountOfLightsPerEntity		(unsigned int amount)
	{ 
		if (amount > MIN_AMOUNT_LIGHTS_PER_ENTITY && amount <= MAX_AMOUNT_LIGHTS_PER_ENTITY)
		{
			amountOfLightsPerEntity = amount;
		}
	}

	void setShadowIntensity				(float intensity)
	{ 
		if (intensity > MIN_SHADOW_INTENSITY && intensity <= MAX_SHADOW_INTENSITY)
		{
			shadowIntesity = intensity;
		}
	}

	void setShadowFilteringAmount		(unsigned short filterAmount)
	{
		if (filterAmount > MIN_FILTER_AMOUNT && filterAmount <= MAX_FILTER_AMOUNT)
		{
			shadowFilteringAmount = filterAmount;
		}
	}

	void setShadowFilterDistance		(float distance)
	{
		if (distance > MIN_FILTER_DISTANCE && distance <= MAX_FILTER_DISTANCE)
		{
			shadowFilterDistance = distance;
		}
	}

	unsigned short	getLightsPerEntity		() const { return amountOfLightsPerEntity;	}
	unsigned short	getShadowFilterAmount	() const { return shadowFilteringAmount;	}
	float			getShadowIntensity		() const { return shadowIntesity;			}
	float			getShadowFilterDistance	() const { return shadowFilterDistance;		}

private:

	unsigned short	amountOfLightsPerEntity = 5;
	unsigned short	shadowFilteringAmount	= 15;
	float			shadowIntesity			= 1.f;
	float			shadowFilterDistance	= 1.f;


};


#endif // !SETTINGS_H
