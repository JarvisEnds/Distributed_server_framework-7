/**
  ******************************************************************************
  * @file           : log.hpp
  * @author         : kryiea
  * @brief          : None
  * @attention      : None
  * @date           : 2023/3/31  20:26
  ******************************************************************************
  */

#ifndef DISTRIBUTED_SERVER_FRAMEWORK_LOG_H
#define DISTRIBUTED_SERVER_FRAMEWORK_LOG_H

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <fstream>
#include <vector>
//#include <stringstream>


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

/**
  * @brief: ��־����
*/
class LogLevel{
public:
    /**
      * @brief: ��־�����ö��
    */
    enum Level{
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

/**
  * @brief: ��־��ʽ��,����־��ʽ��
*/
class LogFormatter{
public:
    typedef std::shared_ptr<logFormatter> ptr;


    /**
      * @brief: ���캯��
      * @param[in]: pattern ��ʽģ��
      * @details:
      *             %m ��Ϣ
      *             %p �ռǵȼ�
      *             %r �ۼƺ�����
      *             %c ��־����
      *             %t �߳�id
      *             %n ����
      *             %d ʱ��
      *             %f �ļ���
      *             %l �к�
      *             %T �Ʊ��
      *             %F Э��id
      *             %N �߳���
    */
    LogFormatter(const std::string& pattern);


    std::string format(LogEvent::ptr event);
private:
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual std::string format(LogEvent::ptr event) = 0;

    };

    void init();

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

/**
  * @brief: ��־���Ŀ��
*/
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}

     virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

protected:
    //��־����
    LogLevel::Level m_level;
};

/**
  * @brief: ��־��
*/
class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerMananger;
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef Spinlock MutexType;

    /**
      * @brief: ���캯��
      * @param[in] name ��־������
    */
    Logger(const std::string& name = "root");

    /**
      * @brief: д��־
      * @param[in] level ��־����
      * @param[in] event ��־�¼�
    */
    void log(LogLevel::Level level, LogEvent::ptr event);

    /**
     * @brief дdebug������־
     * @param[in] event ��־�¼�
     */
    void debug(LogEvent::ptr event);

    /**
     * @brief дinfo������־
     * @param[in] event ��־�¼�
     */
    void info(LogEvent::ptr event);

    /**
     * @brief дwarn������־
     * @param[in] event ��־�¼�
     */
    void warn(LogEvent::ptr event);

    /**
    * @brief дerror������־
    * @param[in] event ��־�¼�
    */
    void error(LogEvent::ptr event);

    /**
     * @brief дfatal������־
     * @param[in] event ��־�¼�
     */
    void fatal(LogEvent::ptr event);

    /**
    * @brief �����־Ŀ��
    * @param[in] appender ��־Ŀ��
    */
    void addAppender(LogAppender::ptr appender);

    /**
     * @brief ɾ����־Ŀ��
     * @param[in] appender ��־Ŀ��
     */
    void delAppender(LogAppender::ptr appender);
    
    /**
      * @brief: �����־Ŀ��
    */
    void clearAppender();

    /**
     * @brief ������־����
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
    * @brief ������־����
    */
    void setLevel(LogLevel::Level val) { m_level = val; }

    /**
      * @brief: ������־����
    */
    const std::string & getname(){return m_name};

    

private:
    std::string m_name;                             //��־����
    LogLevel::Level m_level;                        //��־����
    std::list<LogAppender::ptr> m_appenders;        //Appender����
};






/**
  * @brief: ���������̨��Appender
*/
class StdoutLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;


};

/**
  * @brief: ������ļ���Appender
*/
class FlieLogAppender : public LogAppender{
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    FlieLogAppender(const std::string& filename);

    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;

    /**
      * @brief: ���´���־�ļ�
      * @return: �ɹ�����true
    */
    bool reopen();
private:
    // �ļ�·��
    std::string m_filename;
    // �ļ���
    std::ofstream m_filestream;
    // �ϴδ򿪵�ʱ��
    uint64_t m_lastTime = 0;

};



/**
  * @brief: ��־������
*/
class LoggerManager{
public:
    typedef Spinlock MutexType;

};




















}






















#endif //DISTRIBUTED_SERVER_FRAMEWORK_LOG_H
