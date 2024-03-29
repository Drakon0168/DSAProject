#pragma once

#include "pch.h"

namespace Simplex {
	class WorldObject
	{
	protected:
		vector3 position; //The position of the object
		quaternion orientation; //The rotation of the object
		vector3 scale; //The scale of the object
		vector3 center; //The local center of the mesh
		matrix4 transform; //The ToWorld matrix of the object
		Model* model; // The model that represents the mesh
		int layer; //The collision layer of this object
		int index; // Index within collidables

		float radius; //The radius of the object's bounding sphere
		vector3 globalMin; //The minimum x y and z position of the object in global space
		vector3 globalMax; //The maximum x y and z position of the object in global space
		vector3 localMin; //The minimum x y and z position of the object in local space
		vector3 localMax; //The maximum x y and z positon of the object in local space
		vector3 localHalfWidth; //The local half width of the object
		vector3 globalHalfWidth; //The global half width of the object

		bool renderCollider = false; //Whether or not to draw the collider
		const bool showSphere = true; //Whether or not to draw the sphere collider
		const bool showAABB = true; //Whether or not to draw the AABB collider
		const bool showARBB = true; //Whether or not to draw the ARBB collider
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
#pragma region Accessors
		/*
		USAGE: Returns the object's position
		ARGUMENTS:
		OUTPUT: The object's current position
		*/
		vector3 GetPosition();

		/*
		USAGE: Sets the object's position
		ARGUMENTS:
		-	Vector3 value -> The position to set
		OUTPUT: ---
		*/
		void SetPosition(vector3 value);

		/*
		USAGE: Returns the object's rotation
		ARGUMENTS:
		OUTPUT: The object's current rotation
		*/
		quaternion GetRotation();

		/*
		USAGE: Sets the object's orientation
		ARGUMENTS:
		-	Quaternion value -> The orientation to set
		OUTPUT: ---
		*/
		void SetRotation(quaternion value);


		int GetIndex();

		void SetIndex(int value);
		/*
		USAGE: Returns the object's scale
		ARGUMENTS:
		OUTPUT: The object's current scale
		*/
		vector3 GetScale();

		/*
		USAGE: Returns the object's scale
		ARGUMENTS:
		-	vector3 value -> The value to set scale to
		OUTPUT: ---
		*/
		void SetScale(vector3 value);

		/*
		USAGE: Returns the object's transformation matrix
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		matrix4 GetTransform();

		/*
		USAGE: Returns the object's model
		ARGUMENTS:
		OUTPUT: The object's model
		*/
		Model* GetModel();

		/*
		USAGE: Sets up the object's model
		ARGUMENTS: modelPath
		OUTPUT: ---
		*/
		void LoadModel(std::string modelFilePath, std::string identifier);

		/*
		USAGE: Sets the object's model
		ARGUMENTS:
		-	Model* model -> A reference to the model that the object is using
		OUTPUT:
		*/
		void SetModel(Model* newModel);

		/*
		USAGE: Returns the object's collision layer
		ARGUMENTS:
		OUTPUT: The object's current collision layer
		*/
		int GetLayer();

		void SetLayer(int value);

		/*
		USAGE: Returns the object's sphere collider radius
		ARGUMENTS:
		OUTPUT: The object's spherical radius
		*/
		float GetRadius();

		/*
		USAGE: Returns the vector3 representation of the object's local mins
		ARGUMENTS:
		OUTPUT: The object's local min
		*/
		vector3 GetLocalMin();

		/*
		USAGE: Returns the vector3 representation of the object's local maxs
		ARGUMENTS:
		OUTPUT: The object's local max
		*/
		vector3 GetLocalMax();

		/*
		USAGE: Returns the vector3 representation of the object's global mins
		ARGUMENTS:
		OUTPUT: The object's global min
		*/
		vector3 GetGlobalMin();

		/*
		USAGE: Returns the vector3 representation of the object's global maxs
		ARGUMENTS:
		OUTPUT: The object's global max
		*/
		vector3 GetGlobalMax();

		/*
		USAGE: Returns the vector3 representation of the object's local half width
		ARGUMENTS:
		OUTPUT: The object's local half width
		*/
		vector3 GetLocalHalfWidth();

		/*
		USAGE: Returns the vector3 representation of the object's global half width
		ARGUMENTS:
		OUTPUT: The object's global half width
		*/
		vector3 GetGlobalHalfWidth();

		/*
		USAGE: Returns whether or not the collider is set to draw
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		bool GetRenderCollider();

		/*
		USAGE: Sets whether or not to draw the collider
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void SetRenderCollider(bool value);
#pragma endregion
#pragma region Rendering
		/*
		USAGE: Renders the mesh by the provided camera view and projection
		ARGUMENTS:
		-	matrix4 a_mProjection -> Projection matrix
		-	matrix4 a_mView -> View matrix
		OUTPUT: ---
		*/
		virtual void Render(matrix4 projection, matrix4 view);
		/*
		USAGE: Renders the mesh by the provided camera view and projection
		ARGUMENTS: MyCamera a_mCamera -> the camera I want to see my scene from
		OUTPUT: ---
		*/
		void Render(MyCamera* camera);
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
		-	vector3 rotation -> The amount to rotate by in euler(radian) rotations
		OUTPUT: ---
		*/
		void Rotate(vector3 rotation);

		/*
		USAGE: Changes the orientation by a specified amount
		ARGUMENTS:
		-	quaternion rotation -> The amount to rotate by
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

		/*
		USAGE: Updates the transform based on the scale and local min and max
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void UpdateRadius();

		/*
		USAGE: Updates the transform based on the position orientation and scale
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void UpdateTransform();
#pragma endregion
#pragma region World Object
		/*
		USAGE: Applies logic based changes when this object collides with another object actual
			forces and collision resolution are handled by the physics manager
		ARGUMENTS:
		-	WorldObject other -> The object that has been collided with
		OUTPUT: ---
		*/
		virtual void OnCollision(WorldObject* other);

		/*
		USAGE: Scales the model uniformly in all directions
		ARGUMENTS:
		-	vector3 position -> the position to convert
		OUTPUT: The position in global coordinates
		*/
		vector3 ToWorld(vector3 position);

		/*
		USAGE: Re-Calculates the global min and max variables and sets up the global halfwidth and sphere collider radius
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void CalculateGlobalMinMax();
#pragma endregion
	};
}