//
// Created by emb on 11/18/18.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include "MixerClient.h"
#include "SoftCutClient.h"
#include "OscInterface.h"

static inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
    using namespace crone;
    using std::cout;
    using std::endl;

#if 1
    std::unique_ptr<MixerClient> m = std::make_unique<MixerClient>();
    std::unique_ptr<SoftCutClient> sc = std::make_unique<SoftCutClient>();


    cout << "setting up jack clients.." << endl;
    m->setup();
    sc->setup();

    cout << "starting jack clients.." << endl;
    m->start();
    sc->start();


    cout << "connecting ports... " << endl;
    m->connectAdcPorts();
    m->connectDacPorts();
    m->connect<2, 2>(sc.get(), MixerClient::SINK_CUT, SoftCutClient::SOURCE_ADC);
    sc->connect<6, 6>(m.get(), 0, MixerClient::SOURCE_CUT);

    cout << "starting OSC interface..." << endl;
    OscInterface::init();

    cout << "entering main loop..." << endl;
    while(!OscInterface::shouldQuit())  {
        sleep(100);
    }

    cout << "stopping clients" << endl;
    m->stop();
    sc->stop();
    cout << "cleaning up clients..." << endl;
    m->cleanup();
    sc->cleanup();
    cout << "goodbye" << endl;
#else
    std::unique_ptr<SoftCutClient> sc;
    sc = std::make_unique<SoftCutClient>();

    sc->setup();
    sc->start();

    sc->connectAdcPorts();
    sc->connectDacPorts();

    OscInterface::init();

    cout << "entering main loop..." << endl;
    while(!OscInterface::shouldQuit())  {
        sleep(100);
    }
    sc->stop();
    sc->cleanup();
#endif
    return 0;
}