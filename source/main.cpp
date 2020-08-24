#include <iostream>
#include "renderer/Device.h"
#include "logic/Game.h"

template<class B>
void Launch() {
  auto *my_game = new Game<B>();
  auto *device = new Device<B>();
  if (device->InitBackend()) {
	std::string version = "B.Name()";
	std::cout << "Unable to create an " << version << " context." << std::endl;
	delete device;
	throw std::runtime_error("Unable to create a graphics device.");
  } else {
	if (my_game->SetDevice(device)) {
	  throw std::runtime_error("Game refused the device.");
	}
  }
  my_game->Run();
}

int main(int argc, char *argv[]) {
  // Check for optional argument
  bool gl_33 = false;
  bool gl_46 = false;
  if (argc != 1) {
	if (std::string(argv[1]) == "-gl33") {
	  gl_33 = true;
	} else if (std::string(argv[1]) == "-gl46") {
	  gl_46 = true;
	} else {
	  std::cout << "Unknown argument: " << argv[1] << std::endl;
	}
  } else {
	gl_33 = true;
  }

  if (!gl_33 && !gl_46 || gl_46) {
	// LaunchMax();
	Launch<BackendOgl46>();
  }

  if (!gl_33 && !gl_46 || gl_33) {
	// LaunchMin();
	Launch<BackendOgl33>();
  }

  return 0;
}