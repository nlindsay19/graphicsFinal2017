#include "Shape.h"

#include "gl/datatype/VAO.h"
#include <math.h>

Shape::Shape() :
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_numVertices(0),
    m_VAO(nullptr),
    m_vhandler()
{
    m_vhandler = std::make_unique<VertexHandler>();

}

Shape::~Shape()
{
// You shouldn't need to do anything here
// m_VAO's destructor will automatically be called
// Aren't smart pointers amazing?
}


/**
* @param data - Vector of floats containing the vertex data.
* @param size - number of elements in the array.
* @param drawMode - Drawing mode
* @param numVertices - Number of vertices to be rendered.
*/
void Shape::setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices) {
// Store the vertex data and other values to be used later when constructing the VAO
    m_data = data;
    m_size = size;
    m_drawMode = drawMode;
    m_numVertices = numVertices;
}


/** has unexpectedly finished.
* @param index OpenGL handle to the attribute location. These are specified in ShaderAttribLocations.h
* @param numElementsPerVertex Number of elements per vertex. Must be 1, 2, 3 or 4 (e.g. position = 3 for x,y,z)
* @param offset Offset in BYTES from the start of the array to the beginning of the first element
* @param type Primitive type (FLOAT, INT, UNSIGNED_BYTE)
* @param normalize
*/
void Shape::setAttribute(
     GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type, bool normalize) {
    m_markers.push_back(VBOAttribMarker(index, numElementsPerVertex, offset, type, normalize));
}

void Shape::buildVAO() {
    CS123::GL::VBO vbo = VBO(m_data, m_size, m_markers, m_drawMode);
    m_VAO = std::make_unique<VAO>(vbo, m_numVertices);
}

void Shape::draw() {
if (m_VAO) {
    m_VAO->bind();
    m_VAO->draw();
    m_VAO->unbind();
}
}


