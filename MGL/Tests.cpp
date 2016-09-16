#include "stdafx.h"
#include "MGLContext.h"

/*
	Test cases for debugging some MGL classes.
	The main function of these is to run to completion
	and not crash the program, showing that the errors
	are caught and handled at runtime.

	That said, there is no standard result from these functions
	or what they test, they are used purely for debugging purposes
*/

#ifdef MGL_USER_INCLUDE_TESTS

/****** Test options *****/

#define FILESPEED_ITERATIONS 5
#define MEMORY_ITERATIONS 100000

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

			MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s%s", e.what(), "Error deleting file ", file.c_str());
		}
	}
}

void MGL_TESTS_::MGL_TEST_ALL() {
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- TESTS -----");
	MGL_TEST_CLASSES();
	MGL_TEST_VARIOUS();
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- TESTS -----");
}

/****************************************/
/*********** MGL Test Various ***********/
/****************************************/

void MGL_TESTS_::MGL_TEST_VARIOUS() {
	std::cout << "\n\tBEGIN - MGL_TEST_VARIOUS\n" << std::endl;
	std::cout << "Running tests..." << std::endl;

	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS");

	MGL_TEST_VARIOUS_FILESPEED();
	//MGL_TEST_VARIOUS_MEMMORY();
	
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS");

	MGLI_Log->WriteToFile(MGL_LOG_DIRECTORY"mgl_tests_various.txt", MGL_LOG_ERROR, GL_TRUE);
	MGLI_Log->Flush(MGL_LOG_ERROR);

	std::cout << "\n\tEND - MGL_TEST_VARIOUS\n" << std::endl;
}

void MGL_TESTS_::MGL_TEST_VARIOUS_FILESPEED() {
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS_FILESPEED\n");

	MGLMesh* mesh = nullptr;
	MGLTimer timer = MGLTimer();

	/****** at-at ******/

	// load at-at.obj
	timer.Start();
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"at-at.obj", GL_FALSE);
		delete mesh;
	}
	timer.End();
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "Load of %i at-at.obj averaged :  %fs", FILESPEED_ITERATIONS, timer.GetTime() / (GLfloat)FILESPEED_ITERATIONS);

	// load at-at.mgl
	timer.Start();
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"at-at.mgl", GL_FALSE);
		delete mesh;
	}
	timer.End();
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "Load of %i at-at.mgl averaged :  %fs", FILESPEED_ITERATIONS, timer.GetTime() / (GLfloat)FILESPEED_ITERATIONS);
	
	/****** MGL ******/

	// load ONLY ONE death-star-II.obj (its slow)
	timer.Start();
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"death-star-II.obj", GL_FALSE);
	delete mesh;
	timer.End();
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "Load of 1 death-star-II.obj :  %fs", timer.GetTime());

	
	// load death-star-II.mgl
	timer.Start();
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"death-star-II.mgl", GL_FALSE);
		delete mesh;
	}
	timer.End();
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "Load of %i death-star-II.mgl averaged :  %fs", FILESPEED_ITERATIONS, timer.GetTime() / (GLfloat)FILESPEED_ITERATIONS);


	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS_FILESPEED");
}

void MGL_TESTS_::MGL_TEST_VARIOUS_MEMMORY() {
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS_MEMORY\n");
	MGLI_Log->Disable(MGL_LOG_MAIN); // prevent logging of thousands of loads

	MGLMesh* mesh = nullptr;

	// mesh loading from obj
	for (GLuint i = 0; i < MEMORY_ITERATIONS; ++i) {
		mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_FALSE);
		delete mesh;
	}

	// mesh loading from mgl
	for (GLuint i = 0; i < MEMORY_ITERATIONS; ++i) {
		mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", GL_FALSE);
		delete mesh;
	}

	MGLI_Log->Enable(MGL_LOG_MAIN); // enable main log
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS_MEMORY");
}

/****************************************/
/*********** MGL Test Classes ***********/
/****************************************/

