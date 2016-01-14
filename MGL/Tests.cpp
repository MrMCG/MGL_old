#include "stdafx.h"
#include "MGL.h"

/*
	Test cases for debugging some MGL classes.
	The main function of these is to run to completion
	and not crash the program, showing that the errors
	are caught and handled at runtime.

	That said, there is no standard result from these functions
	or what they test, they are used for debugging purposes
*/

#ifdef MGLDEBUG

/****** Test options *****/

#define FILESPEED_ITERATIONS 1000

/*********************************************/
/*********** MGL Utility Functions ***********/
/*********************************************/

void MGL_TESTS_::MGL_FileCleanup() {
	for (GLuint i = 0; i < createdTestFiles.size(); i += 2) {
		std::string file = createdTestFiles[i] + createdTestFiles[i + 1]; // file name + extension
		try {
			MGLException_IsNotZero::Test(std::remove(file.c_str()));
		}
		catch (MGLException& e) {
			//std::cerr << e.what() << std::endl;
			//std::cerr << "Error deleting file " << file.c_str() << std::endl;

			MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s%s", e.what(), "Error deleting file ", file.c_str());
		}
	}
}

void MGL_TESTS_::MGL_WriteTempLog(const std::string fileName) {
	std::cout << "Dumping current log to "MGL_LOG_DIRECTORY << fileName << std::endl;
	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY+fileName, GL_FALSE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);
}

/****************************************/
/*********** MGL Test Various ***********/
/****************************************/

void MGL_TESTS_::MGL_TEST_VARIOUS() {
	std::cout << "\n\tBEGIN - MGL_TEST_VARIOUS\n" << std::endl;
	MGL_WriteTempLog("mgl_error_log_temp.txt");
	std::cout << "Running tests..." << std::endl;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS");

	MGL_TEST_VARIOUS_FILESPEED();
	
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS");

	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY"mgl_tests_various.txt", MGL_LOG_ERROR, GL_TRUE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);

	std::cout << "\n\tEND - MGL_TEST_VARIOUS\n" << std::endl;
}

void MGL_TESTS_::MGL_TEST_VARIOUS_FILESPEED() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS_FILESPEED\n");

	MGLMesh* mesh = nullptr;
	MGLTimer timer = MGLTimer();

	// timed load num of cube.obj - watch memory usage
	timer.Start();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%i%s", "BEGIN Load of ", FILESPEED_ITERATIONS, " cube.obj");
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj");
		
		delete mesh;
	}
	timer.End();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "END Load - Time: %f", timer.Time());

	// timed load num of cube.mgl - watch memory usage
	timer.Start();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%i%s", "BEGIN Load of ", FILESPEED_ITERATIONS, " cube.mgl");
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl");

		delete mesh;
	}
	timer.End();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "END Load - Time: %f", timer.Time());

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS_FILESPEED");
}

/****************************************/
/*********** MGL Test Classes ***********/
/****************************************/

void MGL_TESTS_::MGL_TEST_CLASSES(const GLboolean cleanFiles) {
	MGLLog::Init(); //just to be sure

	std::string endMessage = "\n\tEND - MGL_TEST_CLASSES - SUCCESS\n";

	// write contents of current log to temp file
	std::cout << "\n\tBEGIN - MGL_TEST_CLASSES\n" << std::endl;
	MGL_WriteTempLog("mgl_error_log_temp.txt");
	std::cout << "Running tests..." << std::endl;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_CLASSES");

	// test if any tests return 0 (GL_FALSE)
	try {
		MGLException_IsZero::Test(MGL_TEST_MGLFILE());
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		endMessage = "\tEND - MGL_TEST_CLASSES - FAILED\n";
	}

	if (cleanFiles) // delete created files
		MGL_FileCleanup();

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, endMessage);
	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY"mgl_tests_classes.txt", MGL_LOG_ERROR, GL_TRUE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);

	std::cout << endMessage;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_MGLFILE");
	std::string endMessage = "\n\tEND - MGL_TEST_MGLFILE - SUCCESS\n";
	GLuint success = GL_TRUE;

	try {
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_COTM());
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_SMTM());
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_LO());
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_LM());
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		endMessage = "\tEND - MGL_TEST_MGLFILE - FAILED\n";
		success = GL_FALSE;
	}

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, endMessage);

	std::cout << endMessage;
	return success;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_COTM() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - ConverOBJToMGL - ");

	// No file
	MGLFileHandle->ConvertOBJToMGL(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.obj", MGL_DEFAULT_TESTS_DIRECTORY"TEST1", GL_FALSE);
	MGLFileHandle->ConvertOBJToMGL(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.obj", MGL_DEFAULT_TESTS_DIRECTORY"TEST2", GL_TRUE);

	// wrong file type
	MGLFileHandle->ConvertOBJToMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", MGL_DEFAULT_TESTS_DIRECTORY"TEST3", GL_FALSE);
	MGLFileHandle->ConvertOBJToMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", MGL_DEFAULT_TESTS_DIRECTORY"TEST4", GL_TRUE);

	// CORRECT
	MGLFileHandle->ConvertOBJToMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", createdTestFiles[0], GL_FALSE);
	MGLFileHandle->ConvertOBJToMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", createdTestFiles[2], GL_TRUE);

	return GL_TRUE;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_LO() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - LoadOBJ - ");

	GLuint success = GL_TRUE;
	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.obj", GL_FALSE);
	delete mesh;

	// file name error
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"!£$^&*().obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"!£$^&*().obj", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", GL_FALSE);
	delete mesh;

	// file load error
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.obj", GL_FALSE);
	delete mesh;

	// CORRECT
	try {
		mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadOBJ(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
		delete mesh;
		success = GL_FALSE;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "%s%s", e.what(), ": Mesh is triangle");
	}

	return success;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_LM() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - LoadMGL - ");

	GLuint success = GL_TRUE;
	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.mgl", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_FALSE);
	delete mesh;

	// file too small
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cubeTiny.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cubeTiny.mgl", GL_FALSE);
	delete mesh;

	// file size mismatch from determined size
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cubeSmall.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cubeSmall.mgl", GL_FALSE);
	delete mesh;

	// initial inputs incorrect
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.mgl", GL_FALSE);
	delete mesh;

	// CORRECT
	try {
		mesh = MGLFileHandle->LoadMGL(createdTestFiles[0] + createdTestFiles[1], GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[0] + createdTestFiles[1], GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[2] + createdTestFiles[3], GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[2] + createdTestFiles[3], GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[4] + createdTestFiles[5], GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[4] + createdTestFiles[5], GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[6] + createdTestFiles[7], GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[6] + createdTestFiles[7], GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
		delete mesh;
		success = GL_FALSE;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "%s%s", e.what(), ": MESH IS TRIANGLE");
	}

	return success;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_SMTM() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - SaveMeshToMGL - ");

	MGLMesh* mesh = nullptr;

	// null mesh
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[4], GL_TRUE);
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[6], GL_FALSE);

	// file name error
	mesh = new MGLMesh(MGL_MESH_QUAD);
	MGLFileHandle->SaveMeshToMGL(mesh, "!£$^&*()", GL_TRUE);
	MGLFileHandle->SaveMeshToMGL(mesh, "!£$^&*()", GL_FALSE);

	// CORRECT
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[4], GL_TRUE);
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[6], GL_FALSE);

	delete mesh;

	return GL_TRUE;
}



#endif