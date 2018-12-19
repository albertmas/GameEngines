#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Axis,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:

	Color color;
	float4x4 transform;
	bool axis = false; bool wire = false;

protected:
	PrimitiveTypes type;
};


class PCube : public Primitive
{
public:
	PCube();
	PCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const override;
public:
	float3 size;
};

class PPlane : public Primitive
{
public:
	PPlane();
	void Create(float x = 0, float y = 0, float z = 0, float d = 200);
	void Render() const;
public:
	float depth;
};

class PAxis : public Primitive
{
public:
	PAxis();
	void Create(float thickness = 2.0f);
	void Render() const;
public:
	float thickness;
};
#endif // __PRIMITIVE_H__