#include "SkeletalModel.h"
#include "Parser.h"

using namespace std;

void SkeletalModel::load(const char *skeletonFile)
{
	loadSkeleton(skeletonFile);

	ComputeTransforms();
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	Parser *reader;
	reader = new Parser();
	bool ok = reader->Open(filename);
	
	char* token = new char[];
	Joint* currJoint;
	Joint* parentJoint = nullptr;
	reader->GetToken(token);
	while (std::strcmp(token,"balljoint")==0)
	{
		currJoint = new Joint();
		m_joints.insert(m_joints.end(), currJoint);
		if (m_rootJoint == nullptr)
		{
			m_rootJoint = currJoint;
			currJoint->parentJoint = currJoint;			
		}  
		else if (parentJoint != nullptr)
		{
			currJoint->parentJoint = parentJoint;
			parentJoint->children.insert(parentJoint->children.end(), currJoint);
		}
		
		reader->SkipWhitespace();
		reader->GetToken(token);
		currJoint->Name = token;
		reader->SkipWhitespace();
		if (reader->FindToken("{"))
		{
			reader->SkipLine();
			reader->SkipWhitespace();
			reader->GetToken(token);
			while ((std::strcmp(token, "}") != 0))
			{
				if (std::strcmp(token, "offset") == 0)
				{
					currJoint->offset[0] = reader->GetFloat();
					currJoint->offset[1] = reader->GetFloat();
					currJoint->offset[2] = reader->GetFloat();
					
				}
				else if (std::strcmp(token, "boxmin") == 0)
				{
					currJoint->boxMin[0] = reader->GetFloat();
					currJoint->boxMin[1] = reader->GetFloat();
					currJoint->boxMin[2] = reader->GetFloat();
				}
				else if (std::strcmp(token, "boxmax") == 0)
				{
					currJoint->boxMax[0] = reader->GetFloat();
					currJoint->boxMax[1] = reader->GetFloat();
					currJoint->boxMax[2] = reader->GetFloat();
				}
				else if (std::strcmp(token, "rotxlimit") == 0)
				{
					currJoint->rotXMinLimit = reader->GetFloat();
					currJoint->rotXMaxLimit = reader->GetFloat();
				}
				else if (std::strcmp(token, "rotylimit") == 0)
				{
					currJoint->rotYMinLimit = reader->GetFloat();
					currJoint->rotYMaxLimit = reader->GetFloat();
				}
				else if (std::strcmp(token, "rotzlimit") == 0)
				{
					currJoint->rotZMinLimit = reader->GetFloat();
					currJoint->rotZMaxLimit = reader->GetFloat();
				}
				else if (std::strcmp(token, "pose") == 0)
				{
					float val = reader->GetFloat();
					if (val < currJoint->rotXMinLimit)
						currJoint->pose[0] = currJoint->rotXMinLimit;
					else if (val > currJoint->rotXMaxLimit)
						currJoint->pose[0] = currJoint->rotXMaxLimit;
					else
						currJoint->pose[0] = val;

					

					val = reader->GetFloat();
					if (val < currJoint->rotYMinLimit)
						currJoint->pose[1] = currJoint->rotYMinLimit;
					else if (val > currJoint->rotYMaxLimit)
						currJoint->pose[1] = currJoint->rotYMaxLimit;
					else
						currJoint->pose[1] = val;

					val = reader->GetFloat();
					if (val < currJoint->rotZMinLimit)
						currJoint->pose[2] = currJoint->rotZMinLimit;
					else if (val > currJoint->rotZMaxLimit)
						currJoint->pose[2] = currJoint->rotZMaxLimit;
					else
						currJoint->pose[2] = val;

					//straight up dragon
					/*currJoint->xRotation.rotateX(currJoint->pose[0]);
					currJoint->yRotation.rotateY(currJoint->pose[1]);
					currJoint->zRotation.rotateZ(currJoint->pose[2]);*/

					 currJoint->xRotation.CreateRotationMatrixX(currJoint->pose[0]);
					currJoint->yRotation.CreateRotationMatrixY(currJoint->pose[1]);
					currJoint->zRotation.CreateRotationMatrixZ(currJoint->pose[2]);
				}
				else if((std::strcmp(token, "balljoint") == 0))
				{
					parentJoint = currJoint;
					break;
				}
				reader->SkipLine();
				reader->SkipWhitespace();
				reader->GetToken(token);
			}
			if ((std::strcmp(token, "}") == 0))
			{
				reader->SkipLine();
				reader->SkipWhitespace();
				reader->GetToken(token);
				while (std::strcmp(token, "balljoint") != 0)
				{
					parentJoint = parentJoint->parentJoint;
					reader->SkipLine();
					reader->SkipWhitespace();
					reader->GetToken(token);
					if (std::strcmp(token, "") == 0)
						break;
					
				}
				
			}
				
		}
		else
		{
			printf("ERROR: ");
		}
		
	}
	reader->Close();
}



void SkeletalModel::drawSkeleton(Utils *utils )
{
	Joint* currentJoint;
	
	glMatrixMode(GL_MODELVIEW);
	if (m_joints.size() != 0)
	{
		for (std::vector<Joint*>::iterator i = m_joints.begin(); i != m_joints.end(); i++)
		{
			currentJoint = *i;

			glPushMatrix();
			
			glMultMatrixf(currentJoint->transform.get());
			utils->drawWireBox(currentJoint->boxMin[0], currentJoint->boxMin[1], currentJoint->boxMin[2], currentJoint->boxMax[0], currentJoint->boxMax[1], currentJoint->boxMax[2]);

			glPopMatrix();
		}
	}

}



void SkeletalModel::ComputeTransforms()
{
	Joint* currJoint;
	if (m_joints.size() != 0)
	{
		for (std::vector<Joint*>::iterator i = m_joints.begin(); i != m_joints.end(); i++)
		{
			currJoint = *i;
			Matrix4 aa = currJoint->yRotation * currJoint->xRotation;
			currJoint->transform = currJoint->zRotation *  aa ;
			
			currJoint->transform.translate(currJoint->offset);
			if (currJoint->parentJoint != currJoint)
				currJoint->transform = currJoint->parentJoint->transform * currJoint->transform;

			
			cout << currJoint->Name.c_str() << " Its parents is ----" << currJoint->parentJoint->Name.c_str() << "\n";
			
		}
	}
}



