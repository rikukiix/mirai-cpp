#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;

	// 源文件使用 UTF-8 编码保存，在 Windows 上需要切换代码页才不会显示乱码
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif	

	// 16 条事件处理线程
	MiraiBot bot("127.0.0.1", 8762, 16);

	// 检查一下版本
	try
	{
		// 获取 mirai-api-http 插件的版本
		string mah_version = bot.GetMiraiApiHttpVersion();
		// 获取 mirai-cpp 的版本
		string mc_version = bot.GetMiraiCppVersion();
		cout << "! mirai-api-http 的版本: " << mah_version
			<< "; 当mirai-cpp 的版本: " << mc_version << "; " << endl;
		if (mah_version != mc_version)
		{
			cout << "! 警告: 你的 mirai-api-http 插件的版本与 mirai-cpp 的版本不同，可能存在兼容性问题。" << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	// 自动重试地进行 Auth
	while (true)
	{
		try
		{
			bot.Auth("AuthKeyASDEWQ", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "Bot Working..." << endl;

	bot.On<NudgeEvent>(
		[&](NudgeEvent e)
		{
			stringstream ss;
			ss << e.FromId.ToInt64() << " " << e.Action << " " << e.Target << " " << e.Suffix;
			if (e.FromKind == NudgeEvent::SubjectKind::Group)
			{
				bot.SendMessage(GID_t(e.SubjectId), MessageChain().Plain(ss.str()));
			}
			else
			{
				bot.SendMessage(QQ_t(e.SubjectId), MessageChain().Plain(ss.str()));
			}
		});


	// 记录轮询事件时的错误
	bot.EventLoop([](const char* errMsg)
		{
			cout << "获取事件时出错: " << errMsg << endl;
		});

	return 0;
}