#ifndef _directional_light_h_
#define _directional_light_h_

#include "GameObject.h"

namespace engine
{
	class DirectionalLight : public GameObject
	{
		// constructor
		public:
			DirectionalLight(Screen * scn) : GameObject(scn) { }

		// destructor
		public:
			~DirectionalLight() { }

		// interface
		public:
			void SetDirection(float x, float y, float z)
			{
				// create material
				D3DMATERIAL9 mtrl;
				ZeroMemory( &mtrl, sizeof(mtrl) );
				mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
				mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
				mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
				mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
				m_device->SetMaterial( &mtrl );

				// create light
				D3DLIGHT9 light;
				ZeroMemory(&light, sizeof(light));

				light.Type = D3DLIGHT_DIRECTIONAL;				

				light.Ambient.r = 1;
				light.Ambient.g = 1;
				light.Ambient.b = 1;
				light.Ambient.a = 1;

				light.Diffuse.r = 0.3;
				light.Diffuse.g = 0.3;
				light.Diffuse.b = 0.3;
				light.Diffuse.a = 0.3;

				light.Specular.r = 0.3;
				light.Specular.g = 0.3;
				light.Specular.b = 0.3;
				light.Specular.a = 0.3;

				light.Direction.x = x;
				light.Direction.y = y;
				light.Direction.z = z;

				m_device->SetLight(0, &light);
				m_device->LightEnable(0, TRUE);

			//	m_device->SetRenderState( D3DRS_AMBIENT, 0x00202020 );
			}
	};
}

#endif