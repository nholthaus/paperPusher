//------------------------------
//
//------------------------------
#include <logerr>
#include <mainWindow.h>

#include <chrono>
#include <thread>

std::atomic<int> count1;
std::atomic<int> count2;
using namespace std::chrono_literals;

//--------------------------------------------------------------------------------------------------
//	MainWindow (public ) []
//--------------------------------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow()
{
}

//--------------------------------------------------------------------------------------------------
//	~MainWindow () []
//--------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	m_joinAll = true;
	if (m_thread1.joinable()) m_thread1.join();
	if (m_thread2.joinable()) m_thread2.join();
}

//--------------------------------------------------------------------------------------------------
//	initialize () []
//--------------------------------------------------------------------------------------------------
void MainWindow::initialize()
{
	//**********************************************************************************************
	// Most likely, any errors within initialization should be FATAL, since they'll probably cause
	// the UI to not be fully constructed.
	//**********************************************************************************************

	//ERR("hello");

	m_thread1 = std::thread([this]
	                        { runThread1(); });
	m_thread2 = std::thread([this]
	                        { runThread2(); });
}

void MainWindow::runThread1()
{
	ERR("Goodbye, cruel world!");
	while (!m_joinAll)
	{
		LOGINFO << "Thread One " << count1.fetch_add(1) << "iteration" << std::endl;
		std::this_thread::sleep_for(1ms);
	}
}
void MainWindow::runThread2()
{
	while (!m_joinAll)
	{
		LOGINFO << "Thread Two " << count2.fetch_add(1) << "iteration" << std::endl;
		std::this_thread::sleep_for(1ms);
	}
}
