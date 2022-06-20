#pragma once
#include <assimp/quaternion.h>
#include <assimp/matrix4x4.h>
#include "../../Defines.h"

namespace Skeleton
{
	struct Node
	{
		aiQuaternion m_rotation;
		aiMatrix4x4 m_transform;
		aiMatrix4x4 m_transformInverted;
		unsigned char m_nameLength;
		char m_name[31];
		unsigned short m_parentId;
		unsigned short m_unk;
		Vector3 m_fallbackTranslation;
	};

	struct AnimationKey
	{
		/* (m_time / 0xFFFF) * animDuration is the timing of key */
		unsigned short m_time;
		unsigned short m_rotIndex;
	};

	struct AnimationKeyTranslate : public AnimationKey
	{
		unsigned short m_locIndex;
	};

	enum class CHANNELMODE : unsigned char
	{
		CHANNELMODE_FALLBACKTRANSLATE,
		CHANNELMODE_TRANSLATE,
		CHANNELMODE_SKIP
	};

	struct AnimationChannel
	{
		CHANNELMODE m_channelMode;
		unsigned char m_keyBytesSize;
		unsigned short m_keyCount;

		union
		{
			/* CHANNELMODE == CHANNELMODE_FALLBACKTRANSLATE */
			AnimationKey* m_fallback;
			/* CHANNELMODE == CHANNELMODE_TRANSLATE */
			AnimationKeyTranslate* m_translate;
		} m_keys;
	};

	struct Animation
	{
		unsigned char m_nameLength;
		char m_name[31];
		unsigned int m_flags;
		unsigned int m_channelCount;
		float m_duration;
		AnimationChannel* m_channels;
	};

	struct SSkeleton
	{
		size_t m_nodeCount;
		unsigned short m_animCount;
		unsigned short m_unk1;
		char m_zeros[0x2C];
		Node* m_nodes;
		Animation* m_animations;
		int m_unk2;
		int m_unk3;
		int m_unk4;
	};
}