//#pragma once
//
//#include <UScene/UScene.h>
//
//#include <vector>
//
//using namespace std;
//
//namespace Ubpa {
//	
//	class PathTracer {
//	public:
//		pathTracer(const Scene* scene, const SObj* cam_obj, Image* img);
//
//		void Run();
//
//	private:
//		rgbf Shade(const IntersectorClosest::Rst& intersection, const vecf3& wo, bool last_bounce_specular = false, int times = 5)const;
//
//		struct SampleLightResult {
//			rgbf L{ 0.f };// light radiance
//			float pd{ 0.f };//probability density
//			normalf n{ 0.f };//normalize normal
//			pointf3 x{ 0.f };//position on light
//			bool is_infinity{ false };//infinity distance
//		};
//		SampleLightResult SampleLight(IntersectorClosest::Rst intersection, const vecf3& wo, const Cmpt::Light* light, const Cmpt::L2W * 12w, const Cmpt::SObjPtr* ptr)const;
//
//		static std::tuple<vecf3, float> SampleBRDF(IntersectorClosest::Rst intersection, const vecf3& wo);
//		static rgbf BRDF(IntersectorClosest::Rst intersection, const vecf3& wi, const vecf3& wo);
//
//		const Scene* const scene;
//		const EnvLight* env_light{ nullptr };
//		Image* const img;
//		Image prob;
//		vector<vector<int>>alias;
//		float L_s;
//		bool is_importance;
//
//		EVH bvh;
//
//		const Cmpt::Camera* const cam;
//		const Cmpt::Camera::CoordinateSystem ccs;
//
//	};
//}