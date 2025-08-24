/*
 * ellipse.cpp
 * Purpose: To define the decarlations made in the ellipse.h file for the ellipse_buffer and the
 * ellipse enity class
 */
#include "dxflib++/include/entities/ellipse.h"
#include "dxflib++/include/mathlib.h"
#include <valarray>

int dxflib::entities::ellipse_buffer::parse(const std::string& cl, const std::string& nl)
{
	int code{entity_buffer_base::parse(cl, nl)}; // group code of the current line
	if (code == -1)
		return 1;
	// Parse switch;
	switch (static_cast<group_codes::ellipse>(code))
	{
	case group_codes::ellipse::center_point_x:
		center_point_x = std::stod(nl);
		return 1;

	case group_codes::ellipse::center_point_y:
		center_point_y = std::stod(nl);
		return 1;

	case group_codes::ellipse::center_point_z:
		center_point_z = std::stod(nl);
		return 1;

	case group_codes::ellipse::end_point_major_axis_x:
		end_point_major_axis_x = std::stod(nl);
		return 1;

	case group_codes::ellipse::end_point_major_axis_y:
		end_point_major_axis_y = std::stod(nl);
		return 1;

	case group_codes::ellipse::end_point_major_axis_z:
		end_point_major_axis_z = std::stod(nl);
		return 1;

	case group_codes::ellipse::ratio_minor_to_major:
		ratio_minor_to_major = std::stod(nl);
		return 1;

	case group_codes::ellipse::start_param:
		start_param = std::stod(nl);
		return 1;

	case group_codes::ellipse::end_param:
		end_param = std::stod(nl);
		return 1;

	default:
		return 0;
	}
}

void dxflib::entities::ellipse_buffer::free()
{
	entity_buffer_base::free();
	center_point_x = 0;
	center_point_y = 0;
	center_point_z = 0;

	end_point_major_axis_x = 0;
	end_point_major_axis_y = 0;
	end_point_major_axis_z = 0;

	ratio_minor_to_major = 0;
	start_param = 0;
	end_param = 0;
}

dxflib::entities::ellipse::ellipse(ellipse_buffer& ab): entity(ab),
	center_point_(ab.center_point_x, ab.center_point_y, ab.center_point_z),
	end_point_major_axis_(ab.end_point_major_axis_x, ab.end_point_major_axis_y, ab.end_point_major_axis_z),
	ratio_minor_to_major_(ab.ratio_minor_to_major), start_param_(ab.start_param),
	end_param_(ab.end_param)
{
}
