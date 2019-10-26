#pragma once

#include "pch.h"

namespace Simplex {
	class WorldObject
	{
	protected:
		vector3 position; //The position of the object
		quaternion orientation; //The rotation of the object
		vector3 scale; //The scale of the object
		//TODO: Add collider / rigidbody to World Object
		MyMesh* model; // The model that represents the mesh
		int layer; //The collision layer of this object
	public:
#pragma region Memory Management
		/*
		USAGE: Initialize the object's fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		virtual void Init(void);
		/*
		USAGE: Releases the object from memory
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		virtual void Release(void);
		/*
		USAGE: Constructor
		ARGUMENTS: ---
		OUTPUT: class object
		*/
		WorldObject();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~WorldObject();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		WorldObject(WorldObject& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		WorldObject& operator=(WorldObject& other);
#pragma endregion
#pragma region Rendering
		/*
		USAGE: Renders the mesh by the provided camera view and projection
		ARGUMENTS:
		-	matrix4 a_mProjection -> Projection matrix
		-	matrix4 a_mView -> View matrix
		OUTPUT: ---
		*/
		void Render(matrix4 a_mProjection, matrix4 a_mView);
		/*
		USAGE: Renders the mesh by the provided camera view and projection
		ARGUMENTS: MyCamera a_mCamera -> the camera I want to see my scene from
		OUTPUT: ---
		*/
		void Render(MyCamera* a_pCamera);
		/*
		USAGE: Will render this mesh a_ToWorlsList size times
		ARGUMENTS:
		-	MyCamera* a_pCamera
		-	std::vector<matrix4> a_ToWorldList
		OUTPUT: ---
		*/
		void Render(MyCamera* a_pCamera, std::vector<matrix4> a_ToWorldList);
#pragma endregion
#pragma region Transformations
		/*
		USAGE: Moves the mesh by a specified amount
		ARGUMENTS:
		-	vector3 displacement -> The distance to move from the current position
		OUTPUT: ---
		*/
		void Translate(vector3 displacement);

		/*
		USAGE: Changes the orientation by a specified amount
		ARGUMENTS:
		-	vector3 rotation -> The amount to rotate by in euler rotations
		OUTPUT: ---
		*/
		void Rotate(vector3 rotation);

		/*
		USAGE: Changes the orientation by a specified amount
		ARGUMENTS:
		-	quaternion rotation -> THe amount to rotate by
		OUTPUT: ---
		*/
		void Rotate(quaternion rotation);

		/*
		USAGE: Scales the model by a set amount in each direction
		ARGUMENTS: 
		-	vector3 scaleAmount -> The amount to scale in each direction
		OUTPUT: ---
		*/
		void Scale(vector3 scaleAmount);

		/*
		USAGE: Scales the model uniformly in all directions
		ARGUMENTS:
		-	float scaleAmount -> the amount to scale by
		OUTPUT: ---
		*/
		void Scale(float scaleAmount);
#pragma endregion
	};
}