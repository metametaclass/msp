#include <FlightController.hpp>
#include <iostream>

struct MyIdent : public msp::Message {
    MyIdent(msp::FirmwareVariant v) : Message(v) {}

    virtual msp::ID id() const override { return msp::ID::MSP_IDENT; }

    msp::ByteVector raw_data;

    virtual bool decode(const msp::ByteVector &data) override {
        raw_data = data;
        return true;
    }
};

struct Callbacks {
    void onIdent(const MyIdent &ident) {
        std::cout << "Raw Ident data: ";
        for(auto d : ident.raw_data) {
            std::cout << int(d) << " ";
        }
        std::cout << std::endl;
    }
};

int main(int argc, char *argv[]) {
    const std::string device =
        (argc > 1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const size_t baudrate = (argc > 2) ? std::stoul(argv[2]) : 115200;

    try {
        Callbacks cbs;
        fcu::FlightController fcu;
        fcu.connect(device, baudrate);

        // subscribe with custom type
        fcu.subscribe(&Callbacks::onIdent, &cbs, 1);

        // Ctrl+C to quit
        std::cin.get();
    }
    catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
