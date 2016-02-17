#pragma once

#include <yaml-cpp\yaml.h>
#include <Box2D\Common\b2Math.h>

namespace YAML
{
	template<>
	struct convert<b2Vec2>
	{
		static Node encode(const b2Vec2& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			return node;
		}

		static bool decode(const Node& node, b2Vec2& value)
		{
			if (!node.IsSequence())
				return false;
			if (node.size() != 2)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			return true;
		}
	};
}