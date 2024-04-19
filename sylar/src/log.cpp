#include "log.h"
namespace sylar
{
    Logger::Logger(const std::string &name = "root") : m_name(name)
    {
    }
    void Logger::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            for (auto &i : m_appenders)
            {
                i->log(level, event);
            }
        }
    }
    // void Logger::debug(LogEvent::ptr event)
    // {
    //     debug(LogLevel::DEBUG, event);
    // }
    // void Logger::info(LogEvent::ptr event)
    // {
    //     debug(LogLevel::INFO, event);
    // }
    // void Logger::warn(LogEvent::ptr event)
    // {
    //     debug(LogLevel::WARN, event);
    // }
    // void Logger::fatal(LogEvent::ptr event)
    // {
    //     debug(LogLevel::FATAL, event);
    // }
    // void Logger::error(LogEvent::ptr event)
    // {
    //     debug(LogLevel::ERROR, event);
    // }
    void Logger::addAppender(LogAppender::ptr appender)
    {
        m_appenders.push_back(appender);
    }
    void Logger::delAppender(LogAppender::ptr appender)
    {
        for (auto it = m_appenders.begin(); it != m_appenders.end(); it++)
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    }
    FileLogAppender::FileLogAppender(const std::string &filename) : m_filename(filename)
    {
    }
    void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            m_filestream << m_formatter->format(event);
        }
    }
    bool FileLogAppender::reopen()
    {
        if (m_filestream)
        {
            m_filestream.close();
        }
        m_filestream.open(m_filename);
    }

    void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            std::cout << m_formatter->format(event) << std::endl;
        }
    }


    std::string LogFormatter::format(LogEvent::ptr event)
    {
        std::stringstream ss;
        for(auto & i:m_items)
        {
            i->format(ss,event);
        }
        return ss.str();
    }
    LogFormatter::LogFormatter(const std::string &patt):m_pattern(patt)
    {
        
    }
    void LogFormatter::init()
    {
    }

} // namespace sylar
