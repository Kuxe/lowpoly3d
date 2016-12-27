#include "renderer.hpp"
#include <thread>
#include <iostream>
#include "model.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include "renderdata.hpp"
#include "terraingenerator.hpp"
#include "spheregenerator.hpp"
#include "cubegenerator.hpp"
#include "planegenerator.hpp"
#include "dummysimulation.hpp"
#include "gamedata.hpp"
#include "args.hxx"
#include <glm/gtc/matrix_transform.hpp>

int generateJSON(const std::string& gamedataPath) {
    Gamedata gamedata;
    gamedata.renderDatas.push_back({glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), 2});
    gamedata.renderDatas.push_back({glm::scale(glm::mat4(2.2f), glm::vec3(-1.3f)), 3});
	try {
		std::ofstream ofsgd(gamedataPath);
		cereal::JSONOutputArchive outarchive(ofsgd);
		outarchive(gamedata);
	} catch (cereal::Exception e) {
		printf("ERROR: cereal could not output Gamedata (%s)\n", e.what());
		return 1;
	}
	printf("Succesfully output gamedata to %s\n", gamedataPath.c_str());
	return 0;
}

void printVas(const auto& vas) {
	printf("Vertex arrays: ");
	for(const auto& va : vas) {
		printf("%i ", va);
	}
	printf("\n");
}

int main(int argc, char** argv) {

	/** Parse args (using nifty library by Taylor C. Richberger https://github.com/Taywee/args)**/
	std::string gamedataPath = "gamedata.json";
	args::ArgumentParser parser("lowpoly3d.", "lowpoly3d says bye!.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag genjsonFlag(parser, "genjson", "Generates a new JSON file for gamedata", {"genjson"});
	args::Flag printVasFlag(parser, "printvas", "Print integer handle to vertex arrays loaded by renderer", {"printvas"});
	args::ValueFlag<float> gametimeSpeedFactor(parser, "gametime", "Increases gametime speed by a factor", {"gt", "gametimespeedfactor"});
	args::Flag forceDaytime(parser, "forcedaytime", "Disable day/night cycle", {"forcedaytime"});
    try {
    	parser.ParseCLI(argc, argv);
    } catch (args::Help) {
    	std::cout << parser;
    	return 0;
    } catch (args::ParseError e) {
    	std::cerr << e.what() << std::endl;
    	std::cerr << parser;
    	return 1;
    }

    /** Generate a new gamedata.json, useful when gamedata class was changed **/
    if(genjsonFlag) {
    	return generateJSON("generated_gamedata.json");
    }

	Renderer renderer;
	if(renderer.initialize()) {


		/** gamedata.json -> gamedata -> when initializing stuff just lookup in gamedata class **/
		Gamedata gamedata;
		{
			try {
			std::ifstream ifsgd("gamedata.json");
			cereal::JSONInputArchive inarchive(ifsgd);
			inarchive(gamedata);
			} catch(cereal::Exception e) {
				printf("ERROR: cereal could not parse Gamedata (%s)\n", e.what());
			}
		}

		if(forceDaytime) gamedata.forceDaytime = true;
		if(gametimeSpeedFactor) gamedata.gametimeSpeedFactor = args::get(gametimeSpeedFactor);

		/** Generate models and load them to renderer (note that ordering of model loading is assumed by JSON) **/
		SphereGenerator sg(gamedata.sunColor, gamedata.sunSubdivides);
		SphereGenerator hemisphereGenerator({125.0f, 125.0f, 125.0f}, 3);
		TerrainGenerator tg;
		PlaneGenerator pg(gamedata.planeNormal, gamedata.planeColor, gamedata.waterSubdivides);
		Model sun = sg.generate();
		Model terrainModel = tg.generate();
		Model waterModel = pg.generate();
		Model hemisphere = hemisphereGenerator.generate();
		auto vas = renderer.loadModels({sun, terrainModel, waterModel, hemisphere});

		/** Print vertex arrays if --printvas flag was passed **/
		if(printVasFlag) {
			printVas(vas);
		}

		/** Use members of gamedata, such as vectors, to initialize different objects in game engine **/
		DummySimulation ds(gamedata);

		//Only proceed if all models were succesfully loaded
		if(std::find(std::begin(vas), std::end(vas), -1) == std::end(vas)) {

			//Run simulation in a thread
			std::thread dsthread(&DummySimulation::run, &ds);

			//Main-thread will remain in here until program terminates
			if(!renderer.render(ds)) {
				printf("ERROR: Something went wrong in renderer\n");
			}

			//Renderer has quit, so terminate simulation and join simulation thread with main thread
			ds.terminate();
			dsthread.join();
		} else {
			printf("ERROR: Will not render because one or more models failed to load\n");
		}
		renderer.terminate();
	}
	printf("Terminating program, bye!\n");
	return 0;
}