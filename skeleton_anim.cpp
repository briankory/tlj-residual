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

#include "engines/stark/skeleton_anim.h"
#include "engines/stark/skeleton.h"
#include "engines/stark/stark.h"

#include "common/stream.h"

namespace Stark {


SkeletonAnim::SkeletonAnim() {
	
}

SkeletonAnim::~SkeletonAnim() {
	for (Common::Array<AnimNode *>::iterator it = _anims.begin(); it != _anims.end(); ++it)
		delete *it;
}

bool SkeletonAnim::createFromStream(Common::ReadStream *stream) {
	_id = stream->readUint32LE();
	_ver = stream->readUint32LE();
	if (_ver == 3) {
		_u1 = 0;
		_time = stream->readUint32LE();
		_u2 = stream->readUint32LE();
	} else {
		_u1 = stream->readUint32LE();
		_u2 = stream->readUint32LE();
		_time = stream->readUint32LE();
	}
	if (_u2 != 0xdeadbabe)
		return false;

	uint32 num = stream->readUint32LE();
	_anims.resize(num);
	for (uint32 i = 0; i < num; ++i) {
		AnimNode *node = new AnimNode();
		node->_bone = stream->readUint32LE();
		uint32 numKeys = stream->readUint32LE();

		for (uint32 j = 0; j < numKeys; ++j) {
			AnimKey *key = new AnimKey();
			key->_time = stream->readUint32LE();
			char *ptr = new char[7 * 4];
			stream->read(ptr, 7 * 4);
			key->_rot = Graphics::Vector3d(get_float(ptr), get_float(ptr + 4), get_float(ptr + 8));
			key->_rotW = get_float(ptr + 12);
			key->_pos = Graphics::Vector3d(get_float(ptr + 16), get_float(ptr + 20), get_float(ptr + 24));
			node->_keys.push_back(key);
		}

		_anims[node->_bone] = node;
	}

	return true;
}

Coordinate SkeletonAnim::getCoordForBone(uint32 time, int boneIdx) {
	Coordinate c;
	// SLERP et al.
	AnimKey *key = _anims[boneIdx]->_keys[0];
	c.setTranslation(key->_pos.x(), key->_pos.y(), key->_pos.z());
	c.setRotation(key->_rotW, key->_rot.x(), key->_rot.y(), key->_rot.z());
	
	return c;
}

} // end of namespace Stark