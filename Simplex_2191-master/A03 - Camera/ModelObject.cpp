#include "ModelObject.h"
using namespace std;
using namespace Simplex;

ModelObject::ModelObject(string path, string ID) : FileReference(path, ID)
{
	modelObject = new Model();
	modelObject->Load(path);
	rigidBody = new RigidBody(modelObject->GetVertexList());
}

//Returns the bounds of this model
RigidBody* ModelObject::GetRigidBody()
{
	return rigidBody;
}
