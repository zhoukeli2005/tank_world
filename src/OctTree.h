#ifndef _oct_tree_h_
#define _oct_tree_h_

#include "Vector.h"
#include "AABB.h"

namespace engine
{
	class OctTreeNode;
	class OctTree;

#define  OctTreeSizeLimit	1

	class OctTreeData
	{
		friend class OctTreeNode;
		friend class OctTree;

		// constructor
		public:
			OctTreeData(int flags) : m_flags(flags), m_oct_tree_prev(NULL), m_oct_tree_next(NULL), m_oct_tree_node(NULL) { }

		public:
			int GetFlags() { return m_flags; }

		// must be override
		private:
			// 返回世界空间的包围盒（位置以及大小）
			virtual math::AABB OctTreeGetBoundBox() = 0;


			// 被碰撞，并返回碰撞的方向（受力方向）（主动碰撞不会被调用）
			virtual void OctTreeConflict(OctTreeData * other, math::Vector direction) = 0;

		private:
			OctTreeData * m_oct_tree_prev;
			OctTreeData * m_oct_tree_next;

			OctTreeNode * m_oct_tree_node;

		private:
			int m_flags;
	};

	class OctTreeNode 
	{
		public:
			OctTreeNode(math::Vector _min, math::Vector _max)
			: m_data_count(0)
			, m_first_data(NULL) 
			, m_min(_min)
			, m_max(_max)
			{
				int i;
				for(i = 0; i < 8; ++i) {
					m_children[i] = NULL;
				}
			}

		public:
			virtual ~OctTreeNode() { }

		public:
			int Contain(const math::AABB & box)
			{
				if(	ContainPoint(box.GetMin()) && ContainPoint(box.GetMax()) ) {
					return TRUE;
				}
				return FALSE;
			}

			int ContainPoint(const math::Vector & point)
			{
				if(	m_min.x <= point.x && m_min.y <= point.y && m_min.z <= point.z &&
					m_max.x >= point.x && m_max.y >= point.y && m_max.z >= point.z ) {
						return TRUE;
				}
				return FALSE;
			}

			void RemoveOctTreeData(OctTreeData * d)
			{
				if(d->m_oct_tree_node != this) {
					Warning("Error!");
					return;
				}

				if(d->m_oct_tree_prev) {
					d->m_oct_tree_prev->m_oct_tree_next = d->m_oct_tree_next;
				} else {
					m_first_data = d->m_oct_tree_next;
				}

				if(d->m_oct_tree_next) {
					d->m_oct_tree_next->m_oct_tree_prev = d->m_oct_tree_prev;
				}

				d->m_oct_tree_node = NULL;
				d->m_oct_tree_next = NULL;
				d->m_oct_tree_prev = NULL;

				m_data_count--;
			}

			OctTreeData * AddOctTreeData(OctTreeData * d, math::Vector direction)
			{
				if(d->m_oct_tree_node) {
					d->m_oct_tree_node->RemoveOctTreeData(d);
				}

				math::AABB boundbox = d->OctTreeGetBoundBox();
				if(!Contain(boundbox)) {
					return NULL;
				}

				if(m_data_count > 0 && !m_children[0] && m_max.x - m_min.x > OctTreeSizeLimit * 2) {
					// 此包围盒已经包含多个物品，并且子空间还没有创建，则进行创建

					math::Vector left_down = m_min;
					math::Vector delta = (m_max - m_min) * 0.5;

					// left down front
					math::Vector curr_left_down = left_down;
					m_children[0] = new OctTreeNode(curr_left_down, curr_left_down + delta);
					
					// right down front
					curr_left_down = left_down + math::Vector(delta.x, 0, 0);
					m_children[1] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// right down back
					curr_left_down = left_down + math::Vector(delta.x, 0, delta.z);
					m_children[2] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// left down back
					curr_left_down = left_down + math::Vector(0, 0, delta.z);
					m_children[3] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// left up front
					curr_left_down = left_down + math::Vector(0, delta.y, 0);
					m_children[4] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// right up front
					curr_left_down = left_down + math::Vector(delta.x, delta.y, 0);
					m_children[5] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// right up back
					curr_left_down = left_down + delta;
					m_children[6] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// left up back
					curr_left_down = left_down + math::Vector(0, delta.y, delta.z);
					m_children[7] = new OctTreeNode(curr_left_down, curr_left_down + delta);

					// reset children
					/*
					OctTreeData * n = m_first_data;
					m_first_data = NULL;
					m_data_count = 0;

					
					while(n) {
						OctTreeData * next = n->m_oct_tree_next;
						n->m_oct_tree_node = NULL;
						n->m_oct_tree_next = n->m_oct_tree_prev = NULL;

						AddOctTreeData(n, direction);

						n = next;
					}*/
				}

				OctTreeData * conflict = NULL;
				// check conflict
				OctTreeData * n = m_first_data;
				while(n) {
					OctTreeData * next = n->m_oct_tree_next;

					if(n->OctTreeGetBoundBox().CheckConflict(boundbox)){
						conflict = n;
						break;;
					}
					n = next;
				}
				
				int succ = 0;
				int i;
				for(i = 0; m_children[0] && i < 8; ++i) {
					if(m_children[i]->Contain(boundbox)) {
						succ = 1;
						OctTreeData * tmp = m_children[i]->AddOctTreeData(d, direction);
						if(tmp && !conflict) {
							conflict = tmp;
						}
						break;
					}
				}

				if(succ) {
					return conflict;
				}

				// add here
				m_data_count++;
				d->m_oct_tree_next = m_first_data;
				if(m_first_data) {
					m_first_data->m_oct_tree_prev = d;
				}
				m_first_data = d;
				d->m_oct_tree_prev = NULL;
				d->m_oct_tree_node = this;
				return conflict;
			}
			

		public:
			OctTreeNode * m_children[8];


			int m_data_count;
			OctTreeData * m_first_data;

			math::Vector	m_min;
			math::Vector	m_max;
	};

	class OctTree
	{
		public:
			OctTree()
			{
				m_root = new OctTreeNode(math::Vector(-2000, -2000, -2000), math::Vector(2000, 2000, 2000));
			}

		public:
			~OctTree();

		public:
			void Remove(OctTreeData * d)
			{
				if(d->m_oct_tree_node) {
					d->m_oct_tree_node->RemoveOctTreeData(d);
				}
			}

			/*
			 *	当物体发生位移，调用这个函数进行碰撞检测，如果有碰撞，会调用 被碰撞物体的 OctTreeData.Conflict 函数， 并返回被碰撞物体
			 *
			 *
			 */
			OctTreeData * CheckConflict(OctTreeData * d, math::Vector direction)
			{
				OctTreeData * other = m_root->AddOctTreeData(d, direction);
				if(other) {
					other->OctTreeConflict(d, direction);
				}

				return other;
			}

		private:
			OctTreeNode * m_root;
	};
}

#endif