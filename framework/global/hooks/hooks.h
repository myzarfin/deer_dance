#ifndef HOOKS_H
#define HOOKS_H
#ifdef _WIN32
#pragma once
#endif

namespace hook {
	extern std::unique_ptr<vmt_manager> client_mode_hook;
	extern std::unique_ptr<vmt_manager> client_hook;
	extern std::unique_ptr<vmt_manager> panel_hook;
	extern std::unique_ptr<vmt_manager> render_hook;
}

#endif // !HOOKS_H