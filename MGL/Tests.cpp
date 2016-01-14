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

#define FILESPEED_ITERATIONS 1000

void MGL_TESTS_::MGL_TEST_CLASSES(const GLboolean cleanFiles) {
	MGLLog::Init(); //just to be sure

	std::string message = "\n\tEND - MGL_TEST_CLASSES - SUCCESS\n";

	// counter for the number of functions ran
	GLuint counter[Counters::max];

	for (GLuint i = 0; i < Counters::max; ++i) {
		counter[i] = 0;
	}

	// write contents of current log to temp file
	std::cout << "\n\tBEGIN - MGL_TEST_CLASSES\n" << std::endl;
	std::cout << "Dumping current log to "MGL_LOG_DIRECTORY"mgl_error_log_temp.txt" << std::endl;
	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY"mgl_error_log_temp.txt", GL_FALSE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);
	std::cout << "Running tests..." << std::endl;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_CLASSES");
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n***************************************");
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "***** EXPECT ERRORS BY THE TONNE! *****");
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "*********** BUT DONT WORRY! ***********");
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "***************************************");

	counter[Counters::MGLFile] = MGL_TEST_MGLFILE();
	
	// ensure number of functions ran is correct
	try {
		MGLException_IsLessThan::Test(counter[Counters::MGLFile], (GLuint)FuncMax::MGLFileMax);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		message = "\tEND - MGL_TEST_CLASSES - FAILED\n";
	}

	if (cleanFiles) // delete created files
		MGL_TEST_FileCleanup();

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, message);
	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY"mgl_tests_all.txt", GL_FALSE, GL_FALSE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);

	std::cout << message;
}

void MGL_TESTS_::MGL_TEST_FileCleanup() {
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

void MGL_TESTS_::MGL_TEST_VARIOUS() {
	std::cout << "\n\tBEGIN - MGL_TEST_VARIOUS" << std::endl;
	std::cout << "Dumping current log to "MGL_LOG_DIRECTORY"mgl_error_log_temp.txt" << std::endl;
	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY"mgl_error_log_temp.txt", GL_FALSE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);
	std::cout << "Running tests..." << std::endl;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS");

	MGL_TEST_VARIOUS_FILESPEED();
	
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS");

	MGLLogHandle->WriteToFile(MGL_LOG_DIRECTORY"mgl_tests_all.txt", GL_FALSE, GL_FALSE);
	MGLLogHandle->Flush(MGL_LOG_ERROR);

	std::cout << "\n\tEND - MGL_TEST_VARIOUS" << std::endl;
}

void MGL_TESTS_::MGL_TEST_VARIOUS_FILESPEED() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_VARIOUS_FILESPEED\n");

	MGLMesh* mesh = nullptr;
	MGLTimer timer = MGLTimer();

	timer.Start();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%i%s", "BEGIN Load of ", FILESPEED_ITERATIONS, " cube.obj");
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.obj");
		
		delete mesh;
	}
	timer.End();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "END Load - Time: %f", timer.Time());

	timer.Start();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%i%s", "BEGIN Load of ", FILESPEED_ITERATIONS, " cube.mgl");
	for (GLuint i = 0; i < FILESPEED_ITERATIONS; ++i) {
		mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cube.mgl");

		delete mesh;
	}
	timer.End();
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "END Load - Time: %f", timer.Time());

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_VARIOUS_FILESPEED");
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE() {
	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tBEGIN - MGL_TEST_MGLFILE");

	GLuint functionCounter = 0;

	// ORDER IS IMPORTANT
	functionCounter += MGL_TEST_MGLFILE_COTM();
	functionCounter += MGL_TEST_MGLFILE_SMTM();
	functionCounter += MGL_TEST_MGLFILE_LO();
	functionCounter += MGL_TEST_MGLFILE_LM();

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n\tEND MGL_TEST_MGLFILE");

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_COTM() {
	GLuint functionCounter = 0;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - ConverOBJToMGL - ");

	// No file
	MGLFileHandle->ConvertOBJToMGL(MGL_TESTS_DIRECTORY"wrongfile.obj", MGL_TESTS_DIRECTORY"TEST1", GL_FALSE);
	MGLFileHandle->ConvertOBJToMGL(MGL_TESTS_DIRECTORY"wrongfile.obj", MGL_TESTS_DIRECTORY"TEST2", GL_TRUE);

	// wrong file type
	MGLFileHandle->ConvertOBJToMGL(MGL_TESTS_DIRECTORY"cube.mgl", MGL_TESTS_DIRECTORY"TEST3", GL_FALSE);
	MGLFileHandle->ConvertOBJToMGL(MGL_TESTS_DIRECTORY"cube.mgl", MGL_TESTS_DIRECTORY"TEST4", GL_TRUE);

	// CORRECT
	MGLFileHandle->ConvertOBJToMGL(MGL_TESTS_DIRECTORY"cube.obj", createdTestFiles[0], GL_FALSE);
	MGLFileHandle->ConvertOBJToMGL(MGL_TESTS_DIRECTORY"cube.obj", createdTestFiles[2], GL_TRUE);

	functionCounter += 6;
	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_LO() {
	GLuint functionCounter = 0;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - LoadOBJ - ");

	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"wrongfile.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"wrongfile.obj", GL_FALSE);
	delete mesh;

	// file name error
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"!£$^&*().obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"!£$^&*().obj", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.mgl", GL_FALSE);
	delete mesh;

	// file load error
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cubeError.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cubeError.obj", GL_FALSE);
	delete mesh;

	functionCounter += 8;

	// CORRECT
	try {
		mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.obj", GL_TRUE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.obj", GL_FALSE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
		delete mesh;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "%s%s", e.what(), ": Mesh is triangle");
	}

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_LM() {
	GLuint functionCounter = 0;

	MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "\n - LoadMGL - ");

	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"wrongfile.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"wrongfile.mgl", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cube.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cube.obj", GL_FALSE);
	delete mesh;

	// file too small
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cubeTiny.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cubeTiny.mgl", GL_FALSE);
	delete mesh;

	// file size mismatch from determined size
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cubeSmall.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cubeSmall.mgl", GL_FALSE);
	delete mesh;

	// initial inputs incorrect
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cubeError.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadMGL(MGL_TESTS_DIRECTORY"cubeError.mgl", GL_FALSE);
	delete mesh;

	functionCounter += 10;

	// CORRECT
	try {
		mesh = MGLFileHandle->LoadMGL(createdTestFiles[0] + createdTestFiles[1], GL_TRUE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[0] + createdTestFiles[1], GL_FALSE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[2] + createdTestFiles[3], GL_TRUE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[2] + createdTestFiles[3], GL_FALSE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[4] + createdTestFiles[5], GL_TRUE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[4] + createdTestFiles[5], GL_FALSE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[6] + createdTestFiles[7], GL_TRUE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[6] + createdTestFiles[7], GL_FALSE);
		functionCounter++;
		MGLException_IsLessThan::Test(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
		delete mesh;
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
		delete mesh;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_FALSE, "%s%s", e.what(), ": MESH IS TRIANGLE");
	}

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_SMTM() {
	GLuint functionCounter = 0;

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

	functionCounter += 6;
	delete mesh;

	return functionCounter;
}



#endif