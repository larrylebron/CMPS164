#include "Logger.h"

// Global static pointer used to ensure a single instance of the class.
Logger* Logger::m_pInstance = NULL;  



/** This function is called to create an instance of the class. 
    Calling the constructor publicly is not allowed. The constructor 
    is private and is only called by this Instance function.
*/  
Logger* Logger::Instance()
{
   if (!m_pInstance) {   // Only allow one instance of class to be generated.
      m_pInstance = new Logger;
   }

   return m_pInstance;
}

bool Logger::pWrite(string filename, string message, bool error) {
	
	string file = "logs\\" + filename; //to place the log in the logs directory

	writer.open(file.c_str(), std::fstream::out | std::fstream::app);

	if (writer.fail()){
		cerr << "Logger failed to open " << filename << "\n";
		writer.clear();
		return false;
	}

	//If fileName hasn't been previously used, add it to the set and append a timestamp in the log file
	if(fileNames.find(filename) == fileNames.end()) {
		fileNames.insert(filename);
		Timer time;
		writer << "\n\n\n" << time.getTimeStamp() << "========================\n";

	}

	writer << message << std::endl;
	writer.close();

	if (error) cerr << message << std::endl;
	else cout << message << std::endl;

	return true;

}


bool Logger::err(string errorMsg) {
	cerr << "ERROR: ";
	if ( pWrite(ERROR_LOG, errorMsg, true) ) return true;

	return false;
}

bool Logger::write(string message) {
	if ( pWrite(GENERAL_LOG, message, false) ) return true;

	return false;
}

bool Logger::write(string filename, string message) {
	if ( pWrite(filename, message, false) ) return true;

	return false;

}