#ifndef MODEL_SERIALIZATION
#define MODEL_SERIALIZATION

#include <SDL\SDL_endian.h>

#include "AnimatedModel.h"
#include "Model.h"

#include "Serialization.h"

using namespace SL;

namespace _3DM
{

	class AnimatedModel;
	class Model;
	//Consts

	const Byte Mat_4_ROW_COL_SIZE		= 4;

	//Amount of bytes allocated for variables that tell how many elements are in a vector.
	const Byte DEFAULT_SIZE_OF_VECTOR	= 2;

	const std::string _3DMA_Signature	= "_3DMA v1 By Richard. T. Sikra";
	const std::string _3DM_Signature	= "_3DM By Richard. T. Sikra";
	

	class _3DM_IO
	{

	public:

		/***************************************************************************************
		*Used to read a 3DMA Model File and return a Model. iStream::tellg is expected to be 0.*
		****************************************************************************************/
		AnimatedModel	readAnimatedModel	(const std::string & path);

		/*********************************************************************
		*Used to write a 3DMA Model File. oStream::tellp is expected to be 0.*
		**********************************************************************/
		void	writeAnimatedModel	(AnimatedModel & model, std::ofstream & oStream);


		/***************************************************************************************
		*Used to read a 3DMA Model File and return a Model. iStream::tellg is expected to be 0.*
		****************************************************************************************/
		Model	readModel(const std::string & path);

		/*********************************************************************
		*Used to write a 3DMA Model File. oStream::tellp is expected to be 0.*
		**********************************************************************/
		void	writeModel(Model & model, std::ofstream & oStream);

		~_3DM_IO();

	private:
		

		bool isBigEndianness = false;

		/***********************************************
		*Sets the ifStream's path, and sets tellG to 0.*
		************************************************/
		void setIfstream(const char * path);
		
		std::ifstream iStream;
		//Takes the Type of element, and a lambda that is capable of retrieving a single element (of Type T) from the file
		template<typename T,  typename Func>
		std::vector<T> readVector(Func func, std::ifstream& iStream)
		{
			std::vector<T> items;
			TwoBytes amountOfitems = 0;

			readBytes(amountOfitems, DEFAULT_SIZE_OF_VECTOR, iStream, isBigEndianness);

			items.resize(amountOfitems);
			for (unsigned int i = 0; i < amountOfitems; i++)
			{
				items[i] = func();
			}

			return items;

		}


		//Takes the Type of element, and a lambda that is capable of writing a single element (of Type T) to the file
		template<typename T, typename Func>
		void writeVector(Func func, std::vector<T>& items, std::ofstream & oStream)
		{
			TwoBytes amountOfItems = static_cast<TwoBytes>(items.size());

			writeBytes(amountOfItems, DEFAULT_SIZE_OF_VECTOR, oStream);
			for (unsigned int i = 0; i < items.size(); i++)
			{
				func(items[i], oStream);
			}
		}
		


		template<typename T>
		void			writeGLMVec(std::ofstream & oStream, std::vector<T>& glmVec, unsigned int amount, int sizeOfElement)
		{
			for (unsigned int i = 0; i < amount; i++)
			{
				for (int j = 0; j < glmVec[i].length(); j++)
				{
					oStream.write(reinterpret_cast<char*>(&glmVec[i][j]), sizeOfElement);
				}
			}
		}

		template<typename T>
		std::vector<T>	readGLMVec (std::ifstream & iStream, unsigned int amount, int sizeOfElement)
		{
			std::vector<T> returnVec;
			returnVec.resize(amount);
			for (unsigned int i = 0; i < amount; i++)
			{
				for (int j = 0; j < returnVec[i].length(); j++)
				{
					iStream.read(reinterpret_cast<char*>(&returnVec[i][j]), sizeOfElement);
				}
			}
			return returnVec;
		}
		
		

		Channel		readChannel			(std::ifstream & iStream);
		Animation		readAnimation		(std::ifstream & iStream);
		AnimatedMesh	readAnimatedMesh	(std::ifstream & iStream);
		AnimationClip	readAnimationClip	(std::ifstream & iStream);
		Mesh			readMesh			(std::ifstream & iStream);
		void readBoneHierarchyFromFile(std::ifstream& iStream, BoneNode & futureRootNode);
		


		void writeChannel				(Channel&,				std::ofstream & oStream);
		void writeAnimation				(Animation& animation,	std::ofstream & oStream);
		void writeAnimatedMesh			(AnimatedMesh& mesh,		std::ofstream & oStream);
		void writeAnimationClip			(AnimationClip& animClip, std::ofstream & oStream);
		void writeBoneHierarchyToFile	(BoneNode& boneNode,		std::ofstream & oStream);
		void writeMesh					(Mesh& mesh,				std::ofstream & oStream);

	};

}

#endif