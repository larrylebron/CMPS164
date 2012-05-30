#include "fileReader.h"



fileReader::fileReader()
{
}


fileReader::~fileReader()
{
}

bool fileReader::readCourseFile(char* filename, CMMPointer<Course> course) {
	// read files into streams
	ifstream mapInput(filename);

	if (mapInput.fail()){
		std::string stringFile(filename); //string filename for passing to the logger
		Logger::Instance()->err("failed to open " + stringFile + " attempting to add .db extension");
		mapInput.clear();

		char fileExt[] = ".db";
		strcat(filename, fileExt);
		Logger::Instance()->write("Attempting to open level file with appended name: " + stringFile + ".db");
		mapInput.open(filename, ifstream::in);

		// throws error if reading failed
		if (mapInput.fail()) 
		{
			Logger::Instance()->err("Error: File not Found: " + stringFile);
			return false;
		}
	}

	string line;
	int lineNumber = 1;
	int numLevelsToParse = 0;
	int beginCount = 0;
	int endCount = 0;
	bool beganTile = false;
	CMMPointer<level> newLevel;

	getline(mapInput, line);

	while (line.length() > 0) {
		vector<string> tokens = strSplit(line);

		// first token to the dataType
		string type = tokens[0];

		//A string stream of the line number for the logger
		std::stringstream sLineNumber;
		sLineNumber << lineNumber;

		// loads course name and number of levels
		if (tokens[0].compare("course") == 0) {
			vector<string> names = strSplit(line, "\"");

			if (tokens.size() < 3) {
				Logger::Instance()->err("Error: Invalid course tokens, line " + sLineNumber.str() );
				return false;
			} else if (names.size() < 2) {
				Logger::Instance()->err("Error: Invalid course name, line " + sLineNumber.str() );
				return false;
			} else { 
				numLevelsToParse = atoi(tokens.back().c_str());
				course->setNumLevels(numLevelsToParse);
				course->setCourseName(names[1]);
			}

			// does check for begin holes and end holes
		} else if (tokens[0].compare("begin_hole") == 0) {
			if (beganTile) {
				Logger::Instance()->err("Error: trying to begin_hole when another begin_hole has started, line " + sLineNumber.str());
				return false;
			} else {
				beginCount++;
				newLevel = new level();
				beganTile = true;
			}
		} else if (tokens[0].compare("end_hole") == 0) {
			if (!beganTile) {
				Logger::Instance()->err("Error: trying to end_hole before a valid begin_hole, line " + sLineNumber.str());
				return false;
			} else {
				endCount++;
				beganTile = false;

				if (!newLevel->checkLevel()) {
					return false;
				} else {
					course->addLevel(beginCount, newLevel);
				}
			}


			// parsing of level name
		} else if (tokens[0].compare("name") == 0) {
			vector<string> names = strSplit(line, "\"");
			if (names.size() < 2) {
				Logger::Instance()->err("Error: Invalid level name, line " + sLineNumber.str() );
				return false;
			} else {
				newLevel->setLevelName(names[1]);
			}

		} else if (tokens[0].compare("par") == 0) {
			if (tokens.size() < 2) {
				Logger::Instance()->err("Error: Invalid par entry, line " + sLineNumber.str() );
				return false;
			} else {
				newLevel->setPar(atoi(tokens[1].c_str()));
			}

			// parsing of actual tile, tee, cups
		} else {
			if (tokens.size() < 5) {
				Logger::Instance()->err("Error: Invalid data in data file, line " + sLineNumber.str() );
				return false;
			} else if (tokens[0].compare(DataTypeTile) == 0) {

				int id = atoi(tokens[1].c_str());
				int numVertices = atoi(tokens[2].c_str());

				// if total number of tokens dont add up, throw an error
				//type, id, numVertices + 3 verts and 1 neighbor per vertex
				if (tokens.size() != 3 + 4 * numVertices) {
					Logger::Instance()->err("Error: Invalid data in data file, line " + sLineNumber.str() );
					return false;
				}

				vector<Vec3f> verticesTemp;
				vector<int> neighborsTemp;

				int index = 3;
				int neighborIndex = tokens.size() - numVertices; 
				for (int i = 0; i < numVertices; i++) {
					// load the vertices
					Vec3f vert = 
						Vec3f (atof(tokens[index].c_str()),
						atof(tokens[index + 1].c_str()),
						atof(tokens[index + 2].c_str()));

					// add the vertices and the neighbor indices
					verticesTemp.push_back(vert);
					neighborsTemp.push_back( atoi(tokens[neighborIndex].c_str()) );

					index += 3;
					neighborIndex ++;
				}

				CMMPointer<tile>* tempTile = 
					new CMMPointer<tile>(new tile(id, verticesTemp, neighborsTemp, TILE_COLOR));

				newLevel->addTile(id, tempTile);

				delete tempTile;

			} else if (tokens[0].compare(DataTypeCup) == 0) {

				int id = atoi(tokens[1].c_str());
				Vec3f position(atof(tokens[2].c_str()),
					atof(tokens[3].c_str()),
					atof(tokens[4].c_str()));
				CMMPointer<cup>* tempCup = new CMMPointer<cup>(new cup(id, position));
				newLevel->addCup(id, tempCup);
				tempCup = 0;
				delete tempCup;
			} else if (tokens[0].compare(DataTypeTee) == 0) {

				int id = atoi(tokens[1].c_str());
				Vec3f position = Vec3f(atof(tokens[2].c_str()),
					atof(tokens[3].c_str()),
					atof(tokens[4].c_str()));
				CMMPointer<tee>* tempTee = new CMMPointer<tee>(new tee(id, position));
				newLevel->addTee(id, tempTee);

				//add a ball in the location of the tee
				CMMPointer<ball>* tempBall = new CMMPointer<ball>(new ball(id, position, BALL_COLOR, BALL_RADIUS));
				newLevel->addBall(id, tempBall);

				tempTee = 0;
				tempBall = 0;
				delete tempTee;
				delete tempBall;
			} else {
				Logger::Instance()->err("Error: Unknown Type in data file, line " + sLineNumber.str() );
				return false;
			}

		}
		lineNumber++;
		getline(mapInput, line);
	}

	mapInput.close();

	course->setNumLevels(numLevelsToParse);

	if (beginCount != endCount || course->getNumLevels() != endCount)
	{

		std::stringstream errorLog;
		errorLog << "Error: Invalid number of levels parse. Expected: " << numLevelsToParse << " being_hole count: " << beginCount << " end_hole count: " << endCount;
		Logger::Instance()->err(errorLog.str());
		return false;
	}

	return true;

}

