#include "../../main.h"

namespace hook {
	std::unique_ptr<vmt_manager> hook::client_mode_hook;
	std::unique_ptr<vmt_manager> hook::client_hook;
	std::unique_ptr<vmt_manager> hook::panel_hook;
	std::unique_ptr<vmt_manager> hook::render_hook;
}