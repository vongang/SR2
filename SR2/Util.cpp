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
			//printf("%f %f %f %f\n", vertex.x, vertex.y, vertex.z, vertex.w);
			vertex.y *= -1;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(line_header, "vn") == 0){
			//cout << "Get vn" << endl;
			Vec4 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(line_header, "f") == 0){
			//cout << "Get f" << endl;
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if (matches != 6){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			//printf("%d %d %d\n", vertexIndex[0], vertexIndex[1], vertexIndex[2]);
			temp_faces.push_back(Face(vertexIndex[0] - 1, vertexIndex[1] - 1, vertexIndex[2] - 1));
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	uint32 _vt_count = static_cast<uint32>(temp_vertices.size());
	uint32 _face_count = static_cast<uint32>(temp_faces.size());
	
	mesh->vertices = new Vec4[_vt_count];
	mesh->faces = new Face[_face_count];

	mesh->name = path;
	mesh->vt_count = _vt_count;
	mesh->face_count = _face_count;

	Vec4* out_vertics = mesh->vertices;
	Face* out_faces = mesh->faces;

	uint32 i = 0, j = 0;
	for_each(temp_vertices.begin(), temp_vertices.end(), [&i, out_vertics](Vec4& vt){out_vertics[i] = vt; i++; });
	for_each(temp_faces.begin(), temp_faces.end(), [&j, out_faces](Face& fc){out_faces[j] = fc; ++j; });
	printf("vertices number: %d, faces number: %d\n", i, j);
	return true;
}