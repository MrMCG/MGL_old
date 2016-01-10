#include "stdafx.h"
#include "MGL.h"

/*
	Test cases for debugging MGL classes.
	The main function of these is to run to completion
	and not crash the program, showing that the scenarios
	are caught and handled at runtime.

	That said, there is no standard result from these functions
	and they are used for debugging purposes
*/

#ifdef MGLDEBUG
void MGL_TESTS_::MGL_TEST_ALL(const GLboolean cleanFiles) {
	std::cout << "\n\tBEGIN - MGL_TEST_ALL" << std::endl;

	std::string message = "\n\tEND - MGL_TEST_ALL - SUCCESS\n";

	// counter for the number of functions ran
	GLuint counter[Counters::max];

	for (GLuint i = 0; i < Counters::max; ++i) {
		counter[i] = 0;
	}

	// ORDER IS IMPORTANT
	counter[MGLFile] = MGL_TEST_MGLFILE();
	counter[MGLLog] = MGL_TEST_MGLLOG();

	// ensure number of functions ran is correct
	try {
		MGLException_IsLessThan::IsSuccessful(counter[MGLFile], (GLuint)MGLFileMax);
		MGLException_IsLessThan::IsSuccessful(counter[MGLLog], (GLuint)MGLLogMax);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		message = "\n\tEND - MGL_TEST_ALL - FAILED\n";
	}

	if (cleanFiles) // delete created files
		MGL_TEST_FileCleanup();

	std::cout << message << std::endl;
}

void MGL_TESTS_::MGL_TEST_FileCleanup() {
	for (GLuint i = 0; i < createdTestFiles.size(); i += 2) {
		std::string file = createdTestFiles[i] + createdTestFiles[i + 1]; // file name + extension
		try {
			MGLException_IsNotZero::IsSuccessful(std::remove(file.c_str()));
		}
		catch (MGLException& e) {
			std::cerr << e.what() << std::endl;
			std::cerr << "Error deleting file " << file.c_str() << std::endl;
		}
	}
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE() {
	std::cout << "\n\tBEGIN - MGL_TEST_MGLFILE" << std::endl;

	GLuint functionCounter = 0;

	// ORDER IS IMPORTANT
	functionCounter += MGL_TEST_MGLFILE_COTM();
	functionCounter += MGL_TEST_MGLFILE_SMTM();
	functionCounter += MGL_TEST_MGLFILE_LO();
	functionCounter += MGL_TEST_MGLFILE_LM();

	std::cout << "\n\tEND MGL_TEST_MGLFILE" << std::endl;

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLLOG() {
	std::cout << "\n\tBEGIN - MGL_TEST_MGLLOG" << std::endl;

	GLuint functionCounter = 0;

	// ORDER IS IMPORTANT
	functionCounter += MGL_TEST_MGLLOG_AL();
	functionCounter += MGL_TEST_MGLLOG_WTF();

	std::cout << "\n\tEND MGL_TEST_MGLLOG" << std::endl;

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_COTM() {
	GLuint functionCounter = 0;

	std::cout << std::endl << " - ConverOBJToFile - " << std::endl;

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

	std::cout << std::endl << " - LoadOBJ - " << std::endl;

	MGLMesh* mesh = nullptr;

	// no file
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"wrongfile.obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"wrongfile.obj", GL_FALSE);
	delete mesh;

	// file name error
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"!£$%^&*().obj", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"!£$%^&*().obj", GL_FALSE);
	delete mesh;

	// wrong file type
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.mgl", GL_TRUE);
	delete mesh;
	mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.mgl", GL_FALSE);
	delete mesh;

	functionCounter += 6;

	// CORRECT
	try {
		mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.obj", GL_TRUE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadOBJ(MGL_TESTS_DIRECTORY"cube.obj", GL_FALSE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

	}
	catch (MGLException& e) {
		std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
	}

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_LM() {
	GLuint functionCounter = 0;

	std::cout << std::endl << " - LoadMGL - " << std::endl;

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
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[0] + createdTestFiles[1], GL_FALSE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[2] + createdTestFiles[3], GL_TRUE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[2] + createdTestFiles[3], GL_FALSE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[4] + createdTestFiles[5], GL_TRUE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[4] + createdTestFiles[5], GL_FALSE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[6] + createdTestFiles[7], GL_TRUE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given

		mesh = MGLFileHandle->LoadMGL(createdTestFiles[6] + createdTestFiles[7], GL_FALSE);
		functionCounter++;
		delete mesh;
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4); // if default triangle given
	}
	catch (MGLException& e) {
		std::cerr << e.what() << ": MESH IS TRIANGLE" << std::endl;
	}

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLFILE_SMTM() {
	GLuint functionCounter = 0;

	std::cout << std::endl << " - SaveMeshToMGL - " << std::endl;

	MGLMesh* mesh = nullptr;

	// null mesh
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[4], GL_TRUE);
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[6], GL_FALSE);

	// file name error
	mesh = new MGLMesh(MGL_MESH_QUAD);
	MGLFileHandle->SaveMeshToMGL(mesh, "!£$%^&*()", GL_TRUE);
	MGLFileHandle->SaveMeshToMGL(mesh, "!£$%^&*()", GL_FALSE);

	// CORRECT
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[4], GL_TRUE);
	MGLFileHandle->SaveMeshToMGL(mesh, createdTestFiles[6], GL_FALSE);

	functionCounter += 6;
	delete mesh;

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLLOG_AL() {
	GLuint functionCounter = 0;

	std::cout << std::endl << " - AddLog - " << std::endl;

	GLfloat test = 12345.12345f;
	std::string str = "testString";

	// test timestamp
	MGLLodHandle->AddLog(GL_TRUE, GL_TRUE, "NEW TEST RUN");
	MGLLodHandle->AddLog(GL_TRUE, GL_FALSE, "NEW TEST RUN");

	// test string functionality
	MGLLodHandle->AddLog(GL_TRUE, GL_TRUE, "TEST1 | %s", str.c_str());
	MGLLodHandle->AddLog(GL_TRUE, GL_FALSE, "TEST2 | %5.2f | %3.0f | %1.1f", 12345.12345f, 12345.12345f, test);

	functionCounter += 4;

	// CORRECT
	try {
		MGLException_IsLessThan::IsSuccessful(MGLLodHandle->GetMainLog()->size(), (GLuint)4);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << ": ADD LOG ERROR" << std::endl;
		functionCounter -= 1;
	}

	return functionCounter;
}

GLuint MGL_TESTS_::MGL_TEST_MGLLOG_WTF() {
	GLuint functionCounter = 0;

	std::cout << std::endl << " - WriteToFile - " << std::endl;

	MGLLodHandle->WriteToFile(GL_TRUE, GL_TRUE, GL_FALSE);
	MGLLodHandle->WriteToFile(GL_TRUE, GL_FALSE, GL_FALSE);
	MGLLodHandle->WriteToFile(GL_TRUE, GL_FALSE, GL_FALSE);

	functionCounter += 3;

	return functionCounter;
}

#endif