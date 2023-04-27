/**
  ******************************************************************************
  * @file           : log.hpp
  * @author         : kryiea
  * @brief          : None
  * @attention      : None
  * @date           : 2023/3/31  20:26
  ******************************************************************************
  */

#ifndef DISTRIBUTED_SERVER_FRAMEWORK_LOG_HPP
#define DISTRIBUTED_SERVER_FRAMEWORK_LOG_HPP

#include <string>
#include <stdint.h>
#include <memory>
#include <list>

namespace sylar{

//��־�¼�
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
private:
    const char* m_file = nullptr;   //�ļ���
    int32_t m_line = 0;             //�к�
    uint32_t m_elapse = 0;          //����������ʼ�����ڵĺ�����
    uint32_t m_threadId = 0;        //�߳�id
    uint32_t m_fiberId = 0;         //Э��id
    uint32_t m_time = 0;            //ʱ���
    std::string m_content;
};

//��־����
class LogLevel{
public:
    enum Level{
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

//��־��ʽ��
class logFormatter{
public:
    typedef std::shared_ptr<logFormatter> ptr;
    std::string format(LogEvent::ptr event);
private:
};

//��־�����
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}

    void log(LogLevel::Level level, LogEvent::ptr event);

private:
    LogLevel::Level m_level;                    //��־����
};

//��־��
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

private:
    std::string m_name;                             //��־����
    LogLevel::Level m_level;                        //��־����
    std::list<LogAppender::ptr> m_appenders;        //Appender����
};

//���������̨��Appender
class StdoutLogAppender : public LogAppender{
};

//����������ļ���Appender
class FlieLogAppender : public LogAppender{
};





















}






















#endif //DISTRIBUTED_SERVER_FRAMEWORK_LOG_HPP
