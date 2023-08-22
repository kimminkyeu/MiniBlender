//
// Created by Minkyeu Kim on 8/20/23.
//

#ifndef SCOOP_AABB_H
#define SCOOP_AABB_H


#include <glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>

// TODO: 구현이 끝나면, Template으로 바꿀 수 있는 부분 체크하기. (glm::vec3, glm::vec2, custom vec2 ... etc)

// ********************************************************************
// NOTE: https://jacco.ompf2.com/  --> 이 사람 내용을 보면서 공부할 것!
// ***********************************************************************
// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf --> 이건 참조용.
// https://mshgrid.com/2021/01/17/aabb-tree/ --> 이것도 참조용.
// ********************************************************************

// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
// Given two bounding boxes we can compute the union with min and max operations.
// These can be made efficient using SIMD.
struct BoundingBox
{
	glm::vec3 m_LowerBound = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 m_UpperBound = glm::vec3(std::numeric_limits<float>::min());

	// compute the surface area of bounding box (표면적)
	float SurfaceArea()
	{
		glm::vec3 d = m_UpperBound - m_LowerBound;
		return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
	}
};

// 두 BoundingBox를 함치는 경우..
BoundingBox Union(BoundingBox A, BoundingBox B)
{
	BoundingBox C;
	C.m_LowerBound = glm::min(A.m_LowerBound, B.m_LowerBound);
	C.m_UpperBound = glm::min(A.m_UpperBound, B.m_UpperBound);
	return C;
}

// NOTE: Input Data is an array of triangles.
template <typename T> // might be vec2, might be vec3
struct Triangle // Triangle[0] = Triangle.v0
{
	T v0;
	T v1;
	T v2;
	T centroid; // center of triangle, used in Subdivide process of BVH

	explicit Triangle(T _v0, T _v1, T _v2) noexcept
		: v0(_v0), v1(_v1), v2(_v2)
	{
		this->centroid = (v0 + v1 + v2) * 0.3333f;
	}

	T& operator[] (int index)
	{
		if (index >= 3) throw std::out_of_range("index out of range");
		return index == 0 ? v0 : (index == 1 ? v1 : v2);
	}

	const T& operator[] (int index) const
	{
		if (index >= 3) throw std::out_of_range("index out of range");
		return index == 0 ? v0 : (index == 1 ? v1 : v2);
	}
};


// ------------------------------------------------
// Implementation of Axis-aligned bounding box
struct AABBNode
{
	BoundingBox m_Bounds; // bounding box of the node

	unsigned int m_Left = 0;
	unsigned int m_Right = 0;

	unsigned int m_PrimitiveStartIndex = 0; // store the index of the first primitive, and the number of primitives.
	unsigned int m_PrimitiveSize = 0; // Node가 가질 primitive 길이.

	bool m_isLeaf = true;
};

// ------------------------------------------------
// Implementation of Axis-aligned bounding box
class AABBTree
{
private: // typedef and namespace scope
	using triangle_type = Triangle<glm::vec3>;
	// ...
private: // member data
	std::vector<AABBNode> m_Nodes; // Node Pool
	unsigned int m_RootIndex = 0; // root index of node pool
	std::vector<triangle_type> m_Primitives; // primitive array
private: // member data tmp
	std::vector<std::shared_ptr<Lunar::Mesh>> m_MeshList;

private:
	// Subdivide space
	void __BuildBVH()
	{
		m_Nodes.push_back(AABBNode());
//		AABBNode* root = &(m_Nodes[m_RootIndex]);
		// to start, assign all triangles to root node.
		AABBNode* root = &(m_Nodes[m_RootIndex]);
		root->m_PrimitiveStartIndex = 0;
		root->m_PrimitiveSize = m_Primitives.size();

		__UpdateNodeBounds(m_RootIndex);
		__Subdivide_recur(m_RootIndex); // subdivide recursively
		__GenerateDebugMesh_recur(m_RootIndex); // for debug render, generate mesh for each bbox;
	}

	void __UpdateNodeBounds(unsigned int nodeIdx)
	{
		AABBNode* targetNode = &(m_Nodes[nodeIdx]);
		BoundingBox* bbox = &(targetNode->m_Bounds);
		const unsigned int start = targetNode->m_PrimitiveStartIndex;
		// update bounding box for node's every primitive
		for (size_t i=0; i<targetNode->m_PrimitiveSize; ++i) {
			for (int v=0; v<3; ++v) {
				// 모든 삼각형의 모든 vertex를 돌면서, vec3 bound와 vec3 v0 중 최소값을 bound에 갱신.
				bbox->m_LowerBound = glm::min(bbox->m_LowerBound, (m_Primitives[start + i])[v]);
				bbox->m_UpperBound = glm::max(bbox->m_UpperBound, (m_Primitives[start + i])[v]);
			}
		}
	}

