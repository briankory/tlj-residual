/* Residual - A 3D game interpreter
 *
 * Residual is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * $URL$
 * $Id$
 *
 */

#ifndef STARK_SKELETON_H
#define STARK_SKELETON_H

#include "common/str.h"
#include "engines/stark/gfx/coordinate.h"

namespace Common {
	class ReadStream;
}

namespace Stark {

class SkeletonAnim;
	
class BoneNode {
public:
	BoneNode() : _parent(-1) { }
	~BoneNode() { }
	Common::String _name;
	float _u1;
	Common::Array<uint32> _children;
	int _parent;
	int _idx;

	Coordinate _animPos;
};

class Skeleton {
public:
	Skeleton();
	~Skeleton();

	bool animate(uint32 time = 0);
	void setNode(uint32 time, BoneNode *bone, const Coordinate &parentCoord);

	bool setAnim(Common::ReadStream *stream);
	bool readFromStream(Common::ReadStream *stream);

	const Common::Array<BoneNode *> &getBones() const { return _bones; }

private:
	Common::Array<BoneNode *> _bones;
	SkeletonAnim *_anim;
};

} // end of namespace Stark

#endif