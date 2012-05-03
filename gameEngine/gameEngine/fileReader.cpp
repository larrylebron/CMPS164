#include "fileReader.h"



fileReader::fileReader()
{
}


fileReader::~fileReader()
{
}

bool fileReader::readFile(char* filename, CMMPointer<level> map) {
	// read files into streams
	ifstream mapInput(filename);

	if (mapInput.fail()){
		cerr << "failed to open " << filename << " attempting to add .db extension\n";
		mapInput.clear();

		char fileExt[] = ".db";
		strcat(filename, fileExt);
		cout << "appended name: " << filename << endl;
		mapInput.open(filename, ifstream::in);

		// throws error if reading failed
		if (mapInput.fail()) 
		{
			cerr << "Error: File not Found - " << filename << endl;
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

		if (tokens.size() < 5) {
			cerr << "Error: Invalid data in data file, line " << lineNumber << endl;
			return false;
		} else if (tokens[0].compare(DataTypeTile) == 0) {

			int id = atoi(tokens[1].c_str());
			int numVertices = atoi(tokens[2].c_str());

			// if total number of tokens dont add up, throw an error
			//type, id, numVertices + 3 verts and 1 neighbor per vertex
			if (tokens.size() != 3 + 4 * numVertices) {
				cerr << "Error: Invalid tile data in data file, line " << lineNumber << endl;
				return false;
			}

			Vec3f* verticesTemp = new Vec3f[numVertices];
			int* neighborsTemp = new int[numVertices];

			int index = 3;
			int neighborIndex = tokens.size() - numVertices; 
			for (int i = 0; i < numVertices; i++) {
				// load the vertices
				Vec3f vert = Vec3f (atof(tokens[index].c_str()),
					atof(tokens[index + 1].c_str()),
					atof(tokens[index + 2].c_str()));

				// add the vertices and the neighbor indices
				verticesTemp[i] = vert;
				neighborsTemp[i] = atoi(tokens[neighborIndex].c_str());

				index += 3;
				neighborIndex ++;
			}

			// calculate normal
			Vec3f a = verticesTemp[0];
			Vec3f b = verticesTemp[1];
			Vec3f c = verticesTemp[2];
			Vec3f ba = a - b;
			Vec3f bc = c - a;
			Vec3f normal = bc.cross(ba).normalize();  // by right hand rule

			CMMPointer<tile>* tempTile = 
				new CMMPointer<tile>(new tile(id, numVertices, numVertices, verticesTemp, neighborsTemp, normal));

			map->addTile(id, tempTile);

			delete[] verticesTemp;
			delete[] neighborsTemp;
			tempTile = 0;
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
			CMMPointer<ball>* tempBall = new CMMPointer<ball>(new ball(id, position));
			map->addBall(id, tempBall);

			tempTee = 0;
			tempBall = 0;
			delete tempTee;
			delete tempBall;
		} else {
			cerr << "Error: Unknown Type in data file, line " << lineNumber << endl;
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
                       toks.push_back(data.substr(start, end - start));
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