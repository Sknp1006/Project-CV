#ifndef H_LOGGER
#define H_LOGGER
#include <iostream>
#include <filesystem>
#include <chrono>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// 日志级别    描述Log工具类
// OFF        关闭：最高级别，不打印日志。
// FATAL      致命：指明非常严重的可能会导致应用终止执行错误事件。
// ERROR      错误：指明错误事件，但应用可能还能继续运行。
// WARN       警告：指明可能潜在的危险状况。
// INFO       信息：指明描述信息，从粗粒度上描述了应用运行过程。
// DEBUG      调试：指明细致的事件信息，对调试应用最有用。
// TRACE      跟踪: 指明程序运行轨迹，比DEBUG级别的粒度更细。
// ALL        所有：所有日志级别，包括定制级别。

namespace commons
{
    class Logger
    {
    public:
        /// @brief 初始化spdlog 日志（在算法的运行目录）
        inline static void Create(std::string DstPath)
        {
            std::filesystem::path dll_path(DstPath);
            std::filesystem::path log_path = dll_path / "log";

            if (!std::filesystem::exists(log_path))
            {
                std::filesystem::create_directories(log_path);
            }
            std::string log_file = (log_path / ("log." + now_YYYYMMDDHHMMSS_SSS() + ".txt")).string();
            init(log_file, 1);
        }
    private:
        /// @brief 初始化spdlog 日志
        /// @param file 日志文件名
        /// @param numOfFile 文件数量
        /// 同时设置: 文件 和 console 两种日志模式
        inline static void init(std::string file, int numOfFile)
        {
            try
            {
                auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file, 1024 * 1024 * 10, numOfFile, true);
                std::vector<spdlog::sink_ptr> sinks{file_sink};
                #ifdef _DEBUG
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                sinks.push_back(console_sink);
                #endif
                auto logger = std::make_shared<spdlog::logger>("project-cv", begin(sinks), end(sinks));
                logger->flush_on(spdlog::level::debug);
                logger->set_level(spdlog::level::trace);
                spdlog::set_default_logger(logger);
            }
            catch (const std::exception &e)
            {
                std::cerr << "exception spdlog init: " << e.what() << std::endl;
                std::cout << "spdlog 日志, 初始化失败." << std::endl;
            }
        }
        /// @brief 获取当前时间
        /// @return 
        inline static const std::string now_YYYYMMDDHHMMSS_SSS()
        {
            auto timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::stringstream ss;
            ss << std::put_time(std::localtime(&timestamp), "%Y%m%d%H%M%S");
            std::string date = ss.str();
            return date;
        }
    };
}; // namespace commons

#endif // H_LOGGER