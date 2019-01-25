#include "../../main.h"

bool key_values::load_from_buffer(key_value* key_valuez, char const* resource_name, const char* buffer, ifile_system* file_system, const char* path_id) {
	using originalfn = int(__thiscall*)(key_value*, char const*, const char*, ifile_system*, const char*);
	uint64_t signature = utilities::find_signature("engine.dll", "55 8B EC 83 EC 38 53 8B 5D 0C");
	
	static originalfn load_from_the_buffer = (originalfn)signature;
	return load_from_the_buffer(key_valuez, resource_name, buffer, file_system, path_id);
}

key_value* key_values::initialize(key_value* key_valuez, char* name) {
	using originalfn = key_value *(__thiscall*)(key_value*, char*);

	uint64_t signature = utilities::find_signature("engine.dll", "FF 15 ? ? ? ? 83 C4 08 89 06 8B C6");
	signature -= 0x42; static originalfn initialize = (originalfn)signature;

	return initialize(key_valuez, name);
}

namespace chams {
	imaterial* chams::create_material(bool ignorez, bool flat, bool wireframe, bool add_shine) {
		static int created = 0;

		static const char tmp[] = {
			"\"%s\"\
			\n{\
			\n\t\"$basetexture\" \"vgui/white_additive\"\
			\n\t\"$envmap\" \"%s\"\
			\n\t\"$normalmapalphaenvmapmask\" \"%i\"\
			\n\t\"$envmapcontrast\" \"%i\"\
			\n\t\"$model\" \"1\"\
			\n\t\"$flat\" \"1\"\
			\n\t\"$nocull\" \"0\"\
			\n\t\"$selfillum\" \"1\"\
			\n\t\"$halflambert\" \"1\"\
			\n\t\"$nofog\" \"0\"\
			\n\t\"$ignorez\" \"%i\"\
			\n\t\"$znearer\" \"0\"\
			\n\t\"$wireframe\" \"%i\"\
			\n}\n"
		};

		const char* baseType = (flat ? "UnlitGeneric" : "VertexLitGeneric");
		char material[512];
		sprintf_s(material, sizeof(material), tmp, baseType, add_shine ? "env_cubemap" : "", add_shine ? 1 : 0, add_shine ? 1 : 0, (ignorez) ? 1 : 0, (wireframe) ? 1 : 0);

		char name[512];
		sprintf_s(name, sizeof(name), "#textured_cham_material%i.vmt", created);
		created++;

		key_value* keyvalues = (key_value*)malloc(sizeof(key_value));
		key_values::initialize(keyvalues, (char*)"UnlitGeneric");
		key_values::load_from_buffer(keyvalues, name, material);

		imaterial* created_material = interfaces::material_system->create_material(name, keyvalues);
		if (!create_material) {
			return nullptr;
		} created_material->increment_reference_count();
		return created_material;
	}
	void chams::create_our_mats() {
		/*global::visible = create_material(false);
		global::hidden = create_material(true);*/
	}
	void chams::force_material(colour_t colour, imaterial* material) {
		if (!material) {
			return;
		}

		float blend[3] = {  (float)colour[0] / 255.f, 
							(float)colour[1] / 255.f, 
							(float)colour[2] / 255.f };
		interfaces::render_view->set_colour_modulation(blend);
		interfaces::model_render->force_material_override(material);
	}
}