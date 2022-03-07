#include "Graphs.hpp"
#include "Renderwindow.hpp"
#include "Values.hpp"

namespace Birb
{
	namespace Widgets
	{
		Graph::Graph()
		{
			borderColor 	= Colors::White;
			backgroundColor = Colors::White;
			graphColor 		= Colors::White;

			borderSize = 3;
		}

		Graph::Graph(const GraphType& graphType, const Rect& rect)
		{
			this->values = std::vector<double>(0);
			this->rect = rect;
			this->type = graphType;

			borderColor 	= Colors::White;
			backgroundColor = Colors::White;
			graphColor 		= Colors::White;

			borderSize = 3;
		}

		Graph::Graph(const GraphType& graphType, const std::vector<double>& values, const Rect& rect)
		{
			this->values = values;
			this->rect = rect;
			this->type = graphType;

			borderColor 	= Colors::White;
			backgroundColor = Colors::White;
			graphColor 		= Colors::White;

			borderSize = 3;
		}

		void Graph::Render()
		{
			/* Only draw the border if there are <=1 points */
			if (values.size() <= 1)
			{
				Render::DrawRect(borderColor, rect, 3);
				return;
			}

			double width = rect.w - (wallOffset * 2);
			Vector2f points[values.size()];
			
			/* Calculate points */
			double highestValue = Math::FindHighestValue(values);
			double lowestValue;
			if (normalizedZeroValue)
				lowestValue = Math::FindLowestValue(values);
			else
				lowestValue = zeroValue;

			for (int i = 0; i < (int)values.size(); i++)
			{
				points[i].x = (((width / (int)values.size()) * (i + 0.5))) + rect.x + wallOffset;
				points[i].y = Math::Normalize(values[i], highestValue, lowestValue, rect.h) + rect.y;
			}

			/* Draw the background if its not the same color as the graph color */
			if (backgroundColor.r != graphColor.r ||
					backgroundColor.g != graphColor.g ||
					backgroundColor.b != graphColor.b ||
					backgroundColor.a != graphColor.a)
				Render::DrawRect(backgroundColor, rect);

			/* Draw lines / blocks with the point positions */
			switch (type)
			{
				case (GraphType::Line):
					Render::DrawLines(graphColor, points, values.size());
					break;

				case (GraphType::Block):
					for (int i = 0; i < (int)values.size(); i++)
						Render::DrawRect(graphColor, Rect(points[i].x - (((width / (int)values.size()) - blockSpacing) / 2), points[i].y, (width / values.size()) - blockSpacing, Math::Normalize(values[i], lowestValue, highestValue, rect.h)));
					break;
			}


			/* Draw the graph border */
			Render::DrawRect(borderColor, rect, 3);
		}
	}
}
