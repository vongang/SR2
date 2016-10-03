#include "Util.h"

bool Util::LoadOBJ(const char* path, Mesh* mesh) {
	printf("Loading OBJ file %s...\n", path);
	std::vector<Vec4> temp_vertices;
	std::vector<Vec4> temp_normals;
	std::vector<Face> temp_faces;
	FILE* file = fopen(path, "r");

	if (file == nullptr) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}

	while (1) {
		char line_header[256];
		int res = fscanf(file, "%s", line_header);
		if (res == EOF)	break;
		if (strcmp(line_header, "v") == 0) {
			Vec4 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(line_header, "vn") == 0){
			//cout << "Get vn" << endl;
			Vec4 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.w = 0.0f;
			temp_normals.push_back(normal);
		}
		else if (strcmp(line_header, "f") == 0){
	
			uint32 vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if (matches != 6){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			temp_faces.push_back(Face(vertexIndex[0] - 1, vertexIndex[1] - 1, vertexIndex[2] - 1, normalIndex[0] - 1, normalIndex[1] - 1, normalIndex[2] - 1 ));
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	if (temp_vertices.size() != temp_normals.size()) {
		printf("Vertices numbers != Nromals numbers\n");
		return false;
	}

	uint32 _vt_count = static_cast<uint32>(temp_vertices.size());
	uint32 _face_count = static_cast<uint32>(temp_faces.size());
	
	mesh->vertices = new Vec4[_vt_count];
	mesh->normals = new Vec4[_vt_count];
	mesh->faces = new Face[_face_count];

	mesh->name = path;
	mesh->vt_count = _vt_count;
	mesh->face_count = _face_count;

	Face* out_faces = mesh->faces;
	uint32 i, j = 0;
	for (i = 0; i < _vt_count; ++i) {
		mesh->vertices[i] = temp_vertices[i];
		mesh->normals[i] = temp_normals[i];
	}

	for_each(temp_faces.begin(), temp_faces.end(), [&j, out_faces](Face& fc){out_faces[j] = fc; ++j; });
	
	printf("vertices number: %d, faces number: %d\n", i, j);
	return true;
}