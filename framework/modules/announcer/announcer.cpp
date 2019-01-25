#include "../../main.h"

void announcer::initialize() {
	interfaces::game_event->add_listener(this, "player_death", false);
	interfaces::game_event->add_listener(this, "player_spawn", false);
}

void announcer::fire_game_event(igame_event* event) {
	if (!global::local_player || !vars::announcer::enabled) {
		return;
	}

	player_info_t local_player;
	interfaces::engineclient->get_player_info(interfaces::engineclient->get_local_player(), &local_player);

	int local_user_id = local_player.user_id;
	if (local_user_id == 0) return;

	if (strcmp(event->get_name(), "player_death") == 0) {
		int attacker = event->get_int("attacker", 0);
		int user_id = event->get_int("userid", 0);
		if (attacker == local_user_id) {
			if (attacker == user_id) return;

			kill_counter++; streak_counter++;

			if ((interfaces::globals->curtime - last_kill_time) < streak_timeout) {
				switch (streak_counter) {
				case 2:
					current_sound = sounds::double_kill;
					break;
				case 3:
					current_sound = sounds::triple_kill;
					break;
				case 4:
					current_sound = sounds::multi_kill;
					break;
				case 5:
					current_sound = sounds::mega_kill;
					break;
				case 6:
					current_sound = sounds::ultra_kill;
					break;
				case 7:
					current_sound = sounds::monster_kill;
					break;
				case 8:
					current_sound = sounds::ludicrous_kill;
					break;
				default:
					current_sound = sounds::holy_shit;
				}
			}
			else {
				streak_counter = 1;
			}

			switch (static_cast<tf_customkills>(event->get_int("customkill", 0))) {
			case tf_customkills::HEADSHOT:
			case tf_customkills::HEADSHOT_DECAPITATION:
			case tf_customkills::PENETRATE_HEADSHOT:
				play_sound(sounds::headshot_kill);
				break;
			}

			switch (kill_counter) {
				case 5:
					play_sound(sounds::killing_spree);
					break;
				case 10:
					play_sound(sounds::rampage);
					break;
				case 15:
					play_sound(sounds::dominating);
					break;
				case 20:
					play_sound(sounds::unstoppable);
					break;
				default:
					if (kill_counter % 5 == 0) {
						play_sound(sounds::godlike);
					}
			}
			last_kill_time = interfaces::globals->curtime;
		}
		return;
	}

	if (strcmp(event->get_name(), "player_spawn") == 0)
	{
		if (event->get_int("userid", 0) == local_user_id) {
			kill_counter = 0;
			last_kill_time = 0.0f;
		}
		return;
	}
} 

void announcer::run() {
	if (!vars::announcer::enabled) {
		return;
	}

	if (current_sound != nullptr) {
		if (strcmp(current_sound, "") != 0) {
			play_sound(current_sound);
			current_sound = nullptr;
		}
	}
}

void announcer::play_sound( char* sound_file) {
	PlaySoundA(sound_file, NULL, SND_ASYNC | SND_MEMORY);
	//interfaces::surface->play_sound(sound_file);
}