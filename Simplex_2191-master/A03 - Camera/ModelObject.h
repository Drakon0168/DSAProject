#pragma once
#include "FileReference.h"
#include "pch.h"
using namespace std;
using namespace Simplex;

class ModelObject : public FileReference
{
		private:
			RigidBody* rigidBody;
			Model* modelObject;

		public:
			ModelObject(string path, string ID);
			RigidBody* GetRigidBody();
};