bool fileReader::readFile(char* filename, CMMPointer<level> map) {
	// read files into streams
	ifstream mapInput(filename);

	if (mapInput.fail()){
		std::string stringFile(filename); //string filename for passing to the logger
		Logger::Instance()->err("failed to open " + stringFile + " attempting to add .db extension");
		mapInput.clear();

		char fileExt[] = ".db";
		strcat(filename, fileExt);
		Logger::Instance()->write("Attempting to open level file with appended name: " + stringFile + ".db");
		mapInput.open(filename, ifstream::in);

		// throws error if reading failed
		if (mapInput.fail()) 
		{
			Logger::Instance()->err("Error: File not Found: " + stringFile);
			return false;
		}
	}



	string line;
	int lineNumber = 1;


	getline(mapInput, line);

	while (line.length() > 0) {
		vector<string> tokens = strSplit(line);

		// first token to the dataType
		string type = tokens[0];

		//A string stream of the line number for the logger
		std::stringstream sLineNumber;
		sLineNumber << lineNumber;

		if (tokens.size() < 5) {
			Logger::Instance()->err("Error: Invalid data in data file, line " + sLineNumber.str() );
			return false;
		} else if (tokens[0].compare(DataTypeTile) == 0) {

			int id = atoi(tokens[1].c_str());
			int numVertices = atoi(tokens[2].c_str());

			// if total number of tokens dont add up, throw an error
			//type, id, numVertices + 3 verts and 1 neighbor per vertex
			if (tokens.size() != 3 + 4 * numVertices) {
				Logger::Instance()->err("Error: Invalid data in data file, line " + sLineNumber.str() );
				return false;
			}

			vector<Vec3f> verticesTemp;
			vector<int> neighborsTemp;

			int index = 3;
			int neighborIndex = tokens.size() - numVertices; 
			for (int i = 0; i < numVertices; i++) {
				// load the vertices
				Vec3f vert = 
					Vec3f (atof(tokens[index].c_str()),
					atof(tokens[index + 1].c_str()),
					atof(tokens[index + 2].c_str()));

				// add the vertices and the neighbor indices
				verticesTemp.push_back(vert);
				neighborsTemp.push_back( atoi(tokens[neighborIndex].c_str()) );

				index += 3;
				neighborIndex ++;
			}

			CMMPointer<tile>* tempTile = 
				new CMMPointer<tile>(new tile(id, verticesTemp, neighborsTemp, TILE_COLOR));

			map->addTile(id, tempTile);

			delete tempTile;

		} else if (tokens[0].compare(DataTypeCup) == 0) {

			int id = atoi(tokens[1].c_str());
			Vec3f position(atof(tokens[2].c_str()),
				atof(tokens[3].c_str()),
				atof(tokens[4].c_str()));
			CMMPointer<cup>* tempCup = new CMMPointer<cup>(new cup(id, position));
			map->addCup(id, tempCup);
			tempCup = 0;
			delete tempCup;
		} else if (tokens[0].compare(DataTypeTee) == 0) {

			int id = atoi(tokens[1].c_str());
			Vec3f position = Vec3f(atof(tokens[2].c_str()),
				atof(tokens[3].c_str()),
				atof(tokens[4].c_str()));
			CMMPointer<tee>* tempTee = new CMMPointer<tee>(new tee(id, position));
			map->addTee(id, tempTee);

			//add a ball in the location of the tee
			CMMPointer<ball>* tempBall = new CMMPointer<ball>(new ball(id, position, BALL_COLOR, BALL_RADIUS));
			map->addBall(id, tempBall);

			tempTee = 0;
			tempBall = 0;
			delete tempTee;
			delete tempBall;
		} else {
			Logger::Instance()->err("Error: Unknown Type in data file, line " + sLineNumber.str() );
			return false;
		}

		lineNumber++;
		getline(mapInput, line);
	}

	mapInput.close();

	if (!map->checkLevel()) {
		return false;
	}

	return true;
}
vector<string> fileReader::strSplit(string data, string delims) {
	// loops through character by character and delimits when the delimiter is found
	vector<string> toks;
	size_t start = 0, end = 0;

	for (size_t i = 0; i < data.length(); i++) {
		// if you dont find the delimiting character, increment the end char ptr
		if (delims.find(data[i]) == string::npos) {
			++end;
			// if a delimiting character is found
			// if it is valid, ie the end ptr is more than the start ptr, add the entry into the vector
		} else if (end > start) {
			string token = data.substr(start, end - start);
			// only save the token if its non ""
			toks.push_back(token);
			// move the end ptr and reset the start ptr
			++end;
			start = end;
			// if a delimiter is found but no additional token is found, increment both ptrs
		} else {
			++start;
			++end;
		}
	}

	//grab the last token (if any)
	if (end > start) {
		toks.push_back(data.substr(start, end - start));
	}

	return toks;
}