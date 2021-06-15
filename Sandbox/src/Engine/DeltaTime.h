#pragma once

//used for measuring changes in time between frames
struct DeltaTime {
public:
	DeltaTime(double time) : m_time(time) {}
	~DeltaTime() {}
	DeltaTime() = delete;

	template<typename T>
	T getMilliseconds() { return (T)(m_time * 1000); }

	template<typename T>
	T getSeconds() { return (T)m_time; }

	//gets the number of seconds since the start of the program
	template<typename T>
	static T getTimeSeconds() { return (T)getGLFWseconds(); }

	//gets the number of milliseconds since the start of the program
	template<typename T>
	static T getTimeMilliseconds() { return (T)getGLFWseconds()*1000; }
private:
	double getGLFWseconds();

	//will be stored as seconds
	const double m_time; 
};