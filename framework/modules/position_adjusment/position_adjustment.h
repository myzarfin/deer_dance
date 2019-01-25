#pragma once

struct backtracking_record {
	c_base_player* entity;
	vec3_t head_position;
};

struct position_adjustment_data {
	int tick_count;
	std::vector<backtracking_record> records;
};

struct tick_record {
	tick_record(float simulationtime, vec3_t headposition) {
		simulation_time = simulationtime; 
		head_position = headposition;
	}

	float simulation_time = -1;
	vec3_t head_position;
	matrix3x4_t bone_matrix[128];
};

namespace position_adjustment {
	extern std::vector<tick_record> ticks[64];
	extern void position_adjustment_start();
	extern void position_adjustment_calc();
	extern void do_position_adjustment();
};