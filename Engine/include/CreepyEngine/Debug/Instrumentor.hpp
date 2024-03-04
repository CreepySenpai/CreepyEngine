#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>
#include <format>

namespace Creepy
{
    struct ProfileResult
    {
        std::string Name;
        int64_t Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
        public:
            void BeginSession(const std::string &name, const std::string &filePath = "result.json") noexcept
            {
                m_fileStream.open(filePath, std::ios::in | std::ios::out);

                WriteHeader();

                m_currentSession = new InstrumentationSession{.Name = name};
            }

            void EndSession() noexcept
            {
                WriteFooter();

                clearUp();

                m_fileStream.close();
                delete m_currentSession;
                m_currentSession = nullptr;
                m_profileCount = 0;
            }

            void WriteProfile(const ProfileResult &result) noexcept
            {
                m_fileStream << "{\n";
                m_fileStream << std::format("\"{}\" : \"{}\", ", "cat", "function");
                m_fileStream << std::format("\"{}\" : {}, ", "dur", (result.End - result.Start));
                m_fileStream << std::format("\"{}\" : \"{}\", ", "name", result.Name);
                m_fileStream << std::format("\"{}\" : \"{}\", ", "ph", "X");
                m_fileStream << std::format("\"{}\" : {}, ", "pid", m_profileCount);
                m_fileStream << std::format("\"{}\" : {}, ", "tid", result.ThreadID);
                m_fileStream << std::format("\"{}\" : {}", "ts", result.Start);
                m_fileStream << "\n},";

                m_fileStream.flush();
            }

            void WriteHeader() noexcept
            {
                m_fileStream << "{\"otherData\": {}, \"traceEvents\": [";
                m_fileStream.flush();
            }

            void WriteFooter() noexcept
            {
                m_fileStream << "]}";
                m_fileStream.flush();
            }


            inline static Instrumentor &GetInstance() noexcept
            {
                static Instrumentor instance;
                return instance;
            }

        private:
            InstrumentationSession *m_currentSession{nullptr};
            std::fstream m_fileStream;
            int m_profileCount{0};
            

            void clearUp() noexcept {
                m_fileStream.seekp(0, std::ios::end);
                auto pos = m_fileStream.tellp();

                std::stringstream stream;
                stream << pos;

                auto currentPosition = std::stoull(stream.str());

                while(currentPosition > 0){
                    currentPosition--;
		
                    m_fileStream.seekp(currentPosition);
                    char c;
                    m_fileStream.get(c);
                    if(c == ','){
                        m_fileStream.seekp(currentPosition);
                        m_fileStream.write(" ", 1);
                        break;
                    }
                }
                
            }

    };

    class InstrumentTimer
    {

        public:
            InstrumentTimer(const char *name) : m_name{name}, m_stop{false}
            {
                m_startPoint = std::chrono::steady_clock::now();
            }

            void Stop()
            {
                auto endPoint = std::chrono::steady_clock::now();

                auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();

                auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

                // float duration = (end - start) * 0.001f;

                uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

                Instrumentor::GetInstance().WriteProfile({.Name = m_name, .Start = start, .End = end, .ThreadID = threadID});

                m_stop = true;
            }

            ~InstrumentTimer()
            {
                if (!m_stop)
                {
                    Stop();
                }
            }

        private:
            const char* m_name;
            std::chrono::time_point<std::chrono::steady_clock> m_startPoint;
            bool m_stop;
    };
}

#define DEBUG_PROFILE_BEGIN_SESSION(name, filePath) Creepy::Instrumentor::GetInstance().BeginSession(name, filePath)
#define DEBUG_PROFILE_END_SESSION() Creepy::Instrumentor::GetInstance().EndSession()
#define DEBUG_PROFILE_SCOPE(name) Creepy::InstrumentTimer timer##__LINE__{name}
#define DEBUG_PROFILE_FUNCTION() DEBUG_PROFILE_SCOPE(__FUNCTION__)