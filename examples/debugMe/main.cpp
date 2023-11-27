#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

#include <quickfix/Application.h>
#include <quickfix/Dictionary.h>
#include <quickfix/Log.h>
#include <quickfix/MessageStore.h>
#include <quickfix/SessionID.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/SocketAcceptor.h>
#include <quickfix/SocketInitiator.h>

int main(int argc, char **argv) {
  // Configure app
  std::cout << "Configuring socket handler" << std::endl;
  auto globalSettings = FIX::Dictionary();
  globalSettings.setString("ConnectionType", "acceptor");

  auto session1Params = FIX::Dictionary();
  session1Params.setString("StartTime", "12:30:00");
  session1Params.setString("EndTime", "23:30:00");
  session1Params.setInt("HeartBtInt", 20);
  session1Params.setInt("SocketAcceptPort", 4000);
  session1Params.setString("DataDictionary", "../spec/FIX41.xml");

  auto settings = new FIX::SessionSettings();
  settings->set(globalSettings);
  settings->set(FIX::SessionID("FIX.4.4", "ME", "THEIR"), session1Params);

  // Create acceptor
  auto storeFactory = new FIX::MemoryStoreFactory();
  auto logFactory = new FIX::ScreenLogFactory(true, true, true);
  auto app = new FIX::NullApplication();
  auto acceptor =
      new FIX::SocketAcceptor(*app, *storeFactory, *settings, *logFactory);

  // Start app
  std::cout << "Starting socket handler" << std::endl;
  acceptor->start();

  // Give some time to the thread to start.
  // Uncomment bellow line to make the program never crash.
  // std::this_thread::sleep_for(std::chrono::milliseconds(250));

  // Close everything without stopping app.
  std::cout << "Deleting everything" << std::endl;
  delete acceptor;
  delete app;
  delete logFactory;
  delete storeFactory;
  delete settings;

  std::cout << "All done. Bye !" << std::endl;
  return 0;
}
