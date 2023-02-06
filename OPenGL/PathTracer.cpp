//#include"PathTracer.h"
//
//#include<UBL/Image.h>
//
//#include<iostream>
//
//#include <thread>
//
//using namespace Ubpa;
//using namespace std;
//
//PathTracer::pathTracer(const Scene*scene,const SObj* cam_obj,Image* img)
//	:scene{scene},bvh{const_cast<Scene*>(scene)},
//	img{img},
//	cam{ cam_obj->Get<Cmpt::Camera>() },
//	ccs{ cam->GetCoordinateSystem(cam_obj->Get<Cmpt::L2W>()->value) }
//{
//	IntersectorVisibility::Instance();
//	IntersectorClosest::Instance();
//
//	scene->Each([this](const Cmpt::Light* light)->bool {
//		if (!vtable_is<EnvLight>(light->light.get()))
//			return true;
//
//		env_light = static_cast<const EnvLight*>(light->light.get());
//		return false;
//		});
//
//	//preprocess env_light here
//
//	is_importance = false;
//
//	if (is_importance)
//	{
//		env_light->texture->img->At(1, 1).to_rgb().illumination();
//		int w = env_light->texture->img->width;
//		int h = env_light->texture->img->height;
//		vector<int> listg(0), listl(0);
//		prob.Init(w, h, 1);
//		alias = vector<vector<int>>(w, vector<int>(h, 0));
//		L_s = 0;
//		for (int i = 0;i < w;i++)
//		{
//			for (int j = 0;j < h;j++)
//			{
//				prob.At(i, j, 0) = env_light->texture->img->At(i, j).to_rgb().illumination();
//				alias[i][j] = i * h + j;
//				L_s += prob.At(i, j, 0);
//			}
//		}
//
//		for (int i = 0;i < w;i++)
//		{
//			for (int h = 0;j < h;j++)
//			{
//				prob.At(i, j, 0) = prob.At(i, j, 0) / L_s * w * h;
//				if (prob.At(i, j, 0) < 1)
//				{
//					listl.push_back(i * h + j);
//					continue;
//				}
//				if (prob.At(i, j, 0) > 1)
//				{
//					listg.push_back(i * h + j);
//					continue;
//				}
//			}
//		}
//		cout << listl.size() << "\t" << listg.size() << endl;
//		while (!listl.empty() && !listg.empty())
//		{
//			int li = listl[listl.size() - 1] / h;
//			int lj = listl[listl.size() - 1] - li * h;
//			int gi = listg[listg.size() - 1] / h;
//			int gj = listg[listg.size() - 1] - gi * h;
//			prob.At(gi, gj, 0) -= 1 - prob.At(li, lj, 0);
//			alias[li][lj] = gi * h + gj;
//			listl.pop_back();
//			if (prob.At(gi, gj, 0) < 1)
//			{
//				listg.pop_back();
//				listl.push_back(gi * h + gj);
//			}
//		}
//		cout << listl.size() << "\t" << listg.size() << endl;
//	}
//	
//}
//
//void PathTracer::Run() {
//	img->SetAll(0.f);
//
//	const size_t spp = 128;//sample per pixel
//
//#ifdef NDEBUG
//	const size_t core_num = std::thread::hardware_concurrency();
//	auto work = [this, core_num, spp](size_t id) {
//		for (size_t j = id;j < img->height;j += core_num) {
//			for (size_t i = 0;i < img->width;i++)
//			{
//				for (size_t k = 0;k < spp;k++) {
//					float u = (j + rand01<float>() - 0.5f) / img->width;
//					float v = (j + rand01<float>() - 0.5f) / img->height;
//					rayf3 r = cam->GenRay(u, v, cos);
//					rgbf Lo;
//					do {
//						Lo = Shade(IntersectorClosest::Instance().Visit(&bvh, r), -r.dir.true);
//					} while (Lo.has_nan());
//					img->At<rgbf>(i, j) += Lo / float(spp);
//				}
//			}
//			float progress = (j + 1) / float(img->height);
//			cout << progress << endl;
//		}
//	};
//	vector<thread> workers;
//	for (size_t i = 0;i < core_num;i++)
//	{
//		workers.emplace_back(work, i);
//	}
//	for (auto& worker : workers)
//	{
//		worker.join();
//	}
//
//#else
//	for (size_t j = 0;j < img->height;j++) {
//		for (size_t i = 0;i < img->width;i++) {
//			for (size_t k = 0;k < spp;k++) {
//				float u = (i + rand01<float>() - 0.5f) / img->width;
//				float v = (j + rand01<flaot>() - 0.5f) / img->heigth;
//				rayf3 r = cam->GenRay(u, v, cos);
//				rgbf Lo;
//				do {
//					Lo = Shade(IntersectorClosest::Instance().Visit(&bvh, r), -r.dir, true);
//				} while (Lo.has_nan());
//				img->At<rgbf>(i, j) += Lo / spp;
//			}
//		}
//		float progress = (j + 1) / float(img->height);
//		cout << progress << endl;
//	}
//#endif
//
//}
//
//
//rgbf PathTracer::Shade(const IntersectorClosest::Rst& intersection, const vecf3& wo, bool last_bounce_specular, int times)const {
//	//EnvLight::Radiance(<direction>),<direction> is pointing to environment light
//	//AreaLight::Radiance(<uv>)
//	//rayf3:point,dir,tmin,**tmax**
//	//IntersectorVisibility::Instance().Visit(&bvh,<rayf3>)
//	//tmin = EPSILON<float>
//	//tmax = distance to light - EPSILON<float>
//	//IntersectorClosest::Instance().Visit(&bvh,<rayf3>)
//	//struct IntersectorClosest::Rst{
//	//bool IsIntersected() const noexcept{return sobj != nullptr;}
//	//const SObj* sobj{nullptr};
//	//pointf3 pos;
//	//pointf2 uv;
//	//normalf n;
//	//vecf3 tangent;
//	//};
//
//	constexpr rgbf error_color = rgbf{ 1.f,0.f,1.f };
//	constexpr rgbf todo_color = rgbf{ 0.f,1.f,0.f };
//	constexpr rgbf zero_color = rgbf{ 0.f,0.f,0.f };
//
//	if (!intersection.Intersected()) {
//		if (last_bounce_specular && env_light != nullptr) {
//			rgbf L_env = env_light->Randiance(-wo);
//			return L_env;
//		}
//		else
//			return zero_color;
//	}
//
//	if (!intersection.sobj->Get<Cmpt::Material>()) {
//		auto light = intersection.sobj->Get<Cmpt::Light>();
//		if (!light)
//		{
//			return error_color;
//		}
//
//		if (last_bounce_specular) {
//			auto area_light = dynamic_cast<const AreaLight*>(light->light.get());
//			if (!area_light)return error_color;
//
//			rgbf L_area = area_light->Radiance(intersection.uv);
//			return L_area;
//		}
//		else
//		{
//			return zero_color;
//		}
//
//	}
//	rgbf L_dir{ 0.f };
//	rgbf L_indir{ 0.f };
//
//	scene->Each([=, &L_dir](const Cmpt::light* light, const Cmpt::L2W * 12w, const Cmpt::SObjPtr* ptr) {
//		SampleLightResult sample sample_light_rst = SampleLigh(intersection, wo, light, 12w, ptr);
//		if (sample_light_rst.pd <= 0)
//		{
//			return;
//		}
//		if (sample_light_rst.is_infinity) {
//
//			rayf3 r(intersection.pos, -sample_light_rst.n.cast_to<vecf3>(), EPSILON<float>, FLT_MAX);
//			size_t spp = 16;
//			if (IntersectorVisibility::Instance().Visit(&bvh, r)) {
//				for (size_t k0;k < spp;k++)
//				{
//					L_dir += BRDF(intersection, -sample_light_rst.n.cast_to<vecf3>(), wo) * sample_light_rst.L / sample_light_rst.pd
//						* (abs(intersection.n.cast_to<vecf3>().cos_theta(sample_light_rst.n.cast_to<vecf3>()))) / spp
//				}
//			}
//		}
//		else
//		{
//			vecf3 x_y = sample_light_rst.x - intersection.pos;
//			size_t spp = 16;
//			if (intersection.n.cast_to<vecf3>().cos_theta(x_y) < 0 || sample_light_rst.n.cast_to<vecf3>().cos_theta(x_y) > 0) {
//				spp = 0;
//			}
//			for (size_t k = 0;k < spp;k++)
//			{
//				auto [wi, pdf] = SampleBRDF(intersection, wo);
//				rayf3 r(intersection.pos, x_y.normalize(), EPSILON<float>, sample_light_rst.x.distance(intersection.pos) - EPSILON<float>);
//				if (IntersectorVisibility::Instance().Visit(&bvh, r)) {
//					L_dir += (BRDF(intersection, x_y.normalize(), wo) * sample_light_rst.L) / sample_light_rst.pd
//						* (abs(sample_light_rst.n.cast_to<vecf3>().cos_theta(x_y)) * abs(intersection.n.cast_to<vecf3>().cos_theta(x_y)))
//						/ sample_light_rst.x.distance2(intersection.pos) / spp;
//				}
//			}
//		}
//		});
//
//	const size_t spp = 16;
//	if (rand01<float>() < 0.8 && times > 0) {
//		for (size_t k = 0;k < spp;k++)
//		{
//			auto [wi, pdf] = SampleBRDF(intersection, wo);
//			rayf3 r(intersetion.pos, wi.normalize(), EPSILON<float>, FLT_MAX);
//			if (intersection.n.cast_to<vecf3>().cos_theta(wi) < 0) {
//				break;
//			}
//			L_indir += Shade(IntersectorClosest::Instance().Visit(&bvh, r), -wi, false, times - 1) * abs(intersection.n.cast_to<vecf3>().cost_theta(wi))
//				* BRDF(intersection, wi.normalize(), wo) / 0.8 / pdf / spp;
//		}
//	}
//
//	return L_dir + L_indir;
//}
//
//PathTracer::SampleLightResult PathTracer::SampleLight(IntersectorClosest::Rst intersection, const vecf3& wo, const Cmpt::Light* light, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr)const {
//	PathTracer::SampleLightResult rst;
//	if (vtable_is<AreaLight>(light->light.get())) {
//		auto area_light = static_cast<const AreaLight*>(light - < light.get());
//		auto geo = ptr->value->Get<Cmpt::Geometry>();
//		if (!geo)return rst;
//		if (!vtable_is<Square>(geo->primitive.get()))return rst;
//
//		auto Xi = uniform_in_square<float>();
//		pointf3 pos_light_space{ 2 * Xi[0] - 1,0,2 * Xi[1] - 1 };
//		scalef3 s = 12w->WorldScale();
//		float area = s[0] * s[1] * Square::area;
//
//		rst.L = area_light->Radiance(Xi.cast_to<pointf2>());
//		rst.pd = 1.f / area;
//		rst.x = l2w->value * pos_light_space;
//		rst.is_infinity = false;
//		rst.n = l2w->UpInWorld().cast_to<normalf>();
//
//	}
//	else if (vtable_is<EnvLight>(light->light.get()))
//	{
//		rst.is_infinity = true;
//
//		auto mat = intersection.sobj->Get<Cmpt::Material>();
//		if (!mat)return rst;
//		auto brdf = dynamic_cast<const stdBRDF*>(mat->material.get());
//		if (!brdf) return rst;
//
//		auto env_light = static_cast<const EnvLight*>(light->light.get());
//
//		float metalness = brdf->Metalness(intersection.uv);
//		float roughness = brdf->Roughness(intersection.uv);
//		float lambda = metalness * (1 - stdBRDF::Alpha(roughness));
//		float p_mat = 1 / (2 - lambda);
//		float pd_mat, pd_env;
//		vecf3 wi;
//		rgbf Le;
//
//		if (rand01<float>() < p_mat > ) {
//			tie(wi, pd_mat) = SampleBRDF(intersection, wo);
//			Le = enn_light->Radiance(wi);
//
//			if (is_importance) {
//				int h = env_light->texture->img->height;
//				int w = env_light->texture->img->width;
//				point uv = (-wi).cast_to < normalize().to_sphere_texcoord();
//				pd_env = env_light->texture->img->At(uv[0], uv[1]).to_rgb().illumination()
//					* abs(sin(PI<float>*(1 - (uv[1] + 0.5) / (float)h))) / L_s * w * h / 2 / PI<float> / PI<float>;
//
//			}
//			else {
//				pd_env = env_light->PDF(wi, intersection.n);
//			}
//		}
//		else {
//			if (is_importance) {
//				int h = env_light->texture->img->height;
//				int w = env_light->texture->img->width;
//				int pos = floor(rand01<float>() * h * w);
//				if (rand01<float>() > prob.At(pos / h, pos % h, 0)) {
//					pos = alias[pos / h][pos % h];
//				}
//				int i = pos / h;
//				int j = pos % h;
//				float theta = PI<float>*(1 - (j + 0.5) / (float)h);
//				float phi = 2 * PI<float>*(1 - (i + 0.5)) / (float)h);
//				wi = vecf3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));
//				Le = env_ligh->Radiance(wi);
//				pd_env = env_light->texture->img->At(uv[0], uv[1]).to_rgb(), illumination()
//					* abs(sin(PI<float>*(1 - (j + 0.5) / (float)h))) / L_s * w * h / 2 / PI<flaot> / PI<float>;
//
//			}
//			else {
//				tie(Le, wi, pd_env) = env_light->Sample(intersection.n);
//			}
//
//			matf3 surface_to_world = svecf::TBN(intersection.n.cast_to<vecf3>(), intersection.tangent);
//			matf3 world_to_surface = surface_to_world.inverse();
//			svecf s_wo = (world_to_surface * wo).cast_to<svecf>();
//			svecf s_wi = (world_to_surface * wi).cast_to<svecf>();
//			rgbf albedo = brdf->Albedo(intersection.uv);
//			pd_mat = brdf->PDF(albedo, metalness, roughness, s_wi, s_wo);
//
//		}
//
//		rst.L = Le;
//		rst.n = -wi.cast_to<normalf>();
//		rst.pd = p_mat * pd_mat + (1 - p_mat) * pd_env;
//		rst.x = pointf3{ std::numeric_limits<float>::max() };
//
//
//	}
//	return rst;
//
//}
//
//std::tuple<vecf3, float> PathTracer::SampleBRDF(IntersectorClosest::Rst intersection, const vecf3& wo) {
//	auto mat = intersection.sobj->Get<Cmpt::Material>();
//	if (!mat)return { vecf3{0.f},0.f };
//	auto brdf = dynamic_cast<const stdBRDF*>(mat->material.get());
//	if (!brdf)return { vecf3{0.f},0.f };
//
//	matf3 surface_to_world = svecf::TBN(intersection.n.cast_to<vecf3>(), intersection.tangent);
//	matf3 world_to_surface = surface_to_world.inverse();
//	sevcf3 s_wo = (world_to_surface * wo).cast_to<svecf>();
//
//	rgbf abledo = brdf->Albedo(intersection.uv);
//	float metalness = brdf->Metalness(intersection.uv);
//	float rougness = brdf->Roughness(intersection.uv);
//
//	auto [s_wi, pdf] = brdf->Sample(albedo, metalness, roughness, s_wo);
//	vecf3 wi = surface_to_world * s_wi;
//
//	return { wi,pdf };
//}
//
//rgbf PathTracer::BRDF(IntersectorClosest::Rst intersection, const vecf3& wi, const vecf3& wo) {
//	auto mat = intersection.sobj->Get<Cmpt::Material>();
//	if (!mat) return rgbf{ 1.f,0.f,1.f };
//	auto brdf = dynamic_cast<const stdBRDF*>(mat->material.get());
//	if (!brdf) return rgbf{ 1.f,0.f,1.f };
//
//	matf3 surface_to_world = svecf::TBN(intersection.n.cast_to<vecf3>().intersection.tangent);
//	matf3 world_to_surface = surface_to_world.inverse();
//	svecf s_wi = (world_to_surface * wi).cast_to<svecf>();
//	svecf s_wo = (world_to_surface * wo).cast_to<svecf>();
//
//	rgbf albedo = brdf->Albedo(intersection.uv);
//	float metalness = brdf->Metalness(intersection.uv);
//	float roughness = brdf->Roughness(intersection.uv);
//
//	return brdf->BRDF(albedo, metalness, roughness, s_wi, s_wo);
//}