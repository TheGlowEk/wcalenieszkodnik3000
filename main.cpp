#include <Windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

void pe2() {
    while (true) {
        int frequency = rand() % (2000 - 400) + 400;
        int duration = rand() % 1000 + 100;
        Beep(frequency, duration);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main() {
    vector<HWND> windows;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0) {
            reinterpret_cast<vector<HWND>*>(lParam)->push_back(hwnd);
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&windows));

    thread shuffler([&]() {
        while (true) {
            random_shuffle(windows.begin(), windows.end());
            for (HWND hwnd : windows) {
                SetForegroundWindow(hwnd);
                this_thread::sleep_for(chrono::milliseconds(10));
            }
            this_thread::sleep_for(chrono::seconds(1));
        }
    });

    thread soundThread(pe2);

    this_thread::sleep_for(chrono::seconds(15));
    ExitWindowsEx(EWX_LOGOFF, 0);

    shuffler.detach();
    return 0;
}
