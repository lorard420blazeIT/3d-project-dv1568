#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::LoadOBJ()
{
	std::wifstream fileIn(filename.c_str());

	if (fileIn)
	{

	}
	else 
	{
		std::cout << "Couldnt open obj file: " << filename.c_str() << std::endl;
	}

	while (fileIn)
	{
		checkChar = fileIn.get();

		switch (checkChar)
		{
		case '#':
			checkChar = fileIn.get();
			while (checkChar != '\n')
			{
				checkChar = fileIn.get();
			}
			break;

		case 'v':
			checkChar = fileIn.get();
			if (checkChar = ' ')
			{
				float vz, vy, vx;
				fileIn >> vx >> vy >> vz;

				if (isRHCoordSystem)
				{
					vertPos.push_back(dx::XMFLOAT3(vx, vy, vz * -1.0f));
				}
				else
				{
					vertPos.push_back(dx::XMFLOAT3(vx, vy, vz));
				}
			}


		}
	}

}
