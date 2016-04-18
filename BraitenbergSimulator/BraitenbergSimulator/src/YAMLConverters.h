#pragma once

#include <yaml-cpp\yaml.h>
#include <Box2D\Common\b2Math.h>
#include "Boundary.h"

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

	template<>
	struct convert<BoundaryType> {
		static Node encode(const BoundaryType& rhs) {
			Node node;
			switch (rhs)
			{
				case BT_TOP:
					node = ("top");
					break;
				case BT_BOTTOM:
					node = ("bottom");
					break;
				case BT_LEFT:
					node = ("left");
					break;
				case BT_RIGHT:
					node = ("right");
					break;
				default:
					node = ("ERROR");
			}
			return node;
		}

		static bool decode(const Node& node, BoundaryType& rhs) {
			if (!node.IsScalar()) {
				return false;
			}
			if (node.as<std::string>() == "top")
				rhs = BT_TOP;
			else if (node.as<std::string>() == "bottom")
				rhs = BT_BOTTOM;
			else if (node.as<std::string>() == "left")
				rhs = BT_LEFT;
			else if (node.as<std::string>() == "right")
				rhs = BT_RIGHT;
			else return false;

			return true;
		}
	};


}