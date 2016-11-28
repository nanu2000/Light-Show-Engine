#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

class ModelBase
{
public:
	virtual void renderSingleMesh(unsigned int index, ShaderBase & shader) = 0;
	virtual void renderAll(ShaderBase & shader) = 0;
	virtual void initialize(ShaderBase& shader) = 0;

	virtual ~ModelBase() {}
	bool isAnimatedModel() { return animatedModel; }

	Transform transform;
protected:
	bool animatedModel = false;

};


#endif