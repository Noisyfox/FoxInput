// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 FOXINPUT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// FOXINPUT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef FOXINPUT_EXPORTS
#define FOXINPUT_API __declspec(dllexport)
#else
#define FOXINPUT_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    double FOXINPUT_API FI_Init(double hwnd);
    double FOXINPUT_API FI_Init_GMS(HWND hwnd);
    double FOXINPUT_API FI_Release();

    double FOXINPUT_API FI_EnableIME(double enable);
    double FOXINPUT_API FI_SetIMEPosition(double x, double y);

    double FOXINPUT_API FI_EnterFullControlMode();
    double FOXINPUT_API FI_ExitFullControlMode();

    double FOXINPUT_API FI_GrabInputText(double enable);

    // 获取最新输入法状态
    double FOXINPUT_API FI_RefreshData();
    // 获取输入法指示器
    LPCSTR FOXINPUT_API FI_GetIndicator();
    // 获取并清空输入的字符串
    LPCSTR FOXINPUT_API FI_PopInputString();
    // 获取候选词数量
    double FOXINPUT_API FI_GetCandidateCount();
    // 获取当前选定的候选词索引
    double FOXINPUT_API FI_GetCandidateSelection();
    // 获取指定索引的候选词
    LPCSTR FOXINPUT_API FI_GetCandidate(double index);
    // 获取用户输入的Composition字符串
    LPCSTR FOXINPUT_API FI_GetCompositionString();

#ifdef __cplusplus
}
#endif

bool InstallWndProc();
bool UninstallWndProc();
