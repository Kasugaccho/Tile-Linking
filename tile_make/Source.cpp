#include "DxLib.h"
#include <vector>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(1, 1, 32);
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	if (DxLib_Init() == -1) return -1;

	std::vector<int> handle;

	handle.emplace_back(LoadGraph("1.png", TRUE));
	if (handle.size() == 0 || handle[0] == -1) { DxLib_End(); return -1; }

	int a_x{}, a_y{};
	GetGraphSize(handle[0], &a_x, &a_y);
	if (a_x <= 0 || a_y <= 0) { DxLib_End(); return -1; }

	constexpr size_t str_size = 32;
	char str[str_size]{};
	for (int i = 2, k = -1;; ++i) {
		snprintf(str, str_size, "%d.png", i);
		k = LoadGraph(str, TRUE);
		if (k == -1) break;
		handle.emplace_back(k);
	}
	const uint32_t handle_size = uint32_t(handle.size());
	const int screen = MakeScreen((a_x << 3), (1 + (handle_size >> 3))*a_y, TRUE);
	SetDrawScreen(screen);
	ClearDrawScreen();

	for (uint32_t i = 0; i < handle_size; ++i) {
		if (handle[i] == -1) { DxLib_End(); return -1; }
		DrawExtendGraph((i & 7) * a_x, (i >> 3) * a_y, (i & 7) * a_x + a_x, (i >> 3) * a_y + a_y, handle[i], TRUE);
	}

	ScreenFlip();
	ProcessMessage();
	SaveDrawScreenToPNG(0, 0, a_x * 8, (1 + (handle_size >> 3))*a_y, "0.png", 0);
	SaveDrawScreenToJPEG(0, 0, a_x * 8, (1 + (handle_size >> 3))*a_y, "0.jpg", 100, TRUE);

	DxLib_End();
	return 0;
}
