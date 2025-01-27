#pragma once

#include "STD.hpp"
#include "Color.hpp"
#include "Utils.hpp"
#include "Rect.hpp"
#include "SceneObject.hpp"

namespace Birb
{
	namespace Widgets
	{
		enum GraphType
		{
			Block, Line, Area
		};

		class Graph : public SceneObject
		{
		public:
			Graph(); ///< Empty constructor
			Graph(const GraphType& graphType, const Rect& rect); ///< Define a graph with no values. Values can be added later
			Graph(const GraphType& graphType, const std::vector<double>& values, const Rect& rect); ///< Define a graph with predefined values
			void Render(); ///< Calculates graph values into 2D points and renders them into a graph

			GraphType type; ///< Sets the appearance/type of the graph
			Rect rect; ///< Dimensions of the graph

			/* Colors */
			Color borderColor; ///< Color for the border around the graph
			Color backgroundColor; ///< Color of the background behind the blocks/line/border
			Color graphColor; ///< Color for the graph blocks/line

			std::vector<double> values; ///< List of values for the graph y-axis
			double zeroValue = 0; ///< Sets the lowest value in the graph
			bool normalizedZeroValue = false; ///< Set to true for normalized min value
			int borderSize; ///< Graph border thickness
			double wallOffset = 5.0; ///< How much empty space there should be between the graph borders and the line/blocks
			double blockSpacing = 10; ///< Sets the amount of padding between blocks in the graph (doesn't work with lines)

			bool operator==(const Graph& other) const
			{
				return 	(type 					== other.type
						&& rect 				== other.rect
						&& borderColor 			== other.borderColor
						&& backgroundColor 		== other.backgroundColor
						&& graphColor 			== other.graphColor
						&& zeroValue 			== other.zeroValue
						&& normalizedZeroValue 	== other.normalizedZeroValue
						&& borderSize 			== other.borderSize
						&& wallOffset 			== other.wallOffset
						&& blockSpacing 		== other.blockSpacing);
			}

		private:
			void RenderFunc() override;
			void SetPos(const Vector2& delta) override;
		};
	}
}