	void __Subdivide_recur(unsigned int idx)
	{
		// TODO: Implement here...
	}

	// Tree를 순회하면서 각 box에 대한 mesh를 생성, m_meshList에 삽입.
	// TODO: meshList가 배열이기에, 이걸 level에 따라 순회하려면 재귀를 BFS로 해줘야 한다.
	// 	     일단은 DFS로 함. 나중에 수정할 예정.
	void __GenerateDebugMesh_recur(unsigned int node_idx)
	{
		if (node_idx >= m_Nodes.size()) {
			return;
		}

		auto bbox = m_Nodes[node_idx].m_Bounds;
		const auto ub = m_Nodes[m_RootIndex].m_Bounds.m_UpperBound;
		const auto lb = m_Nodes[m_RootIndex].m_Bounds.m_LowerBound;

		// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_05#Adding_the_3rd_dimension
		float cube_vertices[] = {
				// front
				lb.x, lb.y,  ub.z,
				ub.x, lb.y,  ub.z,
				ub.x,  ub.y,  ub.z,
				lb.x,  ub.y,  ub.z,
				// back
				lb.x, lb.y, lb.z,
				ub.x, lb.y, lb.z,
				ub.x,  ub.y, lb.z,
				lb.x,  ub.y, lb.z
		};

		unsigned int cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// right
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// left
				4, 0, 3,
				3, 7, 4,
				// bottom
				4, 5, 1,
				1, 0, 4,
				// top
				3, 2, 6,
				6, 7, 3
		};


		auto mesh_ptr = std::make_shared<Lunar::Mesh>();
		mesh_ptr->CreateMesh(cube_vertices, cube_elements, 24, 36);
		m_MeshList.push_back(mesh_ptr);

		if (m_Nodes[node_idx].m_isLeaf) {
			return ;
		}
		// traverse every tree to create mesh
//		__GenerateDebugMesh_recur(m_Nodes[node_idx].m_Left);
//		__GenerateDebugMesh_recur(m_Nodes[node_idx].m_Right);
	}

public:
	void DebugRender()
	{
		for (auto &itr : m_MeshList) {
			itr->RenderMesh(GL_TRIANGLES);
		}
	}

	// https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
	// build AABB tree with VAO & IBO array?


	// NOTE: abb를 모델 매트릭스를 곱해서 덮어쓰는게 아니라, 충돌검사 직전 혹은 그리기 직전에 곱해서 그 임시 값으로 검사하는거다.
	AABBTree(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
	{
		const size_t numOfTriangles = indices.size() / 3;
		m_Primitives.reserve(numOfTriangles);

		// Full Binary Tree 의 max node 개수는 2n-1 개이다.
		const size_t maxNumOfNodes = numOfTriangles * 2 - 1;
		m_Nodes.reserve(maxNumOfNodes);

		// NOTE: 최적화 필요. 일단 triangle array로 변환해서 멤버로 갖고 있지만, 이 과정이 필요가 없다고 보임.
		for (size_t i = 0; i < numOfTriangles; ++i)
		{
			const auto v0 = glm::vec3{vertices[indices[i * 3] * 3], vertices[indices[i * 3] * 3 + 1], vertices[indices[i * 3] * 3 + 2]};
			const auto v1 = glm::vec3{vertices[indices[i * 3 + 1] * 3], vertices[indices[i * 3 + 1] * 3 + 1], vertices[indices[i * 3 + 1] * 3 + 2]};
			const auto v2 = glm::vec3{vertices[indices[i * 3 + 2] * 3], vertices[indices[i * 3 + 2] * 3 + 1], vertices[indices[i * 3 + 2] * 3 + 2]};
			m_Primitives.emplace_back(v0, v1, v2);
		}
		__BuildBVH();
	}

	// NOTE: 일단 복사 생성자 금지. [AABBTree t1 = AABB(...)]
	//		  추후에 이 부분 검토할 것.
	AABBTree& operator= (const AABBTree& other) = delete;

	~AABBTree();

	// Returns whether the AABB contains another AABB completely.
	// Because math is done to compute the minimum and maximum coordinates of the AABBs, overflow is possible for extreme values.
	bool Contains(const AABBTree& other);

	//Returns whether a point in 3D space is contained by the AABB, or not.
	// Because math is done to compute the minimum and maximum coordinates of the AABB, overflow is possible for extreme values.
	bool Contains(const glm::vec3& float3);

	// Determines the squared distance from a point to the nearest point to it that is contained by an AABB.
	glm::vec3 DistanceSquared(const glm::vec3& float3);

	// Returns a string representation of the AABB.
	std::string ToString();

	// Transforms an AABB by a 4x4 transformation matrix,
	// and returns a new AABB that contains the transformed AABB completely.
	AABBTree Transform(const glm::mat4x4 matrix);

};

#endif //SCOOP_AABB_H
