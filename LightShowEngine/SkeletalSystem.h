#ifndef SKELETAL_SYSTEM_H
#define SKELETAL_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include "AnimationKeys.h"

namespace _3DM
{

	/*********************************************************
	* A bone node is equivelent  a single bone in a skeleton.*
	**********************************************************/
	struct BoneNode
	{		
		std::string				name;									//the name of this bone
		std::vector <BoneNode>	children;								//the children of this bone node
	};


	/****************************************************************************************************************
	*A channel contains all of the key frames in a single bones animation. Each bone should have a matching channel.*
	*****************************************************************************************************************/
	struct Channel
	{
		std::string					name;								//the name of this channel
		std::vector <Vector3Key>	positionKeys;						//the position keys of this channel
		std::vector <Vector3Key>	scalingKeys;						//the scaling keys of this channel
		std::vector <QuatKey>		rotationKeys;						//the rotation keys of this channel
	};


	/**********************************************************************************************************************************************
	*An animation contains the channels for the animation, the duration, the ticks per second and the transformations needed to animate the bones.*
	***********************************************************************************************************************************************/
	struct Animation
	{
		float									duration;				//the duration of the animation in ticks
		float									ticksPerSecond;			//ticks per second of this animation

		BoneNode								rootBone;				//the root bone node of the animation.

		std::vector <Channel>					channels;				//the channels in the animation.

		std::map	<std::string, glm::mat4>	boneOffset;				//the offset matricies of the bones

		std::vector <glm::mat4>					boneTransformations;	//the bone transformations that are uploaded to the vertex shader.
	};



	struct AnimationClip
	{
		std::string name;
		float startTime;
		float endTime;
	};


}

#endif