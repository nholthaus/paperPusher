//------------------------------
//	
//------------------------------
#include <mainWindow.h>
#include <logerr>

#include <thread>
#include <chrono>

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
	
	m_thread1 = std::thread([this] { while (true) { LOGINFO << "Thread One " << count1.fetch_add(1) << "iteration" << std::endl; std::this_thread::sleep_for(1ms);} });
	m_thread2 = std::thread([this] { while (true) { LOGINFO << "Thread Two " << count2.fetch_add(1) << "iteration" << std::endl; std::this_thread::sleep_for(1ms);} });
}

