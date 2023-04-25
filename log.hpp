/**
  ******************************************************************************
  * @file           : log.h
  * @author         : mimang
  * @brief          : ��־ģ���װ
  * @attention      : None
  * @date           : 2023/4/15 / 14:19
  ******************************************************************************
  */

#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__


#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include "util.h"
#include "singleton.h"

/**
 * @brief ʹ����ʽ��ʽ����־����level����־д�뵽logger
 */
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getSS()

/**
 * @brief ʹ����ʽ��ʽ����־����debug����־д�뵽logger
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief ʹ����ʽ��ʽ����־����info����־д�뵽logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief ʹ����ʽ��ʽ����־����warn����־д�뵽logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief ʹ����ʽ��ʽ����־����error����־д�뵽logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief ʹ����ʽ��ʽ����־����fatal����־д�뵽logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/**
 * @brief ʹ�ø�ʽ����ʽ����־����level����־д�뵽logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief ʹ�ø�ʽ����ʽ����־����debug����־д�뵽logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief ʹ�ø�ʽ����ʽ����־����info����־д�뵽logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief ʹ�ø�ʽ����ʽ����־����warn����־д�뵽logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief ʹ�ø�ʽ����ʽ����־����error����־д�뵽logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief ʹ�ø�ʽ����ʽ����־����fatal����־д�뵽logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

/**
 * @brief ��ȡ����־��
 */
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

/**
 * @brief ��ȡname����־��
 */
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)

using namespace std;

namespace sylar{

    class Logger;

    /**
    * @brief: ��־����
    */
    class LogLevel{
    public:
        enum Level{
            // δ֪����
            UNKNOW = 0,
            // DEBUG ����
            DEBUG = 1,
            // INFO ����
            INFO = 2,
            // WARN ����
            WARN = 3,
            // ERROR ����
            ERROR = 4,
            // FATAL ����
            FATAL = 5
        };

        /**
        * @brief ����־����ת���ı����
        * @param[in] level ��־����
        */
        static const char* ToString(LogLevel::Level level);
    };

    /**
  * @brief ��־�¼�
  */
    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        /**
         * @brief ���캯��
         * @param[in] logger ��־��
         * @param[in] level ��־����
         * @param[in] file �ļ���
         * @param[in] line �ļ��к�
         * @param[in] elapse �������������ĺ�ʱ(����)
         * @param[in] thread_id �߳�id
         * @param[in] fiber_id Э��id
         * @param[in] time ��־�¼�(��)
         * @param[in] thread_name �߳�����
         */
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
                ,const char* file, int32_t line, uint32_t elapse
                ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
                ,const std::string& thread_name);

        /**
         * @brief �����ļ���
         */
        const char* getFile() const { return m_file;}

        /**
         * @brief �����к�
         */
        int32_t getLine() const { return m_line;}

        /**
         * @brief ���غ�ʱ
         */
        uint32_t getElapse() const { return m_elapse;}

        /**
         * @brief �����߳�ID
         */
        uint32_t getThreadId() const { return m_threadId;}

        /**
         * @brief ����Э��ID
         */
        uint32_t getFiberId() const { return m_fiberId;}

        /**
         * @brief ����ʱ��
         */
        uint64_t getTime() const { return m_time;}

        /**
         * @brief �����߳�����
         */
        const std::string& getThreadName() const { return m_threadName;}

        /**
         * @brief ������־����
         */
        std::string getContent() const { return m_ss.str();}

        /**
         * @brief ������־��
         */
        std::shared_ptr<Logger> getLogger() const { return m_logger;}

        /**
         * @brief ������־����
         */
        LogLevel::Level getLevel() const { return m_level;}

        /**
         * @brief ������־�����ַ�����
         */
        std::stringstream& getSS() { return m_ss;}

        /**
         * @brief ��ʽ��д����־����
         */
        void format(const char* fmt, ...);

        /**
         * @brief ��ʽ��д����־����
         */
        void format(const char* fmt, va_list al);
    private:
        /// �ļ���
        const char* m_file = nullptr;
        /// �к�
        int32_t m_line = 0;
        /// ����������ʼ�����ڵĺ�����
        uint32_t m_elapse = 0;
        /// �߳�ID
        uint32_t m_threadId = 0;
        /// Э��ID
        uint32_t m_fiberId = 0;
        /// ʱ���
        uint64_t m_time = 0;
        /// �߳�����
        std::string m_threadName;
        /// ��־������
        std::stringstream m_ss;
        /// ��־��
        std::shared_ptr<Logger> m_logger;
        /// ��־�ȼ�
        LogLevel::Level m_level;
    };

    /**
     * @brief ��־�¼���װ��
     */
    class LogEventWrap {
    public:

        /**
         * @brief ���캯��
         * @param[in] e ��־�¼�
         */
        LogEventWrap(LogEvent::ptr e);

        /**
         * @brief ��������
         */
        ~LogEventWrap();

        /**
         * @brief ��ȡ��־�¼�
         */
        LogEvent::ptr getEvent() const { return m_event;}

        /**
         * @brief ��ȡ��־������
         */
        std::stringstream& getSS();
    private:
        /**
         * @brief ��־�¼�
         */
        LogEvent::ptr m_event;
    };

    /**
    * @brief: ��־��ʽ��
    */
    class LogFormatter{
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string& pattern);

        //%t      %thread_id %m%n
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        class FormatItem{
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){}
            virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        void init();
    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
    };

    /**
    * @brief: ��־�����
    */
    class LogAppender{
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender(){}

        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
        LogFormatter::ptr getFormatter() const {return m_formatter;}
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;
    };

    /**
    * @brief: ��־��
    */
    class Logger : public std::enable_shared_from_this<Logger>{
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
        LogLevel::Level getLevel() const{return m_level;}
        void setLevel(LogLevel::Level val) {m_level = val;}

        const std::string& getName() const {return m_name;}
    private:
        //��־����
        std::string m_name;
        //��־����
        LogLevel::Level m_level;
        //Appender����
        std::list<LogAppender::ptr> m_appenders;
        LogFormatter::ptr m_formatter;
    };

    /**
    * @brief: ���������̨��Appender
    */
    class StdoutLogAppender : public LogAppender{
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    };

    /**
    * @brief: ����������ļ���Appender
    */
    class FileLogAppender : public LogAppender{
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string& filename);
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

        //���´��ļ������ļ��ɹ�����true
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
}

#endif