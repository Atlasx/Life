#pragma once

#include "Grid.h"
#include "yaml-cpp/yaml.h"

namespace YAML {

	template<>
	struct convert<LifeCore::Grid>
	{
		static Node encode(const LifeCore::Grid& grid)
		{
			Node node;
			node.push_back(grid.GetWidth());
			node.push_back(grid.GetHeight());
			return node;
		}

		static bool decode(const Node& node, LifeCore::Grid& rGrid)
		{
			const size_t width = node[0].as<size_t>();
			const size_t height = node[1].as<size_t>();

			printf("importing grid %lld, %lld\n", width, height);
			return true;
		}
	};

}
