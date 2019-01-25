#include "main.h"

c_base_weapon* c_base_player::get_active_weapon() {
	HANDLE weapon_handle = *reinterpret_cast<HANDLE*>((uintptr_t)this + 0xDB8);
	return (c_base_weapon*)interfaces::entitylist->get_client_entity_from_handle(weapon_handle);
}

const char* c_base_player::get_model_name() {
	return interfaces::model_info->get_model_name(this->get_model());
}

namespace utilities {
	void utilities::vector_transform(const vec3_t& some, const matrix3x4_t& matrix, vec3_t& out) {
		out[0] = some.dot((const vec3_t&)matrix[0]) + matrix[0][3];
		out[1] = some.dot((const vec3_t&)matrix[1]) + matrix[1][3];
		out[2] = some.dot((const vec3_t&)matrix[2]) + matrix[2][3];
	}
	vec3_t utilities::angle_vector(vec3_t meme) {
		auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
		auto cy = cos(meme.y / 180.f * static_cast<float>(PI));
		auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
		auto cp = cos(meme.x / 180.f* static_cast<float>(PI));
		return vec3_t(cp * cy, cp * sy, -sp);
	}
	float utilities::distance_point_to_line(vec3_t point, vec3_t origin, vec3_t direction) {
		auto point_direction = point - origin;

		auto temporary_offset = point_direction.dot(direction) / (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		if (temporary_offset < 0.000001f) {
			return FLT_MAX;
		}

		auto perpendicular_point = origin + (direction * temporary_offset);

		return (point - perpendicular_point).length();
	}
	float utilities::get_field_of_view(const vec3_t& angle, const vec3_t& aim_angle) {
		vec3_t delta = aim_angle - angle;

		auto clamp_angle = [&](vec3_t& vector) {
			while (vector[0] > 89) { vector[0] -= 180; }
			while (vector[0] < -89) { vector[0] += 180; }
			while (vector[1] > 180) { vector[1] -= 360; }
			while (vector[1] < -180) { vector[1] += 360; }
			while (vector[2] != 0) { vector[2] = 0; }
		}; clamp_angle(delta);

		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}

	float utilities::get_field_of_view(vec3_t angle, vec3_t src, vec3_t dst) {
		vec3_t ang, aim; float mag, u_dot_v;
		ang = calc_angle(src, dst);

		make_vector(angle, aim); make_vector(ang, ang);

		mag = sqrtf(pow(aim[0], 2) + pow(aim[1], 2) + pow(aim[2], 2));
		u_dot_v = aim.dot(ang);

		return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
	}

	vec3_t utilities::get_hitbox_position(c_base_player* entity, int hitbox) {
		DWORD* model = entity->get_model();
		if (!model) return vec3_t();

		studiohdr_t *hdr = interfaces::model_info->get_studio_model(model);
		if (!hdr) return vec3_t();

		matrix3x4_t matrix[128];
		if (!entity->setup_bones(matrix, 128, 0x100, 0))
			return vec3_t();

		int  hitbox_set_index = *(int *)((DWORD)hdr + 0xB0);
		if (!hitbox_set_index) return vec3_t();

		mstudiohitboxset_t* set = (mstudiohitboxset_t *)(((PBYTE)hdr) + hitbox_set_index);

		mstudiobbox_t* box = set->get_new_hitbox(hitbox);
		if (!box) return vec3_t();

		vec3_t center = (box->bbmin + box->bbmax) * 0.5f;

		vec3_t final_hitbox;

		utilities::vector_transform(center, matrix[box->bone], final_hitbox);

		return final_hitbox;
	}

	bool utilities::has_condition(c_base_player* entity, int condition) {
		if (entity->get_condition() & condition) {
			return true;
		}
		return false;
	}

	void utilities::angle_vectors(const vec3_t &angles, vec3_t* forward) {
		float sp, sy, cp, cy;

		sy = sinf(DEG2RAD(angles[1]));
		cy = cosf(DEG2RAD(angles[1]));

		sp = sinf(DEG2RAD(angles[0]));
		cp = cosf(DEG2RAD(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	void utilities::make_vector(vec3_t angle, vec3_t& vector) {
		float pitch	= float(angle[0] * PI / 180), yaw = float(angle[1] * PI / 180), tmp = float(cos(pitch));
		vec3_t final_vec = vec3_t(float(-tmp * -cos(yaw)), float(sin(yaw)*tmp), float(-sin(pitch)));
		vector = final_vec;
	}

	void utilities::vector_angle(vec3_t &forward, vec3_t &angles) {
		float yaw, pitch;
		if (forward[1] == 0 && forward[0] == 0) {
			if (forward[2] > 0) {
				pitch = 270;
			}
			else {
				pitch = 90;
			} yaw = 0;
		}
		else {
			yaw = RAD2DEG(atan2f(forward[1], forward[0]));
			if (yaw < 0) { yaw += 360; }

			pitch = RAD2DEG(atan2f(-forward[2], forward.length2d()));
			if (pitch < 0) { pitch += 360; }
		} angles = vec3_t(pitch, yaw, 0);
	}

	vec3_t utilities::calc_angle(vec3_t src, vec3_t dst) {
		vec3_t aim_angles, delta; float hyp; delta = src - dst;
		hyp = sqrtf((delta[0] * delta[0]) + (delta[1] * delta[1]));
		aim_angles[0] = atanf(delta[2] / hyp) * RADPI;
		aim_angles[1] = atanf(delta[1] / delta[0]) * RADPI;
		aim_angles[2] = 0.0f;
		if (delta[0] >= 0.0) aim_angles[1] += 180.0f;
		return aim_angles;
	}

	bool utilities::is_valid_player(c_base_player* entity) {
		if (!entity || entity->is_dead() || entity->is_dormant()) {
			return false;
		}

		if (entity->get_client_class()->class_id != 246) {
			return false;
		}

		if (entity->get_team_num() == global::local_player->get_team_num()) {
			return false;
		}

		return true;
	}

	bool utilities::is_projectile(c_base_player* entity, c_base_weapon* weapon) {
		if (!entity || !weapon) {
			return false;
		}

		int weapon_id = weapon->get_item_definition_index();

		if (entity->get_class_num() == TF2_Soldier && weapon->get_slot() == 0 || weapon_id == WPN_RighteousBison) {
			return true;
		}

		if (entity->get_class_num() == TF2_Pyro && (weapon->get_slot() == 0 || weapon_id != WPN_NewShotgun || weapon_id != WPN_PyroShotgun || weapon_id != WPN_ReserveShooter)) {
			return true;
		}

		if (entity->get_class_num() == TF2_Demoman && weapon->get_slot() != 2) {
			return true;
		}
		
		if (entity->get_class_num() == TF2_Engineer && weapon_id == WPN_RescueRanger || weapon_id == WPN_ShortCircut || weapon_id == WPN_Wrangler || weapon_id == WPN_FestiveWrangler) {
			return true;
		}

		if (entity->get_class_num() == TF2_Medic && weapon->get_slot() == 0) {
			return true;
		}

		if (entity->get_class_num() == TF2_Sniper && weapon_id == WPN_Huntsman || weapon_id == WPN_FestiveHuntsman || weapon_id == WPN_CompoundBow || weapon_id == WPN_Jarate) {
			return true;
		}

		return false;
	}

	bool utilities::is_entity_visible(c_base_player* local_player, c_base_player* entity, vec3_t vec_start, vec3_t vec_end) {
		trace_t tr; ray_t ray; trace_filter filter;

		filter.skip = local_player;
		ray.init(vec_start, vec_end);
		interfaces::enginetrace->trace_ray(ray, MASK_SHOT_HULL, &filter, &tr);

		return (tr.ent == entity || tr.fraction >= 0.99f);
	}

	bool utilities::is_point_visible(c_base_player* local_player, vec3_t vec_start, vec3_t vec_end) {
		trace_t tr; ray_t ray; trace_filter filter;

		filter.skip = local_player;
		ray.init(vec_start, vec_end);
		interfaces::enginetrace->trace_ray(ray, MASK_SOLID, &filter, &tr);

		return (tr.fraction > 0.9f);
	}
}

/**
* netvar_tree - Constructor
*
* Call populate_nodes on every RecvTable under client->GetAllClasses()
*/
void CNetVars::Initialize()
{
	const auto *client_class = interfaces::client->get_all_classes();
	while (client_class != nullptr) {
		const auto class_info = std::make_shared<node>(0);
		auto *recv_table = client_class->table;
		populate_nodes(recv_table, &class_info->nodes);
		nodes.emplace(recv_table->GetName(), class_info);

		client_class = client_class->next_class;
	}
}

/**
* populate_nodes - Populate a node map with brances
* @recv_table:	Table the map corresponds to
* @map:	Map pointer
*
* Add info for every prop in the recv table to the node map. If a prop is a
* datatable itself, initiate a recursive call to create more branches.
*/
void CNetVars::populate_nodes(RecvTable *recv_table, map_type *map)
{
	for (auto i = 0; i < recv_table->GetNumProps(); i++) {
		const auto *prop = recv_table->GetProp(i);
		const auto prop_info = std::make_shared<node>(prop->GetOffset());

		if (prop->GetType() == DPT_DataTable)
			populate_nodes(prop->GetDataTable(), &prop_info->nodes);

		map->emplace(prop->GetName(), prop_info);
	}
}

DWORD WINAPI init(LPVOID lpparam) {
	if (setup::ready_to_blast()) {
		setup::setup_interfaces();
		setup::setup_hooks();
	}
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstdll, DWORD fdwreason, LPVOID lpvreserved) {
	switch (fdwreason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstdll);
		CreateThread(nullptr, 0, init, hinstdll, 0, nullptr);
		return TRUE;
	case DLL_PROCESS_DETACH:
		FreeLibraryAndExitThread(hinstdll, 0);
		return TRUE;
	default:
		return TRUE;
	}
}