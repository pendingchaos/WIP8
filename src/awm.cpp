#include "awm.h"

#include <SDL2/SDL_rwops.h>
#include <glm/glm.hpp>

#include <iostream>
#include <cstdio>

#include "utils/memory.h"
#include "utils/utils.h"
#include "renderer.h"
#include "resource.h"
#include "shader.h"
#include "mesh.h"
#include "vertexbuffer.h"

Mesh *loadAWM(Renderer *renderer,
              ResPtr<Shader> vertexShader,
              const char *filename)
{
    FILE *file = std::fopen(filename, "rb");

    unsigned char m1 = readUInt8(file);
    unsigned char m2 = readUInt8(file);
    unsigned char m3 = readUInt8(file);
    unsigned char m4 = readUInt8(file);

    Mesh *mesh = NULL;

    if (m1 != 'A' or
        m2 != 'W' or
        m3 != 'M' or
        m4 != '1')
    {
        std::cout << "Invalid AWM file: " << filename << std::endl;
    } else
    {
        unsigned int numVertices = readUInt32(file);

        mesh = NEW(Mesh, vertexShader, Mesh::Triangles, numVertices, numVertices);

        VertexBuffer *positionBuffer = mesh->addPositions(renderer, MeshComponent(3, MeshComponent::Float32))->getVertexBuffer();
        VertexBuffer *normalBuffer = mesh->addNormals(renderer, MeshComponent(3, MeshComponent::Float32))->getVertexBuffer();
        VertexBuffer *uvBuffer = mesh->addUVs(renderer, MeshComponent(2, MeshComponent::Float32))->getVertexBuffer();
        VertexBuffer *colorBuffer = mesh->addColors(renderer, MeshComponent(4, MeshComponent::Float32))->getVertexBuffer();
        VertexBuffer *tangentBuffer = mesh->addTangents(renderer, MeshComponent(3, MeshComponent::Float32))->getVertexBuffer();
        VertexBuffer *indexBuffer = mesh->addIndices(renderer, MeshComponent(1, MeshComponent::UInt32))->getVertexBuffer();

        positionBuffer->alloc(sizeof(glm::vec3)*numVertices);
        normalBuffer->alloc(sizeof(glm::vec3)*numVertices);
        uvBuffer->alloc(sizeof(glm::vec2)*numVertices);
        colorBuffer->alloc(sizeof(glm::vec4)*numVertices);
        tangentBuffer->alloc(sizeof(glm::vec3)*numVertices);
        indexBuffer->alloc(sizeof(unsigned int)*numVertices);

        glm::vec3 *positions = (glm::vec3 *)positionBuffer->map(false, true);
        glm::vec3 *normals = (glm::vec3 *)normalBuffer->map(false, true);
        glm::vec2 *uvs = (glm::vec2 *)uvBuffer->map(false, true);
        glm::vec4 *colors = (glm::vec4 *)colorBuffer->map(false, true);
        glm::vec3 *tangents = (glm::vec3 *)tangentBuffer->map(false, true);

        unsigned int *indices = (unsigned int *)indexBuffer->map(false, true);

        for (unsigned int i=0; i<numVertices; i++)
        {
            std::fread(&positions[i], sizeof(glm::vec3), 1, file);

            std::fread(&uvs[i], sizeof(glm::vec2), 1, file);

            std::fread(&normals[i], sizeof(glm::vec3), 1, file);

            unsigned char rcolor[4];
            std::fread(rcolor, 4, 1, file);

            colors[i] = glm::vec4(rcolor[0]/255.0f, rcolor[1]/255.0f, rcolor[2]/255.0f, rcolor[3]/255.0f);

            indices[i] = i;
        }

        for (unsigned int i=0; i<numVertices; i+=3)
        {
            glm::vec3 p0 = positions[i  ];
            glm::vec3 p1 = positions[i+1];
            glm::vec3 p2 = positions[i+2];

            glm::vec2 t0 = uvs[i  ];
            glm::vec2 t1 = uvs[i+1];
            glm::vec2 t2 = uvs[i+2];

            glm::vec3 edge1 = p1 - p0;
            glm::vec3 edge2 = p2 - p0;

            float deltaU1 = t1.x - t0.x;
            float deltaV1 = t1.y - t0.y;

            float deltaU2 = t2.x - t0.x;
            float deltaV2 = t2.y - t0.y;

            float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

            tangents[i  ] = safe_normalize(f * (deltaV2 * edge1 - deltaV1 * edge2));
            tangents[i+1] = tangents[i];
            tangents[i+2] = tangents[i];
        }

        positionBuffer->unmap();
        normalBuffer->unmap();
        uvBuffer->unmap();
        colorBuffer->unmap();
        tangentBuffer->unmap();
        indexBuffer->unmap();

        mesh->calculateAABB();
    }

    std::fclose(file);

    return mesh;
}