void MGL_TESTS_::MGL_TEST_CLASSES(const GLboolean cleanFiles) {
	MGLLog::Init(); //just to be sure

	std::string endMessage = "\n\tEND - MGL_TEST_CLASSES - SUCCESS\n";

	// write contents of current log to temp file
	std::cout << "\n\tBEGIN - MGL_TEST_CLASSES\n" << std::endl;
	std::cout << "Running tests..." << std::endl;

	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_CLASSES");

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

	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, endMessage);
	MGLI_Log->WriteToFile(MGL_LOG_DIRECTORY"mgl_tests_classes.txt", MGL_LOG_ERROR, GL_TRUE);
	MGLI_Log->Flush(MGL_LOG_ERROR);

	std::cout << endMessage;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE() {
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_MGLFILE");
	std::string endMessage = "\n\tEND - MGL_TEST_MGLFILE - SUCCESS\n";
	GLuint success = GL_TRUE;

	try { // ORDER IS IMPORTANT
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_SMTM());
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_OBJ());
		MGLException_IsZero::Test(MGL_TEST_MGLFILE_MGL());

	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		endMessage = "\tEND - MGL_TEST_MGLFILE - FAILED\n";
		success = GL_FALSE;
	}

	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, endMessage);

	return success;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_OBJ() {
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - LoadOBJ - ");

	GLuint success = GL_TRUE;
	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.obj", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.obj", GL_FALSE);
	delete mesh;

	// file name error
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"!£$^&*().obj", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"!£$^&*().obj", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.mgl", GL_FALSE);
	delete mesh;

	// file load error
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.obj", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.obj", GL_FALSE);
	delete mesh;

	// CORRECT
	try {
		mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeAll.obj", GL_TRUE); // polygons in obj
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLI_FileOBJ->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeAll.obj", GL_FALSE); // polygons in obj
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
		delete mesh;
		success = GL_FALSE;
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "%s%s", e.what(), ": Mesh is triangle");
	}

	return success;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_MGL() {
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - LoadMGL - ");

	GLuint success = GL_TRUE;
	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"wrongfile.mgl", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cube.obj", GL_FALSE);
	delete mesh;

	// file too small
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeTiny.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeTiny.mgl", GL_FALSE);
	delete mesh;

	// file size mismatch from determined size
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeSmall.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeSmall.mgl", GL_FALSE);
	delete mesh;

	// initial inputs incorrect
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLI_FileMGL->Load(MGL_DEFAULT_TESTS_DIRECTORY"cubeError.mgl", GL_FALSE);
	delete mesh;

	// CORRECT PARAMS
	try {
		mesh = MGLI_FileMGL->Load(createdTestFiles[0] + createdTestFiles[1], GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLI_FileMGL->Load(createdTestFiles[0] + createdTestFiles[1], GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLI_FileMGL->Load(createdTestFiles[2] + createdTestFiles[3], GL_TRUE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLI_FileMGL->Load(createdTestFiles[2] + createdTestFiles[3], GL_FALSE);
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
		delete mesh;
		success = GL_FALSE;
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "%s%s", e.what(), ": MESH IS TRIANGLE");
	}

	return success;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_SMTM() {
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - SaveMeshToMGL - ");

	MGLMesh* mesh = nullptr;

	// null mesh
	MGLI_FileMGL->SaveMeshToMGL(mesh, createdTestFiles[0], GL_TRUE);
	MGLI_FileMGL->SaveMeshToMGL(mesh, createdTestFiles[2], GL_FALSE);

	// file name error
	mesh = new MGLMesh(MGL_MESH_QUAD);
	MGLI_FileMGL->SaveMeshToMGL(mesh, "!£$^&*()", GL_TRUE);
	MGLI_FileMGL->SaveMeshToMGL(mesh, "!£$^&*()", GL_FALSE);

	// CORRECT PARAMS
	MGLI_FileMGL->SaveMeshToMGL(mesh, createdTestFiles[0], GL_TRUE);
	MGLI_FileMGL->SaveMeshToMGL(mesh, createdTestFiles[2], GL_FALSE);

	delete mesh;

	return GL_TRUE;
}

#endif // MGL_USER_INCLUDE_TESTS